/*
Kenichi Yoshida

*/

//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <PubSubClient.h>
#define BUILTIN_LED 16

// Update these with values suitable for your network.

// 実習室CampusIOT
//const char* ssid = "........";
//const char* password = "........";
const char* ssid = "CampusIoT-WiFi";
const char* password = "0b8b413f2c0fa6aa90e085e9431abbf1fa1b2bd2db0ecf4ae9ce4b2e87da770c";
//
//const char* ssid = "katamachi";
//const char* password = "91d3821d2a3c63298a4f792f0f15d91bcb8aa22e460f4b43fb51dd6c9d202e60";

//// （１）グローバル変数mqtt_serverにMQTTブローカーのサーバーアドレスを指定する。
//const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* mqtt_server = "broker.shiftr.io"; //shiftr.ioのブローカー


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
    ////確認用オフ
    Serial.println("payload[0] == '1' then BUILTIN_LED, LOW");
  } else {
    ////確認用オン
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    Serial.println("else BUILTIN_LED, HIGH");    
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
      //（３）reconnect()関数内で、クライアントIDを「クラス名＋出席番号2桁」にする
//    String clientId = "ESP8266Client-";
//    clientId += String(random(0xffff), HEX);
      String clientId = "se3a00";
    // Attempt to connect
    ////（２）reconnect()関数内でMQTTサービスにアクセスする①ユーザー名と②パスワードを指定する
//    if (client.connect(clientId.c_str(), "dqzavdjw", "1T7iwriR0MHz")) {
    if (client.connect(clientId.c_str(), "87b827db", "049b57f34cabd462")) {
    //// ここで↑自分のチームの①ユーザー名と②パスワードを指定する ↑team-g G班を使用
      Serial.println("connected");
      // Once connected, publish an announcement...
      //（４）reconnect()関数内でトピックをクラス名/チーム名、メッセージを自分の名前（ローマ字で）にしてPublishする
//      client.publish("outTopic", "hello world");
      client.publish("se2a/team-g", "1");
      //↑トピックのルート部分を自分のチーム名にしてもいい
      //（５）reconnect()関数内でトピックを"クラス名/#"にしてクラス名トピックに該当するすべてのトピックをSubscribeする
      // ... and resubscribe
//      client.subscribe("inTopic");
      client.subscribe("se2a/#");
      //↑トピックのルート部分を自分のチーム名にしてもいい
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000 /* 10*/) {
    lastMsg = now;
    ++value;
    //（６）loop（）関数内でHello Worldの文字列を自分の名前（ローマ字）に変える
//    snprintf (msg, 50, "hello world #%d", value);
    snprintf (msg, 50, "1", value);
    //メッセージ＝ペイロードは何を送ってもいい。LEDオンオフのため1を送ってみる
    Serial.print("Publish message: ");
    Serial.println(msg);
    //（７）loop（）関数内でトピックをクラス名/チーム名にして、Publishする
//    client.publish("outTopic", msg);
    client.publish("se2a/team-g", msg);
  }
}
