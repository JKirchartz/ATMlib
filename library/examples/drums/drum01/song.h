#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song music[] = {
  0x03,           // Number of tracks
  0x00, 0x00,     // Address of track 0
  0x03, 0x00,     // Address of track 1
  0x07, 0x00,     // Address of track 2
  0x00,           // Channel 0 entry track (PULSE)
  0x00,           // Channel 1 entry track (SQUARE)
  0x00,           // Channel 2 entry track (TRIANGLE)
  0x01,           // Channel 3 entry track (NOISE)

  //"Track 0"
  0x40, 0,        // FX: SET VOLUME: volume = 0
  0xFE,           // RETURN 
  
  //"Track 1"
  0xFD, 32, 2,    // REPEAT: count = 128 - track = 2
  0xFE,           // RETURN

  //"Track 2"
  0x40, 32,       // FX: SET VOLUME: volume = 32
  0x9F + 1,       // DELAY: ticks = 1
  0x40,  0,       // FX: SET VOLUME: volume = 0
  0x9F + 15,      // DELAY: ticks = 14
  
  0x40, 32,       // FX: SET VOLUME: volume = 32
  0x9F + 1,       // DELAY: ticks = 1
  0x40,  0,       // FX: SET VOLUME: volume = 0
  0x9F + 15,      // DELAY: ticks = 14

  0x40, 32,       // FX: SET VOLUME: volume = 32
  0x9F + 2,       // DELAY: ticks = 2
  0x40, 24,       // FX: SET VOLUME: volume = 24
  0x9F + 2,       // DELAY: ticks = 2
  0x40, 16,       // FX: SET VOLUME: volume = 16
  0x9F + 2,       // DELAY: ticks = 2
  0x40,  8,       // FX: SET VOLUME: volume =  8
  0x9F + 2,       // DELAY: ticks = 2
  0x40,  0,       // FX: SET VOLUME: volume =  0
  0x9F + 24,      // DELAY: ticks = 8
  
  0xFE,           // RETURN
};

#endif
