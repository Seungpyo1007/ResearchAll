#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "senWiFi_Free";
const char *password = "sen2021!wi";
const char *awsEndpoint = "ec2-3-37-185-64.ap-northeast-2.compute.amazonaws.com";
const String awsPath = "/things/ESP32/shadow";
const String awsKeyID = "271676cb21993ae511a7a91...44a0ae1-public.pem.key";
const String awsSecretKey = "271676cb21993ae511a7a91...4a0ae1-private.pem.key";

//  --------- Config ---------- //
//AWS IOT config, change these:
//char wifi_ssid[]       = "senWiFi_Free";
//char wifi_password[]   = "sen2021!wi";
//char aws_endpoint[]    = "ec2-3-37-185-64.ap-northeast-2.compute.amazonaws.com";
//char aws_key[]         = "271676cb21993ae511a7a91...44a0ae1-public.pem.key";
//char aws_secret[]      = "271676cb21993ae511a7a91...4a0ae1-private.pem.key";
//char aws_region[]      = "ec2-3-37-185-64.ap-northeast-2";
//const char* aws_topic  = "$aws/things/ESP32/shadow/update";
//int port = 8707;

WiFiClient client;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Wi-Fi에 연결 중...");
  }
  Serial.println("Wi-Fi에 연결되었습니다");
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.println("아두이노 Uno에서 데이터를 받았습니다: " + data);

    // AWS로 데이터 전송
    sendDataToAWS(data);
  }

  delay(1000); // 1초 대기
}

void sendDataToAWS(String data) {
  if (!client.connected()) {
    if (client.connect(awsEndpoint, 80)) {
      // AWS에 전송할 데이터 구성
      String jsonData = "{\"data\": \"" + data + "\"}";

      // HTTP POST 요청 전송
      client.print(String("POST ") + awsPath + " HTTP/1.1\r\n" +
                   "Host: " + awsEndpoint + "\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Content-Length: " + jsonData.length() + "\r\n" +
                   "Connection: close\r\n\r\n" +
                   jsonData);

      delay(10);
    } else {
      Serial.println("AWS 서버에 연결할 수 없습니다");
    }
  }

  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
}