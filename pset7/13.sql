select people.name from people inner join stars on people.id = stars.person_id where stars.movie_id in
(select movies.id from movies inner join stars on movies.id = stars.movie_id inner join people on stars.person_id = people.id where people.name = 'Kevin Bacon' and people.birth = '1958')
and people.name != 'Kevin Bacon';