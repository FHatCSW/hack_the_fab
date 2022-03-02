#include<Wire.h>
#include<PubSubClient.h>
#include<WiFi.h>
#include<ArduinoJson.h>

/*
 * WLAN Configuration
*/
const char* ssid = "Mikrofab_01"; // FILL ACCORDING TO THE FAB ID
const char* password = "mikrofoo123"; // FILL ACCORDING TO THE FAB ID

/*
 * MQTT Broker configuration + Topic
*/
const char* mqtt_broker = "192.168.88.100"; // FILL ACCORDING TO THE FAB ID
const char* topic = "test/machine_1";

/*
 * NTP Server Configuration for TimeStamps
*/
const char* ntp_server = "pool.ntp.org";

WiFiClient espClient;
PubSubClient client(espClient);

/*
 * JSON Data Format
*/
const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3);
DynamicJsonDocument doc(capacity);
JsonObject data = doc.createNestedObject("data");

float temp, humid = 0.0;
char output[128];
/*
 * getLocalTimeNTP: Get Epoch Time Stamp from NTP server
*/
unsigned long getLocalTimeNTP() {
  time_t now;
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return 0;
  }
  time(&now);
  return now;
}

/*
 * reconnect: connect/reconnect to MQTT broker
*/

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
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
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  configTime(0, 0, ntp_server);
  while (WiFi.status() != WL_CONNECTED) {
       delay(500);
       Serial.print(".");
  }
  Serial.println("WiFi Connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
  client.setServer(mqtt_broker, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Creating JSON Format
  data["temp"].set(random(20, 25));
  data["humid"].set(random(80, 85));
  data["hall"].set(hallRead());
  data["time"].set(getLocalTimeNTP());

  // Output Format
  serializeJson(doc, output);

  Serial.println(output); // Data Format
  client.publish(topic, output);
  delay(5000); // Publish data every 2 seconds to the Broker
}