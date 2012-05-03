#include "stdafx.h"
#include "XKernel.h"

#define _XZEROPAD 1               // Pad with zero
#define _XSIGN    2               // Unsigned/signed long
#define _XPLUS    4               // Show plus
#define _XSPACE   8               // Space if plus
#define _XLEFT    16              // Left justified
#define _XSPECIAL 32              // 0x
#define _XLARGE   64              // Use 'ABCDEF' instead of 'abcdef'

#define _x_is_digit(c) ((c) >= '0' && (c) <= '9')

static char *_xdata_digits = "0123456789abcdefghijklmnopqrstuvwxyz";
static char *_xdata_upper_digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static size_t _xstrnlen(const char *s, size_t count)
{
  const char *sc;
  for (sc = s; *sc != '\0' && count--; ++sc);
  return sc - s;
}

static int _xskip_atoi(const char **s)
{
  int i = 0;
  while (_x_is_digit(**s)) i = i*10 + *((*s)++) - '0';
  return i;
}

static char *_xnumber(char *str, long num, int base, int size, int precision, int type)
{
  char c, sign, tmp[66];
  char *dig = _xdata_digits;
  int i;

  if(type & _XLARGE)  dig = _xdata_upper_digits;
  if(type & _XLEFT) type &= ~_XZEROPAD;
  if(base < 2 || base > 36) return 0;
  
  c = (type & _XZEROPAD) ? '0' : ' ';
  sign = 0;
  if(type & _XSIGN)
  {
    if(num < 0)
    {
      sign = '-';
      num = -num;
      size--;
    }
    else if(type & _XPLUS)
    {
      sign = '+';
      size--;
    }
    else if(type & _XSPACE)
    {
      sign = ' ';
      size--;
    }
  }

  if(type & _XSPECIAL)
  {
    if(base == 16)
      size -= 2;
    else if(base == 8)
      size--;
  }

  i = 0;

  if(num == 0)
    tmp[i++] = '0';
  else
  {
    while (num != 0)
    {
      tmp[i++] = dig[((unsigned long) num) % (unsigned) base];
      num = ((unsigned long) num) / (unsigned) base;
    }
  }

  if(i > precision) precision = i;
  size -= precision;
  if(!(type & (_XZEROPAD | _XLEFT))) while (size-- > 0) *str++ = ' ';
  if(sign) *str++ = sign;
  
  if(type & _XSPECIAL)
  {
    if(base == 8)
      *str++ = '0';
    else if(base == 16)
    {
      *str++ = '0';
      *str++ = _xdata_digits[33];
    }
  }

  if(!(type & _XLEFT)) while (size-- > 0) *str++ = c;
  while (i < precision--) *str++ = '0';
  while (i-- > 0) *str++ = tmp[i];
  while (size-- > 0) *str++ = ' ';

  return str;
}

static char *_xeaddr(char *str, unsigned char *addr, int size, int precision, int type)
{
  char tmp[24];
  char *dig = _xdata_digits;
  int i, len;

  if(type & _XLARGE)  dig = _xdata_upper_digits;
  len = 0;
  for (i = 0; i < 6; i++)
  {
    if(i != 0) tmp[len++] = ':';
    tmp[len++] = dig[addr[i] >> 4];
    tmp[len++] = dig[addr[i] & 0x0F];
  }

  if(!(type & _XLEFT)) while (len < size--) *str++ = ' ';
  for (i = 0; i < len; ++i) *str++ = tmp[i];
  while (len < size--) *str++ = ' ';

  return str;
}

static char *_xiaddr(char *str, unsigned char *addr, int size, int precision, int type)
{
  char tmp[24];
  int i, n, len;

  len = 0;
  for (i = 0; i < 4; i++)
  {
    if(i != 0) tmp[len++] = '.';
    n = addr[i];
    
    if(n == 0)
      tmp[len++] = _xdata_digits[0];
    else
    {
      if(n >= 100) 
      {
        tmp[len++] = _xdata_digits[n / 100];
        n = n % 100;
        tmp[len++] = _xdata_digits[n / 10];
        n = n % 10;
      }
      else if(n >= 10) 
      {
        tmp[len++] = _xdata_digits[n / 10];
        n = n % 10;
      }

      tmp[len++] = _xdata_digits[n];
    }
  }

  if(!(type & _XLEFT)) while (len < size--) *str++ = ' ';
  for (i = 0; i < len; ++i) *str++ = tmp[i];
  while (len < size--) *str++ = ' ';

  return str;
}

static void _xcfltcvt(double value, char *buffer, char fmt, int precision)
{
  int decpt, sign, exp, pos;
  char *digits = NULL;  
  int capexp = 0;
  int magnitude;

  if(fmt == 'G' || fmt == 'E')
  {
    capexp = 1;
    fmt += 'a' - 'A';
  }

  if(fmt == 'g')
  {
    digits = ecvt(value, precision, &decpt, &sign);
    magnitude = decpt - 1;
    if(magnitude < -4  ||  magnitude > precision - 1)
    {
      fmt = 'e';
      precision -= 1;
    }
    else
    {
      fmt = 'f';
      precision -= decpt;
    }
  }

  if(fmt == 'e')
  {
    digits = ecvt(value, precision + 1, &decpt, &sign);

    if(sign) *buffer++ = '-';
    *buffer++ = *digits;
    if(precision > 0) *buffer++ = '.';
    memcpy(buffer, digits + 1, precision);
    buffer += precision;
    *buffer++ = capexp ? 'E' : 'e';

    if(decpt == 0)
    {
      if(value == 0.0)
        exp = 0;
      else
        exp = -1;
    }
    else
      exp = decpt - 1;

    if(exp < 0)
    {
      *buffer++ = '-';
      exp = -exp;
    }
    else
      *buffer++ = '+';

    buffer[2] = (exp % 10) + '0';
    exp = exp / 10;
    buffer[1] = (exp % 10) + '0';
    exp = exp / 10;
    buffer[0] = (exp % 10) + '0';
    buffer += 3;
  }
  else if(fmt == 'f')
  {
    digits = fcvt(value, precision, &decpt, &sign);
    if(sign) *buffer++ = '-';
    if(*digits)
    {
      if(decpt <= 0)
      {
        *buffer++ = '0';
        *buffer++ = '.';
        for (pos = 0; pos < -decpt; pos++) *buffer++ = '0';
        while (*digits) *buffer++ = *digits++;
      }
      else
      {
        pos = 0;
        while (*digits)
        {
          if(pos++ == decpt) *buffer++ = '.';
          *buffer++ = *digits++;
        }
      }
    }
    else
    {
      *buffer++ = '0';
      if(precision > 0)
      {
        *buffer++ = '.';
        for (pos = 0; pos < precision; pos++) *buffer++ = '0';
      }
    }
  }

  *buffer = '\0';
}

static void _xforcdecpt(char *buffer)
{
  while (*buffer)
  {
    if(*buffer == '.') return;
    if(*buffer == 'e' || *buffer == 'E') break;
    buffer++;
  }

  if(*buffer)
  {
    int n = strlen(buffer);
    while (n > 0) 
    {
      buffer[n + 1] = buffer[n];
      n--;
    }

    *buffer = '.';
  }
  else
  {
    *buffer++ = '.';
    *buffer = '\0';
  }
}

static void _xcropzeros(char *buffer)
{
  char *stop;

  while (*buffer && *buffer != '.') buffer++;
  if(*buffer++)
  {
    while (*buffer && *buffer != 'e' && *buffer != 'E') buffer++;
    stop = buffer--;
    while (*buffer == '0') buffer--;
    if(*buffer == '.') buffer--;
    while (*++buffer = *stop++);
  }
}

static char *_xflt(char *str, double num, int size, int precision, char fmt, int flags)
{
  char tmp[80];
  char c, sign;
  int n, i;

  // Left align means no zero padding
  if(flags & _XLEFT) flags &= ~_XZEROPAD;

  // Determine padding and sign char
  c = (flags & _XZEROPAD) ? '0' : ' ';
  sign = 0;
  if(flags & _XSIGN)
  {
    if(num < 0.0)
    {
      sign = '-';
      num = -num;
      size--;
    }
    else if(flags & _XPLUS)
    {
      sign = '+';
      size--;
    }
    else if(flags & _XSPACE)
    {
      sign = ' ';
      size--;
    }
  }

  // Compute the precision value
  if(precision < 0)
    precision = 6; // Default precision: 6
  else if(precision == 0 && fmt == 'g')
    precision = 1; // ANSI specified

  // Convert floating point number to text
  _xcfltcvt(num, tmp, fmt, precision);

  // '#' and precision == 0 means force a decimal point
  if((flags  & _XSPECIAL) && precision == 0) _xforcdecpt(tmp);

  // 'g' format means crop zero unless '#' given
  if(fmt == 'g' && !(flags & _XSPECIAL)) _xcropzeros(tmp);

  n = strlen(tmp);

  // Output number with alignment and padding
  size -= n;
  if(!(flags & (_XZEROPAD | _XLEFT))) while (size-- > 0) *str++ = ' ';
  if(sign) *str++ = sign;
  if(!(flags & _XLEFT)) while (size-- > 0) *str++ = c;
  for (i = 0; i < n; i++) *str++ = tmp[i];
  while (size-- > 0) *str++ = ' ';

  return str;
}

#define _xgetarg(ap,t) ( *(t *)(ap) )

int _xvsprintf(char *buf, const char *fmt, void** args)
//int xvsprintf(char *buf, const char *fmt, va_list args)
{
	int len;
	unsigned long num;
	int i, base;
	char *str;
	char *s;

	int argindex = 0;

	int flags;            // Flags to number()

	int field_width;      // Width of output field
	int precision;        // Min. # of digits for integers; max number of chars for from string
	int qualifier;        // 'h', 'l', or 'L' for integer fields

	for (str = buf; *fmt; fmt++)
	{
			if(*fmt != '%')
			{
				*str++ = *fmt;
				continue;
			}
			
			if(*(fmt+1) == '%')
			{
				fmt++;
				*str++ = *fmt;
				continue;
			}
                  
			// Process flags
			flags = 0;

repeat:
			fmt++; // This also skips first '%'
			switch (*fmt)
			{
			case '-': flags |= _XLEFT;		goto repeat;
			case '+': flags |= _XPLUS;		goto repeat;
			case ' ': flags |= _XSPACE;		goto repeat;
			case '#': flags |= _XSPECIAL;	goto repeat;
			case '0': flags |= _XZEROPAD;	goto repeat;
			}
          
			// Get field width
			field_width = -1;
			if(_x_is_digit(*fmt))
			{
				field_width = _xskip_atoi(&fmt);
			}
			else if(*fmt == '*')
			{
				fmt++;
				//field_width = va_arg(args, int);				
				field_width = *((int*)(args[argindex]));				
				argindex++;
		
				if(field_width < 0)
				{
					field_width = -field_width;
					flags |= _XLEFT;
				}
			}

			// Get the precision
			precision = -1;

			if(*fmt == '.')
			{
				++fmt;    
				if(_x_is_digit(*fmt))
				{
					precision = _xskip_atoi(&fmt);
				}
				else if(*fmt == '*')
				{
					++fmt;
					//precision = va_arg(args, int);
					precision = *((int*)(args[argindex]));
					argindex++;
				}

				if(precision < 0) precision = 0;
			}

			// Get the conversion qualifier
			qualifier = -1;

			if(*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
			{
				qualifier = *fmt;
				fmt++;
			}

			if( !args[argindex] )
			{
				continue;
			}

			// Default base
			base = 10;

			switch (*fmt)
			{
			  case 'c':
					if(!(flags & _XLEFT)) while (--field_width > 0) *str++ = ' ';
					
					//*str++ = (unsigned char) va_arg(args, int);
					*str++ = *((unsigned char*)(args[argindex]));
					argindex++;

					while (--field_width > 0) *str++ = ' ';
					continue;

			  case 's':
					//s = va_arg(args, char *);
					s = ((char*)(args[argindex]));
					argindex++;

					if(!s) s = "<NULL>";
					len = _xstrnlen(s, precision);
					if(!(flags & _XLEFT)) while (len < field_width--) *str++ = ' ';
					for (i = 0; i < len; ++i) *str++ = *s++;
					while (len < field_width--) *str++ = ' ';
					continue;

			  case 'p':
					{						
						if(field_width == -1)
						{
							field_width = 2 * sizeof(void *);
							flags |= _XZEROPAD;
						}

						unsigned long _argnum = *((unsigned long *)(args[argindex]));
						argindex++;

						//str = _xnumber(str, (unsigned long) va_arg(args, void *), 16, field_width, precision, flags);
						str = _xnumber(str, _argnum, 16, field_width, precision, flags);
					}
					continue;

			  case 'n':
					if(qualifier == 'l')
					{
						//long *ip = va_arg(args, long *);
						long *ip = ((long *)(args[argindex]));
						argindex++;

						*ip = (str - buf);
					}
					else
					{
						//int *ip = va_arg(args, int *);
						int *ip = ((int *)(args[argindex]));
						argindex++;

						*ip = (str - buf);
					}
					continue;

			  case 'A':
					flags |= _XLARGE;

			  case 'a':
					{
						unsigned char *ip = ((unsigned char *)(args[argindex]));
						argindex++;

						if(qualifier == 'l')
							//str = _xeaddr(str, va_arg(args, unsigned char *), field_width, precision, flags);
							str = _xeaddr(str, ip, field_width, precision, flags);
						else
							//str = _xiaddr(str, va_arg(args, unsigned char *), field_width, precision, flags);
							str = _xiaddr(str, ip, field_width, precision, flags);
						continue;
					}

			  // Integer number formats - set up the flags and "break"
			  case 'o':
					base = 8;
					break;

			  case 'X':
					flags |= _XLARGE;

			  case 'x':
					base = 16;
					break;

			  case 'd':
			  case 'i':
					flags |= _XSIGN;

			  case 'u':
					break;

			  case 'E':
			  case 'G':
			  case 'e':			  
			  case 'g':
					{
						double fdata = *((double*)(args[argindex]));
						argindex++;
						//str = _xflt(str, va_arg(args, double), field_width, precision, *fmt, flags | _XSIGN);
						str = _xflt(str, fdata, field_width, precision, *fmt, flags | _XSIGN);						
					}
					continue;
			  case 'f':
				  {
					  float fdata = *((float*)(args[argindex]));
					  argindex++;
					  //str = _xflt(str, va_arg(args, double), field_width, precision, *fmt, flags | _XSIGN);
					  str = _xflt(str, fdata, field_width, precision, *fmt, flags | _XSIGN);						
				  }
				  continue;

			  default:
					if(*fmt != '%') *str++ = '%';

					if(*fmt)
						*str++ = *fmt;
					else
						--fmt;

				continue;
			}

			if(qualifier == 'l')
			{
				//num = va_arg(args, unsigned long);
				num = *((unsigned long*)(args[argindex]));
				argindex++;
			}
			else if(qualifier == 'h')
			{
				if(flags & _XSIGN)
				{
					//num = va_arg(args, short);
					num = *((short*)(args[argindex]));
					argindex++;
				}
				else
				{
					//num = va_arg(args, unsigned short);
					num = *((unsigned short*)(args[argindex]));
					argindex++;
				}
			}
			else if(flags & _XSIGN)
			{
				//num = va_arg(args, int);
				num = *((int*)(args[argindex]));
				argindex++;
			}
			else
			{
				//num = va_arg(args, unsigned int);
				num = *((unsigned int*)(args[argindex]));
				argindex++;
			}

			str = _xnumber(str, num, base, field_width, precision, flags);
	  }

	  *str = '\0';
	  return str - buf;
}