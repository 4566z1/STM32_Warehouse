#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <AsyncHTTPRequest_Generic.h>

String host = "";
String url_add = "/api/product/v1.php";
String url_del = "/api/product/v2.php";
String url_get = "/api/product/v3.php";

String WIFI_SSID = "TEST";
String WIFI_PASS = "88888888";

AsyncHTTPRequest request_get;
AsyncHTTPRequest request_post;

void async_post_send(const String& url, const String& data) {
  static bool requestOpenResult;

  if (request_post.readyState() == readyStateUnsent || request_post.readyState() == readyStateDone) {
    requestOpenResult = request_post.open("POST", url.c_str());

    if (requestOpenResult) {
      requestOpenResult = request_post.send(data);
    }
  }
}

void async_get_send(const String& url) {
  static bool requestOpenResult;

  if (request_get.readyState() == readyStateUnsent || request_get.readyState() == readyStateDone) {
    requestOpenResult = request_get.open("GET", url.c_str());

    if (requestOpenResult) {
      requestOpenResult = request_get.send();
    }
  }
}

void async_get_handler(void *optParm, AsyncHTTPRequest *request, int readyState) {
  (void)optParm;

  if (readyState == readyStateDone) {
    if (request->responseHTTPcode() == 200) {
      Serial.print(request->responseText());
    }
  }
}

void product_get() {
  async_get_send("http://" + host + url_get);
}

void product_edit(const String& name, const String& cate, bool is_add = true) {
  String url;
  String payload;

  if (is_add) {
    url = "http://" + host + url_add;
    payload = "name=" + name + "&cate=" + cate;
  } else {
    url = "http://" + host + url_del;
    payload = "name=" + name;
  }

  async_post_send(url, payload);
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

  request_get.setDebug(false);
  request_get.onReadyStateChange(async_get_handler);
  request_post.setDebug(false);
}

void loop() {
  if (Serial.available()) {
    char flag = Serial.read();
    if (flag == 'A') {
      String name = Serial.readStringUntil(',');
      String cate = Serial.readStringUntil(',');
      product_edit(name, cate);
    }
    if (flag == 'D') {
      String name = Serial.readStringUntil(',');
      product_edit(name, "", false);
    }
    if (flag == 'S') {
      host = Serial.readStringUntil(',');
    }
  }

  // Pull data from httpserver
  product_get();

  delay(1);
}
