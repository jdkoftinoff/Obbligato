#pragma once
#ifndef Obbligato_Traits_hpp
#define Obbligato_Traits_hpp

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

namespace Obbligato
{
    namespace Traits
    {
        /// A trait subclass inherits from TrueTrait<T> if type T has a trait
        template <typename T>
        struct TrueTrait
        {
            typedef T true_type;
            typedef void false_type;
        };

        /// A trait subclass inherits from FalseTrait<T> if type T does not have a trait
        template <typename T>
        struct FalseTrait
        {
            typedef void true_type;
            typedef T false_type;
        };

        /// The IsMathVector<T> trait inherits from TrueTrait if type T represents multiple values that can have arithmetic applied.
        template <typename T>
        struct IsMathVector : public FalseTrait< T >
        {
        };

        /// A std::valarray is a MathVector.
        template <typename T>
        struct IsMathVector< std::valarray<T> > : public TrueTrait< std::valarray<T> >
        {
        };

        /** \addtogroup utility_traits Traits class to find appropriate storage type from size field */
        /* @{ */

        template <size_t SZ>
        struct SizeToStorage
        {
            typedef void storage_type;
        };

        template <>
        struct SizeToStorage<1>
        {
            typedef IEEE::Octet storage_type;
        };

        template <>
        struct SizeToStorage<2>
        {
            typedef IEEE::Doublet storage_type;
        };

        template <>
        struct SizeToStorage<4>
        {
            typedef IEEE::Quadlet storage_type;
        };

        template <>
        struct SizeToStorage<8>
        {
            typedef IEEE::Octlet storage_type;
        };

        /**@}*/

        /** \addtogroup storage_traits storage_traits to provide bit information for fundamental types
         *
         * @{
         */

        template <typename StorageType>
        struct Storage
        {
        };


        template <>
        struct Storage<IEEE::Octet>
        {
            typedef IEEE::Octet type;
            static const char * name() { return "Octet"; }
            static const size_t bits=8;
            static const type msb_value=0x80U;
            static const type allbits=~(type(0));
            static const size_t size=1;
            static type value_for_bit(size_t bit) { return msb_value >> bit; }
        };

        template <>
        struct Storage<IEEE::Doublet>
        {
            typedef IEEE::Doublet type;
            static const char * name() { return "Doublet"; }
            static const size_t bits=16;
            static const type msb_value=0x8000U;
            static const type allbits=~(type(0));
            static const size_t size=2;
            static type value_for_bit(size_t bit) { return msb_value >> bit; }
        };


        template <>
        struct Storage<IEEE::Quadlet>
        {
            typedef IEEE::Quadlet type;
            static const char * name() { return "Quadlet"; }
            static const size_t bits=32;
            static const type msb_value=0x80000000UL;
            static const type allbits=~(type(0));
            static const size_t size=4;
            static type value_for_bit(size_t bit) { return msb_value >> bit; }
        };


        template <>
        struct Storage<IEEE::Octlet>
        {
            typedef IEEE::Octlet type;
            static const char * name() { return "Octlet"; }
            static const size_t bits=64;
            static const type msb_value=0x8000000000000000ULL;
            static const type allbits=~(type(0));
            static const size_t size=8;
            static type value_for_bit(size_t bit) { return msb_value >> bit; }
        };


        /* @} */
    }
}

#endif


