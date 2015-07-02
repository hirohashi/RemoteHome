#ifndef _IR_TX_H_
#define _IR_TX_H_
#include <Arduino.h>

class IR_TX {
public:
  // IR_TX(unsigned char *buf_ptr, unsigned int buf_size);
  void init(unsigned char *buf_ptr, unsigned int buf_size);
  void routine(void);
  void enable(void);
  void disable(void);
  bool is_done(void);
  
private:
  unsigned char *buf_ptr;
  unsigned int buf_size, buf_cnt;
  enum {
    WAIT = 1,
    TRANSMIT,
    DONE,
  } state;
};

#endif

