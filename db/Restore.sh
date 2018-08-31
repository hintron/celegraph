echo "Restoring database now..."

sqlite3 celegraph.db <<EOF
.headers on
.mode csv
.import account.csv account
.quit
EOF
echo "Account table restored."

sqlite3 celegraph.db <<EOF
.headers on
.mode csv
.import user.csv user
.quit
EOF
echo "User table restored."
