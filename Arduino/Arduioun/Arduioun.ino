#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>

#define DHTPIN 2 // DHT11 센서의 데이터 핀
#define DHTTYPE DHT11 // DHT11 모델

DHT dht(DHTPIN, DHTTYPE);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // 임의의 MAC 주소
char server[] = "dihealths.com"; // 서버 주소
EthernetClient client;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac);
  dht.begin();
}

void loop() {
  delay(2000); // 2초마다 측정

  float humidity = dht.readHumidity(); // 습도 읽기
  float temperature = dht.readTemperature(); // 온도 읽기

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // JSON 데이터 형식으로 변환
  String data = "{";
  data += "\"humidity\": ";
  data += String(humidity);
  data += ", ";
  data += "\"temperature\": ";
  data += String(temperature);
  data += "}";

  // 서버에 데이터 보내기
  if (client.connect(server, 443)) {
    client.println("GET /temp/hardware HTTP/1.1");
    client.println("Host: dihealths.com");
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);
    client.println();
  } else {
    Serial.println("Connection failed!");
  }

  delay(5000); // 5초 대기
}
