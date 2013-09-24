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
#include "Obbligato/RangeCheck.hpp"

namespace Obbligato {
namespace Endian {

/** \addtogroup endian Endian helpers / data access */
/*@{*/

static inline uint16_t reverse(uint16_t const *vin) {
    uint8_t const *p = (uint8_t const *)(vin);
    return ((uint16_t)(p[1]) << 0) + ((uint16_t)(p[0]) << 8);
}

static inline uint32_t reverse(uint32_t const *vin) {
    uint8_t const *p = (uint8_t const *)vin;
    return ((uint32_t)(p[3]) << 0) + ((uint32_t)(p[2]) << 8) +
           ((uint32_t)(p[1]) << 16) + ((uint32_t)(p[0]) << 24);
}

static inline uint64_t reverse(uint64_t const *vin) {
    uint8_t const *p = (uint8_t const *)vin;
    return ((uint64_t)(p[7]) << 0) + ((uint64_t)(p[6]) << 8) +
           ((uint64_t)(p[5]) << 16) + ((uint64_t)(p[4]) << 24) +
           ((uint64_t)(p[3]) << 32) + ((uint64_t)(p[2]) << 40) +
           ((uint64_t)(p[1]) << 48) + ((uint64_t)(p[0]) << 56);
}

static inline void big_read(uint8_t *host_value, void const *base,
                            ssize_t pos) {
    // fixme
    uint8_t const *b = (uint8_t const *)base;

    *host_value = b[pos];
}

static inline uint8_t big_get_uint8(void const *base, ssize_t pos) {
    uint8_t const *b = (uint8_t const *)base;
    return b[pos];
}

static inline ssize_t big_write(uint8_t v, void *base, ssize_t pos,
                                ssize_t len) {
    ssize_t r = buffer_range_check(pos, len, 1);
    if (r >= 0) {
        uint8_t const *host_value = (uint8_t const *)&v;
        uint8_t *b = ((uint8_t *)base) + pos;

        b[0] = *host_value;
    }
    return r;
}

static inline void big_set(uint8_t v, void *base, ssize_t pos) {
    uint8_t *b = (uint8_t *)base;
    b[pos] = v;
}

static inline void big_read(uint16_t *host_value, void const *base,
                            ssize_t pos) {
    uint8_t const *b = ((uint8_t const *)base) + pos;
    *host_value = (((uint16_t)b[0]) << 8) + b[1];
}

static inline uint16_t big_get_uint16(void const *base, ssize_t pos) {
    uint8_t const *b = ((uint8_t const *)base) + pos;
    return (((uint16_t)b[0]) << 8) + b[1];
}

static inline ssize_t big_write(uint16_t v, void *base, ssize_t pos,
                                ssize_t len) {
    ssize_t r = buffer_range_check(pos, len, 2);
    if (r >= 0) {
        uint16_t const *host_value = (uint16_t const *)&v;
        uint8_t *b = ((uint8_t *)base) + pos;

        b[0] = (uint8_t)((*host_value) >> 8) & 0xff;
        b[1] = (uint8_t)((*host_value) >> 0) & 0xff;
    }
    return r;
}

static inline void big_set(uint16_t v, void *base, ssize_t pos) {
    uint8_t *b = ((uint8_t *)base) + pos;
    b[0] = (uint8_t)((v) >> 8) & 0xff;
    b[1] = (uint8_t)((v) >> 0) & 0xff;
}

static inline void big_read(uint32_t *host_value, void const *base,
                            ssize_t pos) {
    uint8_t const *b = ((uint8_t const *)base) + pos;
    *host_value = (((uint32_t)b[0]) << 24) + (((uint32_t)b[1]) << 16) +
                  (((uint32_t)b[2]) << 8) + b[3];
}

static inline uint32_t big_get_uint32(void const *base, ssize_t pos) {
    uint8_t const *b = ((uint8_t const *)base) + pos;
    return (((uint32_t)b[0]) << 24) + (((uint32_t)b[1]) << 16) +
           (((uint32_t)b[2]) << 8) + b[3];
}

static inline ssize_t big_write(uint32_t v, void *base, ssize_t pos,
                                ssize_t len) {
    ssize_t r = buffer_range_check(pos, len, 2);
    if (r >= 0) {
        uint32_t const *host_value = (uint32_t const *)&v;
        uint8_t *b = ((uint8_t *)base) + pos;

        b[0] = (uint8_t)((*host_value) >> 24) & 0xff;
        b[1] = (uint8_t)((*host_value) >> 16) & 0xff;
        b[2] = (uint8_t)((*host_value) >> 8) & 0xff;
        b[3] = (uint8_t)((*host_value) >> 0) & 0xff;
    }
    return r;
}

static inline void big_set(uint32_t v, void *base, ssize_t pos) {
    uint8_t *b = ((uint8_t *)base) + pos;
    b[0] = (uint8_t)((v) >> 24) & 0xff;
    b[1] = (uint8_t)((v) >> 16) & 0xff;
    b[2] = (uint8_t)((v) >> 8) & 0xff;
    b[3] = (uint8_t)((v) >> 0) & 0xff;
}

static inline ssize_t big_read(uint64_t *host_value, void const *base,
                               ssize_t pos, ssize_t len) {
    ssize_t r = buffer_range_check(pos, len, 8);
    if (r >= 0) {
        uint8_t const *b = ((uint8_t const *)base) + pos;
        *host_value =
            (((uint64_t)b[0]) << (8 * 7)) + (((uint64_t)b[1]) << (8 * 6)) +
            (((uint64_t)b[2]) << (8 * 5)) + (((uint64_t)b[3]) << (8 * 4)) +
            (((uint64_t)b[4]) << (8 * 3)) + (((uint64_t)b[5]) << (8 * 2)) +
            (((uint64_t)b[6]) << (8 * 1)) + b[7];
    }
    return r;
}

static inline uint64_t big_get_uint64(void const *base, ssize_t pos) {
    uint8_t const *b = ((uint8_t const *)base) + pos;

    return (((uint64_t)b[0]) << (8 * 7)) + (((uint64_t)b[1]) << (8 * 6)) +
           (((uint64_t)b[2]) << (8 * 5)) + (((uint64_t)b[3]) << (8 * 4)) +
           (((uint64_t)b[4]) << (8 * 3)) + (((uint64_t)b[5]) << (8 * 2)) +
           (((uint64_t)b[6]) << (8 * 1)) + b[7];
}

static inline ssize_t big_write(uint64_t v, void *base, ssize_t pos,
                                ssize_t len) {
    ssize_t r = buffer_range_check(pos, len, 2);
    if (r >= 0) {
        uint64_t const *host_value = (uint64_t const *)&v;
        uint8_t *b = ((uint8_t *)base) + pos;

        b[0] = (uint8_t)((*host_value) >> (8 * 7)) & 0xff;
        b[1] = (uint8_t)((*host_value) >> (8 * 6)) & 0xff;
        b[2] = (uint8_t)((*host_value) >> (8 * 5)) & 0xff;
        b[3] = (uint8_t)((*host_value) >> (8 * 4)) & 0xff;
        b[4] = (uint8_t)((*host_value) >> (8 * 3)) & 0xff;
        b[5] = (uint8_t)((*host_value) >> (8 * 2)) & 0xff;
        b[6] = (uint8_t)((*host_value) >> (8 * 1)) & 0xff;
        b[7] = (uint8_t)((*host_value)) & 0xff;
    }
    return r;
}

static inline void big_set(uint64_t v, void *base, ssize_t pos) {
    uint8_t *b = ((uint8_t *)base) + pos;
    b[0] = (uint8_t)((v) >> (8 * 7)) & 0xff;
    b[1] = (uint8_t)((v) >> (8 * 6)) & 0xff;
    b[2] = (uint8_t)((v) >> (8 * 5)) & 0xff;
    b[3] = (uint8_t)((v) >> (8 * 4)) & 0xff;
    b[4] = (uint8_t)((v) >> (8 * 3)) & 0xff;
    b[5] = (uint8_t)((v) >> (8 * 2)) & 0xff;
    b[6] = (uint8_t)((v) >> (8 * 1)) & 0xff;
    b[7] = (uint8_t)((v)) & 0xff;
}

static inline ssize_t big_read(float *host_value, void const *base, ssize_t pos,
                               size_t len) {
    ssize_t r = buffer_range_check(pos, len, sizeof(*host_value));

    if (r >= 0) {
        uint32_t host_value_int = big_get_uint32(base, pos);
        float *hp = (float *)&host_value_int;
        *host_value = *hp;
    }
    return r;
}

static inline float big_get_float(void const *base, ssize_t pos) {
    uint32_t host_value_int = big_get_uint32(base, pos);
    float *hp = (float *)&host_value_int;
    return *hp;
}

static inline ssize_t big_write(float v, void *base, ssize_t pos, size_t len) {
    ssize_t r = buffer_range_check(pos, len, 4);

    if (r >= 0) {
        float *fp = &v;
        uint32_t *ip = (uint32_t *)fp;
        uint32_t host_value_int = *ip;
        big_set(host_value_int, base, pos);
    }
    return r;
}

static inline void big_set(float v, void *base, ssize_t pos) {
    uint32_t *host_value_int = ((uint32_t *)&v);
    big_set(*host_value_int, base, pos);
}

static inline ssize_t big_read(double *host_value, void const *base,
                               ssize_t pos, size_t len) {
    ssize_t r = buffer_range_check(pos, len, sizeof(*host_value));

    if (r >= 0) {
        uint64_t host_value_int = big_get_uint64(base, pos);
        double *hp = (double *)&host_value_int;
        *host_value = *hp;
    }
    return r;
}

static inline double big_get(void const *base, ssize_t pos) {
    uint64_t host_value_int = big_get_uint64(base, pos);
    double *hp = (double *)&host_value_int;
    return *hp;
}

static inline ssize_t big_write(double v, void *base, ssize_t pos, size_t len) {
    ssize_t r = buffer_range_check(pos, len, 8);

    if (r >= 0) {
        double *dp = &v;
        uint64_t *ip = (uint64_t *)dp;
        uint64_t host_value_int = *ip;
        big_set(host_value_int, base, pos);
    }
    return r;
}

static inline void big_set(double v, void *base, ssize_t pos) {
    uint64_t *host_value_int = ((uint64_t *)&v);
    big_set(*host_value_int, base, pos);
}

/*@}*/
}
}
