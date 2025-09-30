/**
  ****************************************************************************
  * @file    PT32x0xx_retarget.c
  * @author  Application development team
  * @version V1.0.0
  * @date    2025/2/13
  * @brief   Remap fputc()and fgetc()£¬supports the printf() and scanf() functions of standard C/C++
  *            
  ******************************************************************************
  * @attention
  *
  * The firmware provided is for guidance only. The codes are offered for the convenience of the customers.  
  * Pai-IC assumes no responsibility for any claims about the direct or indirect loss due to the misuse of
  * the content or the codes within the content.  
  * 
  * Copyright (C) by Pai-IC Microelectronics Co., Ltd., all rights reserved 
  ***************************************************************************
**/
  
/* Includes ------------------------------------------------------------------------------------------------*/
#include "PT32x0xx.h"

#if defined (__CC_ARM)
#pragma import(__use_no_semihosting_swi)

#include <stdio.h>
#include <rt_misc.h>

/* Global variables ----------------------------------------------------------------------------------------*/
/** @defgroup RETARGET_Global_Variable Retarget global variables
  * @{
  */
struct __FILE
{
	int handle; /* Add whatever you need here */
};
FILE __stdout;
FILE __stdin;
/**
  * @}
  */

/* Global functions ----------------------------------------------------------------------------------------*/
/** @defgroup RETARGET_Exported_Functions Retarget exported functions
  * @{
  */
int __backspace(FILE *stream)
{
	return 0;
}

int fputc(int ch, FILE *f)
{
#ifdef RETARGET_UART_PORT
	while((RETARGET_UART_PORT->SR & UART_SR_TXE) == 0);   //Waiting Transmit-FIFO all empty
	RETARGET_UART_PORT->DR = (ch & 0x1FF);

	return ch;
#else
	return -1;
#endif
}

int fgetc(FILE *f)
{
	u16 ch;
	
	while((RETARGET_UART_PORT->SR&UART_SR_RXNE) == 0);  // Waiting Receive-FIFO not empty
	ch = RETARGET_UART_PORT->DR;
	
	return ch;
}

void _sys_exit(int return_code)
{
	while(1);
}
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6100100)
__asm (".global __use_no_semihosting\n\t");
void _sys_exit(int x)
{
	x = x;
}

//__use_no_semihosting was requested, but _ttywrch was
void _ttywrch(int ch)
{
	ch = ch;
}

void _sys_command_string(char* cmd, int len)
{
	len = len;
}

FILE __stdout;
int fputc(int ch, FILE *f)
{
#ifdef RETARGET_UART_PORT
	while((RETARGET_UART_PORT->SR & UART_SR_TXE) == 0);   //Waiting Transmit-FIFO all empty
	RETARGET_UART_PORT->DR = (u16)ch;

	return ch;
#else
	return 0;
#endif
}

int fgetc(FILE *f)
{
	u16 ch;
	
	while((RETARGET_UART_PORT->SR&UART_SR_RXNE) == 0);  // Waiting Receive-FIFO not empty
	ch = RETARGET_UART_PORT->DR;
	
	return ch;
}
#elif defined (__GNUC__)
/* the version of GNU GCC must be greater than 4.x */
#include "PT32_Type.h"

#ifndef USING_LIBC
typedef __builtin_va_list           __gnuc_va_list;
typedef __gnuc_va_list              va_list;
#define va_start(v,l)               __builtin_va_start(v,l)
#define va_end(v)                   __builtin_va_end(v)
#define va_arg(v,l)                 __builtin_va_arg(v,l)
#endif




//#define PRINTF_USING_LONGLONG
#define PRINTF_PRECISION
#define PRINTF_SPECIAL

#define CONSOLEBUF_SIZE				128

/* private function */
#define _ISDIGIT(c)  				((unsigned)((c) - '0') < 10)

#define ZEROPAD     				(1 << 0)    /* pad with zero */
#define SIGN        				(1 << 1)    /* unsigned/signed long */
#define PLUS        				(1 << 2)    /* show plus */
#define SPACE       				(1 << 3)    /* space if plus */
#define LEFT        				(1 << 4)    /* left justified */
#define SPECIAL     				(1 << 5)    /* 0x */
#define LARGE       				(1 << 6)    /* use 'ABCDEF' instead of 'abcdef' */

void hw_console_output(const char *str)
{
#ifdef RETARGET_UART_PORT
	while(*str != 0)
	{
		while((RETARGET_UART_PORT->SR & UART_SR_TXE) == 0);   //Waiting Transmit-FIFO all empty
		RETARGET_UART_PORT->DR = *str++;
	}
#endif
}

/**
 * @brief  This function will duplicate a string.
 * @param  n is the string to be duplicated.
 * @param  base is support divide instructions value.
 * @return the duplicated string pointer.
 */
#ifdef PRINTF_USING_LONGLONG
static inline int divide(unsigned long long *n, int base)
#else
static inline int divide(unsigned long *n, int base)
#endif /* PRINTF_USING_LONGLONG */
{
    int res;

    /* optimized for processor which does not support divide instructions. */
#ifdef PRINTF_USING_LONGLONG
    res = (int)((*n) % base);
    *n = (long long)((*n) / base);
#else
    res = (int)((*n) % base);
    *n = (long)((*n) / base);
#endif

    return res;
}

static inline int skip_atoi(const char **s)
{
    int i = 0;
	
    while(_ISDIGIT(**s))
	{
        i = i * 10 + *((*s)++) - '0';
	}

    return i;
}

static char *print_number(char *buf, char *end,
#ifdef PRINTF_USING_LONGLONG
                          unsigned long long num,
#else
                          unsigned long num,
#endif /* KPRINTF_USING_LONGLONG */
                          int base,
                          int qualifier,
                          int s,
#ifdef PRINTF_PRECISION
                          int precision,
#endif /* PRINTF_PRECISION */
                          int type)
{
    char c = 0, sign = 0;
#ifdef PRINTF_USING_LONGLONG
    char tmp[64] = {0};
#else
    char tmp[32] = {0};
#endif /* RT_KPRINTF_USING_LONGLONG */
    int precision_bak = precision;
    const char *digits = NULL;
    static const char small_digits[] = "0123456789abcdef";
    static const char large_digits[] = "0123456789ABCDEF";
    int i = 0;
    int size = 0;

    size = s;

    digits = (type & LARGE) ? large_digits : small_digits;
    if(type & LEFT)
    {
        type &= ~ZEROPAD;
    }

    c = (type & ZEROPAD) ? '0' : ' ';

    /* get sign */
    sign = 0;
    if(type & SIGN)
    {
        switch(qualifier)
        {
			case 'h':
				if ((s16)num < 0)
				{
					sign = '-';
					num = (u16)-num;
				}
				break;
			case 'L':
			case 'l':
				if ((long)num < 0)
				{
					sign = '-';
					num = (unsigned long)-num;
				}
				break;
			case 0:
			default:
				if ((s32)num < 0)
				{
					sign = '-';
					num = (u32)-num;
				}
				break;
        }

        if(sign != '-')
        {
            if(type & PLUS)
            {
                sign = '+';
            }
            else if(type & SPACE)
            {
                sign = ' ';
            }
        }
    }

#ifdef PRINTF_SPECIAL
    if(type & SPECIAL)
    {
        if (base == 2 || base == 16)
        {
            size -= 2;
        }
        else if (base == 8)
        {
            size--;
        }
    }
#endif /* PRINTF_SPECIAL */

    i = 0;
    if(num == 0)
    {
        tmp[i++] = '0';
    }
    else
    {
        while(num != 0)
            tmp[i++] = digits[divide(&num, base)];
    }

#ifdef PRINTF_PRECISION
    if(i > precision)
    {
        precision = i;
    }
    size -= precision;
#else
    size -= i;
#endif /* RT_PRINTF_PRECISION */

    if(!(type & (ZEROPAD | LEFT)))
    {
        if((sign) && (size > 0))
        {
            size--;
        }

        while(size-- > 0)
        {
            if (buf < end)
            {
                *buf = ' ';
            }

            ++buf;
        }
    }

    if(sign)
    {
        if(buf < end)
        {
            *buf = sign;
        }
        --size;
        ++buf;
    }

#ifdef PRINTF_SPECIAL
    if(type & SPECIAL)
    {
        if(base == 2)
        {
            if (buf < end)
                *buf = '0';
            ++ buf;
            if (buf < end)
                *buf = 'b';
            ++ buf;
        }
        else if(base == 8)
        {
            if (buf < end)
                *buf = '0';
            ++ buf;
        }
        else if(base == 16)
        {
            if(buf < end)
            {
                *buf = '0';
            }

            ++ buf;
            if(buf < end)
            {
                *buf = type & LARGE ? 'X' : 'x';
            }
            ++buf;
        }
    }
#endif /* PRINTF_SPECIAL */

    /* no align to the left */
    if(!(type & LEFT))
    {
        while(size-- > 0)
        {
            if(buf < end)
            {
                *buf = c;
            }

            ++buf;
        }
    }

#ifdef PRINTF_PRECISION
    while(i < precision--)
    {
        if(buf < end)
        {
            *buf = '0';
        }

        ++buf;
    }
#endif /* RT_PRINTF_PRECISION */

    /* put number in the temporary buffer */
    while(i-- > 0 && (precision_bak != 0))
    {
        if(buf < end)
        {
            *buf = tmp[i];
        }

        ++buf;
    }

    while(size-- > 0)
    {
        if (buf < end)
        {
            *buf = ' ';
        }

        ++buf;
    }

    return buf;
}

/**
 * @brief  This function will fill a formatted string to buffer.
 *
 * @param  buf is the buffer to save formatted string.
 * @param  size is the size of buffer.
 * @param  fmt is the format parameters.
 * @param  args is a list of variable parameters.
 * @return The number of characters actually written to buffer.
 */
int __vsnprintf(char *buf, u32 size, const char *fmt, va_list args)
{
#ifdef PRINTF_USING_LONGLONG
    unsigned long long num = 0;
#else
    unsigned long num = 0;
#endif /* PRINTF_USING_LONGLONG */
    int i = 0, len = 0;
    char *str = NULL, *end = NULL, c = 0;
    const char *s = NULL;

    u8 base = 0;            /* the base of number */
    u8 flags = 0;           /* flags to print number */
    u8 qualifier = 0;       /* 'h', 'l', or 'L' for integer fields */
    s8 field_width = 0;     /* width of output field */

#ifdef PRINTF_PRECISION
    int precision = 0;      /* min. # of digits for integers and max for a string */
#endif /* PRINTF_PRECISION */

    str = buf;
    end = buf + size;

    /* Make sure end is always >= buf */
    if(end < buf)
    {
        end  = ((char *) - 1);
        size = end - buf;
    }

    for(; *fmt ; ++fmt)
    {
        if(*fmt != '%')
        {
            if(str < end)
            {
                *str = *fmt;
            }

            ++str;
            continue;
        }

        /* process flags */
        flags = 0;

        while(1)
        {
            /* skips the first '%' also */
            ++fmt;
            if(*fmt == '-') flags |= LEFT;
            else if(*fmt == '+') flags |= PLUS;
            else if(*fmt == ' ') flags |= SPACE;
            else if(*fmt == '#') flags |= SPECIAL;
            else if(*fmt == '0') flags |= ZEROPAD;
            else break;
        }

        /* get field width */
        field_width = -1;
        if(_ISDIGIT(*fmt))
        {
            field_width = skip_atoi(&fmt);
        }
        else if(*fmt == '*')
        {
            ++fmt;
            /* it's the next argument */
            field_width = va_arg(args, int);
            if(field_width < 0)
            {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

#ifdef PRINTF_PRECISION
        /* get the precision */
        precision = -1;
        if(*fmt == '.')
        {
            ++fmt;
            if(_ISDIGIT(*fmt))
            {
                precision = skip_atoi(&fmt);
            }
            else if(*fmt == '*')
            {
                ++fmt;
                /* it's the next argument */
                precision = va_arg(args, int);
            }
            if(precision < 0)
            {
                precision = 0;
            }
        }
#endif /* RT_PRINTF_PRECISION */
        /* get the conversion qualifier */
        qualifier = 0;
#ifdef PRINTF_USING_LONGLONG
        if(*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
#else
        if(*fmt == 'h' || *fmt == 'l')
#endif /* PRINTF_USING_LONGLONG */
        {
            qualifier = *fmt;
            ++ fmt;
#ifdef PRINTF_USING_LONGLONG
            if(qualifier == 'l' && *fmt == 'l')
            {
                qualifier = 'L';
                ++fmt;
            }
#endif /* KPRINTF_USING_LONGLONG */
        }

        /* the default base */
        base = 10;

        switch(*fmt)
        {
			case 'c':
				if (!(flags & LEFT))
				{
					while(--field_width > 0)
					{
						if(str < end) *str = ' ';
						++str;
					}
				}

				/* get character */
				c = (u8)va_arg(args, int);
				if(str < end)
				{
					*str = c;
				}
				++str;

				/* put width */
				while(--field_width > 0)
				{
					if(str < end) *str = ' ';
					++str;
				}
				continue;
			case 's':
				s = va_arg(args, char *);
				if(!s)
				{
					s = "(NULL)";
				}

				for(len = 0; (len != field_width) && (s[len] != '\0'); len++);
#ifdef PRINTF_PRECISION
				if(precision > 0 && len > precision)
				{
					len = precision;
				}
#endif /* PRINTF_PRECISION */

				if(!(flags & LEFT))
				{
					while(len < field_width--)
					{
						if(str < end) *str = ' ';
						++str;
					}
				}

				for(i = 0; i < len; ++i)
				{
					if(str < end) *str = *s;
					++str;
					++s;
				}

				while(len < field_width--)
				{
					if(str < end) *str = ' ';
					++str;
				}
				continue;
			case 'p':
				if(field_width == -1)
				{
					field_width = sizeof(void *) << 1;
#ifdef PRINTF_SPECIAL
					field_width += 2; /* `0x` prefix */
					flags |= SPECIAL;
#endif
					flags |= ZEROPAD;
				}
#ifdef PRINTF_PRECISION
				str = print_number(str, end,
								   (unsigned long)va_arg(args, void *),
								   16, qualifier, field_width, precision, flags);
#else
				str = print_number(str, end,
								   (unsigned long)va_arg(args, void *),
								   16, qualifier, field_width, flags);
#endif
				continue;
			case '%':
				if(str < end)
				{
					*str = '%';
				}
				++str;
				continue;
			/* integer number formats - set up the flags and "break" */
			case 'b':
				base = 2;
				break;
			case 'o':
				base = 8;
				break;
			case 'X':
				flags |= LARGE;
			case 'x':
				base = 16;
				break;
			case 'd':
			case 'i':
				flags |= SIGN;
			case 'u':
				break;
			default:
				if(str < end)
				{
					*str = '%';
				}
				++str;

				if(*fmt)
				{
					if(str < end)
					{
						*str = *fmt;
					}
					++str;
				}
				else
				{
					--fmt;
				}
				continue;
			}

#ifdef PRINTF_USING_LONGLONG
			if(qualifier == 'L')
			{
				num = va_arg(args, unsigned long long);
			}
			else if(qualifier == 'l')
#else
			if(qualifier == 'l')
#endif /* PRINTF_USING_LONGLONG */
			{
				num = va_arg(args, unsigned long);
			}
			else if(qualifier == 'h')
			{
				num = (u16)va_arg(args, s32);
				if(flags & SIGN)
				{
					num = (s16)num;
				}
			}
			else
			{
				num = (u32)va_arg(args, unsigned long);
			}
#ifdef PRINTF_PRECISION
			str = print_number(str, end, num, base, qualifier, field_width, precision, flags);
#else
			str = print_number(str, end, num, base, qualifier, field_width, flags);
#endif
    }

    if(size > 0)
    {
        if(str < end)
        {
            *str = '\0';
        }
        else
        {
            end[-1] = '\0';
        }
    }

    /* the trailing null byte doesn't count towards the total
    * ++str;
    */
    return str - buf;
}

/**
 * This function will print a formatted string on system console
 *
 * @param fmt the format
 */
int printf(const char *fmt, ...)
{
    va_list args;
    u32 length;
    static char log_buf[CONSOLEBUF_SIZE];

    va_start(args, fmt);
    /* the return value of vsnprintf is the number of bytes that would be
     * written to buffer had if the size of the buffer been sufficiently
     * large excluding the terminating null byte. If the output string
     * would be larger than the rt_log_buf, we have to adjust the output
     * length. */
    length = __vsnprintf(log_buf, sizeof(log_buf) - 1, fmt, args);
    if(length > CONSOLEBUF_SIZE - 1)
	{
        length = CONSOLEBUF_SIZE - 1;
	}
    hw_console_output(log_buf);

    va_end(args);
	
	return length;
}

#elif defined (__ICCARM__)

#else
	#error Unknown compiler.
#endif
/**
  * @}
  */


