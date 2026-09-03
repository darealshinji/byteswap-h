This header provides endianess and byteswap macros in a unified way for most
compilers and operating systems. The macro names are all uppercase to 
hopefully not conflict with existing functions.
Only 16, 32 and 64 bit signed and unsigned integers are supported.
Mixed endian is not supported.

Reverse byte order:
``` C
uint16_t BSWAP16(uint16_t x);
uint32_t BSWAP32(uint32_t x);
uint64_t BSWAP64(uint64_t x);
```

Convert values between host and big-/little-endian byte order:
``` C
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
```

Convert values between host and network byte order:
``` C
uint16_t HTONS(uint16_t hostshort);
uint32_t HTONL(uint32_t hostlong);
uint64_t HTONLL(uint64_t hostlonglong);

uint16_t NTOHS(uint16_t netshort);
uint32_t NTOHL(uint32_t netlong);
uint64_t NTOHLL(uint64_t netlonglong);
```

Generic macros for all signed and unsigned integers.
Minimum requirement is C11 or C++11.
``` C
uintXX_t BSWAP(uintXX_t x);
uintXX_t HOST_TO_BE(uintXX_t host_bits);
uintXX_t HOST_TO_LE(uintXX_t host_bits);
uintXX_t BE_TO_HOST(uintXX_t big_endian_bits);
uintXX_t LE_TO_HOST(uintXX_t little_endian_bits);
uintXX_t HTON(uintXX_t host_bits);
uintXX_t NTOH(uintXX_t net_bits);
```

The following function returns 1 (true) if byte order is Big Endian
and 0 (false) if it's Little Endian.
In most cases compiler optimizations should turn the return value
into a compile-time constant and optimize the function away.
``` C
static inline int big_endian_host(void);
```

These convenience macros are available too:
``` C
#define HOST_IS_BIG()    (big_endian_host() == 1)
#define HOST_IS_LITTLE() (big_endian_host() == 0)
```
