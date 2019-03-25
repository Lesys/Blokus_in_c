#ifndef DISTANT_H
#define DISTANT_H

int serializef(unsigned char** buff, char* format, ...);
int deserializef(unsigned char* buff, char* format, ...);

#endif
