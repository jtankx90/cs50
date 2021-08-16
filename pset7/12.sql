select tbl1.title from (select * from movies inner join stars on movies.id = stars.movie_id inner join people on stars.person_id = people.id) as tbl1
where tbl1.name in ('Johnny Depp','Helena Bonham Carter') group by tbl1.title HAVING count(tbl1.name)= 2;

