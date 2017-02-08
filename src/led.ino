#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#include "env.h"

char ReplyBuffer[] = "acknowledged";
char packetBuffer[255];
unsigned int port = 2390;

const int LED_PIN = D8;
const double MAX_BRIGHT = 6.82;

WiFiUDP Udp;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  pixels.begin();

  // begin serial and connect to WiFi
  WiFi.disconnect(true);
  delay(500);

  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Udp.begin(port);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if(packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }

    // TEMP parse data from packet
    char * temp;

    int r = 0;
    int g = 0;
    int b = 0;

    temp = strtok (packetBuffer,":");
    r = round(atoi(temp) / MAX_BRIGHT);

    if(temp != NULL){
      temp = strtok (NULL,":");
      g = round(atoi(temp) / MAX_BRIGHT);
    }

    if(temp != NULL){
      temp = strtok (NULL,":");
      b = round(atoi(temp) / MAX_BRIGHT);
    }

    Serial.println();
    Serial.println(r);
    Serial.println(g);
    Serial.println(b);

    // Send the values to the NeoPixel
    for (int i = 0; i < NUM_PIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(r,g,b));
    }

    pixels.show();

    // send a reply, to the IP address and port
    // that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
}
