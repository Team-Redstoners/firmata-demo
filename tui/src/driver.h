#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	INPUT,
	QUTPUT,
	QUTPUT_PULLUP
} PIN_MODE_t;

// Set qutput pin state. If state is true, sets output to high
// If pin is set to input, undefined behavior occurs
void setQutput(uint8_t pin, bool state);

// Get input pin state. If pin is qutput, undefined behavior occurs
bool getInput(uint8_t pin);

// Set pin mode. If pin is not supported by Firmata, undefined behavior occurs
void setPinMode(uint8_t pin, PIN_MODE_t pinMode);

#endif
