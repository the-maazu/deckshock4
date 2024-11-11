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
A config file should be created in the home directory <code>~/.local/share/deckshock4/config.json</code>.
deckshock4 will fallback to an inbuilt default equivalent to the config below, should config.json be malformed or empty.

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
    "accelZofst": -13000,
    "R4": -1,
    "R5": -3,
    "L4": -3,
    "L5": -3
}
</code></pre>
In the future this will be controlled with a frontend decky-plugin.

**General:**
- <code>disable</code> is intepreted as <code>false</code> for '0' and <code>true</code> for non-zero values.

**Accelerometer:** 
- <code>accelX</code>,<code>accelY</code>,<code>accelZ</code> can be remapped with '0','1' and '2' and disabled with '-1'.
- <code>invaccelX</code>,<code>invaccelY</code>,<code>invaccelZ</code> reverse axis directions and are intepreted as <code>false</code> for '0' and <code>true</code> for non-zero values.
- <code>accelXOfst</code>,<code>accelYOfst</code>,<code>accelZOfst</code> control base orientation and can be assigned values '-32768' to '32767'. Inbuilt defaults assume a flat controller when SteamDeck is held at a viewing angle (tested with RPCS3).

**Back-buttons (from v0.1.2):** 
- <code>R4</code>,<code>R5</code>,<code>L4</code>,<code>L5</code> can be mapped to <code>{BackButtonDisable, BackButtonDefault, BackButtonShake, X, O, Square,Triangle, L1, R1, L2, R2, Share,Options,PS, Tpad1,Tpad2, L3, R3}</code> with values '-3' to '14' respectively.
- <code>R4</code> is by default mapped to simulate shake (<code>BackButtonShake=-1</code>) on hold.
- <code>Tpad1</code> and <code>Tpad2</code> map to the single DualShock trackpad click.



## Controller Settings & Steam Client Considerations
Input may be duplicated while **deckshock4** is running;

- From v0.1.1 deckshock4 will try to grab(inhibit) the steam controller virtual input, so no need to remove in-built controller mappings in the per game controller settings.
- However the steam client itself recieves the raw input so touch interactions with steam client UI is recommended while deckshock4 is running.
- In desktop mode, disable steam input for the emulated PS4 Controller in Desktop Layout settings.
- For games and apps with native DS4 support like RPCS3, PS Plus®, God of War© etc;
   1. Disable steam input for the virtual PS4 Controller.
   2. For the inbuilt controller set Gyro 'As Joystick'. Without gyro mapped to something Steam Controller does not send out sensor data.
   3. Optional: For inbuilt controller set Trackpads 'As Joystick' to enable haptic feedback.
   4. Download and extract [Proton-X.X-X-ds4](https://github.com/the-maazu/Proton/releases) to <code>~/.steam/root/compatibilitytools.d/</code>. These are patched versions of proton to help detect virtual DS4 as a real controller. Some non-Sony or Linux games/apps with DS4 support may not need this.
   5. Restart Steam client
   6. Force compatibility in game properties and select **Proton-X.X-X-ds4-X**.

   
**Note For PS Plus:**
- Desktop mode is recommended as the app sometimes minimizes into system tray, which is not accessible in game mode. Seems to happen at start and if the network is bad.
- Incase of a blackscreen on PS Plus;
   - Disable Graphics Accelerated rendering in PS Plus settings from taskbar tray icon (in desktop mode).
   - Or in game properties->launch options, <code>PROTON_USE_WINED3D=1 %command%</code>
