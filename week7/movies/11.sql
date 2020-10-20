SELECT title FROM movies
    JOIN stars ON movies.id = stars.movie_id
    JOIN people ON stars.person_id = people.id AND people.name = "Chadwick Boseman"
    JOIN ratings ON movies.id = ratings.movie_id
    ORDER BY ratings.rating DESC LIMIT 5;