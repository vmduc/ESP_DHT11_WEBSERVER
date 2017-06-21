/*
DEMO WEBSERVER
DHT11
ARDUINO UNO
ESP 8266 V12
*/

#include <ESP8266WiFi.h>
#include "DHT11.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);

int pin = 2;
DHT11 dht11(pin);
int err;
float temp, humi;

double Fahrenheit(double celsius) {
  return ((double)(9 / 5) * celsius) + 32;
}

double Kelvin(double celsius) {
  return celsius + 273.15;
}

/*
const char* ssid = "Mach_Dien_Tu";
const char* password = "Hshopvn123";

*/

const char* ssid = "C17 Hercules";
const char* password = "acerpcksd";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected

  if ((err = dht11.read(humi, temp)) == 0)
  {
    Serial.print("temperature:");
    Serial.print(temp);
    Serial.print(" humidity:");
    Serial.print(humi);
    Serial.println();

    lcd.setCursor(0,1);
    lcd.println("Temperature: ");
    lcd.println(temp);
    lcd.setCursor(0,0);
    lcd.println("Humidity: ");
    lcd.println(humi);
  }

  client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }
  client.flush();
  // Send the response to the client
  datt();
  //  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}
void datt() {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");  // the connection will be closed after completion of the response
  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html xmlns='http://www.w3.org/1999/xhtml'>");
  client.println("<head>\n<meta charset='UTF-8'>");
  client.println("<title>ESP8266 Temperature & Humidity DHT11 Sensor-</title>");
  
  client.println("</head>\n<body>");
  client.println("<H1>DEMO-VMDUC</H2>");
  client.println("<H2>ESP8266 & DHT11 Sensor</H2>");
  client.println("<H3>Humidity / Temperature</H3>");
  client.println("<pre>");
  client.print("Humidity (%)         : ");
  client.println((float)humi, 2);
  client.print("Temperature (°C)  : ");
  client.println((float)temp, 2);
  client.print("Temperature (°F)  : ");
  client.println(Fahrenheit(temp), 2);
  client.print("Temperature (°K)  : ");
  client.println(Kelvin(temp), 2);
  client.println("</pre>");
  client.print("</body>\n</html>");
  delay(DHT11_RETRY_DELAY); //delay for reread
}
