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
#include "Obbligato/Config_OptionGroupsMacOSX.hpp"
#include <CoreFoundation/CoreFoundation.h>

namespace Obbligato
{
#if defined( __APPLE__ )
namespace Config
{

void OptionGroupsMacOSX::parseApplePrefs( string const &application_id )
{
    OptionMap &m = getOptionMap();

    for ( OptionMap::iterator i = m.begin(); i != m.end(); ++i )
    {
        string const &key = i->first;
        OptionBase *option = i->second;

        CFStringRef cfapplication_id;

        if ( application_id.length() == 0 )
        {
            cfapplication_id = kCFPreferencesCurrentApplication;
        }
        else
        {
            cfapplication_id
                = CFStringCreateWithCString( kCFAllocatorDefault,
                                             application_id.c_str(),
                                             kCFStringEncodingUTF8 );
        }

        CFStringRef cfkey = CFStringCreateWithCString(
            kCFAllocatorDefault, key.c_str(), kCFStringEncodingUTF8 );

        CFPropertyListRef cfproplist
            = CFPreferencesCopyAppValue( cfkey, cfapplication_id );

        if ( cfproplist )
        {
            char buf[4096];
            CFStringRef cfproplist_as_cfstring
                = CFStringRef( cfproplist );
            if ( CFStringGetCString( cfproplist_as_cfstring,
                                     buf,
                                     sizeof( buf ),
                                     kCFStringEncodingUTF8 ) )
            {
                string value;
                value.assign( buf );
                option->parse( value );
            }
            CFRelease( cfproplist );
        }

        CFRelease( cfkey );
        if ( application_id.length() != 0 )
        {
            CFRelease( cfapplication_id );
        }
    }
}

void OptionGroupsMacOSX::saveApplePrefs( string const &application_id )
{
    OptionMap const &m = getOptionMap();

    for ( OptionMap::const_iterator i = m.cbegin(); i != m.cend(); ++i )
    {
        string const &key = i->first;
        OptionBase const *option = i->second;

        string value = option->stringValue();

        CFStringRef cfapplication_id;

        if ( application_id.length() == 0 )
        {
            cfapplication_id = kCFPreferencesCurrentApplication;
        }
        else
        {
            cfapplication_id
                = CFStringCreateWithCString( kCFAllocatorDefault,
                                             application_id.c_str(),
                                             kCFStringEncodingUTF8 );
        }

        CFStringRef cfkey = CFStringCreateWithCString(
            kCFAllocatorDefault, key.c_str(), kCFStringEncodingUTF8 );
        CFStringRef cfvalue = CFStringCreateWithCString(
            kCFAllocatorDefault, value.c_str(), kCFStringEncodingUTF8 );

        CFPreferencesSetAppValue( cfkey, cfvalue, cfapplication_id );

        CFRelease( cfvalue );
        CFRelease( cfkey );
        if ( application_id.length() != 0 )
        {
            CFRelease( cfapplication_id );
        }
    }
}
}
#else
const char *config_optiongroupsmacosx_file = __FILE__;
#endif
}
