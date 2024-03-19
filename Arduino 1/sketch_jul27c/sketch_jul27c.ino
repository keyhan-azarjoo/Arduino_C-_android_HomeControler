#include <WiFiClient.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>

WiFiServer server(3818);
int ModemTryConnectionCount = 0;
int CountBeforInfo = 4;
//MyLocalIP`FatherSSID`FatherPass`


void setup() {
    Serial.begin(115200);

  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, INPUT);
  pinMode(D8, INPUT);
  EEPROM.begin(2048);

  //Serial.begin(115200);

  WiFi.disconnect(true);

  WiFi.mode(WIFI_AP_STA);
  IPAddress ip(192, 168, 1, 4);
  IPAddress gatvay(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(ip, gatvay, subnet);
  WiFi.softAP("Children", "Children");
  Serial.println("Created");
  Serial.println("MyAPIP: ");
  Serial.println(WiFi.softAPIP());
  delay(500);

  
  if (EEPROM.read(0) == 1)
  {
    String Info = ReadFromEEPROM(1, '~');

    // Split Readed String
    String STR[50];
    split(STR, Info, '`');
    int u2 = 0;
    while (STR[u2] != "!!@##$%%") {
      u2++;
    }
    // Split Readed String

    Serial.println(Info);
    String IP = STR[0];
    String FatherSSID = STR[1];
    String FatherPass = STR[2];


    // Split Ip
    String SIP[50];
    split(SIP, IP, '.');
    int u = 0;
    while (SIP[u] != "!!@##$%%") {
      u++;
    }
    // Split Ip


    Serial.println("MyLocalIP : " + IP);
    Serial.println("MyPort : 3818");
    Serial.println("Modem SSID : " + FatherSSID);
    Serial.println("Modem Pass : " + FatherPass);

    IPAddress Fip(SIP[0].toInt(), SIP[1].toInt(), SIP[2].toInt(), SIP[3].toInt());
    IPAddress Fgatvay(SIP[0].toInt(), SIP[1].toInt(), SIP[2].toInt(), 1);
    IPAddress Fsubnet(255, 255, 255, 0);

    WiFi.config(Fip, Fgatvay, Fsubnet);

    int ConCount = 0;
    WiFi.begin(FatherSSID, FatherPass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      ModemTryConnectionCount ++;
      if (ModemTryConnectionCount == 30) {
        break;
      }
    }
    if (ModemTryConnectionCount == 30) {
      Serial.println("Wifi Not Found");
      ModemTryConnectionCount = 0;
    } else {
      Serial.println("Conected");
      ModemTryConnectionCount = 0;
    }
    EEPROM.write(0, 0);
    EEPROM.commit();
  }
  server.begin();

}

void loop() {


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








    // Split Received Info
    String STR[50];
    split(STR, res, '`');
    int u2 = 0;
    while (STR[u2] != "!!@##$%%") {
      u2++;
    }
    // Split Received Info\

    // SCommands

    if (STR[0] == "Command") {
      Serial.println("Command is : " + STR[1]);
      SetInfo(STR[1]);
    }
    if (STR[0] == "Config") {
      Serial.println("Config Started");
      SetConfig(STR[1], STR[2], STR[3], STR[4], STR[5]);
    }
    else {
      Serial.println(res);
    }
    // SCommands\

    delay(100);
  }

  delay(500);
}













































//-----------------------------------------------------------------------------------------------------------------------


















void SetConfig(String IP, String FatherSSID, String FatherPass, String MyLocation, String MyInfo) {
  String WholeInfo =  IP + '`' + FatherSSID + '`' + FatherPass + '`' + MyLocation + '`' + MyInfo + '~';
  int j = 1;
  while (j != WholeInfo.length() + 1) {
    EEPROM.write(j, WholeInfo[j - 1]);
    j++;
  }
  EEPROM.write(0, 1);
  EEPROM.commit();
  ESP.restart();
}











































void split(String Ans[50], String Str, char sep) {
  for (int i = 0 ; i < Str.length(); i++) {
    if (Str.charAt(i) == sep) {
    }
  }
  int r = 0 , t = 0, i = 0;
  for ( i = 0 ; i < Str.length(); i++) {
    if (Str.charAt(i) == sep) {
      Ans[t] = Str.substring(r, i);
      r = (i + 1);
      t++;
    }
  }
  Ans[t++] = Str.substring(r, i);
  Ans[t++] = "!!@##$%%";
}




String ReadFromEEPROM(int From, char To) {
  char c = '1';
  String Ans;
  for (int i = From ; c != To ; i++) {
    c = EEPROM.read(i);
    if (c != To) {
      Ans += c;
    }
  }
  return Ans;
}
int FindFirstInfoPointerFromEEPROM(int CountBefor, char To) {
  int counter = 0;
  int j = 0;
  char c = '1';
  for (int i = 1 ; c != To ; i++) {
    c = EEPROM.read(i);
    counter ++;
    if (c != To) {
      if (c == '`') {
        j += 1;
        if (j == CountBefor) {
          counter++;
          return counter;
        }
      }
    }
  }
  return -1;
}
void WriteInEEPROM(int WriteFrom, String STR) {

  int j = WriteFrom;
  int To = WriteFrom + STR.length();
  int k = 0;
  while (j != To) {
    EEPROM.write(j, STR[k]);
    k++;
    j++;
  }
  EEPROM.commit();
}





void SetInfo(String Info) {

  int FirstInfoPointer1 = FindFirstInfoPointerFromEEPROM(CountBeforInfo, '~');
  WriteInEEPROM(FirstInfoPointer1, Info + '~');
  String sss = ReadFromEEPROM(1, '~');

  // Split String
  String STR[50];
  split(STR, Info, '|');
  int u = 0;
  while (STR[u] != "!!@##$%%") {
    Serial.println(String(STR[u]));
    // Split String
    String STR2[50];
    split(STR2, STR[u], ':');
    if (u == 0)
    {
      if (STR2[1] == String('1')) {
        digitalWrite(D0, HIGH);
        Serial.println("D0, HIGH");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D0, LOW);
        Serial.println("D0, LOW");
      }
    }
    if (u == 1) {
      if (STR2[1] == String('1')) {
        digitalWrite(D1, HIGH);
        Serial.println("D1, HIGH");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D1, LOW);
        Serial.println("D1, LOW");
      }
    }
    if (u == 2) {
      if (STR2[1] == String('1')) {
        digitalWrite(D2, HIGH);
        Serial.println("D2, HIGH");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D2, LOW);
        Serial.println("D2, LOW");
      }
    }
    if (u == 3) {
      if (STR2[1] == String('1')) {
        digitalWrite(D3, HIGH);
        Serial.println("D3, HIGH");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D3, LOW);
        Serial.println("D3, LOW");
      }
    }
    if (u == 4) {
      if (STR2[1] == String('1')) {
        digitalWrite(D4, HIGH);
        Serial.println("D4, HIGH");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D4, LOW);
        Serial.println("D4, LOW");
      }
    }
    if (u == 5) {
      if (STR2[1] == String('1')) {
        digitalWrite(D5, HIGH);
        Serial.println("D5, HIGH");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D5, LOW);
        Serial.println("D5, LOW");
      }
    }
    if (u == 6) {
      if (STR2[1] == String('1')) {
        digitalWrite(D6, HIGH);
        Serial.println("D6, HIGH");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D6, LOW);
        Serial.println("D6, LOW");
      }
    }
    u++;
  }
  // Split String

}


