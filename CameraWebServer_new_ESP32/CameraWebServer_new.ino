#include "esp_camera.h"
#include <WiFi.h>
//
// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
//
//            You must select partition scheme from the board menu that has at least 3MB APP space.
//            Face Recognition is DISABLED for ESP32 and ESP32-S2, because it takes up from 15
//            seconds to process single frame. Face Detection is ENABLED if PSRAM is enabled as well

// ===================
// Select camera model
// ===================
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE  // Has PSRAM
#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_CAMS3_UNIT  // Has PSRAM
//#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
//#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
// ** Espressif Internal Boards **
//#define CAMERA_MODEL_ESP32_CAM_BOARD
//#define CAMERA_MODEL_ESP32S2_CAM_BOARD
//#define CAMERA_MODEL_ESP32S3_CAM_LCD
//#define CAMERA_MODEL_DFRobot_FireBeetle2_ESP32S3 // Has PSRAM
//#define CAMERA_MODEL_DFRobot_Romeo_ESP32S3 // Has PSRAM
#include "camera_pins.h"

// ===========================
// Enter your WiFi credentials
// ===========================
const char *ssid = "cook";
const char *password = "hyfhyf666";
const int serverPort = 6666;  // 设置服务器端口号

const int pin_left = 19;
const int pin_right = 20;
const int pin_down = 21;
const int pin_up = 47;
const int pin_rest = 48;
void startCameraServer();
void setupLedFlash(int pin);

WiFiServer server(serverPort);

// 添加全局变量来追踪摄像头状态
bool camera_initialized = false;

// 添加全局变量存储摄像头状态信息
String camera_status = "";

// 封装摄像头初始化函数
bool initCamera() {
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
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    camera_status = "Camera init failed with error 0x" + String(err, HEX);
    Serial.println(camera_status);
    return false;
  }

  sensor_t *s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif

// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif

  camera_status = "Camera initialized successfully";
  camera_initialized = true;
  return true;
}

void setup() {
  pinMode(pin_left,OUTPUT);
  pinMode(pin_right,OUTPUT);
  pinMode(pin_down,OUTPUT);
  pinMode(pin_up,OUTPUT);

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  

  // 尝试初始化摄像头
  if (!initCamera()) {
    Serial.println("Camera initialization failed, continuing with WiFi setup");
  }

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");


  startCameraServer();
  String cameraUrl = "http://" + WiFi.localIP().toString() + ":6666/stream";
  Serial.println("等待Android客户端连接...");
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Android客户端已连接");
    // 发送简单的JSON格式响应
    String jsonResponse = String("{\"status\":\"") + 
                         (camera_initialized ? "success" : "error") + 
                         "\",\"message\":\"" + camera_status + "\"" +
                         (camera_initialized ? ",\"url\":\"" + cameraUrl + "\"" : "") +
                         "}";
    client.println(jsonResponse);
    delay(100);
    client.stop();
  }

  Serial.print(F("Camera Ready! Use 'http://"));
  // Serial.print(WiFi.localIP());
  Serial.print(cameraUrl);
  
  Serial.println(("' to connect"));
  Serial.println("");
  Serial.println(F("WiFi connected"));
  server.begin(); // 开始监听端口
  Serial.println(("Server started"));
  pinMode(pin_left,OUTPUT);
  pinMode(pin_right,OUTPUT);
  pinMode(pin_down,OUTPUT);
  pinMode(pin_up,OUTPUT);
  pinMode(pin_rest,OUTPUT);
  digitalWrite(pin_rest, HIGH);
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.println(c);
        switch(c){
          case '1':// 左
              digitalWrite(pin_left, HIGH);
          break;
          case '2':// 右
              digitalWrite(pin_right, HIGH);
          break;
          case '3':// 下
              digitalWrite(pin_down, HIGH);
          break;
          case '4':// 上
              digitalWrite(pin_up, HIGH);
          break;
          case '0':// 停止
              digitalWrite(pin_left, LOW);
              digitalWrite(pin_right, LOW);
              digitalWrite(pin_down, LOW);
              digitalWrite(pin_up, LOW);
              digitalWrite(pin_rest, HIGH);
          break;
          case '5': //调速模式   
              digitalWrite(pin_down, HIGH);  //已经定义好 上下脚电平同时高就进入调速模式
              digitalWrite(pin_up, HIGH);
          break;
          case '6': //重新回到正常模式
              digitalWrite(pin_left, HIGH);//已经定义好 从调速模式回到正常模式需要让左右高
              digitalWrite(pin_right, HIGH);
          break;
          case '7': //复位从控制板
              esp_restart();//复位esp32控制板
          break;
          case '8': //复位主控制板
              digitalWrite(pin_rest, LOW);//让主控制板复位
              // rest_flag=1;
          break;
          case '9': // 重新连接摄像头
            Serial.println("Attempting to reinitialize camera...");
            if (camera_initialized) {
              esp_camera_deinit();
              camera_initialized = false;
            }
            if (initCamera()) {
              camera_status = "Camera reinitialized successfully";
              Serial.println(camera_status);
              // 发送简单的JSON格式响应
              client.println("{\"status\":\"success\",\"message\":\"" + camera_status + "\"}");
            } else {
              camera_status = "Camera reinitialization failed: " + camera_status;
              Serial.println(camera_status);
              // 发送简单的JSON格式响应
              client.println("{\"status\":\"error\",\"message\":\"" + camera_status + "\"}");
            }
            break;
        }
        // if(rest_flag==1)
        // {
        //   digitalWrite(pin_rest, LOW);//让主控制板复位
        //   rest_flag=0;
        // }else
        // {
        //   digitalWrite(pin_rest, HIGH);//让主控制板复位
        // }

      }
    }
  }
  //Serial.println("Client disconnected");
  client.stop();
}
