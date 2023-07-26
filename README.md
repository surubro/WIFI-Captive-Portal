# ESP8266 WiFi Captive Portal


## Updates to be applied

>[+] Option to add certificates to use HTTPS instead of HTTP.

>[+] Capture usernames and passwords.

>[+] bug fixes while compiling the solution on Arduino.

>[+] It'll requires password to access "PASS" page, password can be set as desired in the code, default password for .bin file is P@ssw0rd!


## Disclaimer
This project is for testing and educational purposes. Use it only against authorized targets and your own networks and devices. I don't take any responsibility for what you do with this program.

## About this project
WiFi captive portal for the NodeMCU (ESP8266 Module) with DNS spoofing.

The built-in LED will blink 5 times when a password is posted.

<b>Warning!</b> Your saved passwords will **not** disappear when you restart/power off the ESP8266.

<b>Note:</b> If you want to see the stored passwords go to "**172.0.0.1**<a>/pass</a>". For changing the SSID, go to "**172.0.0.1**<a>/ssid</a>"

# Showcase coming soon

<a target="_blank" href="#"><img width="700px" src="https://raw.githubusercontent.com/nocerainfosec/ESP8266-WIFI-Captive-Portal/master/src/thumbnail.png"></a>

# Screenshots

<table>
  <tr>
    <th>172.0.0.1/index</th>
    <th>172.0.0.1/post</th> 
    <th>172.0.0.1/pass</th>
    <th>172.0.0.1/ssid</th>
  </tr>
  <tr>
    <td>This is the main page. Here the user will write his username and password and send it.</td>
    <td>This is the post page. The user will be redirected here after posting their credentials.</td>
    <td>This is where the attacker can retrieve all the passwords that has been posted.</td>
    <td>Here the attacker can change the SSID name of the Access Point on the go.</td>
  <tr>
    <td><img width="200px" src="#" title="index"></td>
    <td><img width="200px" src="#" title="post"></td>
    <td><img width="200px" src="#" title="pass"></td>
<td><img width="200px" src="#" title="ssid"></td>
  </tr>
</table>

Support me , if you like the work i do!


# Installation (ESP8266 Flasher - Easy way)

1. Download <a href="https://github.com/nodemcu/nodemcu-flasher"><b>ESP8266 Flasher</b></a>.

2. Download the <b><a href="#">release.bin</b></a> file. (soon)

3. Open the ESP8266 Flasher and select the Node MCU port

<img width="80%" src="#">

4. Then, go to the config tab and select the .bin file you've just downloaded.

<img width="80%" src="#">

5. Finally, go back to the first tab and press "Flash"

6. Your Node MCU is ready!

# Installation (Arduino IDE)

1. Open your <a href="https://www.arduino.cc/en/main/software">Arduino IDE</a> and go to "File -> Preferences -> Boards Manager URLs" and paste the following link:
``http://arduino.esp8266.com/stable/package_esp8266com_index.json``
2. Go to "Tools -> Board -> Boards Manager", search "esp8266" and install esp8266
3. Go to "Tools -> Board" and select you board"
4. Download and open the sketch "<a href="#"><b>WiFi_Captive_Portal.ino</b></a>" (soon)
5. You can optionally change some parameters like the SSID name and texts of the page like title, subtitle, text body...
6. Upload the code into your board.
7. You are done!


## Other Projects you may like from the Original Author:

- **WiFi-Spam**: :email::satellite: Spam thousands of WiFi access points with custom SSIDs.
  - https://github.com/125K/WiFi-Spam
- **PwrDeauther**: :zap: Deauth a specific WiFi access point or an entire channel.
  - https://github.com/125K/PwrDeauther
