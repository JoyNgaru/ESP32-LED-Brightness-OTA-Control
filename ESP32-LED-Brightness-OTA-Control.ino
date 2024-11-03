#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoOTA.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSocket.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const int ledPin = 2;  // LED connected to GPIO2
int pwmChannel = 0;
int pwmFrequency = 5000;
int pwmResolution = 8;
int ledBrightness = 128;  // Initial brightness

void setup() {
  // Setup Serial and WiFi
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize PWM for LED
  ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
  ledcAttachPin(ledPin, pwmChannel);
  ledcWrite(pwmChannel, ledBrightness);

  // Setup OTA
  ArduinoOTA.onStart([]() { Serial.println("Start OTA"); });
  ArduinoOTA.onEnd([]() { Serial.println("\nEnd OTA"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  // Setup WebSocket
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  // Serve web page with slider
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.begin();
}

void loop() {
  ArduinoOTA.handle();
}

// WebSocket Event Handler
void onWebSocketEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t * data, size_t len) {
  if (type == WS_EVT_DATA) {
    int brightness = atoi((char*)data); // Get slider value from client
    ledBrightness = brightness;
    ledcWrite(pwmChannel, ledBrightness);
    client->text(String(brightness));  // Send confirmation back
  }
}

// HTML Page (index_html)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>LED Brightness Control</title>
</head>
<body>
  <h1>Adjust LED Brightness</h1>
  <input type="range" id="brightnessSlider" min="0" max="255" oninput="sendBrightness(this.value)">
  <p>Brightness: <span id="brightnessValue">128</span></p>

  <script>
    let socket = new WebSocket("ws://" + window.location.hostname + "/ws");
    socket.onmessage = function(event) {
      document.getElementById("brightnessValue").innerText = event.data;
    };
    function sendBrightness(value) {
      socket.send(value);
    }
  </script>
</body>
</html>
)rawliteral";
