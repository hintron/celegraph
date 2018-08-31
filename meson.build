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
# For now, first download and build them using the Makefile:
#     make deps
# catch_inc = include_directories('lib/catch')
sqlitecpp_inc = include_directories(join_paths('lib','sqlitecpp'))
msgpack_inc = include_directories(join_paths('lib','msgpack'))
doctest_inc = include_directories(join_paths('lib','doctest'))

server_include_dirs = [msgpack_inc, sqlitecpp_inc]
client_include_dirs = [msgpack_inc]
test_include_dirs = [msgpack_inc, sqlitecpp_inc, doctest_inc]

sqlitecpp_lib = compiler.find_library('SQLiteCpp', dirs: [join_paths(meson.current_source_dir(), 'lib', 'sqlitecpp')])
# Note: sqlitecpp comes with libsqlite3.a. Link on that
sqlite3_lib = compiler.find_library('sqlite3', dirs: [join_paths(meson.current_source_dir(), 'lib', 'sqlitecpp')])
pthread_lib = dependency('threads')
dl_lib = compiler.find_library('dl')

server_libs = [sqlitecpp_lib, sqlite3_lib, pthread_lib, dl_lib]
client_libs = [pthread_lib]
test_libs = server_libs

subdir('src')
subdir('db')
