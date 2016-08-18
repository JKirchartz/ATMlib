
#define Melody const uint8_t PROGMEM



#include <Arduino.h>
#include "Arglib.h"
#include "bitmaps.h"

Arduboy arduboy;
Sprites sprites(arduboy);

// oscillator structure
typedef struct {
  uint8_t  vol;
  uint16_t freq;
  uint16_t phase;
} osc_t;

typedef osc_t Oscillator;

// oscillator memory
osc_t osc[4];
uint8_t pcm;
extern void tick() asm("squawk_playroutine");
static uint16_t sample_rate;
uint16_t cia, cia_count;
bool half;
ISR(TIMER4_OVF_vect, ISR_NAKED) {
  asm volatile(
    "push r2                                          " "\n\t"
    "in   r2,                    __SREG__             " "\n\t"
    "push r18                                         " "\n\t"
    "lds  r18, half \n\t"
    "com  r18 \n\t"
    "sts  half, r18 \n\t"
    "breq continue \n\t"
    "pop  r18                                         " "\n\t"
    "out  __SREG__,              r2                   " "\n\t"
    "pop  r2                                          " "\n\t"
    "reti                                             " "\n\t"
    "continue: \n\t"
    "push r27                                         " "\n\t"
    "push r26                                         " "\n\t"
    "push r0                                          " "\n\t"
    "push r1                                          " "\n\t"

    "lds  r18,                   osc+2*%[mul]+%[fre]  " "\n\t"
    "lds  r0,                    osc+2*%[mul]+%[pha]  " "\n\t"
    "add  r0,                    r18                  " "\n\t"
    "sts  osc+2*%[mul]+%[pha],   r0                   " "\n\t"
    "lds  r18,                   osc+2*%[mul]+%[fre]+1" "\n\t"
    "lds  r1,                    osc+2*%[mul]+%[pha]+1" "\n\t"
    "adc  r1,                    r18                  " "\n\t"
    "sts  osc+2*%[mul]+%[pha]+1, r1                   " "\n\t"

    "mov  r27,                   r1                   " "\n\t"
    "sbrc r27,                   7                    " "\n\t"
    "com  r27                                         " "\n\t"
    "lsl  r27                                         " "\n\t"
    "lds  r26,                   osc+2*%[mul]+%[vol]  " "\n\t"
    "subi r27,                   128                  " "\n\t"
    "muls r27,                   r26                  " "\n\t"
    "lsl  r1                                          " "\n\t"
    "mov  r26,                   r1                   " "\n\t"

    "lds  r18,                   osc+0*%[mul]+%[fre]  " "\n\t"
    "lds  r0,                    osc+0*%[mul]+%[pha]  " "\n\t"
    "add  r0,                    r18                  " "\n\t"
    "sts  osc+0*%[mul]+%[pha],   r0                   " "\n\t"
    "lds  r18,                   osc+0*%[mul]+%[fre]+1" "\n\t"
    "lds  r1,                    osc+0*%[mul]+%[pha]+1" "\n\t"
    "adc  r1,                    r18                  " "\n\t"
    "sts  osc+0*%[mul]+%[pha]+1, r1                   " "\n\t"

    "mov  r18,                   r1                   " "\n\t"
    "lsl  r18                                         " "\n\t"
    "and  r18,                   r1                   " "\n\t"
    "lds  r27,                   osc+0*%[mul]+%[vol]  " "\n\t"
    "sbrc r18,                   7                    " "\n\t"
    "neg  r27                                         " "\n\t"
    "add  r26,                   r27                  " "\n\t"

    "lds  r18,                   osc+1*%[mul]+%[fre]  " "\n\t"
    "lds  r0,                    osc+1*%[mul]+%[pha]  " "\n\t"
    "add  r0,                    r18                  " "\n\t"
    "sts  osc+1*%[mul]+%[pha],   r0                   " "\n\t"
    "lds  r18,                   osc+1*%[mul]+%[fre]+1" "\n\t"
    "lds  r1,                    osc+1*%[mul]+%[pha]+1" "\n\t"
    "adc  r1,                    r18                  " "\n\t"
    "sts  osc+1*%[mul]+%[pha]+1, r1                   " "\n\t"

    "lds  r27,                   osc+1*%[mul]+%[vol]  " "\n\t"
    "sbrc r1,                    7                    " "\n\t"
    "neg  r27                                         " "\n\t"
    "add  r26,                   r27                  " "\n\t"

    "ldi  r27,                   1                    " "\n\t"
    "lds  r0,                    osc+3*%[mul]+%[fre]  " "\n\t"
    "lds  r1,                    osc+3*%[mul]+%[fre]+1" "\n\t"
    "add  r0,                    r0                   " "\n\t"
    "adc  r1,                    r1                   " "\n\t"
    "sbrc r1,                    7                    " "\n\t"
    "eor  r0,                    r27                  " "\n\t"
    "sbrc r1,                    6                    " "\n\t"
    "eor  r0,                    r27                  " "\n\t"
    "sts  osc+3*%[mul]+%[fre],   r0                   " "\n\t"
    "sts  osc+3*%[mul]+%[fre]+1, r1                   " "\n\t"

    "lds  r27,                   osc+3*%[mul]+%[vol]  " "\n\t"
    "sbrc r1,                    7                    " "\n\t"
    "neg  r27                                         " "\n\t"
    "add  r26,                   r27                  " "\n\t"

    "lds  r27,                   pcm                  " "\n\t"
    "add  r26,                   r27                  " "\n\t"
    "sts  %[reg],                 r26                  " "\n\t"

    "lds  r27,                   cia_count+1          " "\n\t"
    "lds  r26,                   cia_count            " "\n\t"
    "sbiw r26,                   1                    " "\n\t"
    "breq call_playroutine                            " "\n\t"
    "sts  cia_count+1,           r27                  " "\n\t"
    "sts  cia_count,             r26                  " "\n\t"
    "pop  r1                                          " "\n\t"
    "pop  r0                                          " "\n\t"
    "pop  r26                                         " "\n\t"
    "pop  r27                                         " "\n\t"
    "pop  r18                                         " "\n\t"
    "out  __SREG__,              r2                   " "\n\t"
    "pop  r2                                          " "\n\t"
    "reti                                             " "\n\t"
    "call_playroutine:                                " "\n\t"

    "lds  r27, cia+1                                  " "\n\t"
    "lds  r26, cia                                    " "\n\t"
    "sts  cia_count+1,           r27                  " "\n\t"
    "sts  cia_count,             r26                  " "\n\t"

    "sei                                              " "\n\t"
    "push r19                                         " "\n\t"
    "push r20                                         " "\n\t"
    "push r21                                         " "\n\t"
    "push r22                                         " "\n\t"
    "push r23                                         " "\n\t"
    "push r24                                         " "\n\t"
    "push r25                                         " "\n\t"
    "push r30                                         " "\n\t"
    "push r31                                         " "\n\t"

    "clr  r1                                          " "\n\t"
    "call squawk_playroutine                          " "\n\t"

    "pop  r31                                         " "\n\t"
    "pop  r30                                         " "\n\t"
    "pop  r25                                         " "\n\t"
    "pop  r24                                         " "\n\t"
    "pop  r23                                         " "\n\t"
    "pop  r22                                         " "\n\t"
    "pop  r21                                         " "\n\t"
    "pop  r20                                         " "\n\t"
    "pop  r19                                         " "\n\t"

    "pop  r1                                          " "\n\t"
    "pop  r0                                          " "\n\t"
    "pop  r26                                         " "\n\t"
    "pop  r27                                         " "\n\t"
    "pop  r18                                         " "\n\t"
    "out  __SREG__,              r2                   " "\n\t"
    "pop  r2                                          " "\n\t"
    "reti                                             " "\n\t"
    :
    : [reg] "M" _SFR_MEM_ADDR(OCR4A),
    [mul] "M" (sizeof(Oscillator)),
    [pha] "M" (offsetof(Oscillator, phase)),
    [fre] "M" (offsetof(Oscillator, freq)),
    [vol] "M" (offsetof(Oscillator, vol))
  );
}

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

byte trackCount;
const word *trackList;
const byte *trackBase;

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

void play(const byte *song) {
  // Read track count
  trackCount = pgm_read_byte(song++);
  // Store track list pointer
  trackList = (word*)song;
  // Store track pointer
  trackBase = (song += (trackCount << 1)) + 4;
  // Fetch starting points for each track
  for (unsigned n = 0; n < 4; n++) {

    channel[n].ptr = getTrackPointer(pgm_read_byte(song++));
    Serial.print(n, HEX);
    Serial.print(" ");
    Serial.println(channel[n].ptr - trackBase, HEX);
  }
}

void tick() {
  ch_t *ch;

  Serial.println("");
  for (unsigned n = 0; n < 4; n++) {
    ch = &channel[n];
    Serial.println("");
    Serial.print(n);
    Serial.print("");
    Serial.print(ch->ptr - trackBase, HEX);
    if (ch->delay) {
      ch->delay--;
    } else {
      do {
        byte cmd = pgm_read_byte(ch->ptr++);
        Serial.println("");
        Serial.print(cmd, HEX);
        if (cmd < 64) {
          // 0 … 63 : NOTE ON/OFF
          osc[n].freq = pgm_read_word(&noteTable[cmd]);
          ch->delay = 1;
        } else if (cmd < 160) {
          // 64 … 159 : SETUP FX
          switch (cmd - 64) {
            case 0: // Set volume
              osc[n].vol = pgm_read_byte(ch->ptr++);
              Serial.print(" ");
              Serial.print(osc[n].vol, HEX);
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
          Serial.print(" ");
          Serial.print(ch->counter, HEX);
          ch->track = pgm_read_byte(ch->ptr++);
          Serial.print(" ");
          Serial.print(ch->track, HEX);
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

extern Melody TheOriginalSquawk[]; // Squawk 1 melody
extern Melody music[];             // Squawk 2 melody

void loop() {
  char q = Serial.read();
  if (q == 'p') play(music);
  if (q == 'x') tick();
  delay(1);
  if (!(arduboy.nextFrame())) return;
  arduboy.clearDisplay();
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(32 * i, 10, TEAMarg, i);
  sprites.drawSelfMasked(43, 50, TEAM_argPart5, 0);
  arduboy.display();
}

void setup() {
  pinMode(5, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);

  /* BEGIN */
  TCCR4A = 0b01000010;
  TCCR4B = 0b00000001;
  OCR4C  = 0xFF;
  OCR4A  = 0x80;
  TIMSK4 = 0b00000100;

  sample_rate = 15625;
  cia = sample_rate / 25; // 25 == tempo
  osc[3].freq = 0x0001; // Seed LFSR
  /* END-BEGIN */

  // Begin playback of melody.
  play(music); // init squawk 2 playroutine
  arduboy.start();
  arduboy.setFrameRate(60);                                 // set the frame rate of the game at 60 fps

}

Melody music[] = {   // A) Sample music =>
  0x07,       // Number of tracks
  0x00, 0x00, // Address of track 0
  0x01, 0x00, // Address of track 1
  0x0A, 0x00, // Address of track 2
  0x0E, 0x00, // Address of track 3
  0x1F, 0x00, // Address of track 4
  0x25, 0x00, // Address of track 5
  0x36, 0x00, // Address of track 6
  0x02,       // Channel 0 entry track (PULSE)
  0x04,       // Channel 1 entry track (SQUARE)
  0x00,       // Channel 2 entry track (TRIANGLE)
  0x06,       // Channel 3 entry track (NOISE)

  //"Track 0",
  0xFE,          // RETURN (empty track used for silent channels)

  //"Track 1",
  0x00 + 20,     // NOTE ON: note = 20
  0x9F +  5,     // DELAY: ticks = 5
  0x3F,          // NOTE OFF
  0x9F +  5,     // DELAY: ticks = 5
  0x00 + 21,     // NOTE ON: note = 21
  0x9F +  5,     // DELAY: ticks = 5
  0x3F,          // NOTE OFF
  0x9F +  5,     // DELAY: ticks = 5
  0xFE,          // RETURN

  //"Track 2",
  0xFD, 31, 1,   // REPEAT: track = 1, count = 32
  0xFE,          // RETURN

  //"Track 3",
  0x00 +  8,     // NOTE ON: note = 20
  0x9F +  3,     // DELAY: ticks = 3
  0x3F,          // NOTE OFF
  0x9F +  1,     // DELAY: ticks = 1
  0x00 +  8,     // NOTE ON: note = 20
  0x9F +  3,     // DELAY: ticks = 3
  0x3F,          // NOTE OFF
  0x9F +  1,     // DELAY: ticks = 1
  0x00 +  9,     // NOTE ON: note = 21
  0x9F +  3,     // DELAY: ticks = 3
  0x3F,          // NOTE OFF
  0x9F +  1,     // DELAY: ticks = 1
  0x00 +  9,     // NOTE ON: note = 21
  0x9F +  3,     // DELAY: ticks = 3
  0x3F,          // NOTE OFF
  0x9F +  1,     // DELAY: ticks = 1
  0xFE,          // RETURN

  //"Track 4",
  0x40, 48,      // FX: SET VOLUME: volume = 48
  0xFD, 31, 3,   // REPEAT: track = 3, count = 32
  0xFE,          // RETURN

  //"Track 5",
  0xE0, 15,      // LONG DELAY: ticks = 80
  0x40, 63,      // FX: SET VOLUME: volume = 63
  0x9F + 4,      // DELAY: ticks = 4
  0x40, 47,      // FX: SET VOLUME: volume = 47
  0x9F + 4,      // DELAY: ticks = 4
  0x40, 31,      // FX: SET VOLUME: volume = 31
  0x9F + 4,      // DELAY: ticks = 4
  0x40, 15,      // FX: SET VOLUME: volume = 15
  0x9F + 4,      // DELAY: ticks = 4
  0x40,  0,      // FX: SET VOLUME: volume = 0
  0xFE,          // RETURN

  //"Track 6",
  0xFD, 7, 5,    // REPEAT: track = 5, count = 8
  0xFE          // RETURN
};

