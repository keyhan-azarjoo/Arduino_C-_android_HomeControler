#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

YunServer server;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Starting bridge...\n");

  Bridge.begin();

  delay(2000);
  Serial.println("Done.\n");
}

void loop() {

  YunClient client = server.accept();

  if (client) {
    // read the command
    String command = "DDDDDDDDD";
    command.trim();        //kill whitespace
    Serial.println(command);

    // Close connection and free resources.
    client.stop();
  }
  delay(50);
}
