#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {
  0x06,                         // Number of tracks
  0x00, 0x00,                   // Address of track 0
  0x03, 0x00,                   // Address of track 1
  0x09, 0x00,                   // Address of track 2
  0x1E, 0x00,                   // Address of track 3
  0x22, 0x00,                   // Address of track 4
  0x29, 0x00,                   // Address of track 5
  0x01,                         // Channel 0 entry track (PULSE)
  0x00,                         // Channel 1 entry track (SQUARE)
  0x00,                         // Channel 2 entry track (TRIANGLE)
  0x03,                         // Channel 3 entry track (NOISE)

  //"Track 0"
  0x40, 0,                      // FX: SET VOLUME: volume = 0
  0xFE,                         // RETURN

  //"Track 1"
  0x40, 63,                     // FX: SET VOLUME: volume = 63
  0xFD, 127, 2,                 // REPEAT: count = 128 - track = 2
  0xFE,                         // RETURN

  // Track2() {
                                // {
  0x00 + 24,                    //   NOTE ON: note = 24 (delay 1 tick)
  0x9F + 23,                    //   DELAY: 23 ticks
  0x00 + 36,                    //   NOTE ON: note = 36 (delay 1 tick)
  0x47, 0x43, 0x00,             //   FX: ARPEGGIO ON: notes =  +4 +3 / don't play third note = OFF / ritrigger = OFF / ticks = 16
  0x9F +  7,                    //   DELAY: 7 ticks
  0x3F,                         //   NOTE OFF (delay 1 tick)
  0x48,                         //   ARP OFF
  0x9F + 15,                    //   DELAY: 15 ticks
                                // } ticks == 48, bytes = 10
                                // {
  0x00 + 22,                    //   NOTE ON: note = 22 (delay 1 tick)
  0x9F + 23,                    //   DELAY: 23 ticks
  0x00 + 34,                    //   NOTE ON: note = 34 (delay 1 tick)
  0x47, 0x43, 0x00,             //   FX: ARPEGGIO ON: notes =  +4 +3 / don't play third note = OFF / ritrigger = OFF / ticks = 16
  0x9F +  7,                    //   DELAY: 7 ticks
  0x3F,                         //   NOTE OFF (delay 1 tick)
  0x48,                         //   ARP OFF
  0x9F + 15,                    //   DELAY: 15 ticks
                                // } ticks == 48, bytes = 10
  0xFE,                         // RETURN

  // Track3() {
  0xFD, 127, 5,                 // REPEAT: count = 128 - track = 5
  0xFE,                         // RETURN
  // } ticks = 0, bytes = 6

  // Track4() {
  0x40, 48,                     // FX: SET VOLUME: volume = 48
  0x42, -16, 1,                 // FX: VOLSLIDE
  0x9F + 12,                    // DELAY
  0xFE,                         // RETURN
  // } ticks = 12, bytes = 7

  // Track5() {
  0xFD, 1, 4,                    // REPEAT: count = 6 - track = 4
  0x9F + 12,                     // DELAY
  0xFD, 1, 4,                    // REPEAT: count = 6 - track = 4
  0x9F + 12,                     // DELAY
  0x49, 4 + 0,                   // RETRIG: point = 1 (*4), speed = 0 (fastest)
  0x40, 48,                      // FX: SET VOLUME: volume = 63
  0x42, -8, 1,                   // FX: VOLSLIDE
  0x9F + 24,                     // DELAY
  0x4A,                          // RETRIG: off
  0xFE,                          // RETURN
  
  
  
  
  // } ticks = 12, bytes = 7

  

};

#endif
