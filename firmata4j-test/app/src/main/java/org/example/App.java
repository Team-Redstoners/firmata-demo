/*
 * This source file was generated by the Gradle 'init' task
 */
package org.example;
import java.io.IOException;
import java.util.HexFormat;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.firmata4j.IODevice;
import org.firmata4j.IODeviceEventListener;
import org.firmata4j.IOEvent;
import org.firmata4j.Pin;
import org.firmata4j.Pin.Mode;
import org.firmata4j.firmata.FirmataDevice;
import org.firmata4j.firmata.FirmataMessageFactory;
import org.firmata4j.firmata.FirmataPin;
import org.firmata4j.fsm.FiniteStateMachine;
import org.firmata4j.transport.NetworkTransport;
import org.firmata4j.transport.SerialTransport;
import org.slf4j.LoggerFactory;
import org.firmata4j.Example;

public class App {
	FirmataDevice device;
	Pin testPin;
	Logger log;

	
    public String getGreeting() {
		log = Logger.getLogger("FirmataTest");
		// device = new FirmataDevice(new NetworkTransport("127.0.0.1:8555"));
    	// device = new FirmataDevice(new CustomTransport("127.0.0.1:8555"));
		device = new FirmataDevice("/dev/ttyUSB1");
    	try {
			log.info("device.start() starting");
    		device.start();
			log.info("device.ensureInitializationIsDone() starting");
	    	device.ensureInitializationIsDone();
			log.info("Initialized Firmata device??");

			testPin = device.getPin(2);
			testPin.setMode(Mode.INPUT);
			// log.info(HexFormat.of().formatHex(FirmataMessageFactory.setMode((byte)2, Mode.INPUT)));
			device.sendMessage(hexStringToByteArray("f40200"));
			device.addEventListener(new IODeviceEventListener() {
				@Override
				public void onStart(IOEvent ev) {
					log.info("start");
				}
				@Override
				public void onStop(IOEvent ev) {

				}
				@Override
				public void onMessageReceive(IOEvent ev, String msg) {

				}
				@Override
				public void onPinChange(IOEvent ev) {
					log.info("event: " + ev.toString());
				}
			});
			while(true) {
				System.out.println("val: " + testPin.getValue());
				Thread.sleep(500);
			}

			// return "Success!";
	    }
	    catch(Exception e) {
			log.log(Level.SEVERE, "Failed to init firmata!", e);
			try {
				// device.stop();
			}
			catch(Exception ex) {}
			return "FAIL";
	   	}
    }

    public static void main(String[] args) {
		try {
			// Example.main(args);
		} catch (Exception e) {
			// TODO: handle exception
		}
		org.slf4j.Logger LOGGER = LoggerFactory.getLogger(FiniteStateMachine.class);
		LOGGER.error("AAAAAAAAAAAAAA");
		System.out.println("Trying to connect...");
        System.out.println(new App().getGreeting());
    }





	// stolen
	public static byte[] hexStringToByteArray(String s) {
		int len = s.length();
		byte[] data = new byte[len / 2];
		for (int i = 0; i < len; i += 2) {
			data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
									+ Character.digit(s.charAt(i+1), 16));
		}
		return data;
	}
}
