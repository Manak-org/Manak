# Define the files we need to compile
# Anything not in this list will not be compiled into Manak.
set(SOURCES
  object_store.cpp
  timer.cpp
)

# Add directory name to sources.
set(DIR_SRCS)
foreach(file ${SOURCES})
  set(DIR_SRCS ${DIR_SRCS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
endforeach()
# Append sources (with directory name) to list of all Manak sources (used at
# the parent scope).
set(MANAK_SRCS ${MANAK_SRCS} ${DIR_SRCS} PARENT_SCOPE)
