package com.nqind.wirecraft;
// why can't vscode find org.bukkit.plugin.java when maven compiles?
// whatever
import org.bukkit.plugin.java.JavaPlugin;

import org.bukkit.event.Listener;
import org.bukkit.Bukkit;
import org.bukkit.event.EventHandler;
import org.bukkit.event.player.PlayerJoinEvent;
import org.bukkit.event.block.BlockRedstoneEvent;
import org.bukkit.block.Block;
import org.bukkit.configuration.file.FileConfiguration;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.FileNotFoundException;

/**
 * Hello world!
 *
 */
public class App extends JavaPlugin implements Listener {
    FileConfiguration config = getConfig();
    FileOutputStream outputFile;

    @Override
    public void onEnable() {
        getLogger().info("h");

        config.addDefault("printAllRedstoneEvents", false);
        config.addDefault("serialDevice", "/dev/ttyUSB0");
        config.options().copyDefaults(true);
        saveConfig();
        getServer().getPluginManager().registerEvents(this, this);
        try {
            outputFile = new FileOutputStream(config.getString("serialDevice"));
        }
        catch(FileNotFoundException e) {

        }
        // 0xf40201 - set pin to output
        // 0xf50201 - turn on
        // byte initBytes[] = "\u00f4\u0002\u0001".getBytes();
        byte initBytes[] = hexStringToByteArray("f40201");
        try {
            outputFile.write(initBytes);
        }
        catch(IOException e) {

        }
    }

    @Override
    public void onDisable() {
        try {
            outputFile.close();
        } catch (IOException e) {
            // TODO: handle exception
        }
    }

    @EventHandler
    public void onPlayerJoin(PlayerJoinEvent event) {
        Bukkit.broadcastMessage("h " + event.getPlayer().getName());
    }

    @EventHandler
    public void onBlockRedstoneEvent(BlockRedstoneEvent event) {
        Block block = event.getBlock();
        if(config.getBoolean("printAllRedstoneEvents")) {
            Bukkit.broadcastMessage("Event on block at x,y,z (" + block.getX() + "," + block.getY() + "," + block.getZ() + ")");
            Bukkit.broadcastMessage("isBlockPowered(): " + block.isBlockPowered() + ", isBlockIndirectlyPowered(): " + block.isBlockIndirectlyPowered());
            Bukkit.broadcastMessage("New/old curr: " + event.getNewCurrent() + "/" + event.getOldCurrent());
        }
        if(block.getX() == 0 && block.getY() == 64 && block.getZ() == 0) {
            Bukkit.broadcastMessage("Output block is " + (block.isBlockPowered() ? "ON" : "OFF"));
            // byte outBytes[] = ("\u00f5\u0002" + (block.isBlockPowered() ? "\u0001" : "\u0000")).getBytes();
            byte outBytes[] = hexStringToByteArray("f5020" + (block.isBlockPowered() ? "1" : "0"));
            try {
                outputFile.write(outBytes);
                outputFile.flush();
            }
            catch(IOException e) {

            }
        }
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
