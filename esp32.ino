#include <WiFi.h>
#include <ESP_Mail_Client.h>

// Thông tin WiFi
const char* WIFI_SSID = "TP-LINK_E0F2";
const char* WIFI_PASSWORD = "46823281";

// Email config
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define AUTHOR_EMAIL "tranduykhanh.uet@gmail.com"
#define AUTHOR_PASSWORD "yomd pkfd amlq dtdi"
#define RECIPIENT_EMAIL "khanhbeti632004@gmail.com"

// SMTP Gmail
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

// Chân nhận tín hiệu
#define SIGNAL_PIN 34
 
// SMTP và Email
SMTPSession smtp;
ESP_Mail_Session session;
SMTP_Message message;

// Biến cờ trạng thái
bool alreadySent = false;

void setup() {
  Serial.begin(115200);
  pinMode(SIGNAL_PIN, INPUT);

  // Kết nối WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Đang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n Đã kết nối WiFi!");

  // Cấu hình session SMTP
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";
}

void loop() {
  int signalState = digitalRead(SIGNAL_PIN);

  if (signalState == HIGH && !alreadySent) {
    Serial.println("Tín hiệu MỨC CAO nhận được - Đang gửi email...");

    // Cấu hình email
    message.sender.name = "ESP32 Canh Bao";
    message.sender.email = AUTHOR_EMAIL;
    message.subject = "ESP32 cảnh báo!";
    message.addRecipient("Người nhận", RECIPIENT_EMAIL);
    message.text.content = "ESP32 vừa nhận được tín hiệu mức cao từ cảm biến!";
    message.text.charSet = "utf-8";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
    // Gửi
    if (!smtp.connect(&session)) {
      Serial.println("Lỗi kết nối SMTP");
      return;
    }
    if (!MailClient.sendMail(&smtp, &message)) {
      Serial.println("Gửi email thất bại: " + smtp.errorReason());
    } else {
      Serial.println("Gửi email thành công!");
    }
    smtp.closeSession();
    alreadySent = true; // Đánh dấu đã gửi
    delay(1000); // Tránh rung nhiễu tín hiệu
  }
  
  // Nếu tín hiệu về LOW → reset cờ
  if (signalState == LOW && alreadySent) {
    alreadySent = false;
    Serial.println(" Tín hiệu đã trở lại MỨC THẤP - Sẵn sàng gửi tiếp lần sau.");
  }
  
  delay(100); // Giảm tải vòng lặp
}