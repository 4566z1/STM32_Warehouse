#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

String host = "";
String url_add = "/api/product/v1.php";
String url_del = "/api/product/v2.php";
String url_get = "/api/product/v3.php";

String WIFI_SSID = "TEST";
String WIFI_PASS = "88888888";

void product_get(){
  WiFiClient client;
  HTTPClient http;
  String url = "http://" + host + url_get;

  http.begin(client, url);

  int httpcode = http.GET();
  if(httpcode > 0){
    Serial.print(http.getString());
  }
}

void product_edit(String name, String cate, bool is_add = true){
  WiFiClient client;
  HTTPClient http;
  String url;
  String payload;

  if(is_add){
    url = "http://" + host + url_add;
    payload = "name=" + name + "&cate=" + cate;
  } else {
    url = "http://" + host + url_del;
    payload = "name=" + name;
  }
  
  http.begin(client, url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpcode = http.POST(payload);
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(0xFFFF);

  // 连接 WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  host = WiFi.gatewayIP().toString() + ":8080";
}

void loop() {
  if(Serial.available()){
    char flag = Serial.read();
    if(flag == 'A'){
      String name = Serial.readStringUntil(',');
      String cate = Serial.readStringUntil(',');
      product_edit(name, cate);
    }
    if(flag == 'D'){
      String name = Serial.readStringUntil(',');
      product_edit(name, "", false);
    }
    if(flag == 'S'){
      host = Serial.readStringUntil(',');
    }
    if(flag == 'G'){
      Serial.readStringUntil(',');
    }
  }

  product_get();

  delay(1);
}
