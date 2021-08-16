-- Keep a log of any SQL queries you execute as you solve the mystery.



--select * from crime_scene_reports where day = '28'and street = 'Chamberlin Street';
--id | year | month | day | street | description
--295 | 2020 | 7 | 28 | Chamberlin Street | Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
--Interviews were conducted today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.

--transcripts mention courthouse query:

--select * from interviews where month = '7' and day = '28' and transcript like '%courthouse%';

--161 | Ruth | 2020 | 7 | 28 | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away. If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
--162 | Eugene | 2020 | 7 | 28 | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
--163 | Raymond | 2020 | 7 | 28 | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.


--161 | Ruth | 2020 | 7 | 28 | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away. If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
-- who exited courthouse
-- select distinct(name) from courthouse_security_logs inner join people on courthouse_security_logs.license_plate = people.license_plate  where day = '28' and hour = '10' and minute > '14' and minute < '30';


--163 | Raymond | 2020 | 7 | 28 | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.
-- select * from flights inner join airports on flights.origin_airport_id = airports.id  where origin_airport_id = 8 and day = '29'
-- who call on 28 aand flew out of fiftyvile on 29 with duration < 60  and on the first flight ( fiftyville id = 36)
-- select distinct(name) from phone_calls inner join people on phone_calls.caller = people.phone_number where day = '28' and name in (select name from passengers inner join people on passengers.passport_number = people.passport_number where flight_id ='36' order by name) and duration <60;

--162 | Eugene | 2020 | 7 | 28 | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- who withdrew money from fifer street
-- select distinct(name) from atm_transactions inner join bank_accounts on atm_transactions.account_number = bank_accounts.account_number inner join people on bank_accounts.person_id = people.id where month ='7' and day= '28' and atm_location = 'Fifer Street' and transaction_type ='withdraw';


-- Findings: Ernest exited courthouse, withdrew money and flew out of fiftyvile on first flight on 29th and called within 10mins of robbery

-- who ernest called on 28th for < 60seconds? Berthold

-- select * from phone_calls inner join people on phone_calls.receiver = people.phone_number where caller in (select phone_number from people where name = 'Ernest') and day = '28'

select distinct(name) from courthouse_security_logs inner join people on courthouse_security_logs.license_plate = people.license_plate  where day = '28' and hour = '10' and minute > '14' and minute < '30'
INTERSECT
select distinct(name) from phone_calls inner join people on phone_calls.caller = people.phone_number where day = '28' and name in (select name from passengers inner join people on passengers.passport_number = people.passport_number where flight_id ='36' order by name) and duration <60
INTERSECT
select distinct(name) from atm_transactions inner join bank_accounts on atm_transactions.account_number = bank_accounts.account_number inner join people on bank_accounts.person_id = people.id where month ='7' and day= '28' and atm_location = 'Fifer Street' and transaction_type ='withdraw';