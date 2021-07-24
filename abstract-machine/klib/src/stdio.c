#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

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
#endif
