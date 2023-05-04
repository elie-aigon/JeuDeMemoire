#include <LiquidCrystal.h>
#define buzzer 3
#define pin_X A0
#define pin_Y A1
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);
const int LED[4] = { 2, 3, 4, 5 };
const int freq = 255;
int lvlCombination[50];
int combinationPlayed[50];
int step = 0;
int lvl = 0;
int inputNumber = 0;
int led_delay = 500;
bool lost = false;

int move_player() {
  int x = analogRead(pin_X);
  int y = analogRead(pin_Y);
  if (x < 300) {
    return 4;
  } else if (x > 700) {
    return 3;
  }
  if (y < 300) {
    return 2;
  } else if (y > 700) {
    return 5;
  }
  return 0;
}
void setup() {
  lcd.begin(16, 2);
  for (int i = 0; i < 4; i++) {
    pinMode(LED[i], OUTPUT);
  }
}
void loop() {
  int randomLed = random(2, 6);

  switch (step) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("  Memory  Game  ");
      lcd.setCursor(1, 1);
      lcd.print("Move  Joystick");

      if (move_player() != 0) {
        lcd.clear();
        step = 1;
      }
      break;

    case 1:
      lcd.setCursor(0, 0);
      lcd.print("lvl: ");
      lcd.setCursor(7, 0);
      lcd.print(lvl);
      lcd.setCursor(0, 1);
      lcd.print("    Memorise    ");
      delay(500);
      lvlCombination[lvl] = randomLed;
      for (int i = 0; i <= lvl; i++) {
        digitalWrite(lvlCombination[i], HIGH);
        delay(led_delay);
        digitalWrite(lvlCombination[i], LOW);
        delay(led_delay);
      }
      step = 2;
      break;
    case 2:
      lcd.setCursor(0, 1);
      lcd.print("      Play      ");
      for (int i = 0; i < 4; i++) {
        if (move_player() != 0) {
          combinationPlayed[lvl] = move_player();
          digitalWrite(combinationPlayed[lvl], HIGH);
          delay(400);
          digitalWrite(combinationPlayed[lvl], LOW);
          delay(100);
          inputNumber++;
          if (inputNumber - 1 == lvl) {
            inputNumber = 0;
            step = 3;
            break;
          }
        }
      }
      delay(100);
      break;

    case 3:
      lcd.setCursor(0, 1);
      lcd.print("  Verification  ");
      delay(1000);
      for (int i = 0; i <= lvl; i++) {
        if (lvlCombination[i] != combinationPlayed[i]) {
          lost = true;
          break;
        }
      }if (lost == true) {
        step = 4;
      } else {
        step = 5;
      }
      break;
    case 4:
      lcd.setCursor(0, 1);
      lcd.print("   You Lose ! ");
      tone(buzzer, freq);
      delay(500);
      noTone(buzzer);
      lvl = 0;
      step = 0;
      lost = false;
      delay(1000);
      break;
    case 5:
      lcd.setCursor(0, 1);
      lcd.print("  You WIN !   ");
      lvl++;
      if (lvl % 5 == 0){
        led_delay = led_delay - 100;
      }
      step = 1;
      delay(1000);
      break;

    default:
      break;
  }
}
