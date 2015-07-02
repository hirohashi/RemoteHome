#include <string.h>
#include "bitope.h"
#include "IR_RX.h"
#include "IR_TX.H"
#include "serial.h"

// Definition of pins.
#define PIN_TX  9
#define PIN_RX  7

#define PIN_LED  13

// Prototypes
void HandleSerialLine(char *cmd);
char *SerialReadLine(void);
void IR_rec(void);

// Unit time in micro seconds for interpretting IR signals.
#define IR_UNIT_TIME  100
#define IR_BUFSIZE    1024

static struct _IRBUF {
  unsigned int bufsize = IR_BUFSIZE;
  unsigned char buf[IR_BUFSIZE];
  int buf_cnt = -1;
} irbuf;

IR_TX IR_tx;
IR_RX IR_rx;

// Main loop.
void loop() {
  static unsigned long ir_last_time = 0;
  
  // Routine work.
  if ( micros() - ir_last_time >= IR_UNIT_TIME ) {
    ir_last_time = micros() - 9;
    digitalWrite(PIN_LED, HIGH);
    IR_tx.routine();
    IR_rx.routine();
    
    digitalWrite(PIN_LED, LOW);
  }
  
  // Serial communiation.
  char *line = SerialReadLine();
  if ( line != NULL ) {
    HandleSerialLine(line);
  }
}

// Initialization
void setup() {
  SerialInit();
  pinMode(PIN_LED, OUTPUT);
  
  IR_tx.init(irbuf.buf, irbuf.bufsize);
  IR_rx.init(irbuf.buf, irbuf.bufsize);
}

// Serial event handller.
void HandleSerialLine(char *cmd)
{
  if ( cmd == NULL )  return; 
  char *arg = strchr(cmd, ':');
  if ( arg != NULL ) {
    *arg = '\0';
    arg++;
  }
  
  if ( !strcmp(cmd, "buf") ) {
    if ( !strcmp(arg, "dump") ) {
      for (int i=0; i<IR_BUFSIZE; i++) {
        char str_byte[3];
        byte2hex(irbuf.buf[i], str_byte);
        str_byte[2] = '\0';
        if ( i > 0 && i % 32  == 0 )  Serial.print("\n");
        Serial.print(str_byte);
      }
      Serial.print("\n\n");
    }
    
    // Start to register the IR code.
    if ( !strcmp(arg, "reg_start") ) {
      IR_rx.disable();
      IR_tx.disable();
      irbuf.buf_cnt = 0;
    }
    if ( !strcmp(arg, "reg_stop") ) {
      while ( irbuf.buf_cnt < IR_BUFSIZE ) {
        irbuf.buf[irbuf.buf_cnt++] = 0;
      }
      irbuf.buf_cnt = 0;
    }
  }
  
  if ( !strcmp(cmd, "dat") ) {
    char ch, cl;
    while ( is_hex(ch = arg[0]) && is_hex(cl = arg[1]) ) {
      irbuf.buf[irbuf.buf_cnt++] = hex2byte(ch, cl);
      arg += 2;
    }
  }
  
  if ( !strcmp(cmd, "rx") ) {
    if ( !strcmp(arg, "enable") )  IR_rx.enable();
    if ( !strcmp(arg, "disable") ) IR_rx.disable();
    if ( !strcmp(arg, "status") )  Serial.print((IR_rx.is_done()) ? "Yes" : "No");  Serial.print("\n");
  }
  
  if ( !strcmp(cmd, "tx") ) {
    if ( !strcmp(arg, "enable") )   IR_tx.enable();
    if ( !strcmp(arg, "disable") )  IR_tx.disable();
    if ( !strcmp(arg, "status") )   Serial.print((IR_tx.is_done()) ? "Yes" : "No");  Serial.print("\n");
  }
}

