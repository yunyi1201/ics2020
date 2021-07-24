#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
/*
static char digits[] = "0123456789ABCDEF";

static size_t printint(char *str, int xx, int base, int sgn) 
{
	char buf[16];
	int i, j, neg;
	unsigned int x;
	j = neg = 0;
	if(sgn && xx < 0){
		neg = 1;
		x = -xx;
	} else {
		x = xx;
	}
	i = 0;
	do{
		buf[i++] = digits[x%base];
	}while((x/base) != 0);
	if(neg) {
		buf[i++] = '-';
	}
	while(--i > 0) 
		str[j++] = buf[i];
	return j;
}
// 状态机
int printf(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	int cnt = 0;
	char ans[2000] = {0};
	cnt = vsprintf(ans, fmt, ap);
	va_end(ap);
	for(int i=0; ans[i]; i++)
		putch(ans[i]);
	return cnt;
}

int
sprintf(char *buf, const char *fmt, ...)
{
		va_list ap;
		va_start(ap, fmt);
		int ret = 0;
		ret = vsprintf(buf, fmt, ap);
		va_end(ap);
    return ret;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
	int state, c, i, cnt, j;
	j = state = cnt = 0;
	for(i=0; fmt[i] != '\0'; i++){
		c = fmt[i] & 0xff;
		if(state == 0) {
			if(c == '%') {
				state = '%';
			}else{
				//putch(c);
				out[j++] = c;
				cnt ++;
			}
		}else if(state == '%') {
			if(c == 'd') {
				j += printint(out+j, va_arg(ap, int), 10, 1);
				cnt++;
			}else if(c == 'x') {
				j += printint(out+j, va_arg(ap, int), 16, 0);
				cnt++;
			}else if(c == 's') {
				char *s = va_arg(ap, char*);
				//putstr(s);
				for(int k=0; s[k]; k++)
					out[j++] = s[k];
				cnt ++;
			} else if(c == '%') {
				//putch(c);
				out[j++] = '%';
				cnt++;
			}else {
				putch('%');
				putch(c);
			}
			state = 0;
		}
	}
	out[j] = '\0';
  return cnt;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  return 0;
}

int sscanf(const char *str, const char *format, ...){
	return 0;
}
*/

#define MAX_BUF 2048
static inline size_t iofmt_int(char *dest, int64_t d, uint32_t base) {
  /* Print all the digits in *dest and
   * return the length of the integer */

  const char map[] = "0123456789abcdef";
  size_t len       = 0;
  bool minus       = false;
  char buffer[65];
  memset(buffer, 0, sizeof(buffer));
  char *buf_ptr = &buffer[0];

  if (d < 0) {
    minus = true;
    d     = -d;
  }

  // Print out the digit in reverse order
  do {
    *buf_ptr++ = map[d % base];
    d /= base;
  } while (d != 0);

  if (minus) {
    *buf_ptr++ = '-';
  }

  len = buf_ptr - buffer;

  // Reverse the buffer
  while (buf_ptr != buffer) {
    *dest++ = *(--buf_ptr);
  }

  return len;
}

int printf(const char *fmt, ...) {
  va_list arg;
  int done = 0;

  /* defined in klib.h, need to be modified.
   * MAX_BUF >= a certain amount can lead to exceed */
  char buf[MAX_BUF];
  memset(buf, 0, sizeof(buf));

  // Passing those arguemnts to sprintf
  va_start(arg, fmt);
  done = vsprintf(buf, fmt, arg);
  va_end(arg);

  int i;
  for (i = 0; i < done; ++i) {
    putch(buf[i]);
  }

  return done;
}

// TODO: to support longer/limited(vsnprintf) output
int vsprintf(char *out, const char *fmt, va_list ap) {
  /* sprintf with va_list argument
   * %d: int32_t
   * %s: char*
   * %c: char
   * %l(non-standard): int64_t */

  char *done = out;
  int d;
  long long l;
  char c;
  char *s;
  void *p;

  while (*fmt) {
    switch (*fmt) {
    case '%': {
      ++fmt;
      switch (*fmt) {
      case 'd':
        d = va_arg(ap, int);
        out += iofmt_int(out, d, 10);
        break;
      case 's':
        s = va_arg(ap, char *);
        memcpy(out, s, strlen(s));
        out += strlen(s);
        break;
      case 'c':
        // Char -> int in argument passing(?)
        c      = va_arg(ap, int);
        *out++ = c;
        break;
      case 'l':
        l = va_arg(ap, int64_t);
        out += iofmt_int(out, l, 10);
        break;
      case 'p': {
        p = va_arg(ap, void *);
        out += iofmt_int(out, (uint32_t)p, 16);
        break;
      }
      case 'x':
        l = va_arg(ap, uint32_t);
        out += iofmt_int(out, l, 16);
        break;
      default: {
        char tmp[] = "<<pattern not implemented>>";
        memcpy(out, tmp, strlen(tmp));
        out += strlen(tmp);
        // assert(0);
        break;
      }
      }
      break;
    }
    default: *out++ = *fmt;
    }
    ++fmt;
  }
  *out = '\0';

  return out - done;
}

int sprintf(char *out, const char *fmt, ...) {
  /* sprintf pass those arguments to vsprintf */
  va_list ap;
  va_start(ap, fmt);
  int done = vsprintf(out, fmt, ap);
  va_end(ap);
  return done;
}

int snprintf(char *out, size_t n, const char *fmt, ...) { return 0; }

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) { return 0; }

#endif
