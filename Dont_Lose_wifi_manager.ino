#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

WiFiServer server(80);

const int OUTPUT_PIN = D7;

void setup() {
  Serial.begin(115200);
  pinMode(OUTPUT_PIN, OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  
  digitalWrite(OUTPUT_PIN, LOW);

  WiFiManager wifiManager;

  wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  wifiManager.autoConnect("Dont Lose Anything 2");
  Serial.println("Connected.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.begin();
  Serial.println(WiFi.localIP());

}

int32_t getRSSI(const char* target_ssid) {
  byte available_networks = WiFi.scanNetworks();

  for (int network = 0; network < available_networks; network++) {
    if (strcmp(WiFi.SSID(network).c_str(), target_ssid) == 0) {
      return WiFi.RSSI(network);
    }
  }
  return 0;
}

void loop() {
  WiFiClient client = server.available();
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port

    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,

        unsigned long before = millis();
        int32_t rssi = getRSSI(WiFi.SSID().c_str());
        unsigned long after = millis();
        Serial.print("Signal strength: ");
        Serial.print(rssi);
        if (rssi < -45)
        { digitalWrite(LED_BUILTIN, LOW);
          tone(D8, 2000, 500);
          Serial.println("device is far ");
        }
        else
        { digitalWrite(LED_BUILTIN, HIGH);

        }
        Serial.println("dBm");

        Serial.print("Took ");
        Serial.print(after - before);
        Serial.println("ms");
        delay(100);
      }


    }
  }


}
