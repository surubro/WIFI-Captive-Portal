# Wifi Captive Portal

## Disclaimer

This code is a WiFi Captive Portal implemented for the ESP8266 and ESP32, specially those XIAO-ESP32-S3 + WIO-SX1262 KITs, which acts as a phishing page to collect passwords. It is essential to understand that this code is only ethical or legal purposes and is explicitly labeled as a "Wifi Penetration testing" for "Educational Purposes" only. Performing such attacks without prior permission is illegal and unethical, and using this code or similar techniques to compromise network security or violate someone's privacy is strictly prohibited.

Please note that this should not be used in any real-world scenario.

## Updates applied

>[+] It is capturing usernames and passwords.

>[+] bug fixes while compiling the solution on Arduino. 

>[+] It now requires password to access "PASS" page, password can be set as desired in the code, default password for .bin file is P@ssw0rd!


## Updates to be applied

>[+] Option to add certificates to use HTTPS instead of HTTP.


## Disclaimer
This project is for testing and educational purposes. Use it only against authorized targets and your own networks and devices. I don't take any responsibility for what you do with this program.

## About this project
WiFi captive portal for the NodeMCU (ESP8266 Module) with DNS spoofing.

The built-in LED will blink 5 times when a password is posted.

<b>Warning!</b> Your saved passwords will **not** disappear when you restart/power off the ESP8266.

<b>Note:</b> If you want to see the stored passwords go to "**172.0.0.1**<a>/pass</a>". For changing the SSID, go to "**172.0.0.1**<a>/ssid</a>"

# Showcase

<img width="700px" src="https://raw.githubusercontent.com/nocerainfosec/WIFI-Captive-Portal/master/src/demo-wifi-captive-portal.gif"></a>

# Screenshots

<table>
  <tr>
    <th>172.0.0.1/index         </th>
    <th>172.0.0.1/post          </th> 
    <th>172.0.0.1/pass          </th>
    <th>172.0.0.1/pass?password=</th>
    <th>172.0.0.1/ssid          </th>
  </tr>
  <tr>
    <td>This is the main page. Here the user will write his username and password and send it.</td>
    <td>This is the post page. The user will be redirected here after posting their credentials.</td>
    <td>This is the protection applied to the pass page, so noone else can snik into it.</td>
    <td>This is where the attacker can retrieve all the passwords that has been posted.</td>
    <td>Here the attacker can change the SSID name of the Access Point on the go.</td>
  <tr>
    <td><img width="200px" src="https://raw.githubusercontent.com/nocerainfosec/WIFI-Captive-Portal/master/src/1_Index_2.png" title="index"></td>
    <td><img width="200px" src="https://raw.githubusercontent.com/nocerainfosec/WIFI-Captive-Portal/master/src/2_Post.png" title="post"></td>
    <td><img width="200px" src="https://raw.githubusercontent.com/nocerainfosec/WIFI-Captive-Portal/master/src/3_Pass.png" title="pass"></td>
    <td><img width="200px" src="https://raw.githubusercontent.com/nocerainfosec/WIFI-Captive-Portal/master/src/3.1_pass.png" title="pass"></td>
    <td><img width="200px" src="https://raw.githubusercontent.com/nocerainfosec/WIFI-Captive-Portal/master/src/4_ssid.png" title="ssid"></td>
  </tr>
</table>

# Installation (ESP8266 Flasher - Easy way)

1. [Download](https://github.com/nodemcu/nodemcu-flasher) ESP8266 Flasher.

2. [Download](https://github.com/nocerainfosec/WIFI-Captive-Portal/releases/download/v2.0/release.bin) release.bin file.

3. Open the ESP8266 Flasher and select config tab and select the .bin file you've just downloaded.

<img width="80%" src="https://raw.githubusercontent.com/nocerainfosec/WIFI-Captive-Portal/master/src/ESP8266Flasher_lDlqjrUe8F.png">

4. Then, head back to the Operation tabs and select the Node MCU port and press "Flash".

<img width="80%" src="https://raw.githubusercontent.com/nocerainfosec/WIFI-Captive-Portal/master/src/ESP8266Flasher_76YPePmTMk.png">

5. Wait until the process finishes.

  <img width="80%" src="https://raw.githubusercontent.com/nocerainfosec/WIFI-Captive-Portal/master/src/ESP8266Flasher_djCGuSmTPI.png">

7. Your Node MCU is ready!

# Installation (Arduino IDE)

1. [Download](https://raw.githubusercontent.com/nocerainfosec/WIFI-Captive-Portal/master/esp8266_wifi_captive_portal.ino) and open the sketch using [Arduino IDE](https://www.arduino.cc/en/main/software)
2. Go to "File -> Preferences -> Boards Manager URLs" and paste the following link:
``http://arduino.esp8266.com/stable/package_esp8266com_index.json``
2. Go to "Tools -> Board -> Boards Manager", search "esp8266" and install esp8266
3. Go to "Tools -> Board" and select you board"

<img width="80%" src="https://raw.githubusercontent.com/nocerainfosec/WIFI-Captive-Portal/master/src/MnslKyFqwb.png">

4. You can optionally change all things like SSID name and texts of the page like title, subtitle, text body, Password of the protected page, style, you are the boss!
5. Upload the code into your board.
6. You are done!

# Installation (ESP32-S3 Flasher - Easy way on Linux)

## 1. Install PlatformIO

PlatformIO is required to compile and flash ESP32 projects. Open a terminal and run:

```bash
# Install Python 3 and pip if not already installed
sudo apt update
sudo apt install python3 python3-pip git -y

# Install PlatformIO Core
python3 -m pip install --user platformio
```

Add PlatformIO to your PATH if necessary:

```bash
export PATH=$HOME/.local/bin:$PATH
```

Check installation:

```bash
pio --version
```


## 2. Download the Captive Portal Project

Clone your GitHub repository or download the project zip:

```bash
git clone https://github.com/nocerainfosec/WIFI-Captive-Portal.git
cd WIFI-Captive-Portal
mkdir esp32s3-captive-portal
cp esp32_xiao-esp32-s3-seedstudio.cpp esp32s3-captive-portal/main.cpp
pio init -b esp32-s3-devkitc-1 -e s3
mv main.cpp src/main.cpp
```

## 2.1 Create the platformio.ini file
```bash
cat > platformio.ini <<EOF
[env:s3]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = 115200
upload_port = /dev/ttyACM0
build_flags = 
  -DARDUINO_USB_MODE=1
  -DARDUINO_USB_CDC_ON_BOOT=1
EOF
``

## 3. Connect the ESP32-S3 Board

Plug your ESP32-S3 dev board into your Linux machine via USB. Check the device path:

```bash
dmesg | tail
# or
ls /dev/ttyACM*
```

You should see something like `/dev/ttyACM0`.


## 4. Install Required Libraries

PlatformIO will handle most dependencies automatically. To ensure the ESP32 framework is available:

```bash
pio platform install espressif32
```


## 5. Build the Firmware

Compile the project:

```bash
pio run
```

If successful, the firmware binary will be in:

```
.pio/build/s3/firmware.bin
```


## 6. Flash the Firmware

Put your ESP32-S3 in bootloader mode: hold **BOOT**, tap **RESET**, then release **BOOT**.
Also! Last but not least!, if you fail to upload the file, make sure your user belongs to the group **dialout**, otherwise you need to create this group and add the user to it:

If you need to create the group and add the user to it:
```bash
sudo usermod -a -G dialout $USER
```

Upload the firmware:

```bash
pio run -t upload
```


## 7. Monitor Serial Output

Check that the captive portal is running:

```bash
pio device monitor
```

You should see messages indicating the SoftAP has started and the DNS server is active.


## 8. Your ESP32-S3 is ready!

The board will broadcast the captive portal SSID (e.g., `EMPRESA - GuestWifi`) and redirect connected clients to your portal page, make changes as you like!


## Ethical purposes only!

Again, I must emphasize that using this code or any similar technique for unauthorized access, phishing, or any illegal activities is strictly prohibited and against the law. It is essential to respect others' privacy and adhere to ethical standards while using technology. If you're interested in learning about network security, ethical hacking, or cybersecurity, I encourage you to pursue formal education or ethical hacking certifications through legitimate channels. Always use your skills and knowledge responsibly and lawfully.

## ❤️ Sponsoring
If you like or rely on the work I do, please consider [sponsoring me](https://github.com/sponsors/nocerainfosec). I do open source all my projects, it would be really great if I could do more of it.




## Other Projects you may like from the Original Author:

- **WiFi-Spam**: :email::satellite: Spam thousands of WiFi access points with custom SSIDs.
  - https://github.com/125K/WiFi-Spam
- **PwrDeauther**: :zap: Deauth a specific WiFi access point or an entire channel.
  - https://github.com/125K/PwrDeauther
