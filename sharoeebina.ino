#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define SysLed 2
#define LedR 15
#define LedG 12
#define LedB 13

MDNSResponder mdns;

// впишите сюда данные, соответствующие вашей сети:
const char* ssid = "zloy_zyxel";
const char* passw = "ROKOLABS2015!";

ESP8266WebServer server(80);

String webPage = "";

// ***************************************************************************************
void setup() 
{
  pinMode(SysLed, OUTPUT);
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(LedB, OUTPUT);
  digitalWrite(SysLed, HIGH);
  digitalWrite(LedR, LOW);
  digitalWrite(LedG, LOW);
  digitalWrite(LedB, LOW);

  Serial.begin(74880);                  // включим последовательный порт? скорость 115200

  wifiInit();
}


void loop() {
//  digitalWrite(SysLed, HIGH);
//  delay(200);
//  digitalWrite(SysLed, LOW);
//  delay(200);
  
  wifiProcess();

}

////////////////////

void wifiInit(void)
{
  webPage += "<h1>Sharoebina Web Server</h1>";
  webPage += "<p>Led R <a href=\"LedROn\"><button>ON</button></a>&nbsp;<a href=\"LedROff\"><button>OFF</button></a></p>";
  webPage += "<p>Led G <a href=\"LedGOn\"><button>ON</button></a>&nbsp;<a href=\"LedGOff\"><button>OFF</button></a></p>";
  webPage += "<p>Led B <a href=\"LedBOn\"><button>ON</button></a>&nbsp;<a href=\"LedBOff\"><button>OFF</button></a></p>";
  webPage += "<p>Sys Led <a href=\"SysOn\"><button>ON</button></a>&nbsp;<a href=\"SysOff\"><button>OFF</button></a></p>";

  delay(1000);
  Serial.println("");

  // ждем соединения
  WiFi.begin(ssid, passw);
  while (WiFi.status() != WL_CONNECTED)
  {
//    delay(500); Serial.print(".");
    digitalWrite(SysLed, LOW);
    delay(250);
    digitalWrite(SysLed, HIGH);
    delay(250);
  }



  Serial.println("");
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP address: "); Serial.println(WiFi.localIP());
  if (mdns.begin("esp8266", WiFi.localIP())) Serial.println("MDNS responder started");




  server.on("/", []()
  {
    server.send(200, "text/html", webPage);
    Serial.println("screen refresh");
  });

  server.on("/LedROn", []()
  {
    server.send(200, "text/html", webPage);
    digitalWrite(LedR, HIGH);
    Serial.println("Led R ON");
  });
  server.on("/LedROff", []()
  {
    server.send(200, "text/html", webPage);
    digitalWrite(LedR, LOW);
    Serial.println("Led R OFF");
  });
  server.on("/LedGOn", []()
  {
    server.send(200, "text/html", webPage);
    digitalWrite(LedG, HIGH);
    Serial.println("Led G ON");
  });
  server.on("/LedGOff", []()
  {
    server.send(200, "text/html", webPage);
    digitalWrite(LedG, LOW);
    Serial.println("Led G OFF");
  });
  server.on("/LedBOn", []()
  {
    server.send(200, "text/html", webPage);
    digitalWrite(LedB, HIGH);
    Serial.println("Led B ON");
  });
  server.on("/LedBOff", []()
  {
    server.send(200, "text/html", webPage);
    digitalWrite(LedB, LOW);
    Serial.println("Led B OFF");
  });
  server.on("/SysOn", []()
  {
    server.send(200, "text/html", webPage);
    digitalWrite(SysLed, LOW);
    Serial.println("Led B ON");
  });
  server.on("/SysOff", []()
  {
    server.send(200, "text/html", webPage);
    digitalWrite(SysLed, HIGH);
    Serial.println("Led B OFF");
  });



  server.begin();
  Serial.println("HTTP server started");
}

void wifiProcess(void)
{
  server.handleClient();
}








