// A simple server temperature reading simulation 

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

int t=-120; //freezing

String Tstr;
String dot=".";
String minus="-";

AsyncWebServer server(80);

const char* ssid = "Redmi9A";
const char* password = "";

const char index_html [] PROGMEM =R"rawliteral(<p style="font-size:70px">Temperature:<span style="color:blue">%T%</span>C</p>)rawliteral";

String processor(const String& var){
  if(var=="T") return Tstr;
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
  if(t>240) t=-120; //hot, let's freeze
  delay(500);
  Serial.print(t);
  Serial.print("\n");
  if(t<0){
    if(t>-10) Tstr=String(minus+(t/10)+dot+((~t+1)-(((~t+1)/10)*10)));
    else Tstr=String((t/10)+dot+((~t+1)-(((~t+1)/10)*10)));
  } 
  else Tstr=String((t/10)+dot+(t-((t/10)*10)));
  Serial.print(Tstr);
  Serial.print("\n");
}
