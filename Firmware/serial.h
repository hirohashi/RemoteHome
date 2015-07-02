#ifndef _SERIAL_H_
#define _SERIAL_H_

// Prototypes
void SerialInit(void);
char *SerialReadLine(void);
void byte2hex(unsigned char n, char *hex);
unsigned char hex2byte(char ch, char cl);
bool is_hex(char c);

#endif /* _SERIAL_H_ */
