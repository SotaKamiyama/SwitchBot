#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

#define SERVO_PIN 2 // ２番ピンで制御する

// アクセスポイント情報
const char* ssid = "CID-R&D-Site3-2.4G";        
const char* passwd = "chuoacjp";
//const char* ssid = "TP-Link_8E08";   // 家のWiFi
//const char* passwd = "18590821"; 
      

// サーボ角度
const int servo_left = 5;
const int servo_center = 90;
//const int servo_right = 135;

// IPアドレス指定
IPAddress local_IP(192, 168, 0, 24);
IPAddress gateway(192, 168, 31, 1);
IPAddress subnet(255, 255, 0, 0);


// WebServerクラス(ポートは80)
WebServer server(80);                  

// サーボクラス ※使用ライブラリは<ESP32Servo.h>
Servo myservo;

void setup() {
  // シリアルポート設定
  Serial.begin(115200);                   //シリアル通信のデータ転送レートを115200bpsで指定
  WiFi.begin(ssid, passwd);               //アクセスポイント接続のためのIDとパスワードの設定

  if (!WiFi.config(local_IP,subnet,gateway)) {
    Serial.println("STA Failed to configure");
  }
  while (WiFi.status() != WL_CONNECTED) { //接続状態の確認
    delay(300);                           //接続していなければ0.3秒待つ
    Serial.print(".");                    //接続しなかったらシリアルモニタに「.」と表示
  }

  // 通信が可能となったら各種情報を表示する
  Serial.println("");                     //シリアルモニタ改行
  Serial.println("WiFi Connected");       //接続したらシリアルモニタに「WiFi Connected」と表示
  Serial.print("IP Address : ");          //シリアルモニタに表示
  Serial.println(WiFi.localIP());         //割り当てられたIPアドレスをシリアルモニタに表示

  // serverにアクセスしたときの処理関数
  server.on("/", ServoHandle);         //TOPページのアドレスにアクセスしたときの処理関数
  server.onNotFound(handleNotFound);      //存在しないアドレスにアクセスしたときの処理関数
  
  // WebServerを起動
  server.begin();
  // サーボ初期設定
  myservo.attach(SERVO_PIN);
}

void loop() {
  // サーバー通信
  server.handleClient();
}

// TOPページにアクセスしたきの処理関数
void ServoHandle() {
  String html;
  //HTML記述
  html = "<!DOCTYPE html>";
  html += "<html lang='ja'>";
  html += "<meta charset=\"utf-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<head>";
  html += "<title>Servo Contlloler</title>";
  html += "</head>";
  html += "<h2>サーボリモコン</h2>";
//  html += "<p>";
//  html += "<form action='' method='post'>";
//  html += "<button type='submit' name='click' value='right'><big>スイッチ 右<big></button>";
//  html += "</form>";
//  html += "</p>";
  html += "<p>";
  html += "<form action='' method='post'>";
  html += "<button type='submit' name='click' value='left'><big>スイッチ 左<big></button>";
  html += "</form>";
  html += "</p>";
  html += "</body>";
  html += "</html>";
  html += "";
  
  // POSTで受信した場合、以下を実行
  if (server.method() == HTTP_POST) 
  {
    /*LED ON
    if (server.arg("click").equals("right"))       //clickの値がrightかどうかの判定
    {
      myservo.write(servo_right);
      delay(500);
      myservo.write(servo_center);
      delay(500);
      
    }*/
    // 左
    if (server.arg("click").equals("left")) //clickの値がleftかどうかの判定
    {
      myservo.write(servo_left);
      delay(500);
      myservo.write(servo_center);
      delay(500);
    }
  }
  server.send(200, "text/html", html);
}

// 存在しないアドレスへアクセスしたときの処理関数
void handleNotFound(void) {
  server.send(404, "text/plain", "Not Found");
}
