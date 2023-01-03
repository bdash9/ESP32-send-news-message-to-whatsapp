/* 
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-send-messages-whatsapp/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// Added support for TFT display (ST7789) - Ben

#include <WiFi.h>    
#include <HTTPClient.h>
#include <UrlEncode.h>
#include <Adafruit_ST7789.h> 
#include <Adafruit_GFX.h>  //Libraries for OLED Display
#include <SPI.h>

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "REPLACE_WITH_YOUR_PHONE_NUMBER";
String apiKey = "REPLACE_WITH_API_KEY";

void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    tft.print("Message sent successfully");
  }
  else{
    tft.println("Error sending the message");
    tft.print("HTTP response code: ");
    tft.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

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
  tft.init(135, 240); // Init ST7789 240x135
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);


  WiFi.begin(ssid, password);
  tft.println("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(".");
  }
  tft.println("");
  tft.print("Connected to WiFi...IP: ");
  tft.println(WiFi.localIP());

  // Send Message to WhatsAPP
  sendMessage("Hello from ESP32!");
}

void loop() {
  
}