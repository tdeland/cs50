SELECT DISTINCT name FROM people
WHERE NOT people.name = 'Kevin Bacon' AND people.id IN(
SELECT person_id FROM stars
WHERE movie_id IN(
SELECT movies.id FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name = 'Kevin Bacon' AND people.birth = 1958));