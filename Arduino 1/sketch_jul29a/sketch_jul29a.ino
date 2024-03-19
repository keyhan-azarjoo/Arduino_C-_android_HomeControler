#include <WiFiClient.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

WiFiServer server(3818);

/* Set these to your desired credentials. */
const char *ssid = "meganet";  //ENTER YOUR WIFI SETTINGS
const char *password = "as07gh09";

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);

  WiFi.mode(WIFI_AP_STA);
  IPAddress ip(192, 168, 4, 4);
  IPAddress gatvay(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(ip, gatvay, subnet);
  WiFi.softAP("Children", "Children");
  Serial.println("Created");
  Serial.println("MyAPIP: ");
  Serial.println(WiFi.softAPIP());
  delay(500);       //This line hides the viewing of ESP as wifi hotspot






  IPAddress Fip(192, 168, 1, 56);
  IPAddress Fgatvay(192, 168, 1, 1);
  IPAddress Fsubnet(255, 255, 255, 0);

//  WiFi.config(Fip, Fgatvay, Fsubnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }



  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

void loop() {

  HTTPClient http;    //Declare object of class HTTPClient

  String ADCData, station, getData, Link;
  //int adcvalue=analogRead(A0);  //Read Analog value of LDR
  //ADCData = String(adcvalue);   //String to interger conversion
  //station = "B";

  Link = "http://keyhantest.speedyserver.ir/home/setg?str=llljgfkgjl" ;

  http.begin(Link);     //Specify request destination

  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection

  delay(5000);  //GET Data at every 5 seconds

}
