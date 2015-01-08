# We need to modify the Doxyfile slightly.  We'll copy the Doxyfile into the
# build directory, update the location of the source, and then run Doxygen and
# it will generate the documentation into the build directory.

# set version information
set(MANAK_MAJOR_VERSION "1")
set(MANAK_MINOR_VERSION "1")
set(MANAK_PATCH_VERSION "0")
set(MANAK_VERSION_NAME "Master")

# First, read the Doxyfile in as a variable.
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile" DOXYFILE_CONTENTS)

# Now, modify all the "INPUT" paths.  I've written each of the three out by
# hand.  If more are added, they'll need to be added here too.
string(REPLACE
    "./src/manak"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/manak"
    DOXYFILE_CONTENTS "${DOXYFILE_CONTENTS}"
)
string(REPLACE
    "./in_doc"
    "${CMAKE_CURRENT_SOURCE_DIR}/doc"
    DOXYFILE_CONTENTS "${DOXYFILE_CONTENTS}"
)
string(REPLACE
    "release_number"
    "${MANAK_MAJOR_VERSION}.${MANAK_MINOR_VERSION}.${MANAK_PATCH_VERSION}"
    DOXYFILE_CONTENTS "${DOXYFILE_CONTENTS}"
)

# Change the STRIP_FROM_PATH so that it works right even in the build directory;
# otherwise, every file will have the full path in it.
string(REGEX REPLACE
    "(STRIP_FROM_PATH[ ]*=) ./"
    "\\1 ${CMAKE_CURRENT_SOURCE_DIR}/"
    DOXYFILE_CONTENTS ${DOXYFILE_CONTENTS})

# Save the Doxyfile to its new location.
file(WRITE "${DESTDIR}/Doxyfile" "${DOXYFILE_CONTENTS}")
