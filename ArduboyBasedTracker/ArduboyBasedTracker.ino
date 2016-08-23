/*
  DEMO: Arduboy Based Tracker

  MADE by TEAM a.r.g. : https://github.com/TEAMarg/ATMlib

  2016 - STG (Davey Taylor)

  License: MIT : https://opensource.org/licenses/MIT

*/


#include <Arduino.h>
#include "Arglib.h"
#include "ATMlib.h"
#include "bitmaps.h"
#include "song.h"

Arduboy arduboy;
Sprites sprites(arduboy);


extern Melody music[];             // Squawk 2 melody

void setup() {
  arduboy.start();
  arduboy.setFrameRate(60);                                 // set the frame rate of the game at 60 fps
  
  Squawk.begin(15625);
  Squawk.play(music);
  Squawk.tune(2.0);
  Squawk.tempo(25);

}

void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.clearDisplay();
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 10, TEAMarg, i);
  sprites.drawSelfMasked(43, 50, TEAM_argPart5, 0);
  arduboy.display();
}



