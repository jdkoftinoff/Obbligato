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
#include "Obbligato/DSP_Oscillator.hpp"

namespace Obbligato {
namespace DSP {

double oscillator_octave_multiplier_table[8] = {
    1.0 / 8.0, 1.0 / 4.0, 1.0 / 2.0, 1.0, 2.0, 4.0, 8.0};

float oscillator_octave_multiplier_table_f[8] = {
    1.0f / 8.0f, 1.0f / 4.0f, 1.0f / 2.0f, 1.0f, 2.0f, 4.0f, 8.0f };

double oscillator_note_frequencies_a440[12] = {
    440.00, // A
    466.16, // A#
    493.92, // B
    523.28, // C
    554.40, // C#
    587.36, // D
    622.24, // D#
    659.28, // E
    698.48, // F
    740.00, // F#
    784.00, // G
    830.64  // G#
};

float oscillator_note_frequencies_a440_f[12] = { 
    440.00f, // A
    466.16f, // A#
    493.92f, // B
    523.28f, // C
    554.40f, // C#
    587.36f, // D
    622.24f, // D#
    659.28f, // E
    698.48f, // F
    740.00f, // F#
    784.00f, // G
    830.64f  // G#
};

}
}
