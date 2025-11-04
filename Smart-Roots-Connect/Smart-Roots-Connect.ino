/*
 Basic MQTT example

 This sketch demonstrates the basic capabilities of the library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 
*/

#include <SPI.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <arduino-timer.h>


//#define SD_CS 5  // Chip select pin for SD card module
// Update these with values suitable for your network.
//File logFile;
auto timer = timer_create_default();
char ssid[] = "xxxxxxxxx";
char password[] = "xxxxxxxxxxx";
const char *mqtt_broker = "xxxxxxxxx";
const char *topicReadings = "xxxxxxxx";
 String topicRemoteToggle  = "";
const char *mqtt_username = "xxxxxxx";
const char *mqtt_password = "xxxxxxxx";
const int mqtt_port = 8883;
const uint DATA_INTERVAL = 10000;   // Uploading
const char* ca_cert= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=" \
"-----END CERTIFICATE-----\n";
WiFiClientSecure espClient;

 String incomingData = "";
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
   Serial.println(WiFi.macAddress());
  if(Serial2.availableForWrite()){
    Serial2.println(WiFi.macAddress());
  }
  String message;
  for (int i=0;i<length;i++) {
    message+=(char)payload[i];
  }
  String subscribedTopic = String(topic);
  if(subscribedTopic==topicRemoteToggle){
   Serial.println("This is the payload we got: "+message);
    // the purpose of this is to fetch the live reading, to which now we will send it over to the arduino
    if(Serial2.availableForWrite()){
    sendToggleCommand(message);
  delay(200); 
  }

  }else{
    Serial.println("Unable to subscribe to topic to toggle componenets");
  }
  
  Serial.println();
}

PubSubClient client(espClient); // add tje c;oemt here

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Fred-Tent_Esp32", mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      
      client.publish("Readings","hello world");
      // ... and resubscribe
      client.subscribe(topicRemoteToggle.c_str());
      Serial.print("Subscribed to: ");
      Serial.println(topicRemoteToggle);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);    
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  // RX=16, TX=17 from Mega
  WiFi.begin(ssid, password);
  Serial.println("We are booted");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.println("Connecting to WiFi..");
   
  }
 topicRemoteToggle = "Toggle/" + WiFi.macAddress();
espClient.setCACert(ca_cert);
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  /* if (!SD.begin(SD_CS)) {
    Serial.println("SD Card Mount Failed!");
  } else {
    Serial.println("SD Card initialized.");

    // Open file for appending
    logFile = SD.open("/data.txt", FILE_APPEND);
    if (!logFile) {
      Serial.println("Failed to open file for writing!");
    } else {
      // Write CSV header if file is empty
      if (logFile.size() == 0) {
        logFile.println("PH,Light,EC,FlowRate,Humidity,Temperature");
      }
      logFile.close();
    }
  }*/
  reconnect();
  timer.every(2000, FetchSerialData);
  timer.every(DATA_INTERVAL,publishToBroker );
}
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  timer.tick();
  delay(10);
}


void subScribeToBrokerToggle(){
if(client.connect(topicRemoteToggle.c_str(), mqtt_username, mqtt_password)){
  
}
}
bool publishToBroker(void *){
   
 
  String ph = extractValue(incomingData, "PH");
  String light = extractValue(incomingData, "Light");
  String ec = extractValue(incomingData, "EC");
  String flow = extractValue(incomingData, "FlowRate");
  String humidity = extractValue(incomingData, "Humidity");
  String temperature = extractValue(incomingData, "Temperature");
  //saveToSD(ph, light, ec, flow, humidity, temperature);
  String msg = "{";
    msg += "\"MacAddress\":\"" + WiFi.macAddress() + "\",";
    msg += "\"PH\":\"" + ph + "\",";
    msg += "\"Light\":\"" + light + "\",";
    msg += "\"EC\":\"" + ec + "\",";
    msg += "\"FlowRate\":\"" + flow + "\",";
    msg += "\"Humidity\":\"" + humidity + "\",";
    msg += "\"Temperature\":\"" + temperature + "\"";
    msg += "}";
 if (client.connect("Veg-Tent-ESP32", mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("Readings",msg.c_str());
  
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }

  return true;
}
bool FetchSerialData(void *){
 if (Serial2.available()) {
    incomingData = Serial2.readStringUntil('>');  // stop at end marker
  int start = incomingData.indexOf('<');
    if (start != -1) {
      incomingData = incomingData.substring(start + 1);
      incomingData.trim();
    } else {
        Serial.println("Malformed data");
   
    }
    return true;      // we return true so this method continually runs, refer to docs of arduino-timer library  
  }
  return true; 
}
/*void saveToSD(String ph, String light, String ec, String flow, String humidity, String temperature) {

  String csvRow = ph + "," + light + "," + ec + "," + flow + "," + humidity + "," + temperature;

  logFile = SD.open("/data.txt", FILE_APPEND);
  if (logFile) {
    logFile.println(csvRow);
    logFile.close();
    Serial.println("Saved to SD: " + csvRow);
  } else {
    Serial.println("Error opening file for writing.");
  }
}
*/
String extractValue(String data, String key) {
  int start = data.indexOf("\"" + key + "\"");
  if (start == -1) return "";

  start = data.indexOf(":", start);
  if (start == -1) return "";

  int firstQuote = data.indexOf("\"", start + 1);
  int secondQuote = data.indexOf("\"", firstQuote + 1);
  if (firstQuote == -1 || secondQuote == -1) return "";

  return data.substring(firstQuote + 1, secondQuote);
}
void sendToggleCommand(String json) {
    String packet = "<" + json + ">";
  Serial2.print(packet); 
  Serial.println(packet);
}

