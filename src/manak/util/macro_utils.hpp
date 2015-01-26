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

//! Check if user has opt for dynamic linking
#ifdef MANAK_USE_DYN_LINK
#define MANAK_INLINE

#else
#define MANAK_INLINE inline
#endif // MANAK_UE_DYNAMIC_LINK

#endif // MANAK_UTIL_MACRO_ARG_COUNT_HPP_INCLUDED
