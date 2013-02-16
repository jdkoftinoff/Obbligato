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
#include "Obbligato/Net.hpp"

#if defined(WIN32) && OB_ENABLE_PCAP
// Windows specific includes for winpcap and required libraries
# include <pcap.h>
# include <iphlpapi.h>
# pragma comment(lib, "IPHLPAPI.lib")
# pragma comment(lib, "ws2_32.lib" )

namespace Obbligato
{
}

#endif

#ifndef WIN32

namespace Obbligato
{
	namespace Net
	{
		volatile int signals_sigterm_seen = 0;
		volatile int signals_sigint_seen = 0;
		volatile int signals_sighup_seen = 0;

		/// Handle SIGTERM
		static void signals_handle_sigterm( int s )
		{
			(void)s;
			signals_sigterm_seen = 1;
		}

		/// Handle SIGINT
		static void signals_handle_sigint( int s )
		{
			(void)s;
			signals_sigint_seen = 1;
		}

		/// Handle SIGHUP
		static void signals_handle_sighup( int s )
		{
			(void)s;
			signals_sighup_seen = 1;
		}


		/// Setup signal handlers
		void signals_init()
		{
			static bool initted=false;

			if( !initted )
			{
				initted=true;

				struct sigaction act;
				act.sa_handler=SIG_IGN;
				sigemptyset(&act.sa_mask);
				act.sa_flags=0;
				sigaction(SIGPIPE, &act, NULL);

				act.sa_handler=signals_handle_sigterm;
				sigemptyset(&act.sa_mask);
				act.sa_flags=0;
				if( sigaction(SIGTERM, &act, NULL) !=0 )
				{
					abort();
				}

				act.sa_handler=signals_handle_sigint;
				sigemptyset(&act.sa_mask);
				act.sa_flags=0;
				if( sigaction(SIGINT, &act, NULL) )
				{
					abort();
				}

				act.sa_handler=signals_handle_sighup;
				sigemptyset(&act.sa_mask);
				act.sa_flags=0;
				if( sigaction(SIGHUP, &act, NULL) )
				{
					abort();
				}
			}
		}

		bool initialize_sockets ()
		{
            static bool initted=false;

            if( !initted )
            {
                initted=true;

                signals_init();
            }
			return true;
		}
	}
}
#endif



#ifdef WIN32

namespace Obbligato
{
	namespace Net
	{
		/// Initialize WS2_32 to version 2.2
		bool initialize_sockets ()
		{
			static bool initted=false;

			if( !initted )
			{
				initted=true;

				WSADATA wsaData;
				WORD version;
				int error;
				version = MAKEWORD ( 2, 2 );
				error = WSAStartup ( version, &wsaData );
				if ( error != 0 )
				{
					return false;
				}
				if ( version != wsaData.wVersion )
				{
					return false;
				}
			}
			return true;
		}
	}
}

#endif

