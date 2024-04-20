#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BUTTON_PIN 2 // Butonun bağlı olduğu pin
#define RED_BUTTON_PIN 8 //Kırmızı butonun bağlı olduğu pin
#define RELAY_PIN 0 //relayin pini

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool lastButtonState = HIGH; // Butonun son durumu
bool currentButtonState = HIGH; // Butonun mevcut durumu

bool isBlinking = false; // Metnin yanıp sönme durumu
bool seciliMetin = false;

unsigned long lastBlinkTime = 0; // Son yanıp sönme zamanı
unsigned long blinkInterval = 500; // Yanıp sönme aralığı (milisaniye)

bool startTextBlink = false; // "BAŞLA" metni yanıp sönme durumu
bool exitTextBlink = false; // "ÇIKIŞ" metni yanıp sönme durumu

void setup() {
  // Seri bağlantıyı başlat
  Serial.begin(9600);

  // Buton pini için INPUT_PULLUP modunu ayarla
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RED_BUTTON_PIN, INPUT_PULLUP);

  // SSD1306 ekranını başlat
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 başlatılamadı!"));
    for(;;);
  }

  // Ekranı temizle
  display.clearDisplay();

  // Metni yazdır
  display.setTextSize(1);            
  display.setTextColor(SSD1306_WHITE); // Metin rengi
  display.setCursor(20,20); // Metnin konumu
  display.println("BASLA"); // "BAŞLA" metnini ekrana yaz
  display.setCursor(20,35);
  display.println("CIKIS"); // "ÇIKIŞ" metnini ekrana yaz

  // Ekrana gönder
  display.display();

  delay(2000); // 2 saniye bekleyin
}

void loop() {
  if(digitalRead(BUTTON_PIN) == LOW){
    seciliMetin = !seciliMetin;
    if(seciliMetin){
      display.setTextSize(1);            
      display.setTextColor(SSD1306_WHITE); // Metin rengi
      display.setCursor(20,20); // Metnin konumu
      display.println("BASLA"); // "BAŞLA" metnini ekrana yaz
      display.setTextColor(SSD1306_BLACK); // Metin rengi
      display.setCursor(20,35);
      display.println("CIKIS"); // "ÇIKIŞ" metnini ekrana yaz
      display.display();
    }
    else{
      display.setTextSize(1);            
      display.setTextColor(SSD1306_BLACK); // Metin rengi
      display.setCursor(20,20); // Metnin konumu
      display.println("BASLA"); // "BAŞLA" metnini ekrana yaz
      display.setTextColor(SSD1306_WHITE); // Metin rengi
      display.setCursor(20,35);
      display.println("CIKIS"); // "ÇIKIŞ" metnini ekrana yaz
      display.display();
    }
  }

  if(digitalRead(RED_BUTTON_PIN) == LOW){
    if(seciliMetin){
        display.clearDisplay();
    }
    else{
        system("shutdown");
    }

  }
  
}
