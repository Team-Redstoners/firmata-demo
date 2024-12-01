#include "driver.h"

#include <stdbool.h>
#include <stdio.h>

bool init_driver(driver_t *driver, const char *dev_path) {
	bool init_success = false;

	driver->serial_dev = NULL;
	driver->serial_dev = fopen(dev_path, "wb");

	if (driver->serial_dev == NULL) {
		printf("Unable to open device at: %s\n", dev_path);
	} else {
		init_success = true;
	}

	return init_success;
}

void exit_driver(driver_t *driver) {
	if (driver->serial_dev) {
		fclose(driver->serial_dev);
	} else {
		printf("No device to close...\n");
	}
}

/* This function arranges bytes into the Firmata/MIDI sequence needed. */
static uint32_t format_firmata_bytes(int n_ubytes, uint8_t ubytes[n_ubytes]) {
	uint32_t fbytes = 0;

	for (uint8_t i = 0; i < n_ubytes - 1; i++) {
		fbytes += ubytes[i];
		fbytes = fbytes << 8;
	}

	fbytes += ubytes[n_ubytes - 1];

	return fbytes;
}

void setQutput(driver_t *driver, uint8_t pin, bool state) {
	uint32_t firmata_bytes = 0;
	uint8_t ubytes[] = {state, pin, CMD_BYTE_SET_STATE};
	firmata_bytes = format_firmata_bytes(3, ubytes);
	fwrite(&firmata_bytes, 3, 1, driver->serial_dev);
}

bool getInput(driver_t *driver, uint8_t pin) {
	uint32_t firmata_bytes = 0;
	uint32_t pin_bit;

	/*
	 * SEND OVER CMD TO ENABLE REPORTING DIGITAL PINS...
	 *
	 * Stick to just port 0 right now for testing purposes...
	 *
	 * I think this is how reading pins works?  It doesn't look like there's a
	 * way to read pins individually... TODO TODO TODO
	 */
	uint8_t ubytes[] = {CMD_BYTE_ENABLE, CMD_BYTE_REPORT_PORT | CMD_BYTE_PORT0};
	firmata_bytes = format_firmata_bytes(2, ubytes);
	fwrite(&firmata_bytes, 2, 1, driver->serial_dev);

	/*
	 * Then I *think* this is where the digital pin values get read...
	 *
	 * It says that a port covers 8 bits, so assume we only receive one byte?
	 * TODO
	 */
	firmata_bytes = 0;
	fread(&firmata_bytes, 1, 1, driver->serial_dev);

	/*
	 * After reading, we can AND the values read from the port with a "1" at the
	 * position of the pin that we want...
	 *
	 * (I'm assuming the pins start at pin 0 from "port 0...")
	 */
	pin_bit = 0x01 << pin;

	if ((firmata_bytes & pin_bit) > 0) {
		return true;
	} else {
		return false;
	}
}

void setPinMode(driver_t *driver, uint8_t pin, PIN_MODE_t pinMode) {
	uint32_t firmata_bytes = 0;
	uint8_t ubytes[] = {pinMode, pin, CMD_BYTE_SET_MODE};
	firmata_bytes = format_firmata_bytes(3, ubytes);
	fwrite(&firmata_bytes, 3, 1, driver->serial_dev);
}
