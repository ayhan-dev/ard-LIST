
#include <ESP8266WiFi.h> 
#include <WiFiClientSecure.h> 
#include <Wire.h>    
#include <YoutubeApi.h>    
#include <ArduinoJson.h>  
#include <LiquidCrystal_I2C.h>  

#define API_KEY    "a1v6j6e4khadb58"  
#define CHANNEL_ID "@ayhan.Gy"      
#define Keyy      16 
#define OFF_KEY   0 

char ChName[]   = ""; 
char ssid[]     = "";  
char password[] = "";    
char Flag       = 0; 
 
LiquidCrystal_I2C lcd(0x27, 16, 2);  
WiFiClientSecure client;  
YoutubeApi api(API_KEY, client); 
 
unsigned long api_mtbs     = 2000;         
unsigned long api_lasttime = 0;       

void setup() { 
  pinMode(Keyy, INPUT_PULLUP);  
  pinMode(OFF_KEY, INPUT);   
 
  Serial.begin(115200); 
  Wire.begin(D2, D1); 
  lcd.begin(0, 2); 
  lcd.backlight();                    
  lcd.clear(); 
  lcd.setCursor(2, 0); 
  lcd.print("Sub Counter");  
 
  delay(1500);  
 
  lcd.clear();  
  lcd.print("Designed by : "); 
  lcd.setCursor(0, 1); 
  lcd.print("    ElecMake   "); 
 
  delay(4000);  
 
  lcd.clear(); 
  lcd.setCursor(0, 0); 
  lcd.print("WiFi Pairing..."); 
  
  WiFi.mode(WIFI_STA); 
  WiFi.disconnect(); 
 
  delay(100); 
 
  Serial.print("Connecting Wifi: "); 
  Serial.println(ssid); 
  WiFi.begin(ssid, password); 
  
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print("."); 
    delay(500); 
  } 
  
  Serial.println(""); 
  Serial.println("WiFi connected."); 
  Serial.print("Your IP: "); 
  IPAddress ip = WiFi.localIP(); 
  Serial.println(ip); 
  Serial.print("Channel:"); 
  client.setInsecure(); 
  lcd.clear(); 
  lcd.print("WiFi IP:"); 
  lcd.setCursor(0, 1); 
  lcd.print(ip); 
  delay(1000); 
  lcd.clear();  
  pollYoutubeAPI(); 
}   
 
void loop() { 
  if (millis() > api_lasttime + api_mtbs) {
    pollYoutubeAPI(); 
  }
  
  if(digitalRead(Keyy) == LOW){ 
    Flag++; 
    
    if(Flag>2) {
      Flag = 0; 
    }
    
    delay(50); 
    
    while(digitalRead(Keyy) == LOW);   
  } 
  
  if(digitalRead(OFF_KEY) == LOW){ 
    lcd.clear(); 
    
    delay(50);  
    
    while(digitalRead(Keyy) == LOW);   
  } 
}    
 
void pollYoutubeAPI() { 
  if (api.getChannelStatistics(CHANNEL_ID)) { 
    Serial.println("---------Stats---------"); 
    Serial.print("Subscriber Count: "); 
    Serial.println(api.channelStats.subscriberCount); 
    Serial.print("View Count: "); 
    Serial.println(api.channelStats.viewCount); 
    Serial.print("Comment Count: "); 
    Serial.println(api.channelStats.commentCount); 
    Serial.print("Video Count: "); 
    Serial.println(api.channelStats.videoCount); 
    Serial.println("-----------------------"); 
    
    if(Flag==1) { 
      lcd.clear(); 
      lcd.setCursor(6, 0); 
      lcd.print("Views "); 
      lcd.setCursor(0, 1);    
      lcd.print(api.channelStats.viewCount); 
    } 
    else if(Flag==2) { 
      lcd.clear(); 
      lcd.setCursor(2, 0); 
      lcd.print("Subscribers "); 
      lcd.setCursor(0, 1);    
      lcd.print(api.channelStats.subscriberCount); 
    } 
    else if(Flag==0) { 
      lcd.clear(); 
      lcd.setCursor(4, 0); 
      lcd.print(ChName); 
    }   
  } 
  
  api_lasttime = millis();    
}





