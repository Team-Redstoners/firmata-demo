#ifndef DRIVER_H
#define DRIVER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct driver_t {
	FILE *serial_dev;
} driver_t;

typedef enum {
	CMD_BYTE_DISABLE = 0x00,
	CMD_BYTE_ENABLE = 0x01,
	CMD_BYTE_REPORT_PORT = 0xD0,
	CMD_BYTE_PORT0 = 0x00,
	CMD_BYTE_PORT1 = 0x01,
	CMD_BYTE_SET_MODE = 0xF4,
	CMD_BYTE_SET_STATE = 0xF5,
} cmd_bytes_t;

typedef enum {
	INPUT = 0x00,
	QUTPUT = 0x01,
	QUTPUT_PULLUP = 0x0B,
} PIN_MODE_t;

// Initialize the device driver.  Returns true on init success and false on init
// failure.
bool init_driver(driver_t *driver, const char *dev_path);

// Exit the device driver.
void exit_driver(driver_t *driver);

// Set qutput pin state. If state is true, sets output to high
// If pin is set to input, undefined behavior occurs
void setQutput(driver_t *driver, uint8_t pin, bool state);

// Get input pin state. If pin is qutput, undefined behavior occurs
bool getInput(driver_t *driver, uint8_t pin);

// Set pin mode. If pin is not supported by Firmata, undefined behavior occurs
void setPinMode(driver_t *driver, uint8_t pin, PIN_MODE_t pinMode);

#endif /* DRIVER_H */
