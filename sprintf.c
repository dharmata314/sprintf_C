#include "sprintf.h"

int add_int(char *buffer, int value) {
  int written = 0;

  do {
    buffer[written++] = '0' + (value % 10);
    value /= 10;
  } while (value > 0);

  for (int i = 0, j = written - 1; i < j; i++, j--) {
    char temp = buffer[i];
    buffer[i] = buffer[j];
    buffer[j] = temp;
  }

  return written;
}

void add_float(char **bufferPtr, double value, int width, int precision) {
  char tempBuffer[BUFFER_SIZE];
  int written = 0;
  int isNegative = 0;

  if (value < 0) {
    isNegative = 1;
    value = -value;
  }

  int intPart = (int)value;
  written += add_int(tempBuffer + written, intPart);

  if (precision >= 0) {
    tempBuffer[written++] = '.';
    double fracPart = value - intPart;

    for (int i = 0; i < precision; i++) {
      fracPart *= 10;
      int digit = (int)fracPart;
      tempBuffer[written++] = '0' + digit;
      fracPart -= digit;
    }

    if (fracPart >= 0.5) {
      tempBuffer[written - 1]++;
      for (int i = written - 1; i > 0 && tempBuffer[i] > '9'; i--) {
        tempBuffer[i] -= 10;
        tempBuffer[i - 1]++;
      }
    }
  } else {
    tempBuffer[written++] = '.';
    double fracPart = value - intPart;

    for (int i = 0; i < 6; i++) {
      fracPart *= 10;
      int digit = (int)fracPart;
      tempBuffer[written++] = '0' + digit;
      fracPart -= digit;
    }

    if (fracPart >= 0.5) {
      tempBuffer[written - 1]++;
      for (int i = written - 1; i > 0 && tempBuffer[i] > '9'; i--) {
        tempBuffer[i] -= 10;
        tempBuffer[i - 1]++;
      }
    }
  }

  for (int i = written; i < width; i++) {
    *(*bufferPtr) = ' ';
    (*bufferPtr)++;
  }

  if (isNegative) {
    *(*bufferPtr) = '-';
    (*bufferPtr)++;
  }

  for (int i = 0; i < written; i++) {
    *(*bufferPtr) = tempBuffer[i];
    (*bufferPtr)++;
  }
}

void add_int_buffer(char **bufferPtr, long value, int width) {
  char tempBuffer[BUFFER_SIZE];
  int written = 0;
  int isNegative = 0;

  if (value < 0) {
    isNegative = 1;
    value = -value;
  }

  do {
    tempBuffer[written++] = '0' + (value % 10);
    value /= 10;
  } while (value > 0);

  if (isNegative) {
    tempBuffer[written++] = '-';
  }

  for (int i = written; i < width; i++) {
    *(*bufferPtr) = ' ';
    (*bufferPtr)++;
  }

  for (int i = written - 1; i >= 0; i--) {
    *(*bufferPtr) = tempBuffer[i];
    (*bufferPtr)++;
  }
}

int my_sprintf(char *buffer, const char *format, ...) {
  char *bufferPtr = buffer;
  const char *formatPtr = format;

  va_list args;
  va_start(args, format);

  while (*formatPtr != '\0') {
    if (*formatPtr == '%') {
      formatPtr++;

      int flag_plus = 0;
      int flag_minus = 0;
      int flag_space = 0;

      while (*formatPtr == '+' || *formatPtr == '-' || *formatPtr == ' ') {
        if (*formatPtr == '+') {
          flag_plus = 1;
        } else if (*formatPtr == '-') {
          flag_minus = 1;
        } else if (*formatPtr == ' ') {
          flag_space = 1;
        }
        formatPtr++;
      }

      int width = 0;
      while (*formatPtr >= '0' && *formatPtr <= '9') {
        width = width * 10 + (*formatPtr - '0');
        formatPtr++;
      }

      int precision = -1;
      if (*formatPtr == '.') {
        formatPtr++;
        precision = 0;
        while (*formatPtr >= '0' && *formatPtr <= '9') {
          precision = precision * 10 + (*formatPtr - '0');
          formatPtr++;
        }
      }

      int length_h = 0;
      int length_l = 0;
      while (*formatPtr == 'h' || *formatPtr == 'l') {
        if (*formatPtr == 'h') {
          length_h++;
        } else if (*formatPtr == 'l') {
          length_l++;
        }
        formatPtr++;
      }

      char specifier = *formatPtr;

      switch (specifier) {
        case 'u': {
          if (length_l > 0) {
            unsigned long num = va_arg(args, unsigned long);
            add_int_buffer(&bufferPtr, num, width);
          }

          else if (length_h > 0) {
            unsigned short num = va_arg(args, unsigned int);
            add_int_buffer(&bufferPtr, num, width);
          }

          else {
            unsigned int num = va_arg(args, unsigned int);
            add_int_buffer(&bufferPtr, num, width);
          }
          break;
        }

        case 'c': {
          char c = va_arg(args, int);
          if (flag_minus) {
            *bufferPtr = c;
            bufferPtr++;
            for (int i = 1; i < width; i++) {
              *bufferPtr = ' ';
              bufferPtr++;
            }
          } else {
            for (int i = 0; i < width - 1; i++) {
              *bufferPtr = ' ';
              bufferPtr++;
            }
            *bufferPtr = c;
            bufferPtr++;
          }
          break;
        }

        case 'd': {
          if (length_l > 0) {
            long num = va_arg(args, long);
            int isNegative = (num < 0);

            if ((flag_plus || flag_space) && !isNegative) {
              *bufferPtr = (flag_plus) ? '+' : ' ';
              bufferPtr++;
              width--;
            }
            add_int_buffer(&bufferPtr, num, width);
          }

          else if (length_h > 0) {
            short num = va_arg(args, int);
            int isNegative = (num < 0);

            if ((flag_plus || flag_space) && !isNegative) {
              *bufferPtr = (flag_plus) ? '+' : ' ';
              bufferPtr++;
              width--;
            }
            add_int_buffer(&bufferPtr, num, width);
          }

          else {
            int num = va_arg(args, int);
            int isNegative = (num < 0);

            if ((flag_plus || flag_space) && !isNegative) {
              *bufferPtr = (flag_plus) ? '+' : ' ';
              bufferPtr++;
              width--;
            }
            add_int_buffer(&bufferPtr, num, width);
          }
          break;
        }

        case 'f': {
          if (length_l > 1) {
            long double f = va_arg(args, long double);
            int isNegative = (f < 0);

            if ((flag_plus || flag_space) && !isNegative) {
              *bufferPtr = (flag_plus) ? '+' : ' ';
              bufferPtr++;
              width--;
            }
            add_float(&bufferPtr, f, width, precision);
          }

          else if (length_l == 1) {
            double f = va_arg(args, double);
            int isNegative = (f < 0);

            if ((flag_plus || flag_space) && !isNegative) {
              *bufferPtr = (flag_plus) ? '+' : ' ';
              bufferPtr++;
              width--;
            }

            add_float(&bufferPtr, f, width, precision);
          } else {
            double f = va_arg(args, double);
            int isNegative = (f < 0);

            if ((flag_plus || flag_space) && !isNegative) {
              *bufferPtr = (flag_plus) ? '+' : ' ';
              bufferPtr++;
              width--;
            }

            if (isNegative || flag_plus || flag_space) {
              width--;
            }

            add_float(&bufferPtr, f, width, precision);
          }
          break;
        }

        case 's': {
          char *s = va_arg(args, char *);
          int len = strlen(s);
          int written;

          if (precision == -1) {
            written = len;
          } else {
            if (precision < len) {
              written = precision;
            } else {
              written = len;
            }
          }
          if (flag_minus) {
            for (int i = 0; i < written; i++) {
              *bufferPtr = s[i];
              bufferPtr++;
            }
            for (int i = written; i < width; i++) {
              *bufferPtr = ' ';
              bufferPtr++;
            }
          } else {
            for (int i = written; i < width; i++) {
              *bufferPtr = ' ';
              bufferPtr++;
            }
            for (int i = 0; i < written; i++) {
              *bufferPtr = s[i];
              bufferPtr++;
            }
          }
          break;
        }

        case '%':
          *bufferPtr = '%';
          bufferPtr++;
          break;
      }

      formatPtr++;
    }

    else {
      *bufferPtr = *formatPtr;
      bufferPtr++;
      formatPtr++;
    }
  }

  *bufferPtr = '\0';

  va_end(args);

  return bufferPtr - buffer;
}
