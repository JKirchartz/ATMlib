#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {
  0x03,                         // Number of tracks
  0x00, 0x00,                   // Address of track 0
  0x03, 0x00,                   // Address of track 1
  0x0A, 0x00,                   // Address of track 2
  0x01,                         // Channel 0 entry track (PULSE)
  0x00,                         // Channel 1 entry track (SQUARE)
  0x00,                         // Channel 2 entry track (TRIANGLE)
  0x00,                         // Channel 3 entry track (NOISE)

  //"Track 0"
  0x40, 0,                      // FX: SET VOLUME: volume = 0
  0xFE,                         // RETURN

  //"Track 1"
  0x40, 63,                     // FX: SET VOLUME: volume = 63
  0xFD, 32, 2,                  // REPEAT: count = 32 - track = 2
  0x00,                         // NOTE OFF (delay 1 tick)
  0xFE,                         // RETURN

  //"Track2"
  0x00 + 25,                    // NOTE ON: note = 25
  0x4E,16,0x00 + 0x00 + 3,      // 
  0x9F + 32,                    // DELAY: 32 ticks
  0x4F,                         // TREMOLO or VIBRATO OFF
  0xFE,                         // RETURN
  
};

#endif
