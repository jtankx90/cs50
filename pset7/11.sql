 select movies.title from movies
 inner join ratings on movies.id = ratings.movie_id
 inner join stars on ratings.movie_id = stars.movie_id
 inner join people on stars.person_id = people.id
 where people.name ='Chadwick Boseman'
 order by ratings.rating DESC
 limit 5;