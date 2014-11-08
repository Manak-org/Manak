/**
 * @file macro_utils.hpp
 * @author Sumedh Ghaisas
 *
 * Some useful preprocessor functions.
 */
#ifndef MANAK_UTIL_MACRO_ARG_COUNT_HPP_INCLUDED
#define MANAK_UTIL_MACRO_ARG_COUNT_HPP_INCLUDED

#define MANAK_STRINGIZE_2(X) #X

//! Converts the parameter into string
#define MANAK_STRINGIZE(X) MANAK_STRINGIZE_2(X)

#define STRING_JOIN2(X, Y) X ##_## Y

//! Joins given parameters
#define STRING_JOIN(X, Y) STRING_JOIN2(X, Y)

#endif // MANAK_UTIL_MACRO_ARG_COUNT_HPP_INCLUDED
