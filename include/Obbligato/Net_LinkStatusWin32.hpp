#pragma once
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

#if defined( _WIN32 ) && AE_ENABLE_PCAP == 1

#include <ObjBase.h>
#include <pcap.h>
#include <iphlpapi.h>

namespace Obbligato
{
namespace Net
{
/// \addtogroup linkstatus
///@{

/// Microsoft Windows specific link status checker.
class LinkStatusWin32 : public LinkStatusBase
{
  public:
    struct LocalInterfaces
    {
        std::string alias;
        std::string guid;
        bool connected;
        bool did_change;
    };

    typedef std::map<ULONG64, LocalInterfaces> Interfaces;

    LinkStatusWin32()
    {
        get_interfaces();
        NotifyIpInterfaceChange( AF_UNSPEC, interface_change_callback, this, true, &m_notification_handle );
    }

    ~LinkStatusWin32()
    {
        CancelMibChangeNotify2( m_notification_handle );
    }

    bool get_link_status( const char *eth )
    {
        std::string name( eth );
        for ( Interfaces::const_iterator it = m_last_state.begin(); it != m_last_state.end(); it++ )
        {
            if ( ( *it ).second.alias == name )
            {
                return ( *it ).second.connected;
            }
        }
        return false;
    }

    std::string get_guid_from_name( std::string &name ) const
    {
        for ( Interfaces::const_iterator it = m_last_state.begin(); it != m_last_state.end(); it++ )
        {
            if ( ( *it ).second.alias == name )
            {
                return ( *it ).second.guid;
            }
        }
        return std::string( "" );
    }

  private:
    void get_interfaces()
    {
        PMIB_IF_TABLE2 table;
        if ( GetIfTable2( &table ) == NO_ERROR )
        {
            for ( ULONG i = 0; i < table->NumEntries; ++i )
            {
                if ( table->Table[i].InterfaceAndOperStatusFlags.HardwareInterface )
                {
                    LocalInterfaces li;
                    OLECHAR *guid;
                    char temp[IF_MAX_STRING_SIZE + 1];
                    WideCharToMultiByte( CP_ACP, 0, table->Table[i].Alias, -1, temp, IF_MAX_STRING_SIZE, NULL, NULL );
                    li.alias = std::string( temp );

                    StringFromCLSID( table->Table[i].InterfaceGuid, &guid );
                    WideCharToMultiByte( CP_ACP, 0, guid, -1, temp, IF_MAX_STRING_SIZE, NULL, NULL );
                    li.guid = std::string( temp );
                    CoTaskMemFree( guid );
                    li.connected = ( table->Table[i].MediaConnectState == MediaConnectStateConnected );
                    m_last_state[table->Table[i].InterfaceLuid.Value] = li;
                }
            }
            FreeMibTable( table );
        }
    }

    static void WINAPI
        interface_change_callback( PVOID caller_context, PMIB_IPINTERFACE_ROW row, MIB_NOTIFICATION_TYPE notification_type )
    {
        LinkStatusWin32 *pthis = static_cast<LinkStatusWin32 *>( caller_context );
        if ( notification_type == MibParameterNotification )
        {
            LinkStatusWin32::Interfaces::iterator it = pthis->m_last_state.find( row->InterfaceLuid.Value );
            if ( it != pthis->m_last_state.end() )
            {
                LocalInterfaces li = ( *it ).second;
                MIB_IF_ROW2 if_row;
                SecureZeroMemory( ( PVOID ) & if_row, sizeof( MIB_IF_ROW2 ) );
                if_row.InterfaceLuid = row->InterfaceLuid;
                if ( GetIfEntry2( &if_row ) == NO_ERROR )
                {
                    if ( ( *it ).second.connected != ( if_row.MediaConnectState == MediaConnectStateConnected ) )
                        ( *it ).second.did_change = true;

                    ( *it ).second.connected = ( if_row.MediaConnectState == MediaConnectStateConnected );
                }
            }
        }
    }
    HANDLE m_notification_handle;
    Interfaces m_last_state;
};
typedef LinkStatusWin32 LinkStatusDefault;

///@}
}
}
#endif
