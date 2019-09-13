#include "gpio.h"
typedef struct {
  uint32_t out;
  uint32_t outset;
  uint32_t outclr;
  uint32_t in;
  uint32_t dir;
  uint32_t dirset;
  uint32_t dirclr;
  uint32_t latch;
  uint32_t detectmode;
  uint32_t reserved[118];
  uint32_t PIN_CNF[31];
} GPIO;

GPIO *gpio = (GPIO*) 0x50000504;

// Inputs: 
//  gpio_num - gpio number 0-31
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {

  if(dir == 1) {
    gpio->dir |= (1 << gpio_num);
  } else {
    gpio->PIN_CNF[gpio_num] &= !(1<<gpio_num);
  }
}

// Set gpio_num high
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_set(uint8_t gpio_num) {
  gpio->out |= (1 << gpio_num);
}

// Set gpio_num low
// Inputs: 
//  gpio_num - gpio number 0-31
void gpio_clear(uint8_t gpio_num) {
  gpio->out &= !(1<<gpio_num);
}

// Inputs: 
//  gpio_num - gpio number 0-31
bool gpio_read(uint8_t gpio_num) {
    // should return pin state
    return (gpio->in >> gpio_num) & 1;
}
