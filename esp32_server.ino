// A simple server temperature reading simulation 

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

uint8_t t=0;
String dot=".";

AsyncWebServer server(80);

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

const char index_html [] PROGMEM =R"rawliteral(<p style="font-size:70px">Temperature:<span style="color:blue">%T%</span>C</p>)rawliteral";

String processor(const String& var){
  if(var=="T") return String((t/10)+dot+(t-((t/10)*10)));
  return String();
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {

    Serial.begin(115200);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    delay(4000);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, processor);
    });

    server.onNotFound(notFound);

    server.begin();
}

void loop() {
  t++;
  delay(1000);
  Serial.print(t);
  Serial.print("\n");
  Serial.print(String((t/10)+dot+(t-((t/10)*10))));
  Serial.print("\n");
}
