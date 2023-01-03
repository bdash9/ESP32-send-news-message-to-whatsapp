#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
//#include <Fonts/FreeSerif9pt7b.h>

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

void setup() {
  Serial.begin(115200);

  // turn on backlite
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

  // turn on the TFT / I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  // initialize TFT
  tft.init(135, 240);  // Init ST7789 240x135
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);

  // Connect to WiFi
  tft.print("Connecting to ");
  tft.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(".");
  }
  tft.println("");
  tft.println("WiFi connected");
  delay(500);
}

void loop() {
  // Make an HTTP GET request to a news API
  //
  //Go here to get your API_KEY https://newsapi.org
  //
  //"Newsapi.org has a limit to how many queries per day...
  //{"status":"error","code":"rateLimited","message":"You have made too many requests recently. 
  //Developer accounts are limited to 100 requests over a 24 hour period (50 requests available every 12 hours). 
  //Please upgrade to a paid plan if you need more requests."}""
  //
  HTTPClient http;
  http.begin("http://newsapi.org/v2/top-headlines?country=us&apiKey=YOUR_API_KEY");
  int httpCode = http.GET();
  String response = http.getString();

  // Parse the response to get the headlines
  DynamicJsonDocument doc(19508);
  //DeserializationError error = deserializeJson(doc, payload);
  DeserializationError error = deserializeJson(doc, response);
  if (error) {
    tft.println("Error parsing news data");
    return;
  }

  // Extract the "title" field from the first article
JsonArray articles = doc["articles"];

//String title = articles[0]["title"];
// Display the title of each article
for (int i = 0; i < articles.size(); i++) {
  String title = articles[i]["title"];

// Scroll the headlines across the screen
// There is a lot of flicker... canvas.println would smooth this out
int16_t x = tft.width();
int16_t y = 0;
while (x > -tft.getCursorX()) {
  tft.setCursor(x, y);  
  x -= 1;
  //tft.setFont(&FreeSerif9pt7b);
  tft.println(title);
  delay(20);
  tft.fillScreen(0);

}
  //delay(60000);
  //delay(600000);
}
}
