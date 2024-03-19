#include <WiFiClient.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);

    IPAddress Fip(192,168,1,32);
    IPAddress Fgatvay(192,168,1, 1);
    IPAddress Fsubnet(255, 255, 255, 0);
    WiFi.config(Fip, Fgatvay, Fsubnet);
    WiFi.begin("meganet", "as07gh09");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

      Serial.println("Conected");
}

void loop() {
  // put your main code here, to run repeatedly:

}
