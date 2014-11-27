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
#include "Obbligato/Config_RegistryWin32.hpp"

#if defined( _WIN32 )

namespace Obbligato
{
namespace Config
{

Win32Registry::Win32Registry( bool writable,
                              const char *machine_keyname,
                              const char *user_keyname,
                              HKEY user_key_base )
    : m_machine_key( 0 )
    , m_user_key( 0 )
    , m_curuser_key( 0 )
    , m_user_key_base( user_key_base )
{
    if ( machine_keyname )
    {
        RegOpenKeyExA( HKEY_LOCAL_MACHINE,
                       machine_keyname,
                       0,
                       writable ? KEY_WRITE : KEY_READ,
                       &m_machine_key );
        RegOpenKeyExA( user_key_base,
                       user_keyname,
                       0,
                       writable ? KEY_WRITE : KEY_READ,
                       &m_user_key );
    }
    else
    {
        int e;
        if ( ( e = RegOpenKeyExA( HKEY_CURRENT_USER,
                                  user_keyname,
                                  0,
                                  writable ? KEY_WRITE : KEY_READ,
                                  &m_machine_key ) ) != 0 )
        {
            LPVOID lpMsgBuf;
            FormatMessageA( FORMAT_MESSAGE_ALLOCATE_BUFFER
                            | FORMAT_MESSAGE_FROM_SYSTEM
                            | FORMAT_MESSAGE_IGNORE_INSERTS,
                            NULL,
                            e,
                            MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
                            (LPSTR)&lpMsgBuf,
                            0,
                            NULL );
            // MessageBoxA( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK );
            LocalFree( lpMsgBuf );
        }
    }
}

Win32Registry::~Win32Registry()
{
    RegCloseKey( m_machine_key );
    RegCloseKey( m_user_key );
}

void Win32Registry::selectUser( const char *user_keyname )
{
    RegCloseKey( m_user_key );
    RegOpenKeyA( m_user_key_base, user_keyname, &m_user_key );
}

long Win32Registry::getInt( const char *section,
                            const char *field,
                            const char *descriptive_field,
                            long default_val )
{
    DWORD val = default_val;
    DWORD size = sizeof( val );
    DWORD type;

    {
        // try read user setting

        if ( RegQueryValueExA(
                 m_user_key, field, 0, &type, (LPBYTE)&val, &size )
             != ERROR_SUCCESS || type != REG_DWORD )
        {
            // didnt work... try read machine key

            if ( RegQueryValueExA( m_machine_key,
                                   field,
                                   0,
                                   &type,
                                   (LPBYTE)&val,
                                   &size ) != ERROR_SUCCESS
                 || type != REG_DWORD )
            {
                // still didnt work. Use default
                // and write default to machine key

                RegSetValueExA( m_machine_key,
                                field,
                                0,
                                REG_DWORD,
                                (LPBYTE)&val,
                                sizeof( val ) );
            }
        }
    }

    return val;
}

bool Win32Registry::getString( const char *section,
                               const char *field,
                               const char *descriptive_field,
                               const char *default_val,
                               char *buf,
                               int buf_size )
{
    DWORD size = buf_size;
    DWORD type;

    {
        // read user key
        if ( RegQueryValueExA(
                 m_user_key, field, 0, &type, (LPBYTE)buf, &size )
             != ERROR_SUCCESS || type != REG_SZ )
        {
            // didnt work. Try read machine key
            if ( RegQueryValueExA( m_machine_key,
                                   field,
                                   0,
                                   &type,
                                   (LPBYTE)buf,
                                   &size ) != ERROR_SUCCESS
                 || type != REG_SZ )
            {
                // still didnt work. write default to machine key
                // and return default.

                strncpy_s( buf, buf_size, default_val, buf_size - 1 );
                buf[buf_size - 1] = 0;

                RegSetValueExA( m_machine_key,
                                field,
                                0,
                                REG_SZ,
                                (LPBYTE)buf,
                                strlen( buf ) + 1 );
                return false;
            }
        }
    }

    return true;
}

bool Win32Registry::writeInt( Win32Registry::Area area,
                              const char *field,
                              const char *descriptive_field,
                              long val )
{
    bool r = false;

    switch ( area )
    {
    case REG_MACHINE:
        r = writeMachineInt( 0, field, descriptive_field, val );
        break;
    case REG_USER:
        r = writeUserInt( 0, field, descriptive_field, val );
        break;
    }
    return r;
}

bool Win32Registry::writeString( Win32Registry::Area area,
                                 const char *field,
                                 const char *descriptive_field,
                                 const char *val )
{
    bool r = false;

    switch ( area )
    {
    case REG_MACHINE:
        r = writeMachineString( 0, field, descriptive_field, val );
        break;
    case REG_USER:
        r = writeUserString( 0, field, descriptive_field, val );
        break;
    }
    return r;
}

bool Win32Registry::writeMachineInt( const char *section,
                                     const char *field,
                                     const char *descriptive_field,
                                     long val )
{
    RegSetValueExA( m_machine_key,
                    field,
                    0,
                    REG_DWORD,
                    (LPBYTE)&val,
                    sizeof( val ) );

    return true;
}

bool Win32Registry::writeMachineString( const char *section,
                                        const char *field,
                                        const char *descriptive_field,
                                        const char *val )
{
    RegSetValueExA( m_machine_key,
                    field,
                    0,
                    REG_SZ,
                    (LPBYTE)val,
                    strlen( val ) + 1 );

    return true;
}

bool Win32Registry::writeUserInt( const char *section,
                                  const char *field,
                                  const char *descriptive_field,
                                  long val )
{
    RegSetValueExA(
        m_user_key, field, 0, REG_DWORD, (LPBYTE)&val, sizeof( val ) );

    return true;
}

bool Win32Registry::writeUserString( const char *section,
                                     const char *field,
                                     const char *descriptive_field,
                                     const char *val )
{
    RegSetValueExA(
        m_user_key, field, 0, REG_SZ, (LPBYTE)val, strlen( val ) + 1 );

    return true;
}

bool Win32Registry::deleteValue( const char *field,
                                 const char *descriptive_field )
{
    RegDeleteValueA( m_machine_key, field );
    return true;
}
}
}

#else

namespace Obbligato
{

const char *config_registrywin32_file = __FILE__;
}
#endif
