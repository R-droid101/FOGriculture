#include <Arduino.h>
#include <math.h>
#include <string.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <Adafruit_BMP280.h>
#include <WebServer.h>
#include <WiFiClient.h>
WiFiClient wifiClient;

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#define OLED_RESET -1
#define DHT_PIN 13
#define DHT_TYPE DHT11
#define POWER_PIN  25
#define SIGNAL_PIN 33

// Wifi SSID and password
const char* ssid = "";
const char* password = "";

DHT dht(DHT_PIN, DHT_TYPE);
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
Adafruit_BMP280 bmp;
WebServer server ( 80 );

// utility function to begin server
void beginServer() {
  server.on("/", handleRoot)
  server.begin();
  Serial.println("HTTP server started");
}

void handleRoot() {
  server.send(200, "text/html", getPage());
}

String getPage() {
  // write the HTML page here
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  bmp.begin(0x76);
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  pinMode(POWER_PIN, OUTPUT); 
  digitalWrite(POWER_PIN, LOW);
  WiFi.enableSTA(true); // to connect to wifi
  WiFi.begin(ssid, password);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  beginServer();
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFont(u8x8_font_chroma48medium8_r);
}

void loop() {

  delay(5000);

  server.handleClient();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  float pressure = bmp.readPressure();

  

  digitalWrite(POWER_PIN, HIGH); 
  delay(10);                 
  int value = analogRead(SIGNAL_PIN);
  digitalWrite(POWER_PIN, LOW);  

  u8x8.clearDisplay();
  u8x8.setCursor(0, 0);
  u8x8.print("Temp: ");
  u8x8.println(temperature);
  u8x8.print("Humidity: ");
  u8x8.println(humidity);
  u8x8.print("Pressure: ");
  u8x8.println(pressure/100);
  u8x8.print("Water: ");
  u8x8.println(value);
}
