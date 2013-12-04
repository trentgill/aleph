#include "compiler.h"
//#include "uc3a0512.h"
#include <avr32/io.h>

#include "print_funcs.h"
#include "twi.h"
#include "gpio.h"

#include "events.h"
#include "i2c.h"


//----------------------------
//----- extern vars
volatile u8 i2c_buffer[I2C_BUFFER_SIZE];


//----------------------------
//----- static vars
static u16 i2c_read_pos = 0;
static u16 i2c_write_pos = 0;


/* typedef struct */
/* { */
/*   //! TWI chip address to communicate with. */
/*   char chip; */
/*   //! TWI address/commands to issue to the other chip (node). */
/*   uint8_t addr[3]; */
/*   //! Length of the TWI data address segment (1-3 bytes). */
/*   int addr_length; */
/*   //! Where to find the data to be written. */
/*   void *buffer; */
/*   //! How many bytes do we want to write. */
/*   unsigned int length; */
/* } twi_package_t; */

twi_package_t packet_tx = { 
  /// this is the standard i2c "address"
  .chip = 102,
  /// this "address" field is not the standard i2c address,
  //// it assumes we are talking to an EEPROM or something.
  //  .addr = {100, 100, 100}, 
  .addr_length=0,
  .length=1 
}; 

twi_package_t packet_rx = { 
  .chip = 100, 
  //  .addr = {100, 100, 100}, 
  //  .addr_length=0,
  .length=1 
}; 


// transmit data in slave mode
// __attribute__((__interrupt__))
u8 i2c_slave_tx(void) {
  u8 val = 0;
  return val;
}

// handler for rx events
//__attribute__((__interrupt__))
void i2c_slave_rx(u8 value) {
  // print_dbg("\r\n slave rx: ");
  // print_dbg_char_hex(value);
  // gpio_toggle_pin(LED_MODE_PIN);

  i2c_buffer[i2c_write_pos] = value;
  i2c_write_pos++;
  if(i2c_write_pos == I2C_BUFFER_SIZE) i2c_write_pos = 0;
}

// stop function - done receiving??
void i2c_slave_stop(void) {
  static event_t e;   
  e.type = kEventI2C;
  e.data = i2c_read_pos;
  event_post(&e);

  i2c_read_pos = i2c_write_pos;
}

// master send
void i2c_master_tx(u8* tx) {
  print_dbg("\r\n i2c_master_tx, data location: 0x");
  print_dbg_hex((u32)tx);
  print_dbg(", data value: 0x");
  print_dbg_char_hex(*tx);
  packet_tx.buffer = tx;
  // print_dbg("\r\n i2c_master_tx, TWI location: 0x");
  // print_dbg_hex((u32)AVR32_TWI_ADDRESS);

  twi_master_write(&AVR32_TWI, &packet_tx);
}

// master receive
void i2c_master_rx(u8* rx) {
  packet_rx.buffer = rx;
  twi_master_read(&AVR32_TWI, &packet_rx);
}
