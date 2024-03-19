#include <Bridge.h>
#include <WiFi.h>
#include <YunClient.h>

#define PORT 255

// Define our client object
YunClient client;

void setup()
{
  
  // Bridge startup
  Bridge.begin();
  Serial.begin(9600);
  while (!Serial); // wait for a serial connection


  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFISOFR);
  IPAddress ip(192, 168, 4, 2);
  IPAddress gatvay(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(ip, gatvay, subnet);
  WiFi.softAP("Father", "Father");
  Serial.println("\r\nFather");
  Serial.println("\r\nCreated");
  Serial.println("MyAPIP: ");
  Serial.println(WiFi.softAPIP());
  delay(500);


}

void loop()
{
  // Make the client connect to the desired server and port
  IPAddress addr(192, 168, 42, 185);
  
  // Or define it using a single unsigned 32 bit value
  // IPAddress addr(0xc0a8sab9); // same as 192.168.42.185
  
  // Or define it using a byte array
  // const uint8 addrBytes = {192, 168, 42, 185};
  // IPAddress addr(addrBytes);

  client.connect(addr, PORT);
  
  // Or connect by a server name and port.
  // Note that the Yun doesn't support mDNS by default, so "Yun.local" won't work
  // client.connect("ServerName.com", PORT);

  if (client.connected())
  {
    Serial.println("Connected to the server.");
    
    // Send something to the client
    client.println("Something...");

    // Cheap way to give the server time to respond.
    // A real application (as opposed to this simple example) will want to be more intelligent about this.
    delay (250);
  
    // Read all incoming bytes available from the server and print them
    while (client.available())
    {
      char c = client.read();
      Serial.print(c);
    }
    Serial.flush();

    // Close the connection
    client.stop();
  }
  else
    Serial.println("Could not connect to the server.");  
    
  // Give some time before trying again
  delay (10000);
}
