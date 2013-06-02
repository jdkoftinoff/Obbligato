#pragma once
#ifndef Obbligato_IOStream_InputState_hpp
#define Obbligato_IOStream_InputState_hpp

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
#include "Obbligato/Noncopyable.hpp"

namespace Obbligato { namespace IOStream {

/// input stream flag state saver
template < typename Ch=char, class Tr=std::char_traits<Ch> >
class BasicIStreamStateSave : Noncopyable
{
    ::std::basic_istream<Ch,Tr> &m_s;
    ::std::ios m_fmt;

public:
    BasicIStreamStateSave( ::std::basic_istream<Ch,Tr> &s_ )
        :
          m_s( s_ ),
          m_fmt( NULL )
    {
        ::std::ios clear_fmt(NULL);
        m_s.copyfmt(clear_fmt);
    }

    ~BasicIStreamStateSave()
    {
        m_s.copyfmt(m_fmt);
    }
};

typedef BasicIStreamStateSave<char,std::char_traits<char> > IStreamStateSave;

template <typename Ch, class Tr >
inline BasicIStreamStateSave< Ch, Tr > save_state( ::std::basic_istream<Ch,Tr> &s )
{
    return BasicIStreamStateSave< Ch, Tr>(s);
}


}}

#endif


