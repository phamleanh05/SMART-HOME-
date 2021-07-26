//Leonardo
//Serial_ & MonitorSerial = Serial;
//HardwareSerial & EspSerial = Serial1;
 
////UNO & M328P
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX uoe for monitor
HardwareSerial & MonitorSerial = Serial;

//
////MEGA2560 
//HardwareSerial & MonitorSerial = Serial;
//HardwareSerial & EspSerial = Serial1; 
 
// set pin numbers:
const int ledPin =  13;      // the number of the LED pin
const int ESP8266_CHPD = 4;
 
// Variables will change:
int ledState = LOW;             // ledState used to set the LED
 
#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE];
 
void setup() { 
  pinMode(ledPin, OUTPUT);  
  //pinMode(ESP8266_CHPD, OUTPUT);
      
  MonitorSerial.begin(9600); // Serial monitor
  EspSerial.begin(9600); // ESP8266
    
  //while (!MonitorSerial) {
   // ; // wait for serial port to connect. Needed for Leonardo only
  //}
 
  MonitorSerial.println(F("ESP8266 demo."));
 
  //hardReset();
  //delay(2000);
  
  clearSerialBuffer();
  
  //test if the module is ready
  //MonitorSerial.print("AT : ");
  //MonitorSerial.println( GetResponse("AT",100) );
    
  //Change to mode 1
  MonitorSerial.print("AT+CWMODE=1 : ");
  MonitorSerial.println( GetResponse("AT+CWMODE=1",10) );
  
  //connect to router
  connectWiFi("Hshop", "hshopa23");
  delay(5000);
        
  //set the multiple connection mode
  MonitorSerial.print(F("AT+CIPMUX=1 : "));
  MonitorSerial.println( GetResponse("AT+CIPMUX=1",10) );
  
  //set the server of port 8888 check "no change" or "OK"
  MonitorSerial.print(F("AT+CIPSERVER=1,8888 : "));
  MonitorSerial.println( GetResponse("AT+CIPSERVER=1,8888", 10) );
 
  //set time out
  //MonitorSerial.print("AT+CIPSTO=15 : ");
  //MonitorSerial.println( GetResponse("AT+CIPSTO=15",10) );
  
   //print the ip addr
  MonitorSerial.print(F("ip address : "));
  MonitorSerial.println( GetResponse("AT+CIFSR", 10) );
  delay(200);
 
   
  MonitorSerial.println();
  MonitorSerial.println(F("Start Webserver"));
 
  digitalWrite(ledPin,ledState);  
}
 
void loop() {
  int ch_id, packet_len;
  char *pb;  
  EspSerial.readBytesUntil('\n', buffer, BUFFER_SIZE);
  
  if(strncmp(buffer, "+IPD,", 5)==0) {
    // request: +IPD,ch,len:data
   MonitorSerial.println(buffer);
    sscanf(buffer+5, "%d,%d", &ch_id, &packet_len);
    if (packet_len > 0) {
      // read serial until packet_len character received
      // start from :
      pb = buffer+5;
      while(*pb!=':') pb++;
      pb++;
      if (strncmp(pb, "GET /on ", 8) == 0) {
        MonitorSerial.print(millis());
        MonitorSerial.print(" : ");
        MonitorSerial.println(buffer);
        MonitorSerial.print( "get led from ch :" );
        MonitorSerial.println(ch_id);
 
        delay(100);
        clearSerialBuffer();
          ledState = HIGH;
        digitalWrite(ledPin, ledState);
        
        homepage(ch_id);
 
      } 
      else if  (strncmp(pb, "GET /off ", 9) == 0) {
        MonitorSerial.print(millis());
        MonitorSerial.print(" : ");
        MonitorSerial.println(buffer);
        MonitorSerial.print( "get led from ch :" );
        MonitorSerial.println(ch_id);
 
        delay(100);
        clearSerialBuffer();
          ledState = LOW;
        digitalWrite(ledPin, ledState);
        
        homepage(ch_id);
      }
      else if (strncmp(pb, "GET / ", 6) == 0) {
        MonitorSerial.print(millis());
        MonitorSerial.print(" : ");
        MonitorSerial.println(buffer);
        MonitorSerial.print( "get Status from ch:" );
        MonitorSerial.println(ch_id);
        
        delay(100);
        clearSerialBuffer();
 
        homepage(ch_id);
      }
    }
  }
  clearBuffer();
}
 
void homepage(int ch_id) {
  String Header;
 
  Header =  "HTTP/1.1 200 OK\r\n";
  Header += "Content-Type: text/html\r\n";
  Header += "Connection: close\r\n";  
  //Header += "Refresh: 5\r\n";
  
  String Content;
  Content = "<a href='./on'>Turn On LED</a><br/>";
  Content += "<a href='./off'>Turn Off LED</a><br />";
 // Content += String(ledState);
  
  Header += "Content-Length: ";
  Header += (int)(Content.length());
  Header += "\r\n\r\n";
  
  
  EspSerial.print("AT+CIPSEND=");
  EspSerial.print(ch_id);
  EspSerial.print(",");
  EspSerial.println(Header.length()+Content.length());
  delay(10);
  
  if (EspSerial.find(">")) {
      EspSerial.print(Header);
      EspSerial.print(Content);
      delay(10);
   }
}
 
 
 
// Get the data from the WiFi module 
// and send it to the debug serial port
String GetResponse(String AT_Command, int wait){
  String tmpData;
  
  EspSerial.println(AT_Command);
  delay(wait);
  while (EspSerial.available() >0 )  {
    char c = EspSerial.read();
    tmpData += c;
    
    if ( tmpData.indexOf(AT_Command) > -1 )         
      tmpData = "";
    else
      tmpData.trim();  // xóa khoảng trắng 2 bên   
          
   }
   return tmpData;
}
 
boolean hardReset() {
  String tmpData;
  
  digitalWrite(ESP8266_CHPD,LOW);
  delay(100);
  digitalWrite(ESP8266_CHPD,HIGH);
  delay(1000);
    
  while ( EspSerial.available() > 0 ) {
    char c = EspSerial.read();
    tmpData +=c;
    EspSerial.write(c);
    if ( tmpData.indexOf("Ready") > -1 ) {
      //Serial.println("Ready");
        clearBuffer();
        return 1;
    } 
  }
}
 
void clearSerialBuffer(void) {
       while ( EspSerial.available() > 0 ) {
         EspSerial.read();
       }
}
 
void clearBuffer(void) {
       for (int i =0;i<BUFFER_SIZE;i++ ) {
         buffer[i]=0;
       }
}
         
boolean connectWiFi(String NetworkSSID,String NetworkPASS) {
  String cmd = "AT+CWJAP=\"";
  cmd += NetworkSSID;
  cmd += "\",\"";
  cmd += NetworkPASS;
  cmd += "\"";
  
  MonitorSerial.println(cmd); 
  MonitorSerial.println(GetResponse(cmd,10));
}
