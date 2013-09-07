#pragma once
#ifndef Obbligato_Constants_hpp
#define Obbligato_Constants_hpp

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

/**
 * \defgroup constants useful floating point constants
 */

/*@{*/

#ifndef OBBLIGATO_PI
/// Pi as double
#define OBBLIGATO_PI (3.14159265358979323846264338327950288419716939937510)
#endif

#ifndef OBBLIGATO_PI_RECIP
/// 1/Pi as double
#define OBBLIGATO_PI_RECIP                                                     \
    (1.0 / 3.14159265358979323846264338327950288419716939937510)
#endif

#ifndef OBBLIGATO_PI_OVER_TWO
/// Pi/2 as double
#define OBBLIGATO_PI_OVER_TWO                                                  \
    (3.14159265358979323846264338327950288419716939937510 / 2.0)
#endif

#ifndef OBBLIGATO_TWO_PI
/// 2 * Pi as double
#define OBBLIGATO_TWO_PI                                                       \
    (2.0 * 3.14159265358979323846264338327950288419716939937510)
#endif

#ifndef OBBLIGATO_TWO_PI_RECIP
/// 1/ (2 * Pi) as double
#define OBBLIGATO_TWO_PI_RECIP                                                 \
    (1.0 / (2.0 * 3.14159265358979323846264338327950288419716939937510))
#endif

#ifndef OBBLIGATO_E
/// value of E
#define OBBLIGATO_E                                                            \
    (2.7182818284590452353602874713526624977572470936999595749669676277240766)
#endif

#ifndef OBBLIGATO_E_RECIP
/// value of 1/E
#define OBBLIGATO_E_RECIP                                                      \
    (1.0 /                                                                     \
     2.7182818284590452353602874713526624977572470936999595749669676277240766)
#endif

#ifndef OBBLIGATO_SQRT2
/// value of sqrt(2)
#define OBBLIGATO_SQRT2 (1.4142135623730950488016887242096980785696718753769480)
#endif

#ifndef OBBLIGATO_SQRT2_RECIP
/// value of 1/sqrt(2)
#define OBBLIGATO_SQRT2_RECIP                                                  \
    (1.0 / 1.4142135623730950488016887242096980785696718753769480)
#endif

#ifndef OBBLIGATO_LOG2_E
#define OBBLIGATO_LOG2_E (1.442695040888963407359924681001892137427)
#endif

#ifndef OBBLIGATO_LOG2_TEN
#define OBBLIGATO_LOG2_TEN (3.321928094887362347870319429489390175865)
#endif

#ifndef OBBLIGATO_LN2
/// value of natural log(2)
#define OBBLIGATO_LN2 (0.69314718055994529)
#endif

#ifndef OBBLIGATO_LN2_RECIP
/// value of reciprocal of natural log(2)
#define OBBLIGATO_LN2_RECIP (1.442695040888963407359924681001892137427)
#endif

#ifndef OBBLIGATO_LOG10_2
/// value of log10(2)
#define OBBLIGATO_LOG10_2 (0.301029995663981195213738894724493026768)
#endif

#ifndef OBBLIGATO_LOG10_2_RECIP
/// value of reciprocal log10(2)
#define OBBLIGATO_LOG10_2_RECIP (3.321928094887362347870319429489390175865)
#endif

#ifndef OBBLIGATO_LN10
/// value of natural log(10)
#define OBBLIGATO_LN10 (2.30258509299404568402)
#endif

#ifndef OBBLIGATO_LN10_RECIP
/// value of reciprocal of natural log(10)
#define OBBLIGATO_LN10_RECIP (1.0 / 2.30258509299404568402)
#endif

#ifndef OBBLIGATO_TEN
/// value of 10
#define OBBLIGATO_TEN (10.0)
#endif

#ifndef OBBLIGATO_TWENTY_RECIP
/// value of 1/20
#define OBBLIGATO_TWENTY_RECIP (1.0 / 20.0)
#endif

#ifndef OBBLIGATO_TWENTY_DIV_LN10
/// value of 20.0 / natural log (10)
#define OBBLIGATO_TWENTY_DIV_LN10 (20.0 / OBBLIGATO_LN10)
#endif

#ifndef OBBLIGATO_NEG_100_DB
/// value of -100 db
#define OBBLIGATO_NEG_100_DB (1e-5)
#endif

#ifndef OBBLIGATO_NEG_200_DB
/// value of -200 db
#define OBBLIGATO_NEG_200_DB (1e-10)
#endif

#ifndef OBBLIGATO_TWO_GIGI
/// value of 2^31
#define OBBLIGATO_TWO_GIGI (2.0 * 1024.0 * 1024.0 * 1024.0)
#endif

#ifndef OBBLIGATO_TWO_GIGI_RECIP
/// value of 1/2^31
#define OBBLIGATO_TWO_GIGI_RECIP (1.0 / (2.0 * 1024.0 * 1024.0 * 1024.0))
#endif

/*@}*/

namespace Obbligato {}

#endif
