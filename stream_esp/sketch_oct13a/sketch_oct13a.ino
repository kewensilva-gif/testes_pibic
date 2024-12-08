#include <WiFi.h>
#include <HTTPClient.h>
#include <esp32cam.h>

const char* WIFI_SSID = "LEVI NEVE";
const char* WIFI_PASS = "andreakewen0918";

const char* serverUrl = "http://192.168.1.40:8080/teste";

void setup() {
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConectado ao WiFi!");
    Serial.print("IP do ESP32-CAM: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        
        Serial.println("\nIniciando conexão HTTP...");
        Serial.print("URL: ");
        Serial.println(serverUrl);

        http.begin(serverUrl);
        
        http.setTimeout(10000);

        int httpResponseCode = http.GET();
        
        Serial.print("Código de Resposta HTTP: ");
        Serial.println(httpResponseCode);

        if (httpResponseCode > 0) {
            String payload = http.getString();
            Serial.println("Resposta do Servidor:");
            Serial.println(payload);
        } else {
            Serial.print("Erro na requisição HTTP: ");
            Serial.println(http.errorToString(httpResponseCode));
        }

        http.end();
    } else {
        Serial.println("WiFi desconectado");
    }

    delay(5000);
}