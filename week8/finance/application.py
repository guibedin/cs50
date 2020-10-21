import os

from datetime import datetime
from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


# Get cash for current user
def get_cash():
    row = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    cash = float(row[0]["cash"])

    return cash

@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    cash = get_cash()
    rows = db.execute("SELECT symbol, shares FROM user_stocks WHERE user_id = :user_id", user_id=session["user_id"])

    # creates list of values, first one is current cash
    # list will be filled with dictionaries, one for each stock the user owns
    values = [{
        "symbol": "CASH",
        "shares": "",
        "price": "",
        "total": cash,
    }]
    
    total_sum = cash
    # creates a list of dicts to display in the table
    for row in rows:
        symbol = row["symbol"]
        shares = int(row["shares"])

        price = lookup(symbol)["price"]
        total = shares * price
        
        total_sum += total
        
        values.append({
            "symbol": symbol,
            "shares": shares,
            "price": price,
            "total": total,
        })

    return render_template("index.html", values=values, total_sum=total_sum)
    #return apology("TODO")

@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """ Make a deposit to the account """
    
    if request.method == "GET":
        return render_template("deposit.html")
    
    deposit = request.form.get("deposit")

    if deposit == "":
        return apology("can't be empty")
    
    deposit = float(deposit)

    if deposit <= 0.0 or deposit > 10000.0:
        return apology("you can deposit between $1.00 and $10,000.00")
    
    total = round(get_cash() + deposit, 2)
    db.execute("UPDATE users SET cash=:total WHERE id=:user_id", total=total, user_id=session["user_id"])
    
    flash(f"Your new account balance is {total}")
    return render_template("deposit.html")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "GET":
        return render_template("buy.html")

    symbol = request.form.get("symbol")
    shares = request.form.get("shares")

    # check if symbol or shares are empty
    if symbol == "":
        return apology("must provide symbol")
    if shares == "":
        return apology("must provide shares")
    try:
        shares = int(shares)
    except ValueError:
        return apology("shares can't have decimal points")

    if shares <= 0:
        return apology("shares must be greater than 0")
    
    quote = lookup(symbol)

    # check if invalid symbol
    if quote == None:
        return apology("symbol not found")

    symbol = quote["symbol"]
    price = quote["price"]
    total = round(price * shares, 2)

    cash = get_cash()
    
    if total > cash:
        return apology("you don't have enough cash for this operation")
    
    # updates transactions table with this buy
    db.execute("INSERT INTO transactions (user_id, symbol, shares, total, buy) VALUES (:user_id, :symbol, :shares, :total, :buy)",
        user_id=session["user_id"], symbol=symbol, shares=shares, total=total, buy=1)

    # get current number of shares from user_stocks
    rows = db.execute("SELECT id, shares FROM user_stocks WHERE user_id = :user_id AND symbol = :symbol", 
        user_id=session["user_id"], symbol=symbol)
    
    # if user doesn't have any shares for the selected stock
    if len(rows) == 0:
        db.execute("INSERT INTO user_stocks (user_id, symbol, shares) VALUES (:user_id, :symbol, :shares)", 
            user_id=session["user_id"], symbol=symbol, shares=shares)
    else:
        id = rows[0]["id"]
        current_shares = rows[0]["shares"]
        shares += current_shares
        db.execute("UPDATE user_stocks SET shares=:shares WHERE id=:id", shares=shares, id=id)

    # updates users table with current cash
    cash = round(cash - total, 2)
    db.execute("UPDATE users SET cash=:cash WHERE id = :user_id", cash=cash, user_id=session["user_id"])
    
    # messages and return template
    flash(f"You bought a total of ${total} of {symbol}.")
    flash(f"Your current balance is ${cash}.")
    return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    
    # list of history of transactions
    history = []

    rows = db.execute("SELECT symbol, shares, total, buy, sell, date FROM transactions WHERE user_id = :user_id", user_id=session["user_id"])

    if len(rows) == 0:
        flash("You don't have any transactions yet!")
        return render_template("history.html")

    for row in rows:
        history.append({
            "type": "sell" if row["sell"] == 1 else "buy",
            "symbol": row["symbol"],
            "shares": row["shares"],
            "total": row["total"],
            "date": row["date"]
        })

    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "GET":
        return render_template("quote.html")

    symbol = request.form.get("symbol")
    quote = lookup(symbol)
    
    if quote == None:
        return apology(f"symbol {symbol} not found.")
    
    return render_template("quoted.html", quote=quote)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 403)

        # Ensure password and confirmation match
        elif password != confirmation:
            return apology("passwords don't match", 403)

        phash = generate_password_hash(password)

        rows = db.execute("SELECT * FROM users WHERE username = :username", username=username)
        if len(rows) > 0:
            return apology("this username is already taken", 403)
        
        # Returns ID after insert
        id = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=username, hash=phash)
        
        # Store session for current registered username to login
        session["user_id"] = id
        
        flash("Registered!")
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    cash = get_cash()
    # empty list of symbol - shares
    user_stocks = []
    # empty list of options for <select>
    options = []

    # list of options with the stocks that the user owns
    rows = db.execute("SELECT id, symbol, shares FROM user_stocks WHERE user_id=:user_id", user_id=session["user_id"])

    if len(rows) == 0:
        flash("You don't have any stocks to sell!")
        return render_template("sell.html")
        
    # fills user_stocks and options lists
    for row in rows:
        user_stocks.append(row)
        options.append({
            "symbol": row["symbol"]
        })

    if request.method == "GET":
        return render_template("sell.html", options=rows)


    symbol = request.form.get("symbol")
    shares = request.form.get("shares")

    # check if symbol or shares are valid
    if symbol == "":
        return apology("must provide symbol")
    if shares == "":
        return apology("must provide shares")
    try:
        shares = int(shares)
    except ValueError:
        return apology("shares can't have decimal points")
    if shares <= 0:
        return apology("shares must be greater than 0")

    # current price of stock being sold
    quote = lookup(symbol)
    if quote == None:
        return apology("invalid symbol")
    
    price = quote["price"]
    # total from the sell
    total = shares * price

    # check every stock the user has
    for stock in user_stocks:
        current_shares = int(stock["shares"])
        id = stock["id"]

        if symbol == stock["symbol"]:
            # trying to sell more shares than user has
            if shares > current_shares:
                return apology("You don't have enough shares")

            # updates transactions table with this sell
            db.execute("INSERT INTO transactions (user_id, symbol, shares, total, sell) VALUES (:user_id, :symbol, :shares, :total, :sell)",
                user_id=session["user_id"], symbol=symbol, shares=shares, total=total, sell=1)

            shares = current_shares - shares
            if shares == 0:
                db.execute("DELETE FROM user_stocks WHERE id=:id", id=id)
            else:
                db.execute("UPDATE user_stocks SET shares=:shares WHERE id=:id", shares=shares, id=id)
            # updates users table with current cash
            cash = round(cash + total, 2)
            db.execute("UPDATE users SET cash=:cash WHERE id = :user_id", cash=cash, user_id=session["user_id"])
        
            # messages and return template
            flash(f"You sold a total of ${total} of {symbol}.")
            flash(f"Your current balance is ${cash}.")

            return render_template("sell.html")
    
    # only gets here if the symbol posted is not found in user_stocks
    return apology(f"You don't have shares of {symbol}")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
