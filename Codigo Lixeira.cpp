#include <ESP8266WiFi.h>
#include <Servo.h>
#include <PubSubClient.h>

//\n
#define trigger D7
#define eco D6
#define trigger_2 D5
#define eco_2 D3


long duracao;
float dist;
long duracao_2;
float dist_2;
char mensagem;


Servo servo;
WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão com Broker.");

    if (client.connect("")) {
      Serial.println("\nConectado ao Broker!");
    } else {
      Serial.print("\nNão foi possível conectar.");
      Serial.print(client.state());
      Serial.println("\nNova tentativa em 3s.");
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin("explorernet-Arthur", "Batata10", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.println(".");
  }
  Serial.println("WiFi Conectado!");

  client.setClient(espClient);
  client.setServer("test.mosquitto.org", 1883);

  servo.attach (D4);

  pinMode(trigger, OUTPUT);
  pinMode(eco, INPUT);
  pinMode(trigger_2, OUTPUT);
  pinMode(eco_2, INPUT);
  Serial.begin(9600);
}

void loop() {
   if (!client.connected()) {
     reconnect();
   }

  sensor1();
  sensor2();

}

void sensor1() {

  digitalWrite(trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  delayMicroseconds(10);

  duracao = pulseIn(eco, HIGH);
  dist = (duracao * 0.0342) / 2;

  if (dist <= 10) {
    servo.write(180);
    Serial.print("Sensor 1 - Distância: ");
    Serial.print(dist);
    Serial.println(" Estado: Aberta.");
    delay(5000);
  } else {
    servo.write(90);
    Serial.print("Sensor 1 - Distância: ");
    Serial.print(dist);
    Serial.println(" Estado: Fechada.");
    delay(1000);
  }
}

void sensor2() {

  digitalWrite(trigger_2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigger_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_2, LOW);
  delayMicroseconds(10);

  duracao_2 = pulseIn(eco_2, HIGH);
  dist_2 = (duracao_2 * 0.0342) / 2;

  if (dist_2 <= 10) {
    client.publish("status/lixeira", "Lixeira está cheia!");
    Serial.println("Lixeira cheia.");
    delay(1000);
  } else {
    Serial.print("Sensor 2 - Distância: ");
    Serial.println(dist_2);
    delay(1000);
  }


}