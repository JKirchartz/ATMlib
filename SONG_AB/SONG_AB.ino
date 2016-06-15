/*
  EXAMPLE SONG FOR THE NEW SQUAWK OR ATMlib

  MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html

  2016 - STG - JO3RI

  License: MIT : https://opensource.org/licenses/MIT

*/


#include "Arglib.h"
#include "ATMlib.h"
#include "example_song.h"
#include "globals.h"
#include "menu.h"
#include "game.h"


extern Melody music[];

typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGameNextLevel,
  stateGamePlaying,
  stateGamePause,
  stateGameOver,
};


void setup() {
  arduboy.start();
    // Set up Squawk to generate samples at 32kHz.
  // Squawk always steals Timer1 for sample crunching.
  Squawk.begin(32000);
  // Begin playback of melody.
  Squawk.play(music);
  // Tune the song to something more suitable for a piezo
  Squawk.tune(2.0);
  // Lower the tempo ever so slightly
  Squawk.tempo(50);
  arduboy.setFrameRate(60);                                 // set the frame rate of the game at 60 fps
}


void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.poll();
  arduboy.clearDisplay();
  ((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
}

