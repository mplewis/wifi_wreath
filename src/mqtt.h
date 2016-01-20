#include <espduino.h>
#include <mqtt.h>

ESP esp(&Serial, &Serial, -1);
MQTT mqtt(&esp);

void process_cmd(String cmd);

void wifiCb(void* response) {
  RESPONSE res(response);
  if (res.getArgc() == 1) {
    uint32_t status;
    res.popArgs((uint8_t*)&status, 4);
    if (status == STATION_GOT_IP) {
      Serial.println("Setup: ESP connected");
      mqtt.connect(0, 0, false);
    } else {
      Serial.println("Setup: ESP disconnected");
      mqtt.disconnect();
    }
  }
}

void mqttConnected(void* response) {
  Serial.println("Setup: MQTT connected");
  mqtt.subscribe("wreath");
}

void mqttData(void* response) {
  RESPONSE res(response);
  res.popString();  // topic
  String data = res.popString();
  Serial.println(data);
  process_cmd(data);
}

void mqtt_setup() {
  Serial.begin(115200);
  while (!esp.ready())
    ;

  Serial.println("Setup: creating MQTT and LWT");
  if (!mqtt.begin(0, 0, 0, 0, 0)) {
    Serial.println("Setup: MQTT setup failed");
    while (1)
      ;
  }
  mqtt.lwt("/lwt", "offline", 0, 0);

  mqtt.connectedCb.attach(&mqttConnected);
  mqtt.dataCb.attach(&mqttData);

  Serial.println("Setup: connecting to ESP");
  esp.wifiCb.attach(&wifiCb);
  esp.wifiConnect(0, 0);
}

void mqtt_process() {
  esp.process();
}
