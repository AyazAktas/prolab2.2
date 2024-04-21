#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define A 10
#define B 9
#define C 8
#define D 7
#define E 6
#define F 5
#define G 4

const int ballSize = 4;          // Topun çapı
const int paddleWidth = 20;      // Palet genişliği
const int paddleHeight = 4;      // Palet yüksekliği
const int paddleSpeed = 3;       // Paletin hareket hızı
const int brickRows = 4;         // Tuğla sıraları
const int brickColumns = 6;      // Tuğla sütunları
const int brickWidth = 20;       // Tuğla genişliği
const int brickHeight = 8;       // Tuğla yüksekliği
const int brickPadding = 2;      // Tuğla arası boşluk
const int brickOffsetX = 0;     // Tuğla başlangıç x konumu
const int brickOffsetY = 2;      // Tuğla başlangıç y konumu

const int buttonPin1 = 2; // Mavi buton
const int buttonPin2 = 11; // Kırmızı buton

const int redLedPin = 8; // Kırmızı led
const int yellowLedPin = 13; // Sarı led
const int greenLedPin = 12; // Yeşil led


int selectedOption = 0;
const int numOptions = 2; // Başla ve Çıkış

String options[] = {"BASLA", "CIKIS"};

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin1, INPUT_PULLUP); // Dahili direnç ile mavi butonun pull-up
  pinMode(buttonPin2, INPUT_PULLUP); // Dahili direnç ile kırmızı butonun pull-up

  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }

  // Tuğlaların ekranın ortasına hizalanması için offset değerleri hesapla
  int brickOffsetX = (SCREEN_WIDTH - (brickColumns * (brickWidth + brickPadding))) / 2;
  int brickOffsetY = (SCREEN_HEIGHT - (brickRows * (brickHeight + brickPadding))) / 2;

  display.display();
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

int score = 0; // Skor değişkeni
int lives = 3; // Can sayısı

const byte digits[][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void basla() {

  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);

  digitalWrite(redLedPin, HIGH);
  digitalWrite(yellowLedPin, HIGH);
  digitalWrite(greenLedPin, HIGH);
   int ballX = (SCREEN_WIDTH - paddleWidth) / 2;
  int ballY = paddleHeight + ballSize;
  int ballDX = 1;
  int ballDY = 1;
  int paddleX = (SCREEN_WIDTH - paddleWidth) / 2;

  // Tuğla matrisini başlat
  int bricks[brickRows][brickColumns];

  int brickCount = 0;
  // Rastgele konumlarda tuğlaları oluştur
  for (int row = 0; row < brickRows; row++) {
    for (int col = 0; col < brickColumns; col++) {
      bricks[row][col] = random(0, 2); // Her tuğla için rastgele bir durum oluştur (0: yok, 1: var)
      if(bricks[row][col] == 1){
        brickCount++;
      }
    }
  }


  // Ana oyun döngüsü
  while (lives > 0) { // Canlar sıfırdan büyük olduğu sürece devam et
    // Potansiyometreden paletin konumunu oku
    int potValue = analogRead(A0);
    paddleX = map(potValue, 0, 1023, 0, SCREEN_WIDTH - paddleWidth);

    // Ekranı temizle
    display.clearDisplay();

    // Topu ve paleti çiz
    display.fillCircle(ballX, ballY, ballSize, SSD1306_WHITE);
    display.fillRect(paddleX, SCREEN_HEIGHT - 8, paddleWidth, paddleHeight, SSD1306_WHITE);
    // Tuğlaları çiz
    for (int row = 0; row < brickRows; row++) {
      for (int col = 0; col < brickColumns; col++) {
        if (bricks[row][col] == 1) {
        int brickX = col * (brickWidth + brickPadding) + brickOffsetX;
        int brickY = row * (brickHeight + brickPadding) + brickOffsetY;
        display.fillRect(brickX, brickY, brickWidth, brickHeight, SSD1306_WHITE);
        }        
      }
    }

    // Eğer hiç kırılmamış tuğla kalmadıysa
    if (brickCount == 0) {
      for (int row = 0; row < brickRows; row++) {
        for (int col = 0; col < brickColumns; col++) {
            bricks[row][col] = random(0, 2); // Her tuğla için rastgele bir durum oluştur (0: yok, 1: var)
            if(bricks[row][col] == 1){
                brickCount++;
            }
        }
    }
    
    // Ekranı temizle
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 20);
    display.println("Tebrikler!");
    display.println("Yeni bölüme geçin!");
    display.display();
    delay(5000); // 5 saniye bekleme
    }

    // Topun hareketini güncelle
    ballX += ballDX;
    ballY += ballDY;

    // Topun sınırları kontrol et
    if (ballX <= 0 || ballX >= SCREEN_WIDTH) {
      ballDX *= -1;
    }
    if (ballY <= 0) {
      ballDY *= -1;
    }
    if (ballY >= SCREEN_HEIGHT) {
      // Top ekranın altına ulaştı, canı azalt
      lives--;
      Serial.print("Kalan Can: ");
      Serial.println(lives);
      if(lives == 2){
        digitalWrite(greenLedPin, LOW);
      }
      if(lives == 1){
        digitalWrite(yellowLedPin, LOW);
      }
      if(lives == 0){
        digitalWrite(redLedPin, LOW);
      }
      delay(1000); // Bir saniye bekleme
      // Yeni bir can varsa, topu ve paleti başlangıç pozisyonuna yerleştir
      if (lives > 0) {
        ballX = (SCREEN_WIDTH - paddleWidth) / 2;
        ballY = paddleHeight + ballSize;
        continue; // Oyunu durdurmadan devam et
      } else {
        // Can kalmadığında oyunu bitir
        break;
      }
    }

    // Topun paletle çarpışmasını kontrol et
    if (ballY >= SCREEN_HEIGHT - 8 - ballSize && ballX >= paddleX && ballX <= paddleX + paddleWidth) {
      ballDY *= -1;
    }

    // Topun tuğlalarla çarpışmasını kontrol et
    for (int row = 0; row < brickRows; row++) {
      for (int col = 0; col < brickColumns; col++) {
        if (bricks[row][col] == 1) {
          int brickX = col * (brickWidth + brickPadding) + brickOffsetX;
          int brickY = row * (brickHeight + brickPadding) + brickOffsetY;
          if (ballY - ballSize <= brickY + brickHeight && ballY + ballSize >= brickY &&
              ballX + ballSize >= brickX && ballX - ballSize <= brickX + brickWidth) {
            ballDY *= -1;
            bricks[row][col] = 0; // Tuğlayı kır
            score++; // Skoru artır
            displayDigit(score % 10); // Skoru güncelle
            brickCount--; // Kırılmamış tuğla sayısını azalt
            Serial.println(score);
          }
        }
      }
    }

    // Oyunu yeniden çiz
    display.display();

    // Biraz gecikme ekle
    delay(10);
  }

  // Oyun bittiğinde
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.print("SKOR: ");
  display.println(score);
  display.display();
  delay(3000); // 3 saniye bekleme
}

// 7 segment display'i güncellemek için fonksiyon
void displayDigit(int digit) {
  int a = 0;
  for (int i = 10; i > 3; i--) {
    digitalWrite(i, !digits[digit][a]); // Değeri tersine çevir
    a++;
  }
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
  if (buttonState1 == LOW) {
    selectedOption = 1 - selectedOption; // Toggle işlemi (0 ise 1, 1 ise 0 yapar)
    delay(200); // debounce
    // Seçilen seçenekle ilgili işlevi çağır
  }

  // Kırmızı butonun durumunu oku
  int buttonState2 = digitalRead(buttonPin2);
  // Kırmızı butona basıldığında seçilen seçeneği seç
  if (buttonState2 == LOW) {
    // Seçilen seçeneği seri monitöre yaz
    Serial.println("Seçilen seçenek: " + options[selectedOption]);
    // Çıkış seçeneği seçildiyse programı sonlandır
    if (selectedOption == 1) {
      // Ekranı temizle
      display.clearDisplay();
      display.print("tesekkurler");
      display.display();
      // Sonsuz döngüde kalır ve işlemi durdurur
      while (true) {
        // Sonsuz döngü
      }
    }
    if (selectedOption == 0) {
      score = 0; // Oyun başladığında skoru sıfırla
      lives = 3; // Canları yeniden ayarla
      basla();
    }
  }
}
