#pragma once
#ifndef Obbligato_Form_hpp
#define Obbligato_Form_hpp

/*
 Copyright (c) 2013, J.D. Koftinoff Software, Ltd. <jeffk@jdkoftinoff.com>
 http://www.jdkoftinoff.com/
 All rights reserved.
 
 Permission to use, copy, modify, and/or distribute this software for any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.
 
 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
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
    
    template <typename T>
    struct PrintfFmtTraits
    {
    };
    
    template <>
    struct PrintfFmtTraits<void *>
    {
        static const char *fmt() { return "%p"; }
    };
    
    template <>
    struct PrintfFmtTraits<bool>
    {
        static const char *fmt() { return "%d"; }
    };
    
    template <>
    struct PrintfFmtTraits<double>
    {
        static const char *fmt() { return "%lf"; }
    };
    
    template <>
    struct PrintfFmtTraits<float>
    {
        static const char *fmt() { return "%f"; }
    };
    
    template <>
    struct PrintfFmtTraits<int>
    {
        static const char *fmt() { return "%d"; }
    };
    
    template <>
    struct PrintfFmtTraits<unsigned int>
    {
        static const char *fmt() { return "%u"; }
    };
    
    
    template <>
    struct PrintfFmtTraits<short>
    {
        static const char *fmt() { return "%hd"; }
    };
    
    template <>
    struct PrintfFmtTraits<unsigned short>
    {
        static const char *fmt() { return "%hu"; }
    };
    
    template <>
    struct PrintfFmtTraits<long>
    {
        static const char *fmt() { return "%ld"; }
    };
    
    template <>
    struct PrintfFmtTraits<unsigned long>
    {
        static const char *fmt() { return "%lu"; }
    };
    
    
    template <>
    struct PrintfFmtTraits<long long>
    {
        static const char *fmt() { return "%lld"; }
    };
    
    template <>
    struct PrintfFmtTraits<unsigned long long>
    {
        static const char *fmt() { return "%llu"; }
    };
    
    template <>
    struct PrintfFmtTraits<int8_t>
    {
        static const char *fmt() { return "%hhd"; }
    };
    
    template <>
    struct PrintfFmtTraits<uint8_t>
    {
        static const char *fmt() { return "%hhu"; }
    };
    
    
    
    template <size_t MaxSizeHint>
    inline std::string form( const char *fmt )
    {
        return std::string(fmt);
    }
    
    template <size_t MaxSizeHint, typename Arg1>
    inline std::string form( const char *fmt,  Arg1 const & arg1 )
    {
        char buf[MaxSizeHint];
#ifdef _MSC_VER
        if( _snprintf_s(buf,sizeof(buf)-1,sizeof(buf)-1, fmt, arg1 ) > static_cast<ssize_t>(sizeof(buf)-1) )
#else
            if( snprintf(buf,sizeof(buf)-1, fmt, arg1 ) > static_cast<ssize_t>(sizeof(buf)-1) )
#endif
            {
                throw std::runtime_error("form() overflow");
            }
        return std::string(buf);
    }
    
    template <size_t MaxSizeHint, typename Arg1, typename Arg2>
    inline std::string form( const char *fmt,  Arg1 const & arg1, Arg2 const & arg2 )
    {
        char buf[MaxSizeHint];
#ifdef _MSC_VER
        if( _snprintf_s(buf,sizeof(buf)-1,sizeof(buf)-1, fmt, arg1, arg2 ) > static_cast<ssize_t>(sizeof(buf)-1) )
#else
            if( snprintf(buf,sizeof(buf)-1, fmt, arg1, arg2 ) > static_cast<ssize_t>(sizeof(buf)-1) )
#endif
            {
                throw std::runtime_error("form() overflow");
            }
        return std::string(buf);
    }
    
    template <size_t MaxSizeHint, typename Arg1, typename Arg2, typename Arg3>
    inline std::string form( const char *fmt,  Arg1 const & arg1, Arg2 const & arg2, Arg3 const & arg3 )
    {
        char buf[MaxSizeHint];
#ifdef _MSC_VER
        if( _snprintf_s(buf,sizeof(buf)-1,sizeof(buf)-1, fmt, arg1, arg2, arg3 ) > static_cast<ssize_t>(sizeof(buf)-1) )
#else
            if( snprintf(buf,sizeof(buf)-1, fmt, arg1, arg2, arg3 ) > static_cast<ssize_t>(sizeof(buf)-1) )
#endif
            {
                throw std::runtime_error("form() overflow");
            }
        return std::string(buf);
    }
    
    template <size_t MaxSizeHint, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
    inline std::string form( const char *fmt,  Arg1 const & arg1, Arg2 const & arg2, Arg3 const & arg3, Arg4 const & arg4 )
    {
        char buf[MaxSizeHint];
#ifdef _MSC_VER
        if( _snprintf_s(buf,sizeof(buf)-1,sizeof(buf)-1, fmt, arg1, arg2, arg3, arg4 ) > static_cast<ssize_t>(sizeof(buf)-1) )
#else
            if( snprintf(buf,sizeof(buf)-1, fmt, arg1, arg2, arg3, arg4 ) > static_cast<ssize_t>(sizeof(buf)-1) )
#endif
            {
                throw std::runtime_error("form() overflow");
            }
        return std::string(buf);
    }
    
    template <size_t MaxSizeHint, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
    inline std::string form( const char *fmt,  Arg1 const & arg1, Arg2 const & arg2, Arg3 const & arg3, Arg4 const & arg4, Arg5 const & arg5 )
    {
        char buf[MaxSizeHint];
#ifdef _MSC_VER
        if( _snprintf_s(buf,sizeof(buf)-1,sizeof(buf)-1, fmt, arg1, arg2, arg3, arg4, arg5 ) > static_cast<ssize_t>(sizeof(buf)-1) )
#else
            if( snprintf(buf,sizeof(buf)-1, fmt, arg1, arg2, arg3, arg4, arg5 ) > static_cast<ssize_t>(sizeof(buf)-1) )
#endif
            {
                throw std::runtime_error("form() overflow");
            }
        return std::string(buf);
    }
    
    template <size_t MaxSizeHint, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
    inline std::string form( const char *fmt,  Arg1 const & arg1, Arg2 const & arg2, Arg3 const & arg3, Arg4 const & arg4, Arg5 const & arg5, Arg6 const & arg6 )
    {
        char buf[MaxSizeHint];
#ifdef _MSC_VER
        if( _snprintf_s(buf,sizeof(buf)-1,sizeof(buf)-1, fmt, arg1, arg2, arg3, arg4, arg5, arg6 ) > static_cast<ssize_t>(sizeof(buf)-1) )
#else
            if( snprintf(buf,sizeof(buf)-1, fmt, arg1, arg2, arg3, arg4, arg5, arg6 ) > static_cast<ssize_t>(sizeof(buf)-1) )
#endif
            {
                throw std::runtime_error("form() overflow");
            }
        return std::string(buf);
    }
    

}

#endif


