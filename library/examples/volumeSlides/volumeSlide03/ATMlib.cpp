/*
  Arduboy Tracker Music Library for Arduino Leonardo "Arduboy"

  2015
  TEAM a.r.g.
  Davey Taylor aka STG
  JO3RI
*/

#include "ATMlib.h"

SQUAWK_CONSTRUCT_ISR(OCR4A)

byte trackCount;
const word *trackList;
const byte *trackBase;
uint8_t pcm __attribute__((used)) = 128;
bool half __attribute__((used));


// Locals
static uint16_t tick_rate = 25;
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

struct ch_t {
  const byte *ptr;
  byte note;

  // Nesting
  word stackPointer[7];
  byte stackCounter[7];
  byte stackTrack[7]; // note 1
  byte stackIndex;
  
  // Looping
  word delay;
  byte counter;
  byte track;
  
  // External FX
  word freq;
  byte vol;
  bool mute;

  // Volume FX
  char volSlide;
  byte volConfig;
  byte volCount;

  // Frequency FX
  char freqSlide;
  byte freqConfig;
  byte freqCount;

  // Arpeggio FX
  byte arpNotes; // notes: base, base+[7:4], base+[7:4]+[3:0]
  byte arpTiming; // [4:0] = tick count, [5] = retrig, [6] = only two, [7] = reserved
  byte arpCount;
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
  channel[3].freq = 0x0001; // xFX

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

// Start grinding samples or Pause playback
void ATMSynth::playPause() {
  TIMSK4 = TIMSK4 ^ 0b00000100; // Disable interrupt
}

// Stop playing, unload melody
void ATMSynth::stop() {
  TIMSK4 = 0; // Disable interrupt
  trackCount = 0; // Unload melody
}

// Sets tempo
void ATMSynth::tempo(uint16_t new_tempo) {
  tick_rate = new_tempo;
  cia = sample_rate / tick_rate; // not atomic?
}

void mutebeforexfx(byte ch) {
  channel[ch].mute = true;
}

void unmuteafterfx(byte ch) {
  channel[ch].mute = false;
}

__attribute__((used))
void ATM_playroutine() {
  ch_t *ch;

  for (unsigned n = 0; n < 4; n++) {
    ch = &channel[n];
    if (ch->delay) {
      ch->delay--;
    } else {

      
      // Apply volume slides
      if(ch->volSlide) {
        if(!ch->volCount) {
          char v = ch->vol;
          v += ch->volSlide;
          if(!(ch->volConfig & 0x80)) {
            if(v < 0) v = 0;
            else if(v > 255) v = 0;
          }
          ch->vol = v;
        }
        if(ch->volCount++ > (ch->volConfig & 0x7F)) {
          ch->volCount = 0;
        }
      }
      
      // Apply frequency slides
      if(ch-> freqSlide) {
        if(!ch->freqCount) {
          uint16_t f = ch->freq;
          f += ch ->freqSlide;
          if(!(ch->freqConfig & 0x24B6)) {
            if(f < 262) f = 262;
            else if(f > 9397) f = 262;
          }
          ch->freq = f;
        }
        if(ch-> freqCount++ > (ch->freqConfig & 0x24B5)) {
          ch->freqCount = 0;
        }
      }
      
      // Apply Arpeggio
      if(ch->arpNotes) {
        if((ch->arpCount & 0x3F) > (ch->arpTiming & 0x3F)) {
          if(ch->arpCount & 0xC0 == 0x00) ch->arpCount = 0x40;
          else if(ch->arpCount & 0xC0 == 0x40) ch->arpCount = (ch->arpTiming & 0x40) ? 0x00 : 0x80;
          else if(ch->arpCount & 0xC0 == 0x80) ch->arpCount = 0x00;
          if(ch->arpCount & 0xC0 == 0x00)
            ch->freq = pgm_read_word(&noteTable[ch->note]);
          else if(ch->arpCount & 0xC0 == 0x40)
            ch->freq = pgm_read_word(&noteTable[ch->note + (ch->arpNotes >> 4)]);
          else if(ch->arpCount & 0xC0 == 0x80) 
            ch->freq = pgm_read_word(&noteTable[ch->note + (ch->arpNotes >> 4) + (ch->arpNotes & 0xF)]);
        } else ch->arpCount++;
      }
      
      do {
        byte cmd = pgm_read_byte(ch->ptr++);
        if (cmd < 64) {
          // 0 … 63 : NOTE ON/OFF
          ch->freq = pgm_read_word(&noteTable[ch->note = cmd]);
          if(ch->arpTiming & 32) ch->arpCount = 0;
          ch->delay = 1;
        } else if (cmd < 160) {
          // 64 … 159 : SETUP FX
          switch (cmd - 64) {
            case 0: // Set volume
              ch->vol = pgm_read_byte(ch->ptr++);
              break;
            case 1: // Slide volume ON
              ch->volSlide = pgm_read_byte(ch->ptr++);
              break;
            case 2: // Slide volume ON advanced
              ch->volSlide = pgm_read_byte(ch->ptr++);
              ch->volConfig = pgm_read_byte(ch->ptr++);
              break;
            case 3: // Slide volume OFF (same as 0x01 0x00)
              ch->volSlide = 0;
              break;
            case 4: // Slide frequency ON
              ch->freqSlide = pgm_read_word(ch->ptr++);
              break;
            case 5: // Slide frequency ON advanced
              ch->freqSlide = pgm_read_word(ch->ptr++);
              ch->freqConfig = pgm_read_word(ch->ptr++);
              break;
            case 6: // Slide frequency OFF
              ch->freqSlide = 0;
              break;
            case 7: // Set Arpeggio
              ch->arpNotes = pgm_read_word(ch->ptr++);
              ch->arpTiming = pgm_read_word(ch->ptr++);
              break;
            case 8: // Arpeggio off
              ch->arpNotes = 0;
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
          ch->stackTrack[ch->stackIndex] = ch->track; // note 1
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
            // Note to self: This is broken! (not anymore - note 1)
            // Loops last called track instead of playing track.
            // Possible solutions:
            //   Search for track based on address?
            //     Pro: No extra RAM Con: Slow
            //   Push "track" to stack (simplest - at least for now)
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
              ch->track = ch->stackTrack[ch->stackIndex]; // note 1
            }
          }
        } else if (cmd == 255) {
          // 255 : EMBEDDED DATA
          ch->ptr += read_vle(&ch->ptr);
        }
      } while (ch->delay == 0);

      ch->delay--;
    
    if(!ch->mute) {
      if(n != 3) osc[n].freq = ch->freq;
      osc[n].vol = ch->vol;
     }
    }
  }
}
