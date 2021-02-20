#include <string.h>
#include <stdint.h>
#include "Util.h"


#ifdef __cpluscplus
extern "C" {
#endif

int strhash(const char* str) {
	int hash = 271;
	const char* s = str;
	while(*s++) { hash += s[0]; hash %= 1024; }
	return hash;
}

uint32_t hash(void *buf, size_t len, uint32_t* hval) {
	uint32_t PRIME_32 = 0x01000193;
	unsigned char* bp = (unsigned char*)buf;
	unsigned char* be = bp+len;

	while(bp<be) {
#if defined(HAHS_SIMPLE) || 1
			*hval *= PRIME_32;
#else
			*hval += (*hval<<1)+(*hval<<4)+(*hval<<7)+(*hval<<8)+(*hval<<24);
#endif
			*hval ^= *bp++;
	}
	return *hval;
}

void mmemset(void* p, int v, size_t len) {
	char* pp = (char*)p;
	while(len--) { *pp++ = v; }
}

int prime_max(int n) {
    // Create a boolean array 
    // "prime[0..n]" and initialize
    // all entries it as true. 
    // A value in prime[i] will
    // finally be false if i is 
    // Not a prime, else true.
    bool prime[n + 1];
    memset(prime, true, sizeof(prime));
 
    for (int p = 2; p * p <= n; p++)
    {
        // If prime[p] is not changed, 
        // then it is a prime
        if (prime[p] == true) 
        {
            // Update all multiples 
            // of p greater than or
            // equal to the square of it
            // numbers which are multiple 
            // of p and are less than p^2 
            // are already been marked.
            for (int i = p * p; i <= n; i += p) prime[i] = false;
        }
    }
 
    // Print all prime numbers
	int pn;
    for (int p = 2; p <= n; p++) { if (prime[p]) pn = p; }
	return pn;
}

#ifdef __cpluscplus
}
#endif
