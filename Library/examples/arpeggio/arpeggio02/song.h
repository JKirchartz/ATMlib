#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {
  0x04,                         // Number of tracks
  0x00, 0x00,                   // Address of track 0
  0x03, 0x00,                   // Address of track 1
  0x09, 0x00,                   // Address of track 2
  0x19, 0x00,                   // Address of track 3
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
  0xFE,                         // RETURN

  //"Track 2"
  0x00 +  36,                   // NOTE ON: note = 36
  0x47, 0x43, 0x00 + 0x00 + 16, // FX: ARPEGGIO ON: notes =  +4 +3 / don't play third note = OFF / ritrigger = OFF / ticks = 16
  0xFD, 47, 3,                  // REPEAT: count = 47 - track = 3
  0x47, 0x34, 0x00 + 0x00 + 16, // FX: ARPEGGIO ON: notes =  +3 +4 / don't play third note = OFF / ritrigger = OFF / ticks = 16
  0xFD, 47, 3,                  // REPEAT: count = 47 - track = 3
  0x48,                         // FX: ARPEGGIO OFF
  0x3F,                         // NOTE OFF
  0xFE,                         // RETURN

  //"Track 3"
  0x9F +  1,                    // DELAY: ticks = 1
  0xFE,                         // RETURN
};

#endif
