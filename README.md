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
