BIN = mkdir database server client test

# Set the compiler
COMPILER = g++
# COMPILER = clang++
# COMPILER = em++

# Locally install 3rd party files, to simplify life (no sudo or messing with os files or os package managers)
MSGPACK_HEADER = -I msgpack
CATCH_HEADER = -I catch
SQLITE_HEADERS = -I sqlitecpp
SQLITE_LIBS = -L sqlitecpp


SERVER_FILES = 		Main.cpp AdminShell.cpp User.cpp ServerState.cpp Server.cpp SQLConnector.cpp Utils.cpp
CLIENT_FILES = 		Client.cpp Utils.cpp
UNITTEST_FILES = 	UnitTest.cpp AdminShell.cpp ServerState.cpp Server.cpp User.cpp SQLConnector.cpp Utils.cpp

# Enable sqlitecpp to use query.getColumnOriginName() via -DSQLITE_ENABLE_COLUMN_METADATA
# Add -DSQLITE_USE_LEGACY_STRUCT to make sqlitecpp only work with sqlite versions <= 3.18
DEFS = -DSQLITE_ENABLE_COLUMN_METADATA


all : $(BIN)

mkdir :
	mkdir -p bin db

database db : mkdir
	sqlite3 db/celegraph.db < db/init_db.sql

# NOTE: With gcc/g++, dependent libraries need to be specified BEFORE the library it depends on
server : mkdir msgpack sqlitecpp
	$(COMPILER) -g $(SERVER_FILES) -std=c++11 $(MSGPACK_HEADER) $(SQLITE_HEADERS) $(SQLITE_LIBS) -lSQLiteCpp -lsqlite3 -ldl -lpthread -o bin/server $(DEFS)

client : mkdir msgpack
	$(COMPILER) -g $(CLIENT_FILES) -std=c++11 $(MSGPACK_HEADER) -lpthread -o bin/client

test : mkdir msgpack catch sqlitecpp
	$(COMPILER) -g $(UNITTEST_FILES) -std=c++11 $(CATCH_HEADER) $(MSGPACK_HEADER) $(SQLITE_HEADERS) $(SQLITE_LIBS) -lSQLiteCpp -lsqlite3 -ldl -lpthread -o bin/test $(DEFS)

clean :
	rm -f bin/server bin/client bin/test;

cleanall : clean
	rm -rf msgpack;
	rm -rf catch;
	rm -rf sqlitecpp;

cleansql :
	rm -rf sqlitecpp;
	rm -rf sqlitecpp_temp;

# Download msgpack-c and set it to v2.1.1 if needed
# Delete the rest of the files and only keep the headers (c++ version is a header-only library)
msgpack :
	@if [ ! -d "msgpack" ]; \
	then \
		echo "Downloading Message Pack..."; \
		git clone https://github.com/msgpack/msgpack-c.git msgpack_temp; \
		cd msgpack_temp; \
		echo "Setting msgpack to v2.1.1"; \
		git checkout cpp-2.1.1 -q; \
		cd ..; \
		mv msgpack_temp/include msgpack; \
		rm -rf msgpack_temp; \
	fi


# Download the Catch testing framework
catch :
	@if [ ! -d "catch" ]; \
	then \
		echo "Downloading Catch test framework..."; \
		git clone https://github.com/philsquared/Catch.git catch_temp; \
		cd catch_temp; \
		echo "Setting catch to v1.10.0"; \
		git checkout v1.10.0 -q; \
		cd ..; \
		mv catch_temp/single_include catch; \
		rm -rf catch_temp; \
	fi

# Download the SQLiteCpp and sqlite3 libraries and headers
sqlitecpp :
	@if [ ! -d "sqlitecpp" ]; \
	then \
		echo "Downloading SQLiteCpp test framework..."; \
		git clone https://github.com/SRombauts/SQLiteCpp.git sqlitecpp_temp; \
		cd sqlitecpp_temp; \
		echo "Setting SQLiteCpp to 2.2.0"; \
		git checkout 2.2.0 -q; \
		mkdir -p build; \
		cd build; \
		cmake ..; \
		make; \
		cd ../..; \
		mkdir -p sqlitecpp; \
		cp -r sqlitecpp_temp/include/SQLiteCpp sqlitecpp/; \
		cp sqlitecpp_temp/build/libSQLiteCpp.a sqlitecpp/; \
		cp sqlitecpp_temp/build/sqlite3/libsqlite3.a sqlitecpp/; \
		cp sqlitecpp_temp/sqlite3/sqlite3.h sqlitecpp/; \
		rm -rf sqlitecpp_temp; \
	fi

# NOTES:
# To NOT use the internal sqlite version that comes with sqlitecpp (e.g. libsqlite3-dev), add the following flag to the cmake command above:
# 		-DSQLITECPP_INTERNAL_SQLITE=OFF
# To use an external version of sqlite that is < 3.19, add the following flag to the cmake command above:
# 		-DSQLITE_USE_LEGACY_STRUCT=ON
# and be sure to also add -DSQLITE_USE_LEGACY_STRUCT to the g++ DEFS make variable above, or you'll get some errors in the headers


.PHONY: all, clean, cleanall, cleansql, mkdir, database, db, server, client, test, msgpack, catch, sqlitecpp



# References:
# How to make a conditional make variable based on files:
# https://stackoverflow.com/questions/1077676/how-to-conditional-set-up-a-makefile-variable-by-testing-if-a-file-exists
# https://www.gnu.org/software/make/manual/html_node/Wildcard-Function.html