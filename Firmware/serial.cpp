#include "Arduino.h"

void SerialInit(void)
{
  Serial.begin(115200);
}

char *SerialReadLine(void)
{
  const int len = 128;
  static char buf[len];
  static int cnt = 0;
  while ( Serial.available() > 0 ) {
    if ( cnt >= len )  cnt = 0;
    buf[cnt] = Serial.read();
    if ( buf[cnt] == '\r' || buf[cnt] == '\n') {
      buf[cnt] = '\0';
      if ( cnt == 0 ) return NULL;
      cnt = 0;
      return buf;
    }
    cnt += 1;
  }
  return NULL;
}

unsigned char hex2byte(char ch, char cl)
{
  unsigned char val = 0;
  char c;
  
  c = ch;
  if ( 'A' <= c && c <= 'F' ) {
    val += c - 'A' + 10;
  } else if ( '0' <= c && c <= '9' ) {
    val += c - '0';
  } else {
    val += 0;
  }
  val <<= 4;
  
  c = cl;
  if ( 'A' <= c && c <= 'F' ) {
    val += c - 'A' + 10;
  } else if ( '0' <= c && c <= '9' ) {
    val += c - '0';
  } else {
    val += 0;
  }
  
  return val;
}

void byte2hex(unsigned char n, char *hex)
{
  unsigned char m;
  char c;
  
  m = n >> 4;
  c = m + '0';
  if ( c < '0' || c > '9' )  c = m - 10 + 'A';
  if ( ('0' <= c && c <= '9') || ('A' <= c && c <= 'F' ) )  hex[0] = c;
  else hex[0] = 'X';
  
  m = n & 0x0F;
  c = m + '0';
  if ( c < '0' || c > '9' )  c = m - 10 + 'A';
  if ( ('0' <= c && c <= '9') || ('A' <= c && c <= 'F' ) )  hex[1] = c;
  else hex[1] = 'X';
}


bool is_hex(char c)
{
  if ( '0' <= c && c <= '9' ) return true;
  if ( 'A' <= c && c <= 'F' ) return true;
  return false;
}

