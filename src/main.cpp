#include <WiFi.h>
#include <WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ================== Configuration ==================

// Wi-Fi credentials
const char* ssid = "HomeSweetHome_2.4";
const char* password = "Ageofultron10!";

// GPIO pin for DS18B20 data
#define ONE_WIRE_BUS 4  // GPIO2 on Seeed XIAO ESP32-C3 (D0)

// ====================================================

// OneWire & DallasTemperature setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int count = 0;

// Create web server on port 80
WebServer server(80);

// ====================================================
// HTML template for the web page
String generateHTML(float temperatureC) {
  String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<title>ESP32-C3 Temperature</title>";
  html += "<style>body{font-family:sans-serif;text-align:center;background:#121212;color:#fff;}";
  html += "h1{font-size:2em;margin-top:1em;}p{font-size:1.5em;}button{margin-top:1em;padding:10px 20px;border:none;border-radius:8px;background:#2196f3;color:#fff;font-size:1em;cursor:pointer;}";
  html += "button:hover{background:#1976d2;}</style></head><body>";
  html += "<h1>ESP32-C3 Temperature Monitor</h1>";
  html += "<p>Current Temperature: <strong>";
  html += String(temperatureC, 2);
  html += " °C</strong></p>";
  html += "<button onclick='location.reload()'>Refresh</button>";
  html += "</body></html>";
  return html;
}

// ====================================================
// Handle root page
void handleRoot() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  count++;
  sensors.setUserDataByIndex(0, count);

  server.send(200, "text/html", generateHTML(tempC));
}

// ====================================================

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting DS18B20 + WebServer example...");

  sensors.begin();

  // Connect to Wi-Fi
  Serial.printf("Connecting to %s", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up web server routes
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();
}
