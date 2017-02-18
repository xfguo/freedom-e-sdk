/////////////////////////////////////////////////////////////////////
////                                                             ////
////  WISHBONE rev.B2 compliant I2C Master controller Testbench  ////
////                                                             ////
////                                                             ////
////  Author: Richard Herveille                                  ////
////          richard@asics.ws                                   ////
////          www.asics.ws                                       ////
////                                                             ////
////  Downloaded from: http://www.opencores.org/projects/i2c/    ////
////                                                             ////
/////////////////////////////////////////////////////////////////////
////                                                             ////
//// Copyright (C) 2001 Richard Herveille                        ////
////                    richard@asics.ws                         ////
////                                                             ////
//// This source file may be used and distributed without        ////
//// restriction provided that this copyright statement is not   ////
//// removed from the file and that any derivative work contains ////
//// the original copyright notice and the associated disclaimer.////
////                                                             ////
////     THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY     ////
//// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   ////
//// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS   ////
//// FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL THE AUTHOR      ////
//// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,         ////
//// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    ////
//// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE   ////
//// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR        ////
//// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF  ////
//// LIABILITY, WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT  ////
//// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  ////
//// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE         ////
//// POSSIBILITY OF SUCH DAMAGE.                                 ////
////                                                             ////
/////////////////////////////////////////////////////////////////////

// Re-written in C by SiFive, Inc.
// See LICENSE for license details.

#include "encoding.h"
#include <stdint.h>
#include "platform.h"
#include "sifive/devices/i2c.h"

static const uint32_t SADR = 16; // 7'b0010_000 - slave address in I2C address space
uint32_t status;

// TODO: addd timeout?
int check_tip_bit() {
  do {
    status = I2C_REG(I2C_STATUS);
  } while (status & 2);            // poll it until it is zero

  return 0;
}

int main(void)
{
  uint32_t data;

  GPIO_REG(GPIO_PULLUP_EN) |= IOF0_I2C_MASK;

  // wait for pullup

  // RTC is running at 32kHz. 
  uint32_t now = CLINT_REG(CLINT_MTIME);
  while (CLINT_REG(CLINT_MTIME) <= (now+6)) ;

  GPIO_REG(GPIO_IOF_SEL)   &= ~IOF0_I2C_MASK;
  GPIO_REG(GPIO_IOF_EN)    |= IOF0_I2C_MASK;

  // test register access
  I2C_REG(I2C_PRESCALER_LO) = 0xfa;
  I2C_REG(I2C_PRESCALER_LO) = 0xc8;
  I2C_REG(I2C_PRESCALER_HI) = 0x00;

  // verify written reg values
  if (I2C_REG(I2C_PRESCALER_LO) != 0xc8)
    return __LINE__;
  if (I2C_REG(I2C_PRESCALER_HI) != 0x00)
    return __LINE__;

  // enable i2c
  I2C_REG(I2C_CONTROL) = 0x80;


  // access slave

  // drive slave address
  I2C_REG(I2C_DATA) = SADR<<1 | 0; // present slave address, set write-bit (0)
  I2C_REG(I2C_CMD)  = 0x90;        // set command (start, write)

  check_tip_bit();

  // send memory address
  I2C_REG(I2C_DATA) = 0x01;        // present slave's memory address
  I2C_REG(I2C_CMD)  = 0x10;        // set command (write)

  check_tip_bit();

  // send memory contents
  I2C_REG(I2C_DATA) = 0xa5;        // present data
  I2C_REG(I2C_CMD)  = 0x10;        // set command (write)


  now = CLINT_REG(CLINT_MTIME);
  while (CLINT_REG(CLINT_MTIME) <= (now+6)) ;


  check_tip_bit();

  // send memory contents for next memory address (auto_inc)
  I2C_REG(I2C_DATA) = 0x5a;        // present data
  I2C_REG(I2C_CMD)  = 0x50;        // set command (write)

  check_tip_bit();


  // access slave (read)

  // drive slave address
  I2C_REG(I2C_DATA) = SADR<<1 | 0; // present slave address, set write-bit (0)
  I2C_REG(I2C_CMD)  = 0x90;        // set command (start, write)

  check_tip_bit();

  // send memory address
  I2C_REG(I2C_DATA) = 0x01;        // present slave's memory address
  I2C_REG(I2C_CMD)  = 0x10;        // set command (write)

  check_tip_bit();

  // drive slave address
  I2C_REG(I2C_DATA) = SADR<<1 | 1; // present slave's address, set read-bit
  I2C_REG(I2C_CMD)  = 0x90;        // set command (start, write)

  check_tip_bit();

  // read data from slave
  I2C_REG(I2C_CMD)  = 0x20;        // set command (read, ack_read)

  check_tip_bit();

  // check data just received
  data = I2C_REG(I2C_DATA);
  if (data != 0xa5)
    return __LINE__;

  // read data from slave
  I2C_REG(I2C_CMD)  = 0x20;        // set command (read, ack_read)

  check_tip_bit();

  // check data just received
  data = I2C_REG(I2C_DATA);
  if (data != 0x5a)
    return __LINE__;

  // read data from slave
  I2C_REG(I2C_CMD)  = 0x20;        // set command (read, ack_read)

  check_tip_bit();

  // read data just received
  data = I2C_REG(I2C_DATA);

  // read data from slave
  I2C_REG(I2C_CMD)  = 0x28;        // set command (read, nack_read)

  check_tip_bit();

  // read data just received
  data = I2C_REG(I2C_DATA);


  // check invalid slave memory address

  // drive slave address
  I2C_REG(I2C_DATA) = SADR<<1 | 0; // present slave address, set write-bit (0)
  I2C_REG(I2C_CMD)  = 0x90;        // set command (start, write)

  check_tip_bit();

  // send memory address
  I2C_REG(I2C_DATA) = 0x10;        // present slave's memory address
  I2C_REG(I2C_CMD)  = 0x10;        // set command (write)

  check_tip_bit();

  // slave should have send NACK
  if (!(status & 0x80))
    return __LINE__;

  // 
  I2C_REG(I2C_CMD)  = 0x40;        // set command (stop)

  check_tip_bit();


  now = CLINT_REG(CLINT_MTIME);
  while (CLINT_REG(CLINT_MTIME) <= (now+6)) ;


  return 0;
}
