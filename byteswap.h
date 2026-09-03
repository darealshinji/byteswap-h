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
  Mixed endian is not supported.


  Reverse byte order:

      uint16_t BSWAP16(uint16_t x);
      uint32_t BSWAP32(uint32_t x);
      uint64_t BSWAP64(uint64_t x);


  Convert values between host and big-/little-endian byte order:

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
      intX_t HOST_TO_BE(intX_t host_bits);
      intX_t HOST_TO_LE(intX_t host_bits);
      intX_t BE_TO_HOST(intX_t big_endian_bits);
      intX_t LE_TO_HOST(intX_t little_endian_bits);
      intX_t HTON(intX_t host_bits);
      intX_t NTOH(intX_t net_bits);


  The following function returns 1 (true) if byte order is Big Endian
  and 0 (false) if it's Little Endian.
  In most cases compiler optimizations should turn the return value
  into a compile-time constant and optimize the function away.

      static inline int big_endian_host(void);


  These convenience macros are available too:

      #define HOST_IS_BIG()    (big_endian_host() == 1)
      #define HOST_IS_LITTLE() (big_endian_host() == 0)

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
# if !defined(HAVE_SYS_PARAM_H) && __has_include(<sys/param.h>)
#  define HAVE_SYS_PARAM_H 1
# endif
# if !defined(HAVE_CXX_HEADER_VERSION) && __has_include(<version>)
#  define HAVE_CXX_HEADER_VERSION 1
# endif
# if !defined(HAVE_CXX_HEADER_BIT) && __has_include(<bit>)
#  define HAVE_CXX_HEADER_BIT 1
# endif
#endif

/* these headers might provide byte order macros */
#ifdef HAVE_ENDIAN_H
# include <endian.h>
#endif
#ifdef HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

#ifdef __STDC_VERSION__
# if !defined(HAVE_STDC11) && __STDC_VERSION__ >= 201112L
#  define HAVE_STDC11 1
# endif
#endif

#ifdef __cplusplus
# if !defined(HAVE_STDCXX11) && __cplusplus >= 201103L
#  define HAVE_STDCXX11 1
# endif
# if !defined(HAVE_STDCXX20) && __cplusplus >= 202002L
#  define HAVE_STDCXX20 1
# endif
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
#endif

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
#elif defined(__cpp_lib_byteswap)
/* C++23 byteswap template */
# define BSWAP16(x)  std::byteswap<uint16_t>(x)
# define BSWAP32(x)  std::byteswap<uint32_t>(x)
# define BSWAP64(x)  std::byteswap<uint64_t>(x)
#else
/* fall back to bit shifting; with optimizations enabled many compilers will
 * recognize the byte swapping and generate the same code as if using builtins */
#define BSWAP16(x) ((((uint16_t)(x) & 0xff00) >> 8) | \
                    (((uint16_t)(x) & 0x00ff) << 8))
#define BSWAP32(x) ((((uint32_t)(x) & 0xff000000) >> 24) | \
                    (((uint32_t)(x) & 0x00ff0000) >>  8) | \
                    (((uint32_t)(x) & 0x0000ff00) <<  8) | \
                    (((uint32_t)(x) & 0x000000ff) << 24))
#define BSWAP64(x) ((((uint64_t)(x) & 0xff00000000000000ULL) >> 56) | \
                    (((uint64_t)(x) & 0x00ff000000000000ULL) >> 40) | \
                    (((uint64_t)(x) & 0x0000ff0000000000ULL) >> 24) | \
                    (((uint64_t)(x) & 0x000000ff00000000ULL) >>  8) | \
                    (((uint64_t)(x) & 0x00000000ff000000ULL) <<  8) | \
                    (((uint64_t)(x) & 0x0000000000ff0000ULL) << 24) | \
                    (((uint64_t)(x) & 0x000000000000ff00ULL) << 40) | \
                    (((uint64_t)(x) & 0x00000000000000ffULL) << 56))
#endif


/* return value is 1 (true) for Big Endian
 * and 0 (false) for Little Endian */
static inline int big_endian_host(void)
{
/* byte order macros defined by compiler or headers */
#if defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__)
    return (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__);
#elif defined(__BYTE_ORDER) && defined(__ORDER_BIG_ENDIAN)
    return (__BYTE_ORDER == __ORDER_BIG_ENDIAN);
#elif defined(_BYTE_ORDER) && defined(_ORDER_BIG_ENDIAN)
    return (_BYTE_ORDER == _ORDER_BIG_ENDIAN);
#elif defined(BYTE_ORDER) && defined(ORDER_BIG_ENDIAN)
    return (BYTE_ORDER == ORDER_BIG_ENDIAN);

/* numeric values in Stratus VOS are always Big Endian, regardless
 * of the endianness of the underlying hardware platform */
#elif defined(__VOS__)
    return 1;

/* x86 is always Little Endian */
#elif defined(__amd64)  || defined(__i386)  || defined(__x86_64) || \
      defined(_M_AMD64) || defined(_M_IX86) || defined(_M_X64)
    return 0;

/* Little Endian ARM and MIPS */
#elif defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || \
      defined(_MIPSEL)   || defined(__MIPSEL)    || defined(__MIPSEL__)
    return 0;

/* Big Endian ARM and MIPS; today architectures like PowerPC or
 * SPARC64 are often bi-endian, so no assumtions here */
#elif defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || \
      defined(_MIPSEB)   || defined(__MIPSEB)    || defined(__MIPSEB__)
    return 1;

/* C++20 std::endian enum values */
#elif defined(__cpp_lib_endian)
    return constexpr (std::endian::native == std::endian::big);

#else

    /* optimizations should replace all of
     * this with a constant return value) */
    const union {
      uint16_t num;
      uint8_t  bytes[2];
    } u = { 0x1234 };

    return (u.bytes[0] == 0x12);

#endif
}

#define HOST_IS_BIG()    (big_endian_host() == 1)
#define HOST_IS_LITTLE() (big_endian_host() == 0)


/* convert between host order and BE/LE */

#define BE16_TO_HOST(x)  (HOST_IS_BIG()    ? (x) : BSWAP16(x))
#define BE32_TO_HOST(x)  (HOST_IS_BIG()    ? (x) : BSWAP32(x))
#define BE64_TO_HOST(x)  (HOST_IS_BIG()    ? (x) : BSWAP64(x))

#define HOST_TO_BE16(x)  (HOST_IS_BIG()    ? (x) : BSWAP16(x))
#define HOST_TO_BE32(x)  (HOST_IS_BIG()    ? (x) : BSWAP32(x))
#define HOST_TO_BE64(x)  (HOST_IS_BIG()    ? (x) : BSWAP64(x))

#define LE16_TO_HOST(x)  (HOST_IS_LITTLE() ? (x) : BSWAP16(x))
#define LE32_TO_HOST(x)  (HOST_IS_LITTLE() ? (x) : BSWAP32(x))
#define LE64_TO_HOST(x)  (HOST_IS_LITTLE() ? (x) : BSWAP64(x))

#define HOST_TO_LE16(x)  (HOST_IS_LITTLE() ? (x) : BSWAP16(x))
#define HOST_TO_LE32(x)  (HOST_IS_LITTLE() ? (x) : BSWAP32(x))
#define HOST_TO_LE64(x)  (HOST_IS_LITTLE() ? (x) : BSWAP64(x))

#define NTOHS(x)         BE16_TO_HOST(x)
#define NTOHL(x)         BE32_TO_HOST(x)
#define NTOHLL(x)        BE64_TO_HOST(x)

#define HTONS(x)         HOST_TO_BE16(x)
#define HTONL(x)         HOST_TO_BE32(x)
#define HTONLL(x)        HOST_TO_BE64(x)


/* generic macros */

#ifdef HAVE_STDCXX11

/* C++ function template similar to std::byteswap() */
template<typename T, typename std::enable_if<std::is_integral<T>::value, bool>::type = true>
constexpr T bswap(T val) noexcept
{
  if constexpr (sizeof(T) == 1) {
    return val;
  } else if constexpr (sizeof(T) == 2) {
    return BSWAP16(val);
  } else if constexpr (sizeof(T) == 4) {
    return BSWAP32(val);
  } else if constexpr (sizeof(T) == 8) {
    return BSWAP64(val);
  }

  static_assert(sizeof(T) <= 8,
                "unsupported parameter type in template `bswap'");
}

# define BSWAP(x) bswap(x)

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

#if defined(HAVE_STDCXX11) || defined(HAVE_STDC11)
# define BE_TO_HOST(x)  (HOST_IS_BIG()    ? (x) : BSWAP(x))
# define LE_TO_HOST(x)  (HOST_IS_LITTLE() ? (x) : BSWAP(x))
# define HOST_TO_BE(x)  (HOST_IS_BIG()    ? (x) : BSWAP(x))
# define HOST_TO_LE(x)  (HOST_IS_LITTLE() ? (x) : BSWAP(x))
# define HTON(x)        HOST_TO_BE(x)
# define NTOH(x)        BE_TO_HOST(x)
#endif

#endif //BYTESWAP_H
