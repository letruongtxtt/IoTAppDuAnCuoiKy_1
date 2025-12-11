#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

const char* ssid = "TOTOLINK_N350RT tầng 2";
const char* password = "11111111";


const char* mqtt_server = "30b0d1b821d348d4bddd7d27004d7548.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;

// Tài khoản MQTT (tạo trong Access Management)
const char* mqtt_user = "truongle";
const char* mqtt_pass = "Truong18012004";

WiFiClientSecure espClient;
PubSubClient client(espClient);

// CA certificate lấy từ HiveMQ Cloud
static const char ca_cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

Servo servo;
// Khai báo chân
//const int rainPin = D2;     // switch mô phỏng mưa
//const int ldrPin  = 4;     // chân A0 (ADC1_CH0)
//const int ledPin  = 25;    // LED báo hiệu
//const int servoPin = 14;   // servo PWM

// Ngưỡng ánh sáng (tuỳ chỉnh theo môi trường)
const int LIGHT_THRESHOLD = 1500; // giá trị LDR lớn = nắng gắt
bool curtainClosed = false;   // false = đang mở, true = đang đóng
String curtainStatus = "OPEN";   // mặc định lúc khởi động

// ====================== WiFi ======================
void setup_wifi() {
  delay(10);
  Serial.println("Kết nối WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

// ====================== MQTT ======================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT... ");
    if (client.connect("ESP32Client", mqtt_user, mqtt_pass)) {
      Serial.println("connected!");
      //client.publish("test/topic", "HELLO from ESP32!");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void servoStop() {
  servo.write(90);  // STOP đối với servo 360°
}

void closeCurtain() {
  //Serial.println("→ CLOSE: quay servo 1.2s");
  servo.write(180);       // quay chiều đóng
  delay(1200);            // thời gian bạn tự chỉnh
  servoStop();            // dừng
  curtainClosed = true;
  Serial.println("→ ACTION: CLOSE CURTAIN");
}

void openCurtain() {
  //Serial.println("→ OPEN: quay servo 1.2s");
  servo.write(0);         // quay chiều mở
  delay(1200);            // chỉnh phù hợp chiều dài rèm
  servoStop();
  curtainClosed = false;
  Serial.println("→ ACTION: OPEN CURTAIN");
}

void setup() {
  Serial.begin(115200);

  // Chân vào
  pinMode(rainPin, INPUT);
  pinMode(ldrPin, INPUT);

  // LED
  pinMode(ledPin, OUTPUT);

  // Servo
  servo.attach(servoPin);
  servo.write(90);     // dừng servo lúc khởi động
  //curtainClosed = false;
  setup_wifi();

  // Load CA cert
  espClient.setCACert(ca_cert);

  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int rainState = digitalRead(rainPin);   // LOW = mưa, HIGH = không mưa
  int ldrValue = analogRead(ldrPin);      // giá trị ánh sáng
  int analogRainValue = analogRead(analogRain); // đọc giá trị cảm biến mưa analog

  bool isRaining = (rainState == LOW);
  String rainStatus = isRaining ? "Yes" : "No";
  bool isSunStrong = (ldrValue > LIGHT_THRESHOLD);

  Serial.println("-----------------------");
  //Serial.print("Rain value: ");
  //Serial.println(analogRainValue);
  Serial.print("RAINING? : ");
  Serial.println(rainStatus);
  Serial.print("CdS: ");
  Serial.print(ldrValue);
  Serial.print(" | Sun strong? : ");
  Serial.println(isSunStrong ? "Yes" : "No");
  //Serial.println("-----------------------");

  bool needClose = (isRaining || isSunStrong);

  // === Điều kiện đóng rèm ===
  if (needClose && !curtainClosed) {
    closeCurtain();
    digitalWrite(ledPin, LOW); // LED ON
    curtainStatus = "CLOSE";
    //Serial.println("→ ACTION: CLOSE CURTAIN");
  }
  // === Điều kiện mở rèm ===
  if (!needClose && curtainClosed) {
    openCurtain();
    digitalWrite(ledPin, HIGH); // LED OFF
    //Serial.println("→ ACTION: OPEN CURTAIN");
    curtainStatus = "OPEN";
  }
  client.publish("home/rain",  rainStatus.c_str());
  client.publish("home/light", String(ldrValue).c_str());
  client.publish("home/curtain", curtainStatus.c_str());
  client.publish("home/rainAnalog", String(analogRainValue).c_str());

  Serial.printf("Rain: %s | Light: %d | Curtain: %s | RainAnalog: %d\n", rainStatus.c_str(), ldrValue, curtainStatus.c_str(), analogRainValue);

  delay(1000);
}