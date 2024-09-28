//include lib
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h> //"DHT.h"

#ifndef STASSID
#define STASSID "tsvtc-w(Ruckus)" //AIMESH92
#define STAPSK  "1q2w#E$R" //92abcd1234
#endif

const char* mSSID     = STASSID;
const char* PASSWORD = STAPSK;

//自定義常數
#define dhtPin 23      //讀取DHT11 Data GPIO
#define dhtType DHT11 //選用DHT11   


//宣告物件
DHT dht(dhtPin, dhtType); // Initialize DHT sensor


//自訂變數
float sensor_temp;
float sensor_humi;

void wifi_init(){
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(mSSID);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(mSSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void connToServer(float *temp, float *humi){
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client; //Creates a client that can connect to to a specified internet IP address and port as defined in client.connect()

    HTTPClient http;

    Serial.print("[HTTP] begin...\n"); 
    //configure traged server and url
    //aws web server ip: 18.232.148.96
    if (http.begin(client, "http://18.232.148.96/web/iot/recieve_data.php?temp="+(String)sensor_temp+"&humi="+(String)sensor_humi)) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      int httpCode = http.GET();
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }
}

void sensor_DHT11(float *temp, float *humi){
  *humi = dht.readHumidity();//讀取濕度
  *temp = dht.readTemperature();//讀取攝氏溫度
  float f = dht.readTemperature(true);//讀取華氏溫度
  if (isnan(*humi) || isnan(*temp) || isnan(f)) {
    Serial.println("無法從DHT傳感器讀取！");
    return;
  }
}

void setup() {
  Serial.begin(115200);//設定鮑率9600
  dht.begin();//啟動DHT
  wifi_init();
}

void loop() {
  //read DHT11 data
  sensor_DHT11(&sensor_temp, &sensor_humi);
  Serial.print("sensor_temp: ");
  Serial.println(sensor_temp);
  Serial.print("sensor_humi: ");
  Serial.println(sensor_humi);
  
  //send data to server
  connToServer(&sensor_temp, &sensor_humi);
    
  delay(5000);//延時5秒
}
