/*
===========================================
programma di log in caldaia
legge tre sensori di temperatura ds18b20
===========================================
*/
#include <ESP8266WiFi.h>
#include <SPI.h>
//#include <UIPEthernet.h>
#include <DHT.h> //<DS18B20MED.h>

const char* ssid = "BASE";
const char* password = "";

#define DHT_PIN 5       // DHT Shield uses pin D4
//#define SENSOR_A  5
//#define SENSOR_B  8 
//#define SENSOR_C  7 

struct
{
    uint32_t total;
    uint32_t ok;
    uint32_t crc_error;
    uint32_t time_out;
    uint32_t connect;
    uint32_t ack_l;
    uint32_t ack_h;
    uint32_t unknown;
} stat = { 0,0,0,0,0,0,0,0};

// settaggio ethernet
WiFiServer server(80);
//IPAddress ip(192, 168, 1, 99); // where xx is the desired IP Address
//IPAddress gateway(192, 168, 1, 1); // set gateway to match your network

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //????
//char server[] = "192.168.1.45";
IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
IPAddress ip(192, 168, 1, 212); // controllare che non sia doppio
IPAddress gateAway(192, 168, 1, 254);
IPAddress DNS(8, 8, 8, 8);
//settaggio sensore
char temp_string_a[7];
//char temp_string_b[7];
//char temp_string_c[7];
//long delayTime = 6000;
//int  delayTimes = 100;
long shortLoop = 100;
long shortDelay = 100;
int lettTodo = 10;

//EthernetServer server(80);
//DS18B20MED mytest (shortDelay, shortLoop, lettTodo);

dht DHT;

void setup() {
  // Set up the data pins for communication with DS18B20 sensors
  //digitalWrite(SENSOR_A, LOW);
  //pinMode(SENSOR_A, INPUT);
  //digitalWrite(SENSOR_B, LOW);
  //pinMode(SENSOR_B, INPUT);
  //digitalWrite(SENSOR_C, LOW);
  //pinMode(SENSOR_C, INPUT);
  
  // give the Ethernet shield a second to initialize:
  //delay(10);
  Serial.begin(9600);
  // start the Ethernet connection:
  WiFi.config(ip, gateAway, subnet);
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");

  //Ethernet.begin(mac, ip, DNS, gateAway);
  //server.begin();
  Serial.println(DHT_LIB_VERSION);
  }
void loop() {
  WiFiClient client = server.available();
  //EthernetClient client = server.available();
  if (client) {
    //Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          mytest.DataConv (mytest.lettCycle(SENSOR_A), temp_string_a);
          client.print("temp A ");
          client.print(temp_string_a);
          /*mytest.DataConv (mytest.lettCycle(SENSOR_B), temp_string_b);
          client.print(" temp B ");
          client.print(temp_string_b);
          mytest.DataConv (mytest.lettCycle(SENSOR_C), temp_string_c);
          client.print(" temp C ");
          client.print(temp_string_c);*/
          client.println("<br />");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    //Serial.println("client disconnected");
  } 
}
