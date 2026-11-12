/**
 Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 SPDX-License-Identifier: MIT
 Copyright (c) 2026 Carsten Janssen

 Permission is hereby  granted, free of charge, to any  person obtaining a copy
 of this software and associated  documentation files (the "Software"), to deal
 in the Software  without restriction, including without  limitation the rights
 to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
 copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
 IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
 FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
 AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
 LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
**/

#ifndef BYTESWAP_H
#define BYTESWAP_H

/**
This header provides endianess and byteswap macros in a unified way for most
compilers and operating systems. The macro names are all uppercase to
hopefully not conflict with existing functions.
Only 16, 32 and 64 bit signed and unsigned integers are supported.
Middle Endian is not fully supported, only simple word order swapping is available.


Reverse byte order:

    uint16_t BSWAP16(uint16_t x);
    uint32_t BSWAP32(uint32_t x);
    uint64_t BSWAP64(uint64_t x);


Reverse word order (middle endian):

    uint32_t WSWAP32(uint32_t x);
    uint64_t WSWAP64(uint64_t x);


Convert values between host and big/little endian byte order:

    uint16_t HOST_TO_BE16(uint16_t host_16bits);
    uint16_t HOST_TO_LE16(uint16_t host_16bits);
    uint16_t BE16_TO_HOST(uint16_t big_endian_16bits);
    uint16_t LE16_TO_HOST(uint16_t little_endian_16bits);

    uint32_t HOST_TO_BE32(uint32_t host_32bits);
    uint32_t HOST_TO_LE32(uint32_t host_32bits);
    uint32_t BE32_TO_HOST(uint32_t big_endian_32bits);
    uint32_t LE32_TO_HOST(uint32_t little_endian_32bits);

    uint64_t HOST_TO_BE64(uint64_t host_64bits);
    uint64_t HOST_TO_LE64(uint64_t host_64bits);
    uint64_t BE64_TO_HOST(uint64_t big_endian_64bits);
    uint64_t LE64_TO_HOST(uint64_t little_endian_64bits);


Convert values between host and network byte order:

    uint16_t HTONS(uint16_t hostshort);
    uint32_t HTONL(uint32_t hostlong);
    uint64_t HTONLL(uint64_t hostlonglong);

    uint16_t NTOHS(uint16_t netshort);
    uint32_t NTOHL(uint32_t netlong);
    uint64_t NTOHLL(uint64_t netlonglong);


Generic macros for all signed and unsigned integers.
Minimum requirement is C11 or C++11.

    intX_t BSWAP(intX_t x);
    intX_t WSWAP(intX_t x);
    intX_t HOST_TO_BE(intX_t host_bits);
    intX_t HOST_TO_LE(intX_t host_bits);
    intX_t BE_TO_HOST(intX_t big_endian_bits);
    intX_t LE_TO_HOST(intX_t little_endian_bits);
    intX_t HTON(intX_t host_bits);
    intX_t NTOH(intX_t net_bits);


These macros are defined as symbolic values for the different
endian types. The predefined macro BSWAP_BYTE_ORDER may be set
to one of these.

    #define BSWAP_BIG_ENDIAN          1234
    #define BSWAP_LITTLE_ENDIAN       4321
    #define BSWAP_WORD_BIG_ENDIAN     2143 // Honeywell 316
    #define BSWAP_WORD_LITTLE_ENDIAN  3412 // PDP-11
    #define BSWAP_UNKNOWN_ENDIAN        -1
    #define BSWAP_H316_ENDIAN         BSWAP_WORD_BIG_ENDIAN
    #define BSWAP_PDP_ENDIAN          BSWAP_WORD_LITTLE_ENDIAN


The following function returns a numeric value for the host byte order.
The compiler may turn the return value into a compile-time constant
and optimize the function away.

    static inline int get_byte_order(void);


Configuration macros can be set too:

    #define GET_BYTE_ORDER_RUNTIME 1   // don't use predefined macros in get_byte_order()
    #define DISABLE_CPLUSPLUS 1        // don't use C++ features when compiling in C++ mode
    #define HAVE_ENDIAN_H 1            // <endian.h> is present
    #define HAVE_SYS_ENDIAN_H 1        // <sys/endian.h> is present
    #define HAVE_CXX_HEADER_VERSION 1  // <version> is present
    #define HAVE_CXX_HEADER_BIT 1      // <bit> is present
    #define HAVE_STDC11 1              // C11 is available
    #define HAVE_STDCXX11 1            // C++11 is available
    #define HAVE_STDCXX20 1            // C++20 is available
**/

#include <stdint.h>

/* MSVC intrinsics */
#ifdef _MSC_VER
# include <intrin.h>
#endif

/* check for header files */
#ifdef __has_include
# if !defined(HAVE_ENDIAN_H) && __has_include(<endian.h>)
#  define HAVE_ENDIAN_H 1
# endif
# if !defined(HAVE_SYS_ENDIAN_H) && __has_include(<sys/endian.h>)
#  define HAVE_SYS_ENDIAN_H 1
# endif
# if defined(__cplusplus) && !defined(DISABLE_CPLUSPLUS)
#  if !defined(HAVE_CXX_HEADER_VERSION) && __has_include(<version>)
#   define HAVE_CXX_HEADER_VERSION 1
#  endif
#  if !defined(HAVE_CXX_HEADER_BIT) && __has_include(<bit>)
#   define HAVE_CXX_HEADER_BIT 1
#  endif
# endif
#endif

/* byte order macros */
#ifdef HAVE_ENDIAN_H
# include <endian.h>
#endif
#ifdef HAVE_SYS_ENDIAN_H
# include <sys/endian.h>
#endif

/* C11 */
#ifdef __STDC_VERSION__
# if !defined(HAVE_STDC11) && __STDC_VERSION__ >= 201112L
#  define HAVE_STDC11 1
# endif
#endif

/* C++11, C++20 */
#if defined(__cplusplus) && !defined(DISABLE_CPLUSPLUS)
# if defined(_MSC_VER) && defined(_MSVC_LANG)
/* MSVC always sets __cplusplus as 199711L unless the
 * compiler option -Zc:__cplusplus was given */
#  define BSWAP_CXX_VERSION _MSVC_LANG
# else
#  define BSWAP_CXX_VERSION __cplusplus
# endif

# if !defined(HAVE_STDCXX11) && BSWAP_CXX_VERSION >= 201103L
#  define HAVE_STDCXX11 1
# endif
# if !defined(HAVE_STDCXX20) && BSWAP_CXX_VERSION >= 202002L
#  define HAVE_STDCXX20 1
# endif
# undef BSWAP_CXX_VERSION

# ifdef HAVE_STDCXX11
#  include <type_traits> /* for templates */
# endif

# if defined(HAVE_CXX_HEADER_VERSION) || defined(HAVE_STDCXX20)
#  include <version> /* library feature checks */
# endif

# if defined(HAVE_CXX_HEADER_BIT) || \
     defined(__cpp_lib_endian) || \
     defined(__cpp_lib_byteswap)
#  include <bit> /* std::endian, std::byteswap */
# endif
#endif /* __cplusplus && !DISABLE_CPLUSPLUS */

/* bswap builtin functions */
#ifdef __has_builtin
# if !defined(HAVE_BUILTIN_BSWAP) && \
    __has_builtin(__builtin_bswap16) && \
    __has_builtin(__builtin_bswap32) && \
    __has_builtin(__builtin_bswap64)
#  define HAVE_BUILTIN_BSWAP 1
# endif
#endif




/* BSWAP macros */

#if defined(HAVE_BUILTIN_BSWAP) || defined(__GNUC__)

/* GNU extension builtins */
# define BSWAP16(x)  __builtin_bswap16(x)
# define BSWAP32(x)  __builtin_bswap32(x)
# define BSWAP64(x)  __builtin_bswap64(x)

#elif defined(_MSC_VER)

/* MSVC byteswap intrinsics */
# pragma intrinsic(_byteswap_ushort, _byteswap_ulong, _byteswap_uint64)
# define BSWAP16(x)  _byteswap_ushort(x)
# define BSWAP32(x)  _byteswap_ulong(x)
# define BSWAP64(x)  _byteswap_uint64(x)

#elif defined(__cpp_lib_byteswap) && !defined(DISABLE_CPLUSPLUS)

/* C++23 byteswap template */
# define BSWAP16(x)  std::byteswap<uint16_t>(x)
# define BSWAP32(x)  std::byteswap<uint32_t>(x)
# define BSWAP64(x)  std::byteswap<uint64_t>(x)

#else

/* fall back to bit shifting; with optimizations enabled many compilers will
 * recognize the byte swapping and generate the same code as if using builtins */
# define BSWAP16(x) ((((uint16_t)(x) & UINT16_C(0xff00)) >> 8) | \
                     (((uint16_t)(x) & UINT16_C(0x00ff)) << 8))
# define BSWAP32(x) ((((uint32_t)(x) & UINT32_C(0xff000000)) >> 24) | \
                     (((uint32_t)(x) & UINT32_C(0x00ff0000)) >>  8) | \
                     (((uint32_t)(x) & UINT32_C(0x0000ff00)) <<  8) | \
                     (((uint32_t)(x) & UINT32_C(0x000000ff)) << 24))
# define BSWAP64(x) ((((uint64_t)(x) & UINT64_C(0xff00000000000000)) >> 56) | \
                     (((uint64_t)(x) & UINT64_C(0x00ff000000000000)) >> 40) | \
                     (((uint64_t)(x) & UINT64_C(0x0000ff0000000000)) >> 24) | \
                     (((uint64_t)(x) & UINT64_C(0x000000ff00000000)) >>  8) | \
                     (((uint64_t)(x) & UINT64_C(0x00000000ff000000)) <<  8) | \
                     (((uint64_t)(x) & UINT64_C(0x0000000000ff0000)) << 24) | \
                     (((uint64_t)(x) & UINT64_C(0x000000000000ff00)) << 40) | \
                     (((uint64_t)(x) & UINT64_C(0x00000000000000ff)) << 56))

#endif

/* word order swapping macros */
#define WSWAP32(x) ((((uint32_t)(x) & UINT32_C(0xffff0000)) >> 16) | \
                    (((uint32_t)(x) & UINT32_C(0x0000ffff)) << 16))
#define WSWAP64(x) ((((uint64_t)(x) & UINT64_C(0xffff000000000000)) >> 48) | \
                    (((uint64_t)(x) & UINT64_C(0x0000ffff00000000)) >> 16) | \
                    (((uint64_t)(x) & UINT64_C(0x00000000ffff0000)) << 16) | \
                    (((uint64_t)(x) & UINT64_C(0x000000000000ffff)) << 48))




/* symbolic values for different endianess types */
#define BSWAP_BIG_ENDIAN          1234
#define BSWAP_LITTLE_ENDIAN       4321
#define BSWAP_WORD_BIG_ENDIAN     2143 /* Honeywell 316 */
#define BSWAP_WORD_LITTLE_ENDIAN  3412 /* PDP-11 */
#define BSWAP_UNKNOWN_ENDIAN        -1
#define BSWAP_H316_ENDIAN         BSWAP_WORD_BIG_ENDIAN
#define BSWAP_PDP_ENDIAN          BSWAP_WORD_LITTLE_ENDIAN




/* byte order macros defined by compiler and/or system headers */
#if (defined(__BYTE_ORDER__) && \
     defined(__ORDER_BIG_ENDIAN__) && \
     (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)) \
    || \
    (defined(__BYTE_ORDER) && \
     defined(__BIG_ENDIAN) && \
     (__BYTE_ORDER == __BIG_ENDIAN))

# define BSWAP_BYTE_ORDER  BSWAP_BIG_ENDIAN

#elif (defined(__BYTE_ORDER__) && \
       defined(__ORDER_LITTLE_ENDIAN__) && \
       (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)) \
    || \
      (defined(__BYTE_ORDER) && \
       defined(__LITTLE_ENDIAN) && \
       (__BYTE_ORDER == __LITTLE_ENDIAN))

# define BSWAP_BYTE_ORDER  BSWAP_LITTLE_ENDIAN

#elif (defined(__BYTE_ORDER__) && \
       defined(__ORDER_PDP_ENDIAN__) && \
       (__BYTE_ORDER__ == __ORDER_PDP_ENDIAN__)) \
    || \
      (defined(__BYTE_ORDER) && \
       defined(__PDP_ENDIAN) && \
       (__BYTE_ORDER == __ORDER_PDP_ENDIAN))

# define BSWAP_BYTE_ORDER  BSWAP_WORD_LITTLE_ENDIAN

/* Big Endian detection macros */
#elif  defined(__ARMEB__)     || \
       defined(__THUMBEB__)   || \
       defined(__AARCH64EB__) || \
       defined( _MIPSEB)      || \
       defined(__MIPSEB)      || \
       defined(__MIPSEB__)    || \
       /* defined by some ancient compilers; these are different from the __*_ENDIAN
        * macros which are related to the __BYTE_ORDER macros above */
      (defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__))

# define BSWAP_BYTE_ORDER  BSWAP_BIG_ENDIAN

/* Little Endian detection macros */
#elif  defined(__ARMEL__)     || \
       defined(__THUMBEL__)   || \
       defined(__AARCH64EL__) || \
       defined( _MIPSEL)      || \
       defined(__MIPSEL)      || \
       defined(__MIPSEL__)    || \
      (defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__))

# define BSWAP_BYTE_ORDER  BSWAP_LITTLE_ENDIAN

/* Big Endian exclusive architecture (z/arch and Motorola 68K) */
#elif defined(__s390x__)      || \
      defined(__zarch__)      || \
      defined(__SYSC_ZARCH__) || \
      defined(__m68k__)

# define BSWAP_BYTE_ORDER  BSWAP_BIG_ENDIAN

/* Little Endian exclusive architecture (x86 and Blackfin) */
#elif defined(__amd64)  || \
      defined(__i386)   || \
      defined(__x86_64) || \
      defined(_M_AMD64) || \
      defined(_M_IX86)  || \
      defined(_M_X64)   || \
      defined(__BFIN__)

# define BSWAP_BYTE_ORDER  BSWAP_LITTLE_ENDIAN

#endif


/* return byte order */
static inline int get_byte_order(void)
{
#if defined(BSWAP_BYTE_ORDER) && !defined(GET_BYTE_ORDER_RUNTIME)

    return BSWAP_BYTE_ORDER;

/* C++20 std::endian enum values */
#elif defined(__cpp_lib_endian) && !defined(DISABLE_CPLUSPLUS)
    if constexpr (std::endian::native == std::endian::big) {
        return BSWAP_BIG_ENDIAN;
    } else if constexpr (std::endian::native == std::endian::little) {
        return BSWAP_LITTLE_ENDIAN;
    } else {
        return BSWAP_UNKNOWN_ENDIAN;
    }

#else

    const union {
      uint32_t value;
      uint8_t  data[4];
    } u = {
      .data[0] = 0x01,
      .data[1] = 0x02,
      .data[2] = 0x03,
      .data[3] = 0x04
    };

    switch (u.value)
    {
    case 0x01020304: return BSWAP_BIG_ENDIAN;
    case 0x04030201: return BSWAP_LITTLE_ENDIAN;
    case 0x02010403: return BSWAP_WORD_BIG_ENDIAN;
    case 0x03040102: return BSWAP_WORD_LITTLE_ENDIAN;
    default:         return BSWAP_UNKNOWN_ENDIAN;
    }

#endif
}




/* convert between host order and BE/LE */

#define BE16_TO_HOST(x)  (get_byte_order() == BSWAP_BIG_ENDIAN ? (x) : BSWAP16(x))
#define BE32_TO_HOST(x)  (get_byte_order() == BSWAP_BIG_ENDIAN ? (x) : BSWAP32(x))
#define BE64_TO_HOST(x)  (get_byte_order() == BSWAP_BIG_ENDIAN ? (x) : BSWAP64(x))

#define HOST_TO_BE16(x)  BE16_TO_HOST(x)
#define HOST_TO_BE32(x)  BE32_TO_HOST(x)
#define HOST_TO_BE64(x)  BE64_TO_HOST(x)

#define LE16_TO_HOST(x)  (get_byte_order() == BSWAP_LITTLE_ENDIAN ? (x) : BSWAP16(x))
#define LE32_TO_HOST(x)  (get_byte_order() == BSWAP_LITTLE_ENDIAN ? (x) : BSWAP32(x))
#define LE64_TO_HOST(x)  (get_byte_order() == BSWAP_LITTLE_ENDIAN ? (x) : BSWAP64(x))

#define HOST_TO_LE16(x)  LE16_TO_HOST(x)
#define HOST_TO_LE32(x)  LE32_TO_HOST(x)
#define HOST_TO_LE64(x)  LE64_TO_HOST(x)

#define NTOHS(x)         BE16_TO_HOST(x)
#define NTOHL(x)         BE32_TO_HOST(x)
#define NTOHLL(x)        BE64_TO_HOST(x)

#define HTONS(x)         HOST_TO_BE16(x)
#define HTONL(x)         HOST_TO_BE32(x)
#define HTONLL(x)        HOST_TO_BE64(x)




/* generic macros / C++ templates */

#if defined(__cpp_lib_byteswap) && !defined(DISABLE_CPLUSPLUS)

/* C++23 */
# define BSWAP(x)  std::byteswap(x)

#elif defined(HAVE_STDCXX11) && !defined(DISABLE_CPLUSPLUS)

namespace Byte_Swap
{
    /* C++ function template similar to std::byteswap() */
    template<typename T, typename std::enable_if<std::is_integral<T>::value, bool>::type = true>
    T bswap(T val) noexcept
    {
        if (sizeof(T) == 1) {
            return val;
        } else if (sizeof(T) == 2) {
            return BSWAP16(val);
        } else if (sizeof(T) == 4) {
            return BSWAP32(val);
        } else if (sizeof(T) == 8) {
            return BSWAP64(val);
        }

        static_assert(sizeof(T) <= 8,
            "unsupported parameter type in template `Byte_Swap::bswap'");
    }
}

# define BSWAP(x)  Byte_Swap::bswap(x)

#elif defined(HAVE_STDC11)

/* generic macro */
# define BSWAP(x) \
    _Generic((x), \
        int8_t   : (x), \
        int16_t  : BSWAP16(x), \
        int32_t  : BSWAP32(x), \
        int64_t  : BSWAP64(x), \
        uint8_t  : (x), \
        uint16_t : BSWAP16(x), \
        uint32_t : BSWAP32(x), \
        uint64_t : BSWAP64(x) \
    )

#endif


#if defined(HAVE_STDCXX11) && !defined(DISABLE_CPLUSPLUS)

namespace Byte_Swap
{
    template<typename T, typename std::enable_if<std::is_integral<T>::value, bool>::type = true>
    T wswap(T val) noexcept
    {
        if (sizeof(T) < 4) {
            return val;
        } else if (sizeof(T) == 4) {
            return WSWAP32(val);
        } else if (sizeof(T) == 8) {
            return WSWAP64(val);
        }

        static_assert(sizeof(T) <= 8,
            "unsupported parameter type in template `Byte_Swap::wswap'");
    }
}

# define WSWAP(x)  Byte_Swap::wswap(x)

#elif defined(HAVE_STDC11)

/* generic macro */
# define WSWAP(x) \
    _Generic((x), \
        int8_t   : (x), \
        int16_t  : (x), \
        int32_t  : WSWAP32(x), \
        int64_t  : WSWAP64(x), \
        uint8_t  : (x), \
        uint16_t : (x), \
        uint32_t : WSWAP32(x), \
        uint64_t : WSWAP64(x) \
    )

#endif


#ifdef BSWAP
# define BE_TO_HOST(x)  (get_byte_order() == BSWAP_BIG_ENDIAN    ? (x) : BSWAP(x))
# define LE_TO_HOST(x)  (get_byte_order() == BSWAP_LITTLE_ENDIAN ? (x) : BSWAP(x))
# define HOST_TO_BE(x)  (get_byte_order() == BSWAP_BIG_ENDIAN    ? (x) : BSWAP(x))
# define HOST_TO_LE(x)  (get_byte_order() == BSWAP_LITTLE_ENDIAN ? (x) : BSWAP(x))
# define HTON(x)        HOST_TO_BE(x)
# define NTOH(x)        BE_TO_HOST(x)
#endif


#endif //BYTESWAP_H
