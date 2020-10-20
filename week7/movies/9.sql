SELECT DISTINCT p1.name FROM people p1
    JOIN stars s1 ON s1.person_id = p1.id
    JOIN movies m1 ON m1.id = s1.movie_id AND m1.year = 2004
    ORDER BY p1.birth;