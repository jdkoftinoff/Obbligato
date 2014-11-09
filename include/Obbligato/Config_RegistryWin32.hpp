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
#include "Obbligato/Config.hpp"

#if defined( _WIN32 )

namespace Obbligato
{
namespace Config
{

class Win32Registry
{
  public:
    enum Area
    {
        REG_MACHINE = 0,
        REG_USER,
    };

    Win32Registry( bool writable,
                   const char *machine_keyname,
                   const char *user_keyname = "",
                   HKEY user_key_base = HKEY_CURRENT_USER );

    ~Win32Registry();

    void selectUser( const char *user_keyname );

    long getInt( const char *section,
                 const char *field,
                 const char *descriptive_field,
                 long default_val );

    bool getString( const char *section,
                    const char *field,
                    const char *descriptive_field,
                    const char *default_val,
                    char *buf,
                    int buf_size );

    bool writeInt( Area area,
                   const char *field,
                   const char *descriptive_field,
                   long val );

    bool writeString( Area area,
                      const char *field,
                      const char *descriptive_field,
                      const char *val );

    bool writeMachineInt( const char *section,
                          const char *field,
                          const char *descriptive_field,
                          long val );

    bool writeMachineString( const char *section,
                             const char *field,
                             const char *descriptive_field,
                             const char *val );

    bool writeUserInt( const char *section,
                       const char *field,
                       const char *descriptive_field,
                       long val );

    bool writeUserString( const char *section,
                          const char *field,
                          const char *descriptive_field,
                          const char *val );

    bool deleteValue( const char *field,
                      const char *descriptive_field = 0 );

  private:
    HKEY m_machine_key;
    HKEY m_user_key;
    HKEY m_curuser_key;
    HKEY m_user_key_base;
};
}
}

#endif
