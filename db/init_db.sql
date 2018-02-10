
------------------------
-- Tables
------------------------

DROP TABLE IF EXISTS account;
CREATE TABLE account (
    id integer not null primary key autoincrement,
    valid integer not null,
    accountname text not null unique collate nocase,
    email text not null collate nocase,
    session text collate nocase,
    active integer not null,
    key text not null collate nocase,
    salt text not null collate nocase
);

DROP TABLE IF EXISTS user;
CREATE TABLE user (
    id integer not null primary key autoincrement,
    account_id integer,
    -- Identification:
    firstname text not null,
    lastname text not null
    -- Stats:
    -- Location:
);


------------------------
-- Views
------------------------

-- DROP VIEW IF EXISTS view_players;
-- CREATE VIEW view_players AS
-- SELECT
--     *
-- FROM players
-- LEFT JOIN accounts on players.account_id = accounts.id
-- ;


-- References
-- How object-oriented class inheritance does NOT fit with sql schemas
-- https://stackoverflow.com/a/193222