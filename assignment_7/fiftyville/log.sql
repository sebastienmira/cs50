-- Keep a log of any SQL queries you execute as you solve the mystery.

--check what are the crime_scene_reports description for the crime
SELECT description
FROM crime_scene_reports
WHERE year=2023 AND month=7 AND day=28 AND street='Humphrey Street';
--Outcome: theft took place at 10:15 am at the Humphrey Street bakery. Interviews were conducted today with three witnesse who were present

--check interviews transcripts and names
SELECT name, transcript
FROM interviews
WHERE year=2023 AND month=7 AND day=28;
--Outcome: -Ruth saw thief fetting in a car in the bakery parking lot. Check security footage from bakery parking lot.
--.........-Eugene recognized thief. Saw him earlier in the morrning before arriving to Ema's bakery. Saw him by the ATM on Legget Street withdrawing money. Check atm transactions
--.........-Raymond overheard thief talking for less than a minute. On the call, also asked the accomplice to purchase the earliest flight for the next morning. check phone_calls and flights.

--check for the owners license plates in bakery security logs between 1015 and 1030. And check using ATM on legget street earlier in the morning. And calling someone for less than a minute that day(only Diana or Bruce).
SELECT name
FROM people
WHERE license_plate IN(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year=2023 AND month=7 AND day=28 AND hour=10 AND minute>15 AND minute<30
)
AND
id IN(
    SELECT person_id
    FROM bank_accounts
    WHERE account_number in(
        SELECT account_number
        FROM atm_transactions
        WHERE atm_location='Leggett Street' and year=2023 AND month=7 AND day=28
    )
)
AND phone_number IN(
    SELECT caller
    FROM phone_calls
    WHERE year=2023 AND month=7 AND day=28 and duration<60
)
AND passport_number IN(
    SELECT passport_number
    FROM passengers
    WHERE flight_id IN(
        SELECT id
        FROM flights
        WHERE year=2023 AND month=7 AND day=29 and origin_airport_id=(
            SELECT ID
            FROM airports
            WHERE city='Fiftyville'
        )
    )
)
;

--We know that the accomplice and the thief embarked together and were caller and receiver in the call (diana or bruce are the thief)


SELECT name
FROM people
WHERE phone_number IN(
    SELECT receiver
    FROM phone_calls
    WHERE year=2023 AND month=7 AND day=28 and duration<60 and caller in(
        SELECT phone_number
        FROM PEOPLE
        WHERE name='Bruce'
    )
);

--ACCOMPLICES (Philip(Diana) or Robin(Bruce))

SELECT city
FROM airports
WHERE id in(
    SELECT destination_airport_id
    FROM flights
    WHERE year=2023 AND month=7 AND day=29 AND id IN(
        SELECT flight_id
        FROM passengers
        WHERE passport_number IN(
            SELECT passport_number
            FROM people
            WHERE name='Diana' OR name='Bruce'
        )
    )
    ORDER BY hour ASC
);

--New york was destination | Thief Bruce | Accomplice Robin
