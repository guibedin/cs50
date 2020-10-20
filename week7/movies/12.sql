SELECT movies.title FROM movies
    JOIN stars s1 ON movies.id = s1.movie_id
    JOIN stars s2 ON movies.id = s2.movie_id 
    JOIN people p1 ON p1.id = s1.person_id
    JOIN people p2 ON p2.id = s2.person_id
    WHERE p1.name = "Johnny Depp" AND p2.name = "Helena Bonham Carter";