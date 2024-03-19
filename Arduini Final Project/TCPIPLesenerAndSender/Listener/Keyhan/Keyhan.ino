#include<ESP8266WiFi.h>
#include<WiFiClient.h>

#define Max_Clients 10

char* ssid = "meganet";
char* password = "as07gh09";
String paylod[Max_Clients] = "";

WiFiClient client;//
WiFiServer server(1234);

void setup() {
  Serial.begin(115200);
  Serial.println("");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\r\nWiFi Connected\r\nConnectMe With this URL: http://");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.print("Server PORT: 1234");
  server.setNoDelay(true);
}

void loop() {
  if (server.Client()) {
    //for (int i = 0; i < Max_Clients; i++) {
      if (!client or !client.connected) {
        if (client)
          client.stop();
        client = server.available();
      }
    //}
  }
  for (int i = 0; i < Max_Clients; i++) {
    if (client[i] && client[i].connected() && client[i].available()) {
      paylod[i] = "";
      while (client[i].available()) {
        char ch = client.read();
        paylod[i] += ch;
      }
      Serial.print("New Data Received: " + paylod);
    }
  }
  
}
