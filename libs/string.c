#include "kernel.h"

void *memcpy(void *dest, const void *src, size_t count)
{
    const char *sp = (const char *)src;
    char *dp = (char *)dest;
    for(; count != 0; count--) *dp++ = *sp++;
    return dest;
}

void *memset(void *dest, char val, size_t count)
{
    char *temp = (char *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

size_t strlen(const char *str)
{
    size_t retval;
    for(retval = 0; *str != '\0'; str++) retval++;
    return retval;
}

unsigned char *strcpy(unsigned char *dest, const unsigned char *src, size_t count)
{
//	int i;
//	for(i=0;i<count;i++)
//		dest[i]=src[i];

	while((*dest++=*src++)!='\0') ;
	{
	}
	return dest;
}

unsigned char *strset(unsigned char *dest, unsigned char val, size_t count)
{
	size_t i;
	for(i=0;i<count;i++)
		dest[i]=val;
	return dest;
}

int strcmp(unsigned char *s, unsigned char *t)
{
	for( ;*s == *t; s++, t++)
		if(*s=='\0')
			return 0;

	return *s-*t;
}

int strcmpl(unsigned char *s, unsigned char *t, size_t len)
{
	int i = 0;
	while(i<len)
	{
		if(s[i]!=t[i])
			break;
		i++;
	}

	if(i==len)
		return 0;
	else
		return s[i]-t[i];
}
