#include <WiFi.h>
#include <HTTPClient.h>
#include "esp_camera.h"

// Configurações de Wi-Fi
const char* ssid = "LEVI NEVE";
const char* password = "andreakewen0918";

// URL do servidor
const char* serverUrl = "http://localhost:8080/teste";

void setup() {
    Serial.begin(115200);

    // Conecta ao Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao Wi-Fi...");
    }
    Serial.println("Conectado ao Wi-Fi");
    
    // Inicializa a câmera
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    // config.pin_d0 = Y2_GPIO_NUM;
    // config.pin_d1 = Y3_GPIO_NUM;
    // config.pin_d2 = Y4_GPIO_NUM;
    // config.pin_d3 = Y5_GPIO_NUM;
    // config.pin_d4 = Y6_GPIO_NUM;
    // config.pin_d5 = Y7_GPIO_NUM;
    // config.pin_d6 = Y8_GPIO_NUM;
    // config.pin_d7 = Y9_GPIO_NUM;
    // config.pin_xclk = XCLK_GPIO_NUM;
    // config.pin_pclk = PCLK_GPIO_NUM;
    // config.pin_vsync = VSYNC_GPIO_NUM;
    // config.pin_href = HREF_GPIO_NUM;
    // config.pin_sscb_sda = SIOD_GPIO_NUM;
    // config.pin_sscb_scl = SIOC_GPIO_NUM;
    // config.pin_pwdn = PWDN_GPIO_NUM;
    // config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    if (psramFound()) {
        config.frame_size = FRAMESIZE_UXGA; // Tamanho da imagem
        config.jpeg_quality = 10;          // Qualidade da imagem
        config.fb_count = 2;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    if (!esp_camera_init(&config)) {
        Serial.println("Erro ao inicializar a câmera");
        return;
    }
}

void loop() {
    Serial.printf("\n\nchega aqui\n\n");
    Serial.printf("%d",WiFi.status());
    if (WiFi.status() == WL_CONNECTED) {
        // Captura uma imagem
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) {
            Serial.println("Falha ao capturar imagem");
            return;
        }

        // Envia a imagem via POST
        HTTPClient http;
        http.begin(serverUrl);
        Serial.printf("chega aqui");
        // http.addHeader("Content-Type", "image/jpeg");
        // int httpResponseCode = http.POST(fb->buf, fb->len);
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            Serial.printf("Resposta do servidor: %d\n", httpResponseCode);
        } else {
            Serial.printf("Erro ao enviar imagem: %s\n", http.errorToString(httpResponseCode).c_str());
        }

        http.end();
        esp_camera_fb_return(fb);
    }
    delay(5000); // Aguarde antes de capturar outra imagem
}
