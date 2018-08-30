BIN = mkdir database server client test

# Set the compiler using the CXX implicit make variable
# Default is g++
# See https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html
# CXXFLAGS is the implicit variable for flags passed to CXX
# How to pass in make arguments:
# 	make CXX=clang++
# See https://stackoverflow.com/questions/2969222/make-gnu-make-use-a-different-compiler

# Locally install 3rd party files, to simplify life (no sudo or messing with os files or os package managers)
MSGPACK_HEADER = -I lib/msgpack
CATCH_HEADER = -I lib/catch
SQLITE_HEADERS = -I lib/sqlitecpp
SQLITE_LIBS = -L lib/sqlitecpp

SERVER_FILES = 		src/Main.cpp src/AdminShell.cpp src/User.cpp src/ServerState.cpp src/Server.cpp src/SQLConnector.cpp src/Utils.cpp
CLIENT_FILES = 		src/Client.cpp src/Utils.cpp
UNITTEST_FILES = 	src/UnitTest.cpp src/AdminShell.cpp src/ServerState.cpp src/Server.cpp src/User.cpp src/SQLConnector.cpp src/Utils.cpp

# Enable sqlitecpp to use query.getColumnOriginName() via -DSQLITE_ENABLE_COLUMN_METADATA
# Add -DSQLITE_USE_LEGACY_STRUCT to make sqlitecpp only work with sqlite versions <= 3.18
DEFS = -DSQLITE_ENABLE_COLUMN_METADATA


all : $(BIN)

mkdir :
	mkdir -p bin db lib

database db : mkdir
	sqlite3 db/celegraph.db < db/init_db.sql

# NOTE: With gcc/g++, dependent libraries need to be specified BEFORE the library it depends on
server : mkdir msgpack sqlitecpp
	$(CXX) -g $(SERVER_FILES) -std=c++11 $(MSGPACK_HEADER) $(SQLITE_HEADERS) $(SQLITE_LIBS) -lSQLiteCpp -lsqlite3 -ldl -lpthread -o bin/server $(DEFS) $(CXXFLAGS)

client : mkdir msgpack
	$(CXX) -g $(CLIENT_FILES) -std=c++11 $(MSGPACK_HEADER) -lpthread -o bin/client $(CXXFLAGS)

test : mkdir msgpack catch sqlitecpp
	$(CXX) -g $(UNITTEST_FILES) -std=c++11 $(CATCH_HEADER) $(MSGPACK_HEADER) $(SQLITE_HEADERS) $(SQLITE_LIBS) -lSQLiteCpp -lsqlite3 -ldl -lpthread -o bin/test $(DEFS) $(CXXFLAGS)


clean :
	rm -f bin;

cleanall : clean
	rm -rf lib;

cleandeps :
	rm -rf lib;


deps: msgpack sqlitecpp doctest

# Download msgpack-c and set it to v2.1.1 if needed
# Delete the rest of the files and only keep the headers (c++ version is a header-only library)
msgpack : mkdir
	@if [ ! -d "lib/msgpack" ]; \
	then \
		echo "Downloading Message Pack..."; \
		cd lib; \
		git clone https://github.com/msgpack/msgpack-c.git msgpack_temp; \
		cd msgpack_temp; \
		echo "Setting msgpack to v2.1.1"; \
		git checkout cpp-2.1.1 -q; \
		cd ..; \
		mv msgpack_temp/include msgpack; \
		rm -rf msgpack_temp; \
		cd ..; \
	fi


# Download the Catch testing framework
catch : mkdir
	@if [ ! -d "lib/catch" ]; \
	then \
		echo "Downloading Catch test framework..."; \
		cd lib; \
		git clone https://github.com/philsquared/Catch.git catch_temp; \
		cd catch_temp; \
		echo "Setting catch to v1.10.0"; \
		git checkout v1.10.0 -q; \
		cd ..; \
		mv catch_temp/single_include catch; \
		rm -rf catch_temp; \
		cd ..; \
	fi


# Download the Doctest testing framework
doctest : mkdir
	@if [ ! -d "lib/doctest" ]; \
	then \
		echo "Downloading Doctest test framework..."; \
		cd lib; \
		git clone https://github.com/onqtam/doctest doctest_temp; \
		cd doctest_temp; \
		echo "Setting doctest to v2.0.0"; \
		git checkout 2.0.0 -q; \
		cd ..; \
		mv doctest_temp/doctest doctest; \
		rm -rf doctest_temp; \
		cd ..; \
	fi


# Download the SQLiteCpp and sqlite3 libraries and headers
sqlitecpp : mkdir
	@if [ ! -d "lib/sqlitecpp" ]; \
	then \
		echo "Downloading SQLiteCpp test framework..."; \
		cd lib; \
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
		cd ..; \
	fi

# NOTES:
# To NOT use the internal sqlite version that comes with sqlitecpp (e.g. libsqlite3-dev), add the following flag to the cmake command above:
# 		-DSQLITECPP_INTERNAL_SQLITE=OFF
# To use an external version of sqlite that is < 3.19, add the following flag to the cmake command above:
# 		-DSQLITE_USE_LEGACY_STRUCT=ON
# and be sure to also add -DSQLITE_USE_LEGACY_STRUCT to the g++ DEFS make variable above, or you'll get some errors in the headers


.PHONY: all, clean, cleanall, mkdir, database, db, server, client, test, msgpack, catch, sqlitecpp, doctest, cleandeps, deps



# References:
# How to make a conditional make variable based on files:
# https://stackoverflow.com/questions/1077676/how-to-conditional-set-up-a-makefile-variable-by-testing-if-a-file-exists
# https://www.gnu.org/software/make/manual/html_node/Wildcard-Function.html