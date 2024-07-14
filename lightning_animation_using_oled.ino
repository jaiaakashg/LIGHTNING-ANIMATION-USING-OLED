#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

TaskHandle_t lightningTask;

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display.display();
  delay(2000);

 
  xTaskCreatePinnedToCore(
    lightningTaskCode,  
    "LightningTask",     
    10000,               
    NULL,                
    1,                   
    &lightningTask,     
    1);                  
}

void loop() {
  drawCloud();
  delay(random(2000, 5000)); 
}

void drawCloud() {
  display.clearDisplay();
  
  display.fillCircle(40, 20, 10, SSD1306_WHITE);
  display.fillCircle(60, 15, 13, SSD1306_WHITE);
  display.fillCircle(80, 20, 10, SSD1306_WHITE);
  display.fillCircle(50, 25, 10, SSD1306_WHITE);
  display.fillCircle(70, 25, 10, SSD1306_WHITE);

  display.display();
}

void lightningTaskCode(void * parameter) {
  for(;;) {
    drawLightning();
    delay(random(2000, 5000)); 
  }
}

void drawLightning() {
  int strikes = random(2, 6); 

  for (int i = 0; i < strikes; i++) {
    display.clearDisplay();
    drawCloud();

    int startX = random(30, 100); 
    int startY = 30;
    int currentX = startX;
    int currentY = startY;
    
    for (int j = 0; j < random(10, 20); j++) {
      int nextX = currentX + random(-10, 11);
      int nextY = currentY + random(5, 15);
      
      if (nextX < 0) nextX = 0;
      if (nextX >= SCREEN_WIDTH) nextX = SCREEN_WIDTH - 1;
      if (nextY >= SCREEN_HEIGHT) nextY = SCREEN_HEIGHT - 1;
      
      display.drawLine(currentX, currentY, nextX, nextY, SSD1306_WHITE);
      
      if (random(0, 100) < 30) { 
        int branchX = nextX;
        int branchY = nextY;
        for (int k = 0; k < random(3, 8); k++) {
          int branchNextX = branchX + random(-10, 11);
          int branchNextY = branchY + random(5, 15);
          
          if (branchNextX < 0) branchNextX = 0;
          if (branchNextX >= SCREEN_WIDTH) branchNextX = SCREEN_WIDTH - 1;
          if (branchNextY >= SCREEN_HEIGHT) branchNextY = SCREEN_HEIGHT - 1;
          
          display.drawLine(branchX, branchY, branchNextX, branchNextY, SSD1306_WHITE);
          
          branchX = branchNextX;
          branchY = branchNextY;
        }
      }
      
      currentX = nextX;
      currentY = nextY;
    }
    
    display.display();
    delay(50); 
  }
}

