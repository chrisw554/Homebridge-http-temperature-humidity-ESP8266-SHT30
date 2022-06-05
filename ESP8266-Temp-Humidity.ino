//Libraries
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WEMOS_SHT3X.h>
 
SHT3X sht30(0x45);

// Wifi Info
const char* ssid = ""; // Your Wifi SSID
const char* password = ""; // Your Wifi Passphrase


// Begin Wifi Server
WiFiServer server(80);

void setup() 
{
  Serial.begin(9600);
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server and print the IP Address
  server.begin();
  Serial.print("Server started: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}


void loop() {

if(sht30.get()==0){
    Serial.print("Temperature in Celsius : ");
    Serial.println(sht30.cTemp);
    //Serial.print("Temperature in Fahrenheit : ");
    //Serial.println(sht30.fTemp);
    Serial.print("Relative Humidity : ");
    Serial.println(sht30.humidity);
    Serial.println();
  }
  else
  {
    Serial.println("Error reading temperature and humidity!");
  }

// Check if a client has connected
  WiFiClient client = server.available();


  if (client) {
    
  
    // Wait until the client sends some data
    Serial.println("New Client");
    while(!client.available()){
      delay(1);
    }
  
   if (client.available()) {
      // Read the first line of the request
      String request = client.readStringUntil('\r');
      Serial.println(request);

      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println(""); //  do not forget this one
      
     if (request.indexOf("/temp/status") != -1)  {
        Serial.println("Responding to request...");
        client.print("{ \"temperature\": ");
        client.print(sht30.cTemp);
        client.print(", \"humidity\": ");
        client.print(sht30.humidity);
        client.println(" }");
      }
      client.flush();  
  
  delay(1000);
  }
  }
}
