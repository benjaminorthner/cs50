SELECT DISTINCT(name) FROM people
JOIN stars ON stars.person_id = people.id
JOIN movies ON movies.id = stars.movie_id
WHERE title IN

--- movies in which 1958 Kevin bacon starred
(
SELECT title FROM movies
JOIN stars ON stars.movie_id = movies.id
JOIN people ON people.id = stars.person_id
WHERE name = "Kevin Bacon"
AND birth = 1958
)

-- exclude Kevin Bacon himself
AND name != "Kevin Bacon";


-- alternate version

SELECT name FROM people
WHERE id IN
(
    SELECT person_id FROM stars
    WHERE movie_id in
    (
        SELECT movies.id FROM movies
        JOIN stars ON stars.movie_id = movies.id
        JOIN people ON people.id = stars.person_id
        WHERE name = "Kevin Bacon"
        AND birth = 1958
    )
)
AND name != "Kevin Bacon";