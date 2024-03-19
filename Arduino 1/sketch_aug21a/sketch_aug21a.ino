#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>



char* ssid = "meganet";
char* pass = "as07gh09";
WiFiServer server(3818);

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  EEPROM.begin(512);
  Serial.begin(115200);

  WiFi.disconnect(true);

  WiFi.mode(WIFI_AP_STA);
  IPAddress ip1(192, 168, 1, 20);
  IPAddress gatvay1(192, 168, 1, 1);
  IPAddress subnet1(255, 255, 255, 0);
  WiFi.softAPConfig(ip1, gatvay1, subnet1);
  WiFi.softAP("ArdoinoKey", "ArdoinoKey");
  Serial.println("\n\rCreated");
  Serial.println("MyIP: ");
  Serial.println(WiFi.softAPIP());
  delay(3000);

  if (EEPROM.read(0) == 192) {
    int value1 = EEPROM.read(0);
    int value2 = EEPROM.read(1);
    int value3 = EEPROM.read(2);
    int value4 = EEPROM.read(3);

    IPAddress ip(value1, value2, value3, value4);
    IPAddress gatvay(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(ip, gatvay, subnet);

  }

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n\rconnected");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Port: 3818");
  if (EEPROM.read(0) != 192) {
    String* MyIP;
    String I = WiFi.localIP().toString();
    split(MyIP, I, ',');
    
    EEPROM.write(0, MyIP[0].toInt());
    EEPROM.write(1, MyIP[1].toInt());
    EEPROM.write(2, MyIP[2].toInt());
    EEPROM.write(3, MyIP[3].toInt());
    EEPROM.commit();
    delay(200);
    ESP.restart();
  }
  int value = EEPROM.read(0);
  Serial.println(value);

  server.begin();
}

void loop() {

  WiFiClient client = server.available();
  if (!client) {
    client.stop();
    return;
  }
  int c = 0;
  while (!client.available() && c < 1000) {
    delay(10);
    c++;
  }
  if (client.available()) {
    String res;
    while (client.available()) {
      char ch = client.read();
      res += ch;
    }
    if (res == "D0=On") {

      digitalWrite(D0, HIGH);
      delay(2000);
      Serial.print("D0 On\r\n");
      client.print("D0 On\r\n");

    }
    if (res == "D0=Off") {
      digitalWrite(D0, LOW);
      delay(2000);
      Serial.print("D0 Off\r\n");
      client.print("D0 Off\r\n");
    }
    if (res == "D1=On") {

      digitalWrite(D1, HIGH);
      delay(2000);
      Serial.print("D1 On\r\n");
      client.print("D1 On\r\n");

    }
    if (res == "D1=Off") {
      digitalWrite(D1, LOW);
      delay(2000);
      Serial.print("D1 Off\r\n");
      client.print("D1 Off\r\n");
    }
    if (res == "Reset") {

      //ESP.reset(); //az void setub reset mishavad va sakht afzari reset nemishavad vali
      ESP.restart();//sakht afzari reset mishavad va behtarast
      Serial.print("D1 Off\r\n");
      client.print("D1 Off\r\n");
    }


  }
  client.stop();


}
void split(String* buffer, String s1, char sep) {
  for (int i = 0 , j = 0 ; i < s1.length()-1 ; i++) {
    if (s1[i] != sep) buffer[j] += s1[i];
      else j++;
  }
}

