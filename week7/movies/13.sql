SELECT DISTINCT p2.name FROM people p1 
    JOIN stars s1 ON s1.person_id = p1.id
    JOIN movies m1 ON m1.id = s1.movie_id AND p1.name = "Kevin Bacon" AND p1.birth = 1958
    JOIN stars s2 ON m1.id = s2.movie_id
    JOIN people p2 ON p2.id = s2.person_id AND p2.name != "Kevin Bacon";