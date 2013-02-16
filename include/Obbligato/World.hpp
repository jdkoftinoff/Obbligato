#pragma once
#ifndef Obbligato_World_hpp
#define Obbligato_World_hpp

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

#include "Obbligato/Platform.hpp"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <map>
#include <utility>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <vector>
#include <exception>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <memory>
#include <iterator>
#include <climits>
#include <cfloat>
#include <complex>
#include <valarray>
#include <map>

#define OBBLIGATO_JOIN2_(a,b) a##b
#define OBBLIGATO_JOIN2(a,b) OBBLIGATO_JOIN2_(a,b)

#define OBBLIGATO_JOIN3_(a,b,c) a##b##c
#define OBBLIGATO_JOIN3(a,b,c) OBBLIGATO_JOIN3_(a,b,c)


namespace Obbligato
{
    using ::std::string;

    namespace Config
    {
        class OptionGroups;
    }
}


#endif


