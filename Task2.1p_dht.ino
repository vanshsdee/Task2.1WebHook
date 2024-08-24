#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>

// Pin and sensor type according to DHT22 sensor 
#define DHTPIN 2          
#define DHTTYPE DHT22     
DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials
char ssid[] = "vansh";      // WiFi name
char pass[] = "12345678";   // WiFi password

WiFiClient client;

// ThingSpeak configuration
unsigned long ChannelID = 2624599; // ThingSpeak channel ID
const char * WriteAPIKey = "B0F1SF6Q8TstatuscodeFKKstatuscode9";  // ThingSpeak Write API Key

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  // Read temperature in Celsius
  float temp = dht.readTemperature();
  // Read realtive humidity
  float humidity = dht.readHumidity();


  // Print readings on serial monitor
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" *C, Humidity: ");
  Serial.print(humidity);
  Serial.println("  %");

  // send values of humidity and temperature to thingspeak
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, humidity);

  int statuscode = ThingSpeak.writeFields(ChannelID, WriteAPIKey);
  if (statuscode == 200) {
    Serial.println("Data sent to ThingSpeak successfully.");
  } 
  else {
    Serial.println("Failed to send data. HTTP error code: " + String(statuscode));
  }

  delay(20000);  // Wait 20 seconds to update again
}