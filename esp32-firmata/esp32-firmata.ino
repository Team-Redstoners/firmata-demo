/*
 * ConfigurableFirmata standard example file, for serial communication.
 *
 *
 * ===================== REQUIREMENTS =====================
 * Tested to compile on:
 * - Arduino IDE 2.3.2
 * - ConfigurableFirmata library version 3.3.0
 * - esp32 Board Package *by Espressif*, version 3.0.7
 * Compiled against Adafruit Feather ESP32-S2, because I forgot what board we're using
 */

#include <ConfigurableFirmata.h>

// Use these defines to easily enable or disable certain modules

// #define ENABLE_I2C
// #define ENABLE_SPI
#define ENABLE_ANALOG
#define ENABLE_DIGITAL
// #define ENABLE_DHT
// #define ENABLE_FREQUENCY

#ifdef ENABLE_DIGITAL
#include <DigitalInputFirmata.h>
DigitalInputFirmata digitalInput;

#include <DigitalOutputFirmata.h>
DigitalOutputFirmata digitalOutput;
#endif

#ifdef ENABLE_ANALOG
#include <AnalogInputFirmata.h>
AnalogInputFirmata analogInput;

#include <AnalogOutputFirmata.h>
AnalogOutputFirmata analogOutput;
#endif

#ifdef ENABLE_I2C
#include <Wire.h>
#include <I2CFirmata.h>
I2CFirmata i2c;
#endif

#ifdef ENABLE_SPI
#include <Wire.h>
#include <SpiFirmata.h>
SpiFirmata spi;
#endif

#ifdef ENABLE_SERIAL
#include <SerialFirmata.h>
SerialFirmata serial;
#endif

#include <FirmataExt.h>
FirmataExt firmataExt;


#include <FirmataReporting.h>
FirmataReporting reporting;

void systemResetCallback() {
	firmataExt.reset();
}

void initTransport() {
	// Uncomment to save a couple of seconds by disabling the startup blink sequence.
	// Firmata.disableBlinkVersion();
  
	Firmata.begin(57600);
}

void initFirmata() {
#ifdef ENABLE_DIGITAL
	firmataExt.addFeature(digitalInput);
	firmataExt.addFeature(digitalOutput);
#endif
	
#ifdef ENABLE_ANALOG
	firmataExt.addFeature(analogInput);
	firmataExt.addFeature(analogOutput);
#endif
	
#ifdef ENABLE_I2C
	firmataExt.addFeature(i2c);
#endif
	
#ifdef ENABLE_SERIAL
	firmataExt.addFeature(serial);
#endif
	
  firmataExt.addFeature(reporting);
#ifdef ENABLE_SPI
	firmataExt.addFeature(spi);
#endif

	Firmata.attach(SYSTEM_RESET, systemResetCallback);
}

void setup() {
	// Set firmware name and version.
	// Do this before initTransport(), because some client libraries expect that a reset sends this automatically.
	Firmata.setFirmwareNameAndVersion("ConfigurableFirmata", FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
	initTransport();
	Firmata.sendString(F("Booting device. Stand by..."));
	initFirmata();

	Firmata.parse(SYSTEM_RESET);
  Firmata.sendString(F("Done"));
}

void loop() {
	while(Firmata.available())  {
		Firmata.processInput();
		if (!Firmata.isParsingMessage())  {
			break;
		}
	}

	firmataExt.report(reporting.elapsed());
}
