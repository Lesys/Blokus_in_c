#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <arpa/inet.h>

const int SHORT_SIZE = sizeof(unsigned short);
const int INT_SIZE = sizeof(unsigned int);

// Serialize the arguments according to the formatting string
// It only accepts:
//   %h (unsigned short)
//   %i (unsigned int)
//   %s (string, length should not exceed 65535)
//   () (represents an array, you should pass the length of the array
//       in a short int then the array)
// Returns:
//   - length of data put in buff on success
//   - 0 otherwise

int serializef(unsigned char** buff, char* format, ...) {

    // Empty format string
    if (!strlen(format)) {
        return 0;
    }

    else {

        int i, j;

        va_list arg;
        va_start(arg, format);
        
        unsigned short len;
        unsigned short tmp_short;
        unsigned int tmp_int;
        char* tmp_str;
        unsigned short* atmp_short;
        unsigned int* atmp_int;
        char** atmp_str;

        int offset = 0;

        for (i = 0; format[i] == '(' || (format[i] == '%' && format[++i]); i++) {
            switch(format[i]) {
                
                case 'h':
                       // Unsigned short 
                    tmp_short = (unsigned short) va_arg(arg, unsigned int);
                    tmp_short = htons(tmp_short);
                    memcpy(*buff + offset, &tmp_short, SHORT_SIZE);
                    offset += SHORT_SIZE;
                    break;

                case 'i':
                    // Unsigned int
                    tmp_int = htonl(va_arg(arg, unsigned int));
                    memcpy(*buff + offset, &tmp_int, INT_SIZE);
                    offset += INT_SIZE;
                    break;

                case 's':
                    // String
                    // We first add the length of the string
                    tmp_str = va_arg(arg, char*);
                    if (tmp_str) {
                        len = strlen(tmp_str) + 1;
                        len = htons(len);
                    }
                    else {
                        len = 0;
                    }
                    memcpy(*buff + offset, &len, SHORT_SIZE);

                    // Then we add the string itself
                    len = ntohs(len);
                    offset += SHORT_SIZE;
                    memcpy(*buff + offset, tmp_str, len);
                    offset += len;
                    break;

                case '(':
                    // Array of values
                    // We first add the length of the array
                    tmp_short = (unsigned short) va_arg(arg, unsigned int);
                    tmp_short = htons(tmp_short);
                    memcpy(*buff + offset, &tmp_short, SHORT_SIZE);
                    offset += SHORT_SIZE;
                    tmp_short = ntohs(tmp_short);

                    while (format[++i] == '%' && format[++i]) {

                        switch(format[i]) {
                            
                            case 'h':
                                // Unsigned short 
                                atmp_short = va_arg(arg, unsigned short*);
                                for (j = 0; j < tmp_short; j++) {
                                    atmp_short[j] = htons(atmp_short[j]);
                                    memcpy(*buff + offset, &atmp_short[j], SHORT_SIZE);
                                    offset += SHORT_SIZE;
                                }
                                break;

                            case 'i':
                                // Unsigned int
                                atmp_int = va_arg(arg, unsigned int*);
                                for (j = 0; j < tmp_short; j++) {
                                    atmp_int[j] = htonl(atmp_int[j]);
                                    memcpy(*buff + offset, &atmp_int[j], INT_SIZE);
                                    offset += INT_SIZE;
                                }
                                break;

                            case 's':
                                // String
                                atmp_str = va_arg(arg, char**);
                                for (j = 0; j < tmp_short; j++) {
                                    // We first add the length of the string
                                    if (atmp_str[j]) {
                                        len = strlen(atmp_str[j]) + 1;
                                        len = htons(len);
                                    }
                                    else {
                                        len = 0;
                                    }
                                    memcpy(*buff + offset, &len, SHORT_SIZE);

                                    // Then we add the string itself
                                    len = ntohs(len);
                                    offset += SHORT_SIZE;
                                    memcpy(*buff + offset, atmp_str[j], len);
                                    offset += len;
                                }
                                break;

                            default :
                                return 0;
                        }
                    }
                    break;
                default :
                    return 0;
            }
        }
        return offset;
    }
}

// Deserialize the arguments according to the formatting string
// It only accepts:
//   %h (unsigned short)
//   %i (unsigned int)
//   %s (string, length should not exceed 65535)
//   () (represents an array, you should pass the length of the array
//       in a short int then the array)
// Returns:
//   - number of fields read
//   - 0 otherwise
int deserializef(unsigned char* buff, char* format, ...) {
    
    // Empty format string
    if (!strlen(format)) {
        return 0;
    }

    else {
        
        int i, j, done = 0;

        va_list arg;
        va_start(arg, format);

        unsigned short len;
        unsigned short alen;
        unsigned short* ptr_short;
        unsigned int* ptr_int;
        char** ptr_str;
        unsigned short** aptr_short;
        unsigned int** aptr_int;
        char*** aptr_str;
        
        unsigned int offset = 0;

        for (i = 0; format[i] == '(' || (format[i] == '%' && format[++i]); i++) {
          
            switch(format[i]) {
                
                case 'h':
                    // Unsigned short 
                    ptr_short = va_arg(arg, unsigned short*);
                    memcpy(ptr_short, buff + offset, SHORT_SIZE);
                    *ptr_short = ntohs(*ptr_short);
                    offset += SHORT_SIZE;
                    done++;
                    break;

                case 'i':
                    // Unsigned int
                    ptr_int = va_arg(arg, unsigned int*);
                    memcpy(ptr_int, buff + offset, INT_SIZE);
                    *ptr_int = ntohl(*ptr_int);
                    offset += INT_SIZE;
                    done++;
                    break;

                case 's':
                    // String
                    ptr_str = va_arg(arg, char**);

                    // We first get the length
                    memcpy(&len, buff + offset, SHORT_SIZE);
                    len = ntohs(len);
                    offset += SHORT_SIZE;

                    // Then allocate and fill in the string
                    *ptr_str = calloc(1, len);
                    if (!*ptr_str) {
                        return done;
                    }
                    else {
                        memcpy(*ptr_str, buff + offset, len);
                    }
                    offset += len;
                    done++;
                    break;
 
 
                case '(':
                    // Array of values
                    // We first add the length of the array
                    ptr_short = va_arg(arg, unsigned short*);
                    memcpy(&alen, buff + offset, SHORT_SIZE);
                    alen = ntohs(alen);
                    *ptr_short = alen;
                    offset += SHORT_SIZE;
                    done++;
                    
                    while (format[++i] == '%' && format[++i]) {

                        switch(format[i]) {
                            
                            case 'h':
                                // Unsigned short 
                                aptr_short = va_arg(arg, unsigned short**);
                                *aptr_short = calloc(alen, SHORT_SIZE);
                                for (j = 0; j < alen; j++) {
                                    memcpy(*aptr_short + j, buff + offset, SHORT_SIZE);
                                    *aptr_short[j] = ntohs(*aptr_short[j]);
                                    offset += SHORT_SIZE;
                                }
                                break;

                            case 'i':
                                // Unsigned int
                                aptr_int = va_arg(arg, unsigned int**);
                                *aptr_short = calloc(alen, INT_SIZE);
                                for (j = 0; j < alen; j++) {
                                    memcpy(*aptr_int + j, buff + offset, INT_SIZE);
                                    *aptr_int[j] = ntohs(*aptr_int[j]);
                                    offset += INT_SIZE;
                                }
                                break;

                            case 's':
                                // String
                                ptr_str = calloc(alen, sizeof(char*));
                                for (j = 0; j < alen; j++) {
                                    // We first get the length
                                    memcpy(&len, buff + offset, SHORT_SIZE);
                                    len = ntohs(len);
                                    offset += SHORT_SIZE;

                                    // Then allocate and fill in the string
                                    ptr_str[j]  = calloc(1, len);
                                    if (!ptr_str[j]) {
                                        return done;
                                    }
                                    else {
                                        memcpy(ptr_str[j], buff + offset, len);
                                    }
                                    offset += len;
                                }
                                aptr_str = va_arg(arg, char***);
                                *aptr_str = ptr_str;
                                break;

                            default :
                                return 0;

                        }

                        done++;

                    }
    
                default :
                    return done;

            }

        }
        return done;

    }

}
