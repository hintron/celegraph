#!/bin/bash
# Filename:    Backup.sh
# Author:      Joseph DeVictoria
# Date:        March_30_2017
# Purpose:     Simple script to backup database to csv files.

echo "Backup up database now..."

sqlite3 Oldentide.db <<EOF
.headers on
.mode csv
.output accounts.csv
SELECT * FROM accounts;
.quit
EOF
echo "Accounts complete."

sqlite3 Oldentide.db <<EOF
.headers on
.mode csv
.output players.csv
SELECT * FROM players;
.quit
EOF
echo "Players complete."

sqlite3 Oldentide.db <<EOF
.headers on
.mode csv
.output npcs.csv
SELECT * FROM npcs;
.quit
EOF
echo "NPCs complete."

sqlite3 Oldentide.db <<EOF
.headers on
.mode csv
.output items.csv
SELECT * FROM items;
.quit
EOF
echo "Items complete."

