/*
  Arduboy Tracker Music Library for Arduino Leonardo "Arduboy"

  2015
  TEAM a.r.g.
  Davey Taylor
*/

#include "ATMlib.h"

SQUAWK_CONSTRUCT_ISR(OCR4A)

byte trackCount;
const word *trackList;
const byte *trackBase;
uint8_t pcm;
bool half;


// Locals
static uint16_t tick_rate = 50;
static uint16_t sample_rate;

//Imports
extern uint16_t cia;

// Exports
osc_t osc[4];


const word noteTable[64] PROGMEM = {
  262,  277,  294,  311,  330,  349,  370,  392,  415,  440,  466,  494,
  523,  554,  587,  622,  659,  698,  740,  784,  831,  880,  932,  988,
  1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
  2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951,
  4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902,
  8372, 8870, 9397,    0,
};

typedef struct ch_t {
  const byte *ptr;
  word stackPointer[7];
  byte stackCounter[7];

  byte stackIndex;
  word delay;
  byte counter;
  byte track;
};

ch_t channel[4];

uint16_t read_vle(const byte **pp) {
  word q = 0;
  byte d;
  do {
    q <<= 7;
    d = pgm_read_byte(*pp++);
    q |= (d & 0x7F);
  } while (d & 0x80);
  return q;
}

static inline const byte *getTrackPointer(byte track) {
  return trackBase + pgm_read_word(&trackList[track]);
}

ATMSynth ATM;

// Initializes ATMSynth
// Sets up the ports, and the sample grinding ISR
void ATMSynth::begin(uint16_t hz) {
  sample_rate = hz;
  cia = sample_rate / tick_rate;

  osc[3].freq = 0x0001; // Seed LFSR
  
  pinMode(5, OUTPUT);
  pinMode(13, OUTPUT);
  TCCR4A = 0b01000010;    // Fast-PWM 8-bit
  TCCR4B = 0b00000001;    // 62500Hz
  OCR4C  = 0xFF;          // Resolution to 8-bit (TOP=0xFF)
  OCR4A  = 0x80;
  TIMSK4 = 0b00000100;    
}

// Load a melody stream and start grinding samples
void ATMSynth::play(const byte *song) {
  // Read track count
  trackCount = pgm_read_byte(song++);
  // Store track list pointer
  trackList = (word*)song;
  // Store track pointer
  trackBase = (song += (trackCount << 1)) + 4;
  // Fetch starting points for each track
  for (unsigned n = 0; n < 4; n++) {
    channel[n].ptr = getTrackPointer(pgm_read_byte(song++));
  }
}

// Pause playback
void ATMSynth::pause() {
  TIMSK4 = 0; // Disable interrupt
}

// Start grinding samples
void ATMSynth::play() {
  TIMSK4 = 0b00000100; // Enable interrupt
}

// Stop playing, unload melody
void ATMSynth::stop() {
  pause();
  trackCount = 0; // Unload melody
}

// Sets tempo
void ATMSynth::tempo(uint16_t new_tempo) {
  tick_rate = new_tempo;
  cia = sample_rate / tick_rate; // not atomic?
}


void ATM_playroutine() {
  ch_t *ch;

  for (unsigned n = 0; n < 4; n++) {
    ch = &channel[n];
    if (ch->delay) {
      ch->delay--;
    } else {
      do {
        byte cmd = pgm_read_byte(ch->ptr++);
        if (cmd < 64) {
          // 0 … 63 : NOTE ON/OFF
          osc[n].freq = pgm_read_word(&noteTable[cmd]);
          ch->delay = 1;
        } else if (cmd < 160) {
          // 64 … 159 : SETUP FX
          switch (cmd - 64) {
            case 0: // Set volume
              osc[n].vol = pgm_read_byte(ch->ptr++);
              break;
          }
        } else if (cmd < 224) {
          // 160 … 223 : DELAY
          ch->delay = cmd - 159;
        } else if (cmd == 224) {
          // 224: LONG DELAY
          ch->delay = read_vle(&ch->ptr) + 65;
        } else if (cmd < 252) {
          // 225 … 251 : RESERVED
        } else if (cmd == 252 || cmd == 253) {
          // 252 (253) : CALL (REPEATEDLY)
          // Stack PUSH
          ch->stackCounter[ch->stackIndex] = ch->counter;
          ch->counter = cmd == 252 ? 1 : pgm_read_byte(ch->ptr++);
          ch->track = pgm_read_byte(ch->ptr++);
          ch->stackPointer[ch->stackIndex] = ch->ptr - trackBase;
          ch->stackIndex++;
          ch->ptr = getTrackPointer(ch->track);
        } else if (cmd == 254) {
          // 254 : RETURN
          if (ch->counter > 0) {
            // Repeat track
            ch->counter--;
            // Note to self: This is broken!
            // Loops last called track instead of playing track.
            // Possible solutions:
            //   Search for track based on address?
            //     Pro: No extra RAM Con: Slow
            //   Push "track" to stack
            //     Pro: Fast Con: Requires extra RAM
            ch->ptr = getTrackPointer(ch->track);
          } else {
            // Check stack depth
            if (ch->stackIndex == 0) {
              // End-Of-File
              ch->delay = 0xFFFF;
            } else {
              // Stack POP
              ch->stackIndex--;
              ch->ptr = ch->stackPointer[ch->stackIndex] + trackBase;
              ch->counter = ch->stackCounter[ch->stackIndex];
            }
          }
        } else if (cmd == 255) {
          // 255 : EMBEDDED DATA
          ch->ptr += read_vle(&ch->ptr);
        }
      } while (ch->delay == 0);
      ch->delay--;
    }
  }
}
