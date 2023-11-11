#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ESPAsyncWebServer.h>
#include <esp_camera.h>


#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27
#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

const char* ssid = "SBRT";
const char* password = "Robotic$3";

// Define motor pins
const int motorA1 = 15; 
const int motorA2 = 2;
const int motorB1 = 32;
const int motorB2 = 33;

AsyncWebServer server(80);
WebSocketsServer webSocket(81);

const char indexHtml[]  = R"RAW(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <title>ESP32 Direction Controller</title>
  </head>
  <body>
    <h1>ESP32 Direction Controller</h1>
    <p>Use WASD keys to control the direction:</p>
    <p>W - Forward, A - Left, S - Backward, D - Right</p>
    <div>
      <figure style="height: 49vh">
        <div id="stream-container" class="image-container">
          <img id="stream" src="" />
        </div>
      </figure>
    </div>

    <script>
      const view = document.getElementById("stream");
      var gateway = `ws://${window.location.hostname}:81`; // WebSocket URL
      var websocket;

      function initWebSocket() {
        console.log("Trying to open a WebSocket connection...");
        websocket = new WebSocket(gateway);
        websocket.onopen = function (evt) {
          console.log("WebSocket connection opened");
        };
        websocket.onclose = function (evt) {
          console.log("WebSocket connection closed");
        };
        websocket.onerror = function (evt) {
          console.log("WebSocket error: " + evt.data);
        };
        websocket.onmessage = function (evt) {
          if (evt.data instanceof Blob) {
            var urlObject = URL.createObjectURL(evt.data);
            view.src = urlObject;
          }
        };
      }

      window.addEventListener("load", initWebSocket);

      function sendCommand(command) {
        if (websocket.readyState === WebSocket.OPEN) {
          websocket.send(command);
        }
      }

      document.addEventListener("keydown", function (event) {
        if (event.key === "w" || event.key === "W") {
          sendCommand("forward");
        } else if (event.key === "a" || event.key === "A") {
          sendCommand("left");
        } else if (event.key === "s" || event.key === "S") {
          sendCommand("backward");
        } else if (event.key === "d" || event.key === "D") {
          sendCommand("right");
        }
      });

      // Event listener for keyup to stop movement
      document.addEventListener("keyup", function (event) {
        if (["w", "W", "a", "A", "s", "S", "d", "D"].includes(event.key)) {
          sendCommand("stop");
        }
      });
    </script>
  </body>
</html>
)RAW";
void moveForward() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
}

void moveBackward() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, HIGH);
}

void turnLeft() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, HIGH);
  digitalWrite(motorB2, LOW);
}

void turnRight() {
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}

void stopCar() {
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  String message = String((char *)data).substring(0, len);
  Serial.println(message);

  if (message == "forward") {
    moveForward();
  } else if (message == "backward") { 
    moveBackward();
  } else if (message == "left") { 
    turnLeft();
  } else if (message == "right") { 
    turnRight();
  } else if (message == "stop") {
    stopCar();
  }
}

void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", client_num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", client_num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:
      handleWebSocketMessage(nullptr, payload, length);
      break;
  }
}


void setup() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

   camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
    if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

    esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_SVGA);

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("Camera Ready! Use 'http://" + WiFi.localIP().toString() + "' to connect");

server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", indexHtml);
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
});


  server.begin();
  Serial.println("HTTP server started");
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  Serial.println("WebSocket server started");
}

void loop() {
  webSocket.loop();

  if(webSocket.connectedClients()) {
    //Serial.println("WebSocket client is connected. Attempting to get camera frame...");
    camera_fb_t * fb = esp_camera_fb_get();
    if(fb) {
      //Serial.println("Camera frame captured, sending over WebSocket...");
      webSocket.broadcastBIN(fb->buf, fb->len);
      esp_camera_fb_return(fb);
    } else {
      //Serial.println("Failed to capture camera frame!");
    }
  } else {
    //                                                                                                              Serial.println("No WebSocket clients connected.");
  }

  delay(10); // A short delay to prevent excessive printing to the serial monitor
}





