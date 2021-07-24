#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static char digits[] = "0123456789ABCDEF";

static void printint(int xx, int base, int sgn) 
{
	char buf[16];
	int i, neg;
	unsigned int x;
	neg = 0;
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
		putch(buf[i]);
}
// 状态机
int printf(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	int state, c, i, cnt;
	state = cnt = 0;
	for(i=0; fmt[i] != '\0'; i++){
		c = fmt[i] & 0xff;
		if(state == 0) {
			if(c == '%') {
				state = '%';
			}else{
				putch(c);
			}
		}else if(state == '%') {
			if(c == 'd') {
				printint(va_arg(ap, int), 10, 1);
				cnt++;
			}else if(c == 'x') {
				printint(va_arg(ap, int), 16, 0);
				cnt++;
			}else if(c == 's') {
				char *s = va_arg(ap, char*);
				putstr(s);
				cnt ++;
			}else if(c == 'c') {
				putch(va_arg(ap, unsigned int));
				cnt ++;
			}else if(c == '%') {
				putch(c);
			}else {
				putch('%');
				putch(c);
			}
			state = 0;
		}
	}
  return cnt;
}

static int
sputc(char *s, char c)
{
  *s = c;
  return 1;
}

static int
sprintint(char *s, int xx, int base, int sign)
{
  char buf[16];
  int i, n;
  unsigned int x;

  if(sign && (sign = xx < 0))
    x = -xx;
  else
    x = xx;

  i = 0;
  do {
    buf[i++] = digits[x % base];
  } while((x /= base) != 0);

  if(sign)
    buf[i++] = '-';

  n = 0;
  while(--i >= 0)
    n += sputc(s+n, buf[i]);
  return n;
}

int
sprintf(char *buf, const char *fmt, ...)
{
  va_list ap;
  int i, c;
  int off = 0;
  char *s;
  //for(int j=0; buf[j]; j++) 
	//buf[j] = '\0';
	va_start(ap, fmt);
  for(i = 0; (c = fmt[i] & 0xff) != 0; i++){
    if(c != '%'){
      off += sputc(buf+off, c);
      continue;
    }
    c = fmt[++i] & 0xff;
    if(c == 0)
      break;
    switch(c){
    case 'd':
      off += sprintint(buf+off, va_arg(ap, int), 10, 1);
      break;
    case 'x':
      off += sprintint(buf+off, va_arg(ap, int), 16, 0);
      break;
    case 's':
				s = va_arg(ap, char *);
        for(; *s; s++)
        	off += sputc(buf+off, *s);
      break;
    case '%':
      off += sputc(buf+off, '%');
      break;
    default:
      // Print unknown % sequence to draw attention.
      off += sputc(buf+off, '%');
      off += sputc(buf+off, c);
      break;
    }
  }
  return off;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
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
#endif
