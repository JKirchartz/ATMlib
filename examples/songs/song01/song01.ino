#include <Arduino.h>
#include <ATMlib.h>
#include "Arglib.h"
#include "bitmaps.h"
#include "song.h"


Arduboy arduboy;
Sprites sprites(arduboy);


void setup() {
  arduboy.start();
  // set the framerate of the game at 60 fps
  arduboy.setFrameRate(60);
    // Begin playback of song.
    // Initializes ATMSynth and samplerate
    // Lower the tempo ever so slightly
    ATM.play(music);
}

void loop() {

  if (!(arduboy.nextFrame())) return;
  arduboy.poll();
  arduboy.clearDisplay();
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 10, TEAMarg, i);
  sprites.drawSelfMasked(43, 50, TEAM_argPart5, 0);
  if (arduboy.justPressed(B_BUTTON)) ATM.play(music, 15625, 50);
  if (arduboy.justPressed(A_BUTTON)) ATM.stop();
  arduboy.display();
}

