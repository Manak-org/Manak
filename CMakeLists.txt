cmake_minimum_required(VERSION 2.8.5)

project(manak C CXX)

# set version information
set(MANAK_MAJOR_VERSION "0")
set(MANAK_MINOR_VERSION "0")
set(MANAK_PATCH_VERSION "0")
set(MANAK_VERSION_NAME "master")

# Recurse into the rest of the project.
add_subdirectory(src/manak)
