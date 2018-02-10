#!/bin/bash
# Filename:    Restore.sh
# Author:      Joseph DeVictoria
# Date:        March_30_2017
# Purpose:     Simple script to restore database from csv files.

echo "Restoring database now..."

sqlite3 Oldentide.db <<EOF
.headers on
.mode csv
.import accounts.bac accounts
.quit
EOF
echo "Accounts Restored."

sqlite3 Oldentide.db <<EOF
.headers on
.mode csv
.import players.bac players
.quit
EOF
echo "Players Restored."

sqlite3 Oldentide.db <<EOF
.headers on
.mode csv
.import npcs.bac npcs
.quit
EOF
echo "NPCs Restored."

sqlite3 Oldentide.db <<EOF
.headers on
.mode csv
.import items.bac items
.quit
EOF
echo "Items Restored."

