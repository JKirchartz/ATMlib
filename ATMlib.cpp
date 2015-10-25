/*
Arduboy Tracker Music Library for Arduino Leonardo "Arduboy"

2015
TEAM a.r.g.
Davey Taylor 
*/

/*
Track table
  [Count] UBYTE
  [Address track 0] UWORD
        ...
  [Address track N] UWORD
Channel entry tracks
  [Channel 0 track] UBYTE
  [Channel 1 track] UBYTE
  [Channel 2 track] UBYTE
  [Channel 3 track] UBYTE
Track 0
  [Command]
    ([Params])
  [Command]
    ([Params])
        ...
Track 1
        ...
Track N
*/

/*
COMMAND list
    0       = stop playing note
    1 … 127 = start playing note, NOTE = note_tab[COMMAND - 1];
  128 … 159 = effect
    [params] UNK    = effect specific parameters, format is effect specific
  160 … 223 = delay, TICKS = COMMAND - 159; // see below
  224       = delay
    [ticks] VLE     = number of ticks to delay until next event (add 64)
  225 … 251 = RESERVED
  252       = call
    [track index]
  253       = loop call
    [loop count]
    [track index]
  254       = ret
  255       = data chunk
    [length] VLE    = length in bytes of data to follow
    [data] UNK      = binary data chunk
*/

#include "ATMlib.h"




const word noteTable[84] PROGMEM = {
  3424, 3232, 3048, 2880, 2712, 2560, 2416, 2280, 2152, 2032, 1920, 1814,
  1712, 1616, 1524, 1440, 1356, 1280, 1208, 1140, 1076, 1016,  960,  907,
   856,  808,  762,  720,  678,  640,  604,  570,  538,  508,  480,  453,
   428,  404,  381,  360,  339,  320,  302,  285,  269,  254,  240,  226,
   214,  202,  190,  180,  170,  160,  151,  143,  135,  127,  120,  113,
   107,  101,   95,   90,   85,   80,   75,   71,   67,   63,   60,   56,
    53,   50,   47,   45,   42,   40,   37,   35,   33,   31,   30,   28,
};

typdef struct ch_t {
  uint8_t *ptr;
  word stackPointer[7];
  byte stackCounter[7];
  
  byte stackIndex;
  word delay;
  byte counter;
  byte track;
};
ch_t channel[4]; 

byte trackCount;
word *trackList;
byte *trackBase;


uint16_t read_vbru(const void **pp) {
  // TODO: read variable length field, update pointer
}

int16_t read_vbrs(const void **pp) {
  // TODO: read variable length field, update pointer
}

static inline byte *getTrackPointer(byte track) {
  return trackBase + pgm_read_word(&trackList[track]);
}

void play(const byte *song) {
  // Read track count
  trackCount = pgm_read_byte(song++);
  // Store track list pointer
  trackList = song++;
  // Store track pointer
  trackBase = song += (trackCount << 1);
  // Fetch starting points for each track
  for(unsigned n = 0; n < 4; n++) {
    channel[n].ptr = getTrackPointer(pgm_read_byte(song++));
  }
}

void tick() {
  ch_t *ch;
  
  for(n = 0; n < 4; n++) {
    ch = &channel[n];
    if(ch->delay) {
      ch->delay--;
    } else {
      cmd = *(ch->ptr++);
      if(cmd & 0x80) {
        if(cmd < 160) {
          // Effect set up commands
          switch(cmd - 128) {
            case 0: // volume
              osc[n].volume = *(ch->ptr++);
              break;
            // TODO: effects 0 to 31
          }
        } else if(cmd < 224) {
          ch->delay = cmd - 159;
        } else if(cmd == 224) { // DELAY
          ch->delay = read_vbru(&ch->ptr) + 65;
        } else if(cmd == 252 || cmd == 253) { // LOOP (CALL)
          // Push return data to stack
          ch->stackPointer[ch->stackIndex] = ch->ptr - trackBase;
          ch->stackCounter[ch->stackIndex++] = ch->counter;
          ch->counter = cmd == 253 ? *ch->ptr++ : 0;
          // Point to new track
          ch->track = *ch->ptr;
          ch->ptr = getTrackPointer(ch->track);
        } else if(cmd == 254) { // RETURN
          if(ch->counter) {
            // Repeat track
            ch->ptr = getTrackPointer(ch->track);
            ch->counter--;
          } else {
            // TODO: What if stackIndex is 0? Stop? Once final ch stop - restart?
            // Pop return data from stack
            ch->ptr = ch->stackPointer[--ch->stackIndex] + trackBase;
            ch->counter = ch->stackCounter[ch->stackIndex];
          }
        } else if(cmd == 255) {
          // Send embedded data to callback
          word length = read_vbru(&ch->ptr);
          customData(length, ch->ptr);
          ch->ptr += length;
        }
      } else {
        if(cmd == 127) {
          osc[n].freq = 0;
        } else {
          // Start note
          osc[n].freq = pgm_read_word(&noteTable[cmd]);
        }
      }      
    }
  }
}
