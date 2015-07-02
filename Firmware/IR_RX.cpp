#include "IR_RX.h"

#define PIN_RX  7

// IR_RX::IR_RX(unsigned char *buf_ptr, unsigned int buf_size)
void IR_RX::init(unsigned char *buf_ptr, unsigned int buf_size)
{
    pinMode(PIN_RX, INPUT);
    this->buf_ptr = buf_ptr;
    this->buf_size = buf_size;
    this->buf_cnt = 0;
    state = WAIT;
}

void IR_RX::enable(void)
{
  state = READY;
}

void IR_RX::disable(void)
{
  state = WAIT;
}

bool IR_RX::is_done(void)
{
  if ( state == DONE )  return true;
  return false;
}

void IR_RX::routine(void)
{
  if ( state != READY && state != SAMPLING )  return;
  
  unsigned char sig = 0;
  if ( digitalRead(PIN_RX) == 0 )   sig = 1;
  
  if ( state == READY && sig ) {
    state = SAMPLING;
    buf_cnt = 0;
  }
  if ( state != SAMPLING ) {
    return;
  }
  
  unsigned int ir_buf_idx = buf_cnt / 8;
  unsigned char ir_buf_bit = buf_cnt % 8;
  buf_cnt++;
  if ( ir_buf_idx >= buf_size ) {  // Detect the end of the buffer.
    state = DONE;
    return;
  }
  buf_ptr[ir_buf_idx] <<= 1;
  buf_ptr[ir_buf_idx] |= sig;
}

