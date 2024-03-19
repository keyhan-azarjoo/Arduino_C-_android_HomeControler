#include <ESP8266WiFi.h>

const char* ssid     = "meganet";
const char* password = "as07gh09";

const char* host = "192.168.1.11";//
const int httpPort = 3818;

int value = 0;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println("\t\n");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

WiFiClient client;
int counter = 0;

void loop() {
  delay(30000);
  // Use WiFiClient class to create TCP connections
  client.stop();
  if (client.connect(host, httpPort)) {
    counter++;
    
    // This will send the request to the server
    client.print("ON" + String(counter));
    client.flush();
    Serial.println("Requesting URL: ON" + String(counter));
  }
  unsigned long timeout = millis();
  //  while (client.available() == 0) {
  //    if (millis() - timeout > 5000) {
  //      Serial.println(">>> Client Timeout !");
  //      client.stop();
  //      return;
  //    }
  //  }
  if (client.available()) {
    // Read all the lines of the reply from server and print them to Serial
    String line = "";
    while (client.available()) {
      char ch  = client.read();
      line += ch;
    }
    Serial.print("Paylod: " + line);
    Serial.println();
    Serial.println("closing connection");
    client.stop();
  }
}
