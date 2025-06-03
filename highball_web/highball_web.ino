#include <WiFi.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "TU_WIFI";
const char* password = "TU_PASSWORD";

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("Error montando SPIFFS");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado");
  Serial.println(WiFi.localIP());

  // Ruta para archivos web
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  // Endpoint de prueba para dispensar
  server.on("/dispensar", HTTP_GET, [](AsyncWebServerRequest *request) {
    String prop = request->getParam("proporcion")->value();
    Serial.println("Dispensar: " + prop + "% alcohol");
    request->send(200, "text/plain", "Dispensando: " + prop + "%");
  });

  server.begin();
}

void loop() {}
