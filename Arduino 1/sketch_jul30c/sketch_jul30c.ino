#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>


WiFiServer server(3818);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_AP_STA);
  IPAddress ip(192, 168, 4, 4);
  IPAddress gatvay(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(ip, gatvay, subnet);
  WiFi.softAP("Children", "Children");

  Serial.println("\r\nCreated");
  Serial.println("MyAPIP: ");
  Serial.println(WiFi.softAPIP());
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:




  //Wifi Listener
  WiFiClient client = server.available();
  if (!client) {
    client.stop();
    return;
  }
  int c = 0;
  while (!client.available() && c < 100) {
    delay(10);
    c++;
  }





  if (client.available()) {
    String res;
    while (client.available()) {
      char ch = client.read();
      res += ch;
    }
    //Wifi Listener\

    Serial.print(res);
  }
}
