select distinct(people.name) from people inner join directors on people.id = directors.person_id inner join ratings on directors.movie_id = ratings.movie_id where ratings.rating >= '9.0' order by people.name;