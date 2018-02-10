echo "Backup up database now..."

sqlite3 celegraph.db <<EOF
.headers on
.mode csv
.output account.csv
SELECT * FROM account;
.quit
EOF
echo "Account table complete."

sqlite3 celegraph.db <<EOF
.headers on
.mode csv
.output user.csv
SELECT * FROM user;
.quit
EOF
echo "User table complete."

