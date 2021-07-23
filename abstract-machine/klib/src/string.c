#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
	int n;
	for(n=0; s[n]; n++)
		;
  return n;
}

char *strcpy(char* dst,const char* src) {
	char *s = dst;
	while((*dst++ = *src++) != '\0')
		;
  return s;
}

char* strncpy(char* dst, const char* src, size_t n) {
  return NULL;
}

char* strcat(char* dst, const char* src) {
	int n, i;
	for(n=0; dst[n] != '\0'; n++)
		;
	for(i=0; src[i] != '\0'; i++)
		dst[n+i] = src[i];
	dst[n+i] = '\0';
	return dst;
}

int strcmp(const char* s1, const char* s2) {
	while(*s1 && *s1 == *s2)
		s1++, s2++;
	return (unsigned char)*s1 - (unsigned char)*s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  return 0;
}

void* memset(void* v,int c,size_t n) {
	char *dst = (char*)v;
	int i;
	for(i=0; i<n; i++)
		dst[i] = c;
  return v;
}

void* memmove(void* dst,const void* src,size_t n) {
  return NULL;
}

void* memcpy(void* out, const void* in, size_t n) {
  return NULL;
}

int memcmp(const void* s1, const void* s2, size_t n) {
	const char* p1 = s1, *p2 = s2;
	while(n--){
		if(*p1 != *p2)
			return *p1 - *p2;
		p1++; p2++;
	}
  return 0;
}

#endif
