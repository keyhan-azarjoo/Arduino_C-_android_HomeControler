#include <WiFiClient.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

WiFiServer server(3818);
int ModemTryConnectionCount = 0;
int CountBeforInfo = 6;
//MyLocalIP`FatherSSID`FatherPass`

void setup() {
  Serial.begin(115200);
  EEPROM.begin(2048);

  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, INPUT);
  pinMode(D8, INPUT);


  //EEPROM.write(0, 0);
  //EEPROM.commit();


  WiFi.disconnect(true);

  WiFi.mode(WIFI_AP_STA);
  IPAddress ip1(192, 168, 4, 4);
  IPAddress gatvay1(192, 168, 4, 1);
  IPAddress subnet1(255, 255, 255, 0);
  WiFi.softAPConfig(ip1, gatvay1, subnet1);
  WiFi.softAP("KeyhanIP2", "KeyhanIP2");
  Serial.println("\n\rCreated");
  Serial.println("MyIP: ");
  Serial.println(WiFi.softAPIP());


  delay(500);
  ConnectToWifi();

}

void loop() {
  if ((WiFi.status() != WL_CONNECTED)) {
    ConnectToWifi();
    delay(50);
  }

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
    split(STR, res, char(23));
    int u2 = 0;
    while (STR[u2] != String(char(4))) {
      u2++;
    }
    // Split Received Info\

    // SCommands

    if (STR[0] == "Command") {
      Serial.println("\r\nCommand is : " + STR[1]);
      SetInfo(STR[1]);
    }
    if (STR[0] == "Config") {
      Serial.println("Config Started");
      SetConfig(STR[1], STR[2], STR[3], STR[4], STR[5]);
    }
    if (STR[0] == "GetInfo") {

      Serial.println("Send Info");
      InfoRequest();
    }
    else {
      Serial.println(res);
    }
    // SCommands\
  }

  delay(50);
}

}



//-----------------------------------------------------------------------------------------------------------------------



void InfoRequest()
{
  if (EEPROM.read(0) == 1)
  {
    String Info = "Info" +  String(char(23)) + ReadFromEEPROM(1, 3);
    Serial.println("Request ansver : " + Info);
    SendMessageToIP("", Info);
  }
  else
  {
    String Mac = "Mac" +  String(char(23)) + WiFi.macAddress() + String(char(3)) ;
    Serial.println("Request ansver : " + Mac);
    SendMessageToIP("", Mac);
  }



}


void SendMessageToIP(String IP, String Message) {

}


String SendMessageToCloud(String Message) {

  HTTPClient http;    //Declare object of class HTTPClient
  String ADCData, station, getData, Link, ans;
  //int adcvalue=analogRead(A0);  //Read Analog value of LDR
  //ADCData = String(adcvalue);   //String to interger conversion
  //station = "B";

  Link = "http://keyhantest.speedyserver.ir/home/setg?str=" + Message ;

  http.begin(Link);     //Specify request destination

  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println("Code" + httpCode); //Print HTTP return code
  Serial.println("Payl" + payload);  //Print request response payload
  Serial.println("Mess" + Message);
  http.end();  //Close connection

  delay(5000);  //GET Data at every 5 seconds
  return payload;
}

void SetConfig(String MyLocalIP, String ModemSSID, String ModemPass, String MyUsername, String MyPassWord) {

  String MyInfo = "A:0|B:0|C:0|D:0|E:0|F:0|G:0|H:0|I:0";
  String Mac = WiFi.macAddress();
  String WholeInfo =  MyLocalIP + char(23) + ModemSSID + char(23) + ModemPass + char(23) + MyUsername + char(23) + MyPassWord + char(23) + Mac + char(23) + MyInfo + char(3);
  Serial.println(WholeInfo);
  int j = 1;
  while (j != WholeInfo.length() + 2) {

    EEPROM.write(j, WholeInfo[j - 1]);
    Serial.println( WholeInfo[j - 1]);
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
  Ans[t++] =  char(4);
}
void split(String Ans[50], String Str, int sep) {
  for (int i = 0 ; i < Str.length(); i++) {
    if (Str.charAt(i) == char(sep)) {
    }
  }
  int r = 0 , t = 0, i = 0;
  for ( i = 0 ; i < Str.length(); i++) {
    if (Str.charAt(i) == char(sep)) {
      Ans[t] = Str.substring(r, i);
      r = (i + 1);
      t++;
    }
  }
  Ans[t++] = Str.substring(r, i);
  Ans[t++] =  char(4);
}




String ReadFromEEPROM(int From, int To) {

  char c = '1';
  String Ans;
  for (int i = From ; c != char(To) ; i++) {
    c = EEPROM.read(i);
    Ans += c;
  }
  return Ans;// + String(char(To));
}


int FindFirstInfoPointerFromEEPROM(int CountBefor, char To) {
  int counter = 0;
  int j = 0;
  char c = '1';
  for (int i = 1 ; c != To ; i++) {
    c = EEPROM.read(i);
    counter ++;
    if (c != To) {
      if (c == char(23)) {
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
  EEPROM.write(0, 1);
  EEPROM.commit();
}





void SetInfo(String Info) {

  int FirstInfoPointer1 = FindFirstInfoPointerFromEEPROM(CountBeforInfo, char(3));
  WriteInEEPROM(FirstInfoPointer1, Info + char(3));
  String WholeInfo = ReadFromEEPROM(1, 3);

  // Split String
  String STR[50];
  split(STR, Info, '|');
  int u = 0;
  while (STR[u] != String(char(4))) {
    Serial.println(String(STR[u]));
    // Split String
    String STR2[50];
    split(STR2, STR[u], ':');
    if (u == 0)
    {
      if (STR2[1] == String('1')) {
        digitalWrite(D0, HIGH);
        Serial.print("  D0, HIGH  ");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D0, LOW);
        Serial.print("  D0, LOW  ");
      }
    }
    if (u == 1) {
      if (STR2[1] == String('1')) {
        digitalWrite(D1, HIGH);
        Serial.print("  D1, HIGH  ");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D1, LOW);
        Serial.print("  D1, LOW  ");
      }
    }
    if (u == 2) {
      if (STR2[1] == String('1')) {
        digitalWrite(D2, HIGH);
        Serial.print("  D2, HIGH  ");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D2, LOW);
        Serial.print("  D2, LOW  ");
      }
    }
    if (u == 3) {
      if (STR2[1] == String('1')) {
        digitalWrite(D3, HIGH);
        Serial.print("  D3, HIGH  ");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D3, LOW);
        Serial.print("  D3, LOW  ");
      }
    }
    if (u == 4) {
      if (STR2[1] == String('1')) {
        digitalWrite(D4, HIGH);
        Serial.print("  D4, HIGH  ");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D4, LOW);
        Serial.print("  D4, LOW  ");
      }
    }
    if (u == 5) {
      if (STR2[1] == String('1')) {
        digitalWrite(D5, HIGH);
        Serial.print("  D5, HIGH  ");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D5, LOW);
        Serial.print("  D5, LOW  ");
      }
    }
    if (u == 6) {
      if (STR2[1] == String('1')) {
        digitalWrite(D6, HIGH);
        Serial.print("  D6, HIGH  ");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D6, LOW);
        Serial.print("  D6, LOW  ");
      }
    }
    if (u == 7) {
      if (STR2[1] == String('1')) {
        digitalWrite(D7, HIGH);
        Serial.print("  D7, HIGH  ");
      }
      if (STR2[1] == String('0')) {
        digitalWrite(D7, LOW);
        Serial.print ("  D7, LOW  ");
      }
    }
    u++;
  }
  // Split String

  String Inf =  ReadFromEEPROM(1, 23);
  String D = SendMessageToCloud(Info);
  Serial.print("Web Answer : " + D);

}




void ConnectToWifi() {
  String IP, ModemSSID , ModemPass , MyUserName , MyPassWord , Mac , State ;
  if (EEPROM.read(0) == 1)
  {
    String WholeInf = ReadFromEEPROM(1, 3);
    Serial.println(WholeInf);
    // Split Readed String
    String STR[50];
    split(STR, WholeInf, 23);

    int u2 = 0;
    while (STR[u2] != String (char(4))) {
      u2++;
    }
    // Split Readed String

    //Serial.println(Inf);
    IP = STR[0];
    ModemSSID = STR[1];
    ModemPass = STR[2];
    MyUserName = STR[3];
    MyPassWord = STR[4];
    Mac = STR[5];
    State = STR[6];
    // Split Ip
    String SIP[10];
    split(SIP, IP, '.');
    int u = 0;
    while (SIP[u] != String(char(4))) {
      u++;
    }
    // Split Ip
    IPAddress Fip(SIP[0].toInt(), SIP[1].toInt(), SIP[2].toInt(), SIP[3].toInt());
    IPAddress Fgatvay(SIP[0].toInt(), SIP[1].toInt(), SIP[2].toInt(), 1);
    IPAddress Fsubnet(255, 255, 255, 0);
    WiFi.config(Fip, Fgatvay, Fsubnet);

    int ConCount = 0;

    WiFi.begin(ModemSSID, ModemPass);
    Serial.println("Try To Connect To : " + ModemSSID);
    Serial.println("Password : " + ModemPass);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      ModemTryConnectionCount ++;
      if (ModemTryConnectionCount == 20) {
        break;
      }
    }
    if (ModemTryConnectionCount == 20) {
      Serial.println("\r\nWifi Not Found");
      ModemTryConnectionCount = 0;
    } else
    {
      Serial.println("\r\nConected");
      String IPaddress;
      String Mac;
      IPaddress =  WiFi.localIP().toString();
      Mac = WiFi.macAddress();
      Serial.println("MyLocalIP : " + IPaddress);
      Serial.println("Mack : " + Mac);

      String MyInf = IPaddress + String(char(23)) + ModemSSID + String(char(23)) + ModemPass +  String(char(23)) + MyUserName +  String(char(23)) + MyPassWord +  String(char(23)) + Mac  +  String(char(23))  + State;
      Serial.print(MyInf);
      WriteInEEPROM(1, MyInf);
    }
    //EEPROM.write(0, 0);
    //EEPROM.commit();
  }
  server.begin();
}


