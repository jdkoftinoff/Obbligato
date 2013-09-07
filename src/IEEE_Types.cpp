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
#include "Obbligato/IEEE_Types.hpp"

namespace Obbligato {
namespace IEEE {

bool parse_hexdigit(uint8_t &val, char c) {
    bool r = false;
    if (c >= '0' && c <= '9') {
        val = c - '0';
        r = true;
    } else {
        c = c & 0xdf;
        if (c >= 'A' && c <= 'F') {
            val = c - 'A' + 10;
            r = true;
        }
    }
    return r;
}

bool parse_hex(Octet &val, char c1, char c2) {
    bool r = true;
    uint8_t high = 0;
    uint8_t low = 0;
    r &= parse_hexdigit(high, c1);
    r &= parse_hexdigit(low, c2);
    if (r) {
        val = (high << 4) + low;
    }
    return r;
}

bool parse_hex(Octet &val, std::string const &s, std::string::size_type pos) {
    bool r = false;
    if (s.length() >= pos + 2) {
        r = parse_hex(val, s[pos], s[pos + 1]);
    }
    return r;
}

void put_hexdigit(std::string &result, Octet v) {
    char c = '0';
    v &= 0x0f;
    if (v < 0xa) {
        c = '0' + v;
    } else {
        c = 'A' - 0xa + v;
    }
    result.push_back(c);
}

void put_hex(std::string &result, Octet v) {
    put_hexdigit(result, v >> 4);
    put_hexdigit(result, v);
}

bool parse_hex(Doublet &val, std::string const &s, std::string::size_type pos) {
    bool r = false;
    if (s.length() >= pos + 4) {
        Octet high;
        if (parse_hex(high, s[pos], s[pos + 1])) {
            Octet low;
            if (parse_hex(low, s[pos + 2], s[pos + 3])) {
                val = (static_cast<Doublet>(high) << 8) + low;
                r = true;
            }
        }
    }
    return r;
}

void put_hex(std::string &result, Doublet v) {
    put_hex(result, static_cast<Octet>(v >> 8));
    put_hex(result, static_cast<Octet>(v));
}

bool parse_hex(Quadlet &val, std::string const &s, std::string::size_type pos) {
    bool r = false;
    if (s.length() >= pos + 8) {
        Doublet high;
        if (parse_hex(high, s, pos)) {
            pos += 4;
            Doublet low;
            if (parse_hex(low, s, pos)) {
                val = (static_cast<Quadlet>(high) << 16) + low;
                r = true;
            }
        }
    }
    return r;
}

void put_hex(std::string &result, Quadlet v) {
    put_hex(result, static_cast<Doublet>(v >> 16));
    put_hex(result, static_cast<Doublet>(v));
}

bool parse_hex(Octlet &val, std::string const &s, std::string::size_type pos) {
    bool r = false;
    if (s.length() >= pos + 16) {
        Quadlet high;
        if (parse_hex(high, s, pos)) {
            pos += 8;
            Quadlet low;
            if (parse_hex(low, s, pos)) {
                val = (static_cast<Octlet>(high) << 32) + low;
                r = true;
            }
        }
    }
    return r;
}

void put_hex(std::string &result, Octlet v) {
    put_hex(result, static_cast<Quadlet>(v >> 32));
    put_hex(result, static_cast<Quadlet>(v));
}

bool EUI48::from_string(const std::string &v) {
    // AA:BB:CC:DD:EE:FF or AA-BB-CC-DD-EE-FF
    char sep = '-';
    if (v.length() >= 17) {
        EUI48 result;

        // parse first octet
        if (!parse_hex(result.m_value[0], v, 0)) {
            return false;
        }

        // accept either : or - as separator
        if (v[2] == ':' || v[2] == '-') {
            sep = v[2];
        }

        // parse second octet
        if (!parse_hex(result.m_value[1], v, 3)) {
            return false;
        }

        // require separator
        if (v[5] != sep) {
            return false;
        }

        // parse third octet
        if (!parse_hex(result.m_value[2], v, 6)) {
            return false;
        }

        // require separator
        if (v[8] != sep) {
            return false;
        }

        // parse fourth octet
        if (!parse_hex(result.m_value[3], v, 9)) {
            return false;
        }

        // require separator
        if (v[11] != sep) {
            return false;
        }

        // parse fifth octet
        if (!parse_hex(result.m_value[4], v, 12)) {
            return false;
        }

        // require separator
        if (v[14] != sep) {
            return false;
        }

        // parse sixth octet
        if (!parse_hex(result.m_value[5], v, 15)) {
            return false;
        }

        assign(result);
        return true;
    }
    return false;
}

std::string EUI48::to_string(char sep) const {
    std::string r;
    put_hex(r, m_value[0]);
    r.push_back(sep);
    put_hex(r, m_value[1]);
    r.push_back(sep);
    put_hex(r, m_value[2]);
    r.push_back(sep);
    put_hex(r, m_value[3]);
    r.push_back(sep);
    put_hex(r, m_value[4]);
    r.push_back(sep);
    put_hex(r, m_value[5]);
    return r;
}

bool MAC48::from_string(const std::string &v) {
    // AA:BB:CC:DD:EE:FF or AA-BB-CC-DD-EE-FF
    char sep = '-';
    if (v.length() >= 17) {
        MAC48 result;

        // parse first octet
        if (!parse_hex(result.m_value[0], v, 0)) {
            return false;
        }

        // accept either : or - as separator
        if (v[2] == ':' || v[2] == '-') {
            sep = v[2];
        }

        // parse second octet
        if (!parse_hex(result.m_value[1], v, 3)) {
            return false;
        }

        // require separator
        if (v[5] != sep) {
            return false;
        }

        // parse third octet
        if (!parse_hex(result.m_value[2], v, 6)) {
            return false;
        }

        // require separator
        if (v[8] != sep) {
            return false;
        }

        // parse fourth octet
        if (!parse_hex(result.m_value[3], v, 9)) {
            return false;
        }

        // require separator
        if (v[11] != sep) {
            return false;
        }

        // parse fifth octet
        if (!parse_hex(result.m_value[4], v, 12)) {
            return false;
        }

        // require separator
        if (v[14] != sep) {
            return false;
        }

        // parse sixth octet
        if (!parse_hex(result.m_value[5], v, 15)) {
            return false;
        }

        assign(result);
        return true;
    }
    return false;
}

std::string MAC48::to_string(char sep) const {
    std::string r;
    put_hex(r, m_value[0]);
    r.push_back(sep);
    put_hex(r, m_value[1]);
    r.push_back(sep);
    put_hex(r, m_value[2]);
    r.push_back(sep);
    put_hex(r, m_value[3]);
    r.push_back(sep);
    put_hex(r, m_value[4]);
    r.push_back(sep);
    put_hex(r, m_value[5]);
    return r;
}

bool EUI64::from_string(const std::string &v) {
    // AA:BB:CC:DD:EE:FF:GG:HH or AA-BB-CC-DD-EE-FF-GG-HH
    char sep = '-';
    if (v.length() >= 23) {
        EUI64 result;

        // parse first octet
        if (!parse_hex(result.m_value[0], v, 0)) {
            return false;
        }

        // accept either : or - as separator
        if (v[2] == ':' || v[2] == '-') {
            sep = v[2];
        }

        // parse second octet
        if (!parse_hex(result.m_value[1], v, 3)) {
            return false;
        }

        // require separator
        if (v[5] != sep) {
            return false;
        }

        // parse third octet
        if (!parse_hex(result.m_value[2], v, 6)) {
            return false;
        }

        // require separator
        if (v[8] != sep) {
            return false;
        }

        // parse fourth octet
        if (!parse_hex(result.m_value[3], v, 9)) {
            return false;
        }

        // require separator
        if (v[11] != sep) {
            return false;
        }

        // parse fifth octet
        if (!parse_hex(result.m_value[4], v, 12)) {
            return false;
        }

        // require separator
        if (v[14] != sep) {
            return false;
        }

        // parse sixth octet
        if (!parse_hex(result.m_value[5], v, 15)) {
            return false;
        }

        // require separator
        if (v[17] != sep) {
            return false;
        }

        // parse seventh octet
        if (!parse_hex(result.m_value[6], v, 18)) {
            return false;
        }

        // require separator
        if (v[20] != sep) {
            return false;
        }

        // parse seventh octet
        if (!parse_hex(result.m_value[7], v, 21)) {
            return false;
        }

        assign(result);
        return true;
    }
    return false;
}

std::string EUI64::to_string(char sep) const {
    std::string r;
    put_hex(r, m_value[0]);
    r.push_back(sep);
    put_hex(r, m_value[1]);
    r.push_back(sep);
    put_hex(r, m_value[2]);
    r.push_back(sep);
    put_hex(r, m_value[3]);
    r.push_back(sep);
    put_hex(r, m_value[4]);
    r.push_back(sep);
    put_hex(r, m_value[5]);
    r.push_back(sep);
    put_hex(r, m_value[6]);
    r.push_back(sep);
    put_hex(r, m_value[7]);
    return r;
}
}
}
