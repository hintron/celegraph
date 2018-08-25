server_sources = [
    'Main.cpp',
    'AdminShell.cpp',
    'User.cpp',
    'ServerState.cpp',
    'Server.cpp',
    'SQLConnector.cpp',
    'Utils.cpp',
]
server = executable(
    'celegraph-server', # executable name
    server_sources, # source files
    include_directories : server_include_dirs,
    dependencies : server_libs,
    install : true,
)


client_sources = [
    'Client.cpp',
    'Utils.cpp',
]
client = executable(
    'celegraph-client', # executable name
    client_sources, # source files
    include_directories : client_include_dirs,
    dependencies : client_libs,
    install : true,
)

test_sources = [
    'UnitTest.cpp',
    'AdminShell.cpp',
    'ServerState.cpp',
    'Server.cpp',
    'User.cpp',
    'SQLConnector.cpp',
    'Utils.cpp',
]
test = executable(
    'celegraph-test', # executable name
    test_sources, # source files
    include_directories : test_include_dirs,
    dependencies : test_libs,
    install : true,
)




# client = executable(
#     'client', # executable name
#     'Client.cpp', # source files to compile
#     include_directories : inc, # previously declared include directories in root :code:`meson.build`
#     link_with : libuuid, # linking executable with shared previously declared shared library :code:`libuuid`
#     install : true
# ) # :code:`uuidgen` executable be part of project installation

# test = executable(
#     'test', # executable name
#     'main.cpp', # source files to compile
#     include_directories : inc, # previously declared include directories in root :code:`meson.build`
#     link_with : libuuid, # linking executable with shared previously declared shared library :code:`libuuid`
#     install : true
# ) # :code:`uuidgen` executable be part of project installation