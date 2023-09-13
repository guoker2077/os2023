#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>


#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void my_putch(char ch)
{
  putch(ch);
}

int printf(const char *fmt, ...) {
  
  if(fmt==NULL) return -1;

  size_t ret_num = 0;
  char *pstr = fmt;
  int int_value = 0;
  size_t hex_value = 0;
  //float flo_value=0;
  char *str_value = NULL;
  int value_count = 0;

  va_list p_args;
  va_start(p_args,fmt);
  while(*pstr!='\0')
  {
    switch(*pstr)
    {
      case '%':
        pstr++;
        switch(*pstr)
        {
          case '%':
            my_putch('%');
            ret_num++;
            pstr++;
            break;
          case 'd':
            int_value = va_arg(p_args,int);
            if(int_value<0)
            {
              my_putch('-');
              ret_num++;
              int_value = -int_value;
            }
            char ch_int[32]={0};
            int temp;
            while(int_value)
            {
              temp = int_value %10;
              int_value/=10;
              ch_int[value_count++] = temp;
            }
            ret_num += value_count;
            while(value_count)
              my_putch(ch_int[--value_count]);
            pstr++;
            break;
          case 'x':
              char match[]={'0','1','2','3','4','5','6','7',
              '8','9','a','b','c','d','e','f'};
              hex_value = va_arg(p_args,size_t);
              char ch_hex[16]={0};
              int temp;
              while(hex_value)
              {
                temp = int_value % 16;
                hex_value/=16;
                ch_hex[value_count++]=match[temp-1];
              }
              ret_num += value_count;
              while(value_count)
                my_putch(ch_hex[--value_count]);
              pstr++;
              break;
          case 'c':
            int_value = va_arg(p_args,int);
            my_putch((char)int_value);
            ret_num++;
            break;
          case 's':
            str_value = va_arg(p_args,char *);
            while(*str_value)
            {
              my_putch(*str_value);
              value_count++;
              str_value++;
            }
            ret_num+=value_count;
            pstr++;
            break;
          default:
            my_putch('!');
            break;
        }
        break;
      case '\t':
        my_putch(*pstr);
        ret_num+=4;
        pstr++;
        break;
      default:
        my_putch(*pstr);
        ret_num++;
        pstr++;
        break;
    }
  }
  va_end(p_args);

  return ret_num;
  //panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
