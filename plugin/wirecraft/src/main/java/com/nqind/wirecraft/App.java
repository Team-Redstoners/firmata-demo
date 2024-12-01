package com.nqind.wirecraft;
// why can't vscode find org.bukkit.plugin.java when maven compiles?
// whatever
import org.bukkit.plugin.java.JavaPlugin;

/**
 * Hello world!
 *
 */
public class App extends JavaPlugin {

    @Override
    public void onEnable() {
        getLogger().info("h");
    }

    @Override
    public void onDisable() {
        getLogger().info("bye");
    }
}
