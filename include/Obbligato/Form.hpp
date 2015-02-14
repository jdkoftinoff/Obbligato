#pragma once
/*
 Copyright (c) 2013, J.D. Koftinoff Software, Ltd.
 <jeffk@jdkoftinoff.com>
 http://www.jdkoftinoff.com/
 All rights reserved.

 Permission to use, copy, modify, and/or distribute this software for
 any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 WARRANTIES
 WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "Obbligato/World.hpp"

namespace Obbligato
{

template <size_t MaxSizeHint>
inline std::string form( const char *fmt )
{
    return std::string( fmt );
}

#if __cplusplus >= 201103L

template <size_t MaxSizeHint, typename... Args>
inline std::string form( const char *fmt, Args... args )
{
    char buf[MaxSizeHint];
#ifdef _MSC_VER
    if ( _snprintf_s(
             buf, sizeof( buf ) - 1, sizeof( buf ) - 1, fmt, args... )
         > static_cast<ssize_t>( sizeof( buf ) - 1 ) )
#else
    if ( snprintf( buf, sizeof( buf ) - 1, fmt, args... )
         > static_cast<ssize_t>( sizeof( buf ) - 1 ) )
#endif
    {
        throw std::runtime_error( "form() overflow" );
    }
    return std::string( buf );
}

template <typename FirstArg>
void emit_formstring( std::stringstream &result, FirstArg &&first )
{
    result << first;
}

template <typename FirstArg, typename... Args>
void emit_formstring( std::stringstream &result,
                      FirstArg &&first,
                      Args &&... args )
{
    result << first;
    emit_formstring( result, args... );
}

template <typename... Args>
std::string formstring( Args &&... args )
{
    std::stringstream ostr;
    emit_formstring( ostr, args... );
    return ostr.str();
}
#else

template <size_t MaxSizeHint, typename Arg1T>
inline std::string form( const char *fmt, const Arg1T &arg1 )
{
    char buf[MaxSizeHint];
#ifdef _MSC_VER
    if ( _snprintf_s(
             buf, sizeof( buf ) - 1, sizeof( buf ) - 1, fmt, arg1 )
         > static_cast<ssize_t>( sizeof( buf ) - 1 ) )
#else
    if ( snprintf( buf, sizeof( buf ) - 1, fmt, arg1 )
         > static_cast<ssize_t>( sizeof( buf ) - 1 ) )
#endif
    {
        throw std::runtime_error( "form() overflow" );
    }
    return std::string( buf );
}

template <size_t MaxSizeHint, typename Arg1T, typename Arg2T>
inline std::string
    form( const char *fmt, const Arg1T &arg1, const Arg2T &arg2 )
{
    char buf[MaxSizeHint];
#ifdef _MSC_VER
    if ( _snprintf_s( buf,
                      sizeof( buf ) - 1,
                      sizeof( buf ) - 1,
                      fmt,
                      arg1,
                      arg2 )
         > static_cast<ssize_t>( sizeof( buf ) - 1 ) )
#else
    if ( snprintf( buf, sizeof( buf ) - 1, fmt, arg1, arg2 )
         > static_cast<ssize_t>( sizeof( buf ) - 1 ) )
#endif
    {
        throw std::runtime_error( "form() overflow" );
    }
    return std::string( buf );
}

template <size_t MaxSizeHint,
          typename Arg1T,
          typename Arg2T,
          typename Arg3T>
inline std::string form( const char *fmt,
                         const Arg1T &arg1,
                         const Arg2T &arg2,
                         const Arg3T &arg3 )
{
    char buf[MaxSizeHint];
#ifdef _MSC_VER
    if ( _snprintf_s( buf,
                      sizeof( buf ) - 1,
                      sizeof( buf ) - 1,
                      fmt,
                      arg1,
                      arg2,
                      arg3 )
         > static_cast<ssize_t>( sizeof( buf ) - 1 ) )
#else
    if ( snprintf( buf, sizeof( buf ) - 1, fmt, arg1, arg2, arg3 )
         > static_cast<ssize_t>( sizeof( buf ) - 1 ) )
#endif
    {
        throw std::runtime_error( "form() overflow" );
    }
    return std::string( buf );
}

template <size_t MaxSizeHint,
          typename Arg1T,
          typename Arg2T,
          typename Arg3T,
          typename Arg4T>
inline std::string form( const char *fmt,
                         const Arg1T &arg1,
                         const Arg2T &arg2,
                         const Arg3T &arg3,
                         const Arg4T &arg4 )
{
    char buf[MaxSizeHint];
#ifdef _MSC_VER
    if ( _snprintf_s( buf,
                      sizeof( buf ) - 1,
                      sizeof( buf ) - 1,
                      fmt,
                      arg1,
                      arg2,
                      arg3,
                      arg4 )
         > static_cast<ssize_t>( sizeof( buf ) - 1 ) )
#else
    if ( snprintf( buf, sizeof( buf ) - 1, fmt, arg1, arg2, arg3, arg4 )
         > static_cast<ssize_t>( sizeof( buf ) - 1 ) )
#endif
    {
        throw std::runtime_error( "form() overflow" );
    }
    return std::string( buf );
}

template <size_t MaxSizeHint,
          typename Arg1T,
          typename Arg2T,
          typename Arg3T,
          typename Arg4T,
          typename Arg5T>
inline std::string form( const char *fmt,
                         const Arg1T &arg1,
                         const Arg2T &arg2,
                         const Arg3T &arg3,
                         const Arg4T &arg4,
                         const Arg5T &arg5 )
{
    char buf[MaxSizeHint];
#ifdef _MSC_VER
    if ( _snprintf_s( buf,
                      sizeof( buf ) - 1,
                      sizeof( buf ) - 1,
                      fmt,
                      arg1,
                      arg2,
                      arg3,
                      arg4,
                      arg5 )
         > static_cast<ssize_t>( sizeof( buf ) - 1 ) )
#else
    if ( snprintf(
             buf, sizeof( buf ) - 1, fmt, arg1, arg2, arg3, arg4, arg5 )
         > static_cast<ssize_t>( sizeof( buf ) - 1 ) )
#endif
    {
        throw std::runtime_error( "form() overflow" );
    }
    return std::string( buf );
}

template <typename Arg1T>
std::string formstring( const Arg1T &arg1 )
{
    std::stringstream ostr;
    ostr << arg1;
    return ostr.str();
}

template <typename Arg1T, typename Arg2T>
std::string formstring( const Arg1T &arg1, const Arg2T &arg2 )
{
    std::stringstream ostr;
    ostr << arg1 << arg2;
    return ostr.str();
}

template <typename Arg1T, typename Arg2T, typename Arg3T>
std::string formstring( const Arg1T &arg1,
                        const Arg2T &arg2,
                        const Arg3T &arg3 )
{
    std::stringstream ostr;
    ostr << arg1 << arg2 << arg3;
    return ostr.str();
}

template <typename Arg1T,
          typename Arg2T,
          typename Arg3T,
          typename Arg4T>
std::string formstring( const Arg1T &arg1,
                        const Arg2T &arg2,
                        const Arg3T &arg3,
                        const Arg4T &arg4 )
{
    std::stringstream ostr;
    ostr << arg1 << arg2 << arg3 << arg4;
    return ostr.str();
}

template <typename Arg1T,
          typename Arg2T,
          typename Arg3T,
          typename Arg4T,
          typename Arg5T>
std::string formstring( const Arg1T &arg1,
                        const Arg2T &arg2,
                        const Arg3T &arg3,
                        const Arg4T &arg4,
                        const Arg5T &arg5 )
{
    std::stringstream ostr;
    ostr << arg1 << arg2 << arg3 << arg4 << arg5;
    return ostr.str();
}

template <typename Arg1T,
          typename Arg2T,
          typename Arg3T,
          typename Arg4T,
          typename Arg5T,
          typename Arg6T>
std::string formstring( const Arg1T &arg1,
                        const Arg2T &arg2,
                        const Arg3T &arg3,
                        const Arg4T &arg4,
                        const Arg5T &arg5,
                        const Arg6T &arg6 )
{
    std::stringstream ostr;
    ostr << arg1 << arg2 << arg3 << arg4 << arg5 << arg6;
    return ostr.str();
}

#endif
}
