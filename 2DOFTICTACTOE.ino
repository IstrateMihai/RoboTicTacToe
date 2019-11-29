#include "Arduino.h"
#include "AX12A.h"

#include "tictactoe.h"

#define DirectionPin   (10u)
#define BaudRate      (1000000)


void setup() {
ax12a.begin(BaudRate, DirectionPin, &Serial);

randomSeed(analogRead(0));
  // put your setup code here, to run once:
}

void loop() {
int d = TicTacToe(0.10,0.10,0.45,0.15);
}
 
 
