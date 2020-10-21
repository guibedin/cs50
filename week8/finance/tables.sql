DROP TABLE 'users';
DROP TABLE 'transactions';
DROP TABLE 'user_stocks';

CREATE TABLE 'users' (
    'id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 
    'username' TEXT NOT NULL, 
    'hash' TEXT NOT NULL, 
    'cash' NUMERIC NOT NULL DEFAULT 10000.00 
);

CREATE UNIQUE INDEX 'username' ON "users" ("username");

CREATE TABLE 'transactions' (
    'id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, 
    'user_id' INTEGER NOT NULL,
    'symbol' TEXT NOT NULL,
    'shares' NUMERIC NOT NULL,
    'total' NUMERIC NOT NULL,
    'buy' NUMERIC NOT NULL DEFAULT 0,
    'sell' NUMERIC NOT NULL DEFAULT 0,
    'date' TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY('user_id') REFERENCES users('id')
);

CREATE TABLE 'user_stocks' (
    'id' INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    'user_id' INTEGER NOT NULL,
    'symbol' TEXT NOT NULL UNIQUE,
    'shares' INTEGER NOT NULL,
    FOREIGN KEY('user_id') REFERENCES users('id')
);

CREATE UNIQUE INDEX 'symbol' ON "user_stocks" ("symbol");