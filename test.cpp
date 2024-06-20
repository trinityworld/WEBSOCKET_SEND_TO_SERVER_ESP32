#include <Arduino.h>
TaskHandle_t TASK1;
TaskHandle_t TASK2;
// #include <WiFi.h>
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// const char *ssid = "dlink-9878";
// const char *password = "dlink422";
// 18 pins to seven segment in 1st core
// data transfer to server in 1st core
// parallel get pluse continuse in 2nd core

const int segmentPins[8] = {15, 4, 16, 17, 5, 18, 19, 21};
const int digitpins[10] = {3, 1, 22, 23, 13, 12, 14, 27, 26, 25};
const byte digitCodes[10] =
    {
        B00111111, // 0
        B00000110, // 1
        B01011011, // 2
        B01001111, // 3
        B01100110, // 4
        B01101101, // 5
        B01111101, // 6
        B00000111, // 7
        B01111111, // 8
        B01101111  // 9
};
// AsyncWebServer server(80);
// AsyncWebSocket ws("/ws");
// String handlemessage;
// int counter;
// const char index_html[] PROGMEM = R"rawliteral(
// <!DOCTYPE HTML><html>
// <head>
//   <title></title>
//   <meta name="viewport" content="width=device-width, initial-scale=1">
//   <style>
//   html {
//     font-family: Arial, Helvetica, sans-serif;
//     text-align: left;
//   }

//   h1{
//     font-size: 1.2rem;
//     font-weight: bold;
//     color: #143642;
//     text-align:left;
//     }

// </style>
// <title></title>
// <meta name="viewport" content="width=device-width, initial-scale=1">
// <link rel="icon" href="data:,">
// </head>
// <body>
//   <div class="content">
//   <h1 id="counterValue">0</h1>
//   </div>
// <script>
//   var gateway = `ws://${window.location.hostname}/ws`;
//   var websocket;
//   window.addEventListener('load', onLoad);
//   function initWebSocket() {
//     console.log('Trying to open a WebSocket connection...');
//     websocket = new WebSocket(gateway);
//     websocket.onopen    = onOpen;
//     websocket.onclose   = onClose;
//     websocket.onmessage = onMessage;
//   }
//   function onOpen(event) {
//     console.log('Connection opened');
//   }
//   function onClose(event) {
//     console.log('Connection closed');
//     setTimeout(initWebSocket, 2000);
//   }
//   function onMessage(event)
//   {
//     console.log(event.data);
//     var counterValue = parseInt(event.data);
//     document.getElementById('counterValue').innerText = counterValue;
//   }

//   function onLoad(event) {
//     initWebSocket();
//   }
// </script>
// </body>
// </html>
// )rawliteral";

// void notifyClients(String handlemessage)
// {
//   ws.textAll(String(handlemessage));
// }

// void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
// {
//   AwsFrameInfo *info = (AwsFrameInfo *)arg;
//   if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
//   {
//     data[len] = 0;
//     handlemessage = (char *)data;
//     notifyClients(handlemessage);
//   }
// }

// void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
//              void *arg, uint8_t *data, size_t len)
// {
//   switch (type)
//   {
//   case WS_EVT_CONNECT:
//     Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
//     break;
//   case WS_EVT_DISCONNECT:
//     Serial.printf("WebSocket client #%u disconnected\n", client->id());
//     break;
//   case WS_EVT_DATA:
//     handleWebSocketMessage(arg, data, len);
//     break;
//   case WS_EVT_PONG:
//   case WS_EVT_ERROR:
//     break;
//   }
// }

// void initWebSocket()
// {
//   ws.onEvent(onEvent);
//   server.addHandler(&ws);
// }

void displayDigitTask(void *pvParameters)
{
  int gpio_pin = 0;
  int set_digit = 5;
  while (1)
  {

    for (int i = 0; i < 10; i++)
    {
      digitalWrite(digitpins[i], LOW);
    }
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(segmentPins[i], (digitCodes[set_digit] >> i) & 0x01);
    }
    digitalWrite(digitpins[gpio_pin], HIGH);
    // counter++;
  }
}

void get_pluseTask(void *pvParameters)
{
  while (1)
  {
    // bool pluse_status = (GPIO_REG_READ(GPIO_OUT1_REG) >> 33) & 0x1;
    // if (pluse_status)
    Serial.println("1");
    // else
    //   Serial.println("0");
  }
}

void setup()
{
  for (int i = 0; i < 8; i++)
  {
    pinMode(segmentPins[i], OUTPUT);
  }
  for (int i = 0; i < 10; i++)
  {
    pinMode(digitpins[i], OUTPUT);
  }
  pinMode(GPIO_NUM_33, OUTPUT); // get pluse
  // Serial.begin(115200);
  delay(10);

  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.print("Connected to ");
  // Serial.println(WiFi.localIP());
  // initWebSocket();
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  //           { request->send_P(200, "text/html", index_html); });

  // server.begin();
  delay(1000);

  xTaskCreatePinnedToCore(
      displayDigitTask,
      "TASK1",
      10000,
      NULL,
      1,
      &TASK1,
      0);
  delay(500);
  xTaskCreatePinnedToCore(
      get_pluseTask,
      "TASK2",
      10000,
      NULL,
      1,
      &TASK2,
      1);
  delay(500);
}

void loop()
{
  //   notifyClients(String(counter));
  // delay(1000);
  // ws.cleanupClients();
}
