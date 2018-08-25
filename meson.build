project(
    'Celegraph',
    'cpp',
    version: '0.0.1',
    license: 'MIT',
    default_options: ['cpp_std=c++11'],
)

# Enable sqlitecpp to use function.getColumnOriginName()
add_project_arguments('-DSQLITE_ENABLE_COLUMN_METADATA', language : 'cpp')

# Set include paths BEFORE subdir() command, or else vars will not be visible
compiler = meson.get_compiler('cpp')

# Custom installed libraries
# For now, download and build them using the Makefile:
#     make msgpack
#     make sqlitecpp
#     make catch
catch_inc = include_directories('lib/catch')
sqlitecpp_inc = include_directories('lib/sqlitecpp')
msgpack_inc = include_directories('lib/msgpack')

server_include_dirs = [msgpack_inc, sqlitecpp_inc]
client_include_dirs = [msgpack_inc]
test_include_dirs = [msgpack_inc, sqlitecpp_inc, catch_inc]

sqlitecpp_lib = compiler.find_library('sqlitecpp', dirs: ['/mnt/c/Users/micro/me/code/celegraph/lib/sqlitecpp'])
# Note: sqlitecpp comes with libsqlite3.a. Link on that
sqlite3_lib = compiler.find_library('sqlite3', dirs: ['/mnt/c/Users/micro/me/code/celegraph/lib/sqlitecpp'])
pthread_lib = compiler.find_library('pthread')
dl_lib = compiler.find_library('dl')

server_libs = [sqlitecpp_lib, sqlite3_lib, pthread_lib, dl_lib]
client_libs = [pthread_lib]
test_libs = server_libs


subdir('src')

# meson.add_install_script(['sqlite3', 'db/celegraph.db', '<', 'db/init_db.sql'])
