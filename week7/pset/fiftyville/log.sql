-- Find the description of the crime scene that matche the given date and location
SELECT description FROM crime_scene_reports WHERE day = 28 AND month = 7 AND year = 2021 AND street LIKE "Humphrey Street";
-->  Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time
--   – each of their interview transcripts mentions the bakery.

-->  Littering took place at 16:36. No known witnesses.

--- Find the transcripts & names of the people interviewed that day who mentioned the bakery
SELECT name, transcript FROM interviews WHERE day = 28 AND month = 7 AND year = 2021 AND transcript LIKE "%bakery%";
--> | Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
--> | Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
--> | Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
--> | Emma    | I'm the bakery owner, and someone came in, suspiciously whispering into a phone for about half an hour. They never bought anything.                                                                                                                                                                                 |

-- Get names of people whose license plates were seen in the courthouse within 10 min of the theft
SELECT name FROM people
WHERE license_plate IN
(
    SELECT license_plate FROM bakery_security_logs
    WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10
    AND (minute > 5 AND minute < 25)
);
--> Vanessa
--> Barry
--> Brandon
--> Iman
--> Sofia
--> Luca
--> Diana
--> Kelsey
--> Bruce
--> Sophia

--Find people who made phone call, were at atm and were at bakery
SELECT name FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
WHERE account_number IN
(
    SELECT account_number FROM atm_transactions
    WHERE atm_location LIKE "%Leggett Street%"
    AND year = 2021 AND month = 7 AND day = 28
    AND transaction_type LIKE "withdraw"
)
AND name IN
(
    SELECT name FROM people
    WHERE license_plate IN
    (
        SELECT license_plate FROM bakery_security_logs
        WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10
        AND (minute > 5 AND minute < 25)
    )
)
AND phone_number IN
(
    SELECT caller FROM phone_calls
    WHERE year = 2021 AND month = 7 and day = 28
    AND duration < 60
);
--> Bruce
--> Diana

-- Find receiver of suspicious phone calls
SELECT name FROM people
WHERE phone_number IN
(
    SELECT receiver FROM phone_calls
    WHERE year = 2021 AND month = 7 and day = 28
    AND duration < 60
    AND caller IN
    (
        SELECT phone_number FROM people
        WHERE name LIKE "Bruce" OR name LIKE "Diana"
    )
);

--From this we find that the conversations were between
--> Diana  | Philip
--> Bruce | Robin


--Find people who were on the earliest flight out on the following day
SELECT name FROM people
WHERE passport_number IN
(
    SELECT passport_number FROM passengers
    WHERE flight_id IN
    (
        SELECT id FROM flights
        WHERE year = 2021 AND month = 7 AND day = 29
        AND origin_airport_id IN
        (
            SELECT id FROM airports
            WHERE city LIKE "Fiftyville"
        )
        ORDER BY hour, minute
    LIMIT 1
    )
);
-->  Kenny
-->  Sofia
-->  Taylor
-->  Luca
-->  Kelsey
-->  Edward
-->  Bruce
-->  Doris

-- THUS WE CAN CONCLUDE "Bruce" is the thief and "Robin" is his accomplice

--Find the destination airport of the earliest flight out on the next day
SELECT city FROM airports
WHERE id IN
(
    SELECT destination_airport_id FROM flights
    WHERE year = 2021 AND month = 7 AND day = 29
    AND origin_airport_id IN
    (
        SELECT id FROM airports
        WHERE city LIKE "Fiftyville"
    )
    ORDER BY hour, minute
    LIMIT 1
);

--> New York City
