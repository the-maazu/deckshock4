# deckshock4
DualShock® 4 emulator for Steam Deck®

## In(unin)stallation & Execution
1. Download the latest [release ](https://github.com/the-maazu/deckshock4/releases) **deckshock4-vX.X.X.tar.gz**.
2. Extract into subfolder
3. Open terminal in subfolder and execute <code>$ ./install.sh</code>
4. You can run **deckshock4**
   - In terminal, <code>$ deckshock4</code>,
   - or add as non-steam game and set target to <code>~/.local/bin/deckshock4</code>.
5. To stop **deckshock4**
   - Hold **STEAM** button for 10 seconds,
   - CTRL-C (in terminal)
   - or close the terminal.
6. To uninstall, open terminal in extracted subfolder and execute <code>$ ./uninstall.sh</code>

## Configuration (from v0.1.0)
A config file should be created in the home directory <code>$~/.local/share/deckshock4/config.json</code>.
deckshock4 will fallback to an inbuilt default equivalent to the config below, should confi.json be malformed or empty (tested with RPCS3).

You can mix up the mapping to <code>accelX</code>,<code>accelY</code> and <code>accelZ</code> with values 0,1 and 2 and adjust offset(orientation) as you see fit. <code>disable</code> and <code>invaccel</code> (reverse axis) are intepreted as <code>false</code> for "0" and <code>true</code> for non-zero values.

<pre><code>
{
    "disable": 0,
    "accelX": 0,
    "accelY": 1,
    "accelZ": 2,
    "invaccelX": 0,
    "invaccelY": 0,
    "invaccelZ": 0,
    "accelXofst": 0,
    "accelYofst": -5000,
    "accelZofst": -13000
}
</code></pre>

In the future this will be controlled with a frontend decky-plugin.

### Controller Settings & Steam Client Considerations
Input is duplicated while **deckshock4** is running;

- In game mode, it is recommended to only interact with device using touch screen while the client is in focus.
- In desktop mode, disable steam input for the emulated 'PS4 Controller' in Desktop Layout settings.
- For games and apps with native DS4 support like PS Plus®, God of War© etc;
   1. Remove all Steam controller mappings for inbuilt controller leaving only Gyro set 'As Joystick'.
   2. Optional: Enable Trackpads 'As Joystick' for haptic feedback. 
   3. Disable steam input for the virtual 'PS4 Controller'.
   4. Download and extract [Proto-X.X-X-ds4-X](https://github.com/the-maazu/Proton/releases) to <code>~/.steam/root/compatibilitytools.d/</code>. These are patched versions of proton to help detect virtual DS4 as a real controller. Some non-Sony games with DS4 support may not need this.
   5. Restart Steam client
   6. Force compatibility in game properties and select **Proton-X.X-X-ds4-X**.
   
**Note For PS Plus:**
- Desktop mode is recommended as the app sometimes minimizes into system tray, which is not accessible in game mode. Seems to happen at start and if the network is bad.
- Incase of a blackscreen on PS Plus;
   - Disable Graphics Accelerated rendering in PS Plus settings from taskbar tray icon (in desktop mode).
   - Or in game properties->launch options, <code>PROTON_USE_WINED3D=1 %command%</code>
