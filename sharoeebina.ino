#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

#define SysLed 2
#define LedR 15
#define LedG 12
#define LedB 13

MDNSResponder mdns;

const char* ssid = "zloy_zyxel";
const char* passw = "ROKOLABS2015!";
const char* token = "123456";
int blinkDelay = 3000;
int serverCellDelay = 4000;

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
//  
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
  Serial.println("INIT Sharoebina");

  // ждем соединения
  WiFi.begin(ssid, passw);
  while (WiFi.status() != WL_CONNECTED)
  {
//    delay(500); Serial.print(".");
    digitalWrite(SysLed, LOW);
    delay(250);
    digitalWrite(SysLed, HIGH);
    delay(250);
    Serial.print(".");
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

void wifiProcess(void) {
  server.handleClient();


  if(WiFi.status() == WL_CONNECTED) {

        HTTPClient http;

        Serial.print("[HTTP] begin...\n");
        // configure traged server and url
        http.begin("http://sharaeebina.herokuapp.com/status?id=123456"); //HTTP

        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                blink(payload.toInt());
                Serial.println(payload);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();

        delay(serverCellDelay);
    }
}

void blink(int data) {
  bool needR = (data & 1 << 2) != 0;
  bool needG = (data & 1 << 1) != 0;
  bool needB = (data & 1 << 0) != 0;
  
  if (needR) {
      digitalWrite(LedR, HIGH);
      Serial.printf("R %d\n", data & 1 << 2);
  }

  if (needG) {
      digitalWrite(LedG, HIGH);
      Serial.printf("G %d\n", data & 1 << 1);
  }

  if (needB) {
      digitalWrite(LedB, HIGH);
      Serial.printf("B %d\n", data & 1 << 0);
  }

  delay(blinkDelay);

  digitalWrite(LedR, LOW);
  digitalWrite(LedG, LOW);
  digitalWrite(LedB, LOW);
}






