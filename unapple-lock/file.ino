#include "DigiKeyboard.h"


/*  
Version 2 
Dev Ayhan 
// null info
  */
int num[] = {39,30,31,32,33,34,35,36,37,38};
int index[5] = {0,0,0,0,0};
int count = 0;

void setup() {
  DigiKeyboard.update();
  DigiKeyboard.sendKeyStroke(0);
  delay(3000);
}

void loop() {
  int i;
  for (i = 4; i >= 0; i--) {
    DigiKeyboard.sendKeyPress(num[index[i]]);
    delay(100);
    
    if (index[i] == 9) {
      index[i] = 0;
    } else {
      index[i]++;
      break;
    }
  }
  
  if (i < 0) {
    DigiKeyboard.sendKeyStroke(40);
    delay(1000);
    count++;
    
    if (count == 5) {
      DigiKeyboard.sendKeyStroke(40);
      delay(1000);
      count = 0;
    }
  }
}
