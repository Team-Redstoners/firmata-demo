package org.example;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.firmata4j.Parser;

/**
 * Allows connections over the serial interface.
 *
 * @author Ali Kia
 */
public class CustomTransport implements org.firmata4j.transport.TransportInterface {
    private Parser parser;
    private Thread readerThread;
    private Socket socket;
    private DataOutputStream out;
    private DataInputStream in;
    private final InetAddress ip;
    private final int port;

    private Logger log;

    /**
     * Creates network transport using a sting as address.
     *
     * Address should specify host and port. Examples:
     * <ul>
     *   <li>"192.168.1.10:36363"</li>
     *   <li>"explorer-bot.local:72727"</li>
     * </ul>
     * @param address host and port
     */
    public CustomTransport(String address) {
        String[] parts = address.split(":");
        if (parts.length != 2) {
            throw new IllegalArgumentException("Address must specify host and port but received " + address);
        }
        try {
            this.ip = InetAddress.getByName(parts[0]);
        } catch (UnknownHostException e) {
            throw new IllegalArgumentException(e);
        }
        this.port = Integer.valueOf(parts[1]);
        log = Logger.getLogger("FirmataTest");
        log.info("HELLO");
    }
    
    public CustomTransport(InetAddress ip, int port) {
        this.ip = ip;
        this.port = port;
        log = Logger.getLogger("FirmataTest");
        log.info("HELLO");
    }

    @Override
    public void start() throws IOException {
        try {
            socket = new Socket(ip, port);
            log.info("socket made");
            socket.setReuseAddress(true);
            log.info("setReuseAddress made");
            socket.setSoTimeout(1500);
            log.info("setSoTimeout made");
            socket.setSoLinger(true, 1500);
            log.info("setSoLinger made");
            socket.setSoTimeout(1500);
            log.info("setSoTimeout made");
            out = new DataOutputStream(socket.getOutputStream());
            log.info("DataOutputStream made");
            in = new DataInputStream(socket.getInputStream());
            log.info("DataInputStream made");
            readerThread = new Thread(new Reader(), "firmata-network-transport");
            log.info("readerThread made");
            readerThread.start();
            log.info("readerThread.start made");
            if(out == null) {
                log.log(Level.SEVERE, "out (socket.getOutputStream()) is NULL", socket);
            }
        }
        catch(Exception ex) {
            Logger.getLogger("CustomTransport").log(Level.SEVERE, "Failed to start() CustomTransport", ex);
        }
    }

    @Override
    public void stop() throws IOException {
        try {
            readerThread.interrupt();
            readerThread.join();
        } catch (Exception e) {
            Thread.currentThread().interrupt();
        }
        try {
            if (out != null) {
                out.close();
            }
        } finally {
            try {
                if (in != null) {
                    in.close();
                }
            } finally {
                if (socket != null) {
                    socket.close();
                }
            }
        }
        out = null;
        in = null;
        socket = null;

    }

    @Override
    public void write(byte[] bytes) throws IOException {
        out.write(bytes);
    }

    @Override
    public void setParser(Parser parser) {
        this.parser = parser;
    }

    private class Reader implements Runnable {

        @Override
        public void run() {
            byte[] buf = new byte[100];
            int readIn;
            while (!Thread.currentThread().isInterrupted()) {
                try {
                    readIn = in.read(buf);
                } catch (SocketTimeoutException e) {
                    break;  // We try to reconnect, hearthbeats (1*second) missing
                } catch (IOException e) {
                    break;
                }
                if (readIn == -1) {
                    break;  // Connection closed
                }
                byte[] data = new byte[readIn];
                System.arraycopy(buf, 0, data, 0, readIn);
                parser.parse(data);
            }
        }

    }

}
