#include "IR_TX.h"

#define PIN_TX 9

#define IR_tx_one()  TCCR1A = _BV(COM1A0)
#define IR_tx_zero() TCCR1A = 0

// IR_TX::IR_TX(unsigned char *buf_ptr, unsigned int buf_size)
void IR_TX::init(unsigned char *buf_ptr, unsigned int buf_size)
{   
  // Memory initialization.
  this->buf_ptr = buf_ptr;
  this->buf_size = buf_size;
  this->buf_cnt = 0;
  state = WAIT;
  
  // Timer CTC mode settings.
#if PIN_TX != 9
#error "PIN_LED must be nine due to function limitations of a timer coutner."
#endif
  pinMode(PIN_TX,OUTPUT);
  
  TCCR1A = _BV(COM1A0);
  TCCR1B = _BV(WGM12) | _BV(CS10); //set to CTC mode with match at OCR register.
  OCR1A = 211;
  
  IR_tx_zero();
}

void IR_TX::routine(void)
{
  if ( state != TRANSMIT ) {
    return;
  }
  
  unsigned int ir_buf_idx = buf_cnt / 8;
  unsigned char ir_buf_bit = buf_cnt % 8;
  buf_cnt++;
  if ( ir_buf_idx >= buf_size ) {  // Detect the end of the buffer.
    state = DONE;
    return;
  }
  static unsigned char dat_tx;
  if ( ir_buf_bit == 0 )  dat_tx = buf_ptr[ir_buf_idx];
  
  if ( dat_tx & 0x80 )  IR_tx_one();
  else  IR_tx_zero();
  dat_tx <<= 1;
}

void IR_TX::enable(void)
{
  state = TRANSMIT;
  buf_cnt = 0;
}

void IR_TX::disable(void)
{
  state = WAIT;
}

bool IR_TX::is_done(void)
{
  if ( state == DONE ) {
    return true;
  }
  return false;
}

