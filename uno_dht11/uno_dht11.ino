#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
#include <stdlib.h>

#define DHTPIN1 2
#define DHTPIN2 3
#define DHTTYPE DHT11

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "snickerbo.herokuapp.com";
static char tempstr1[15];
static char humstr1[15];
static char tempstr2[15];
static char humstr2[15];

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,1,51);
EthernetClient client;
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

unsigned long postTimer;

void setup() {
    Serial.begin(9600);
}

void loop()
{
    if(millis()-postTimer >= 10000UL) {
      postTimer = millis();
      postData();
    }
}

void postData() {
  
    // start the Ethernet connection:
    if (Ethernet.begin(mac) == 0) {
      // try to congifure using IP address instead of DHCP:
      Ethernet.begin(mac, ip);
    }
    // give the Ethernet shield a second to initialize:
    delay(1000);
  
    if (client.connect(server, 80)) {
      
      dtostrf(dht1.readTemperature(),5, 1, tempstr1);
      dtostrf(dht1.readHumidity(),5, 1, humstr1);
      dtostrf(dht2.readTemperature(),5, 1, tempstr2);
      dtostrf(dht2.readHumidity(),5, 1, humstr2);
      
      String PostData="macaddr=testaddr"; //add supersecret api key
      PostData=PostData+"&temperature1=";
      PostData=PostData+tempstr1;
      PostData=PostData+"&humidity1=";
      PostData=PostData+humstr1;
      PostData=PostData+"&temperature2=";
      PostData=PostData+tempstr2;
      PostData=PostData+"&humidity2=";
      PostData=PostData+humstr2;
      Serial.println(PostData);
  
      client.println("POST /postdata HTTP/1.1");
      client.println("Host:  snickerbo.herokuapp.com");
      client.println("User-Agent: Arduino/1.0");
      client.println("Connection: close");
      client.println("Content-Type: application/x-www-form-urlencoded;");
      client.print("Content-Length: ");
      client.println(PostData.length());
      client.println();
      client.println(PostData);

      client.flush();
      client.stop();
    } 
    else {
      Serial.println("Client not connected - could not send data.");
    }
}
