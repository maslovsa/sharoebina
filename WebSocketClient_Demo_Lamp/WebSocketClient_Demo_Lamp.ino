#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

#define SysLed 2
#define LedR 15
#define LedG 12
#define LedB 13

int blinkDelay = 500;
int requestkDelay = 500;

const char* ssid     = "zloy_zyxel";
const char* password = "ROKOLABS2015!";
char path[] = "/";
char host[] = "https://sharaebina-node.herokuapp.com";

  
WebSocketClient webSocketClient;

// Use WiFiClient class to create TCP connections
WiFiClient client;

void setup() {
  pinMode(SysLed, OUTPUT);
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(LedB, OUTPUT);
  digitalWrite(SysLed, HIGH);
  digitalWrite(LedR, LOW);
  digitalWrite(LedG, LOW);
  digitalWrite(LedB, LOW);
  
  Serial.begin(74880);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(3000);
  Serial.println("Try to connect"); 

  // Connect to the websocket server
  if (client.connect(host, 8000)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    while(1) {
      // Hang on failure
    }
  }

  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = host;
  Serial.println("Try to handshake"); 
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
    while(1) {
      // Hang on failure
    }  
  }

}

void loop() {
  String data;

  if (client.connected()) {
    
    webSocketClient.getData(data);
    if (data.length() > 0) {
      Serial.print("Received data << ");
      Serial.println(data);
      blink(data.toInt());
    }
    
    Serial.println("sent >> hello");
    webSocketClient.sendData("ready");
    
  } else {
    Serial.println("Client disconnected.");
    while (1) {
      // Hang on disconnect.
    }
  }
  
  // wait to fully let the client disconnect
  delay(requestkDelay);
  
}

/// Private

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

