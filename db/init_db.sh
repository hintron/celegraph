#!/bin/bash

# Specify DESTDIR to change the location of the install, like so:
#     DESTDIR=~/.celegraph ninja install
# See https://mesonbuild.com/Installing.html#destdir-support
echo "Initializing the Celegraph database to ${DESTDIR}${MESON_INSTALL_PREFIX}/celegraph"
mkdir -p "${DESTDIR}${MESON_INSTALL_PREFIX}/celegraph"
sqlite3 "${DESTDIR}${MESON_INSTALL_PREFIX}/celegraph/celegraph.db" < "${MESON_SOURCE_ROOT}/db/init_db.sql"
