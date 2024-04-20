#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int buttonPin1 = 2; // Mavi buton
const int buttonPin2 = 8; // Kırmızı buton

int selectedOption = 0;
const int numOptions = 2; // Başla ve Çıkış

String options[] = {"BASLA", "CIKIS"};

void setup() {
  Serial.begin(9600);
  
  pinMode(buttonPin1, INPUT_PULLUP); // Dahili direnç ile mavi butonun pull-up
  pinMode(buttonPin2, INPUT_PULLUP); // Dahili direnç ile kırmızı butonun pull-up

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void basla() {
  // Ekranı temizle
  display.clearDisplay();
  display.display();
  // Yeni ekrana geçiş mesajını yaz
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.println("Selam!");
  display.display();
}
void loop() {
  // Menüyü göster
  display.clearDisplay();
  display.setCursor(0, 0);
  
  // Seçilen seçeneğin yanında bir nokta göster
  if (selectedOption == 0) {
    display.print("* ");
  }
  display.println(options[0]);
  
  if (selectedOption == 1) {
    display.print("* ");
  }
  display.println(options[1]);
  
  display.display();

  // Mavi butonun durumunu oku
  int buttonState1 = digitalRead(buttonPin1);
  // Mavi butona basıldığında seçilen seçeneği değiştir
  if(buttonState1 == LOW) {
    selectedOption = 1 - selectedOption; // Toggle işlemi (0 ise 1, 1 ise 0 yapar)
    delay(200); // debounce
    // Seçilen seçenekle ilgili işlevi çağır
  }

  // Kırmızı butonun durumunu oku
  int buttonState2 = digitalRead(buttonPin2);
  // Kırmızı butona basıldığında seçilen seçeneği seç
  if(buttonState2 == LOW) {
    // Seçilen seçeneği seri monitöre yaz
    Serial.println("Seçilen seçenek: " + options[selectedOption]);
    // Çıkış seçeneği seçildiyse programı sonlandır
    if (selectedOption == 1) {
      // Ekranı temizle
      display.clearDisplay();
      display.display();
      // Sonsuz döngüde kalır ve işlemi durdurur
  while(true) {
    // Sonsuz döngü
  }
  }
  if(selectedOption == 0){
    
    basla();
  }

  }
}
