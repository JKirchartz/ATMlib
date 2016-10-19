#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {
  0x03,                         // Number of tracks
  0x00, 0x00,                   // Address of track 0
  0x03, 0x00,                   // Address of track 1
  0x0F, 0x00,                   // Address of track 2
  0x01,                         // Channel 0 entry track (PULSE)
  0x00,                         // Channel 1 entry track (SQUARE)
  0x00,                         // Channel 2 entry track (TRIANGLE)
  0x00,                         // Channel 3 entry track (NOISE)

  //"Track 0"
  0x40, 0,                      // FX: SET VOLUME: volume = 0
  0xFE,                         // RETURN (empty track used for silent channels)

  //"Track 1"
  0x40, 63,                     // FX: SET VOLUME: volume = 63
  0x00 +  36,                   // NOTE ON: note = 36
  0x47, 0x43, 0x00 + 0x20 + 4,  // FX: ARPEGGIO ON: notes =  +4 +3 - only play Second note = ON - ritrigger = ON - ticks = 4
  0xFD, 23, 2,                  // REPEAT: count = 12 - track = 2
  0x3F,                         // NOTE OFF
  0x9F +  16,                   // DELAY: ticks = 16
  0xFE,                         // RETURN

  //"Track 2"
  0x9F +  1,                    // DELAY: ticks = 1
  0xFE,                         // RETURN
};

#endif
