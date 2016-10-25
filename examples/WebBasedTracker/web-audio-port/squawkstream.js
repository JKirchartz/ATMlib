/*
 * Emulates the Squawk Stream player
 *   setSource(data)
 *   tick()
 *   getTickCount()
 */

// Note frequencies (scales with sample rate for now)
var noteTable = [
     0,
   262,  277,  294,  311,  330,  349,  370,  392,  415,  440,  466,  494,
   523,  554,  587,  622,  659,  698,  740,  784,  831,  880,  932,  988,
  1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
  2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951,
  4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902,
  8372, 8870, 9397, 
];

function SquawkStream(sampleRate) {
  var synth;
  var data;
  var tickCount = 0;

  // Oscillator DMEP-type testing (Destroy My Ears, Plz)
  var testIx = 0.0;
  function DMEP() {
    testIx += 0.01;
    synth.setVolume(0, Math.floor(((Math.sin(testIx *  2) + 1.0) / 2.0) * 64));
    synth.setFrequency(0, Math.floor(((Math.sin(testIx *  3) + 1.0) / 2.0) * (8192 - 256)) + 256);
    synth.setVolume(1, Math.floor(((Math.sin(testIx *  5) + 1.0) / 2.0) * 64));
    synth.setFrequency(1, Math.floor(((Math.sin(testIx *  7) + 1.0) / 2.0) * (8192 - 256)) + 256);
    synth.setVolume(2, Math.floor(((Math.sin(testIx * 11) + 1.0) / 2.0) * 64));
    synth.setFrequency(2, Math.floor(((Math.sin(testIx * 13) + 1.0) / 2.0) * (8192 - 256)) + 256);
    synth.setVolume(3, Math.floor(((Math.sin(testIx * 17) + 1.0) / 2.0) * 64));
    if(noise.checked) synth.setFrequency(3, 1);
  }

  // Data access functions (big-endian)
  function readByte(address) { return data[address]; }
  function trackAddress(track) { return (data[(track << 1) + 1] << 8) | data[(track << 1) + 2]; }

  // Channel class - contains the core mysic data processing code
  function Channel(id) {
    var ptr          = 0;     // Pointer (into stream)
    var note        = 0;

    // Nesting
    var stackPointer = new Array();
    var stackCounter = new Array();
    var stackTrack = new Array(); // note 1
    var stackIndex   = 0;

    // Looping
    var delay        = 0;
    var counter      = 0;     // Counter (for looping)
    var track        = 0;

    // External FX
    //var freq         = 0;
    //var vol          = 0;
    //boolean mute     = 0;

    // Volume fx
    var volSlide     = 0;
    var volConfig    = 0;
    var volCount     = 0;

    // Frequency FX
    var freqSlide   = 0;
    var freqConfig  = 0;
    var freqCount   = 0;

    // Arpeggio FX
    var arpNotes    = 0;     // notes: base, base+[7:4], base+[7:4]+[3:0]
    var arpTiming   = 0;     // [7] = reserved, [6] = not third note ,[5] = retrigger, [4:0] = tick count
    var arpCount    = 0;

    // Retrig
    var reConfig    = 0;     // [7:2] = , [1:0] = speed
    var reCount     = 0;

    // transposition
    var tranConfig  = 0;

    //Tremolo or Vibrato
    var treviDepth  = 0;
    var treviConfig = 0;
    var treviCount  = 0;

    //Glissando
    var glisConfig  = 0;
    var glisCount   = 0;

    // == BIG-ENDIAN DATA ACCESS FUNCTIONS == //
    function readByte() { return typeof data == "function" ? data() : data[ptr++]; }
    function readWord() { return (readByte() << 8) | readByte(); }
    function readVLE(address) {
      var word = 0;
      do {
        word <<= 7;
        var d = readByte();
        word |= (d & 0x7F);
      } while(d & 0x80);
      return word & 0xFFFF;
    }
    // ====================================== //

    // == INTERFACE == //
      
    // Returns channel ID, used for iteration
    function _id() {
      return id;
    };

    // Force write pointer, used during track entry point setup
    function _jumpTo(address) {
      ptr = address;
    }

    // All hail "THE PLAYROUTINE"
    function _play() {

      // TODO: run effects here
      if(noise.checked) synth.setFrequency(3, 1);

      // Noise retriggering
      if (reConfig != 0) {
        if (reCount >= (reConfig & 0x03)) {
          synth.setFrequency(id, noteTable[reConfig >> 2]);
          reCount = 0;
        } else reCount++;
      }

      //Apply Glissando -> WORKING HURRAY :)
      if (glisConfig != 0) {
        if (glisCount >= (glisConfig & 0x7F))
        {
          if ((glisConfig & 0x80) != 0)note -= 1;
          else note += 1;
          if (note < 1) note = 1;
          else if (note > 63) note = 63;
          synth.setFrequency(id, noteTable[note]);
          glisCount = 0;
        }
        else glisCount++;
      }

      // Apply volume slides
      if (volSlide != 0) {
        if (volCount == 0) {
          var v = this.vol;
          //v += volSlide;
          //if ((volConfig & 0x80) != 0) {
          //  if (v < 0) v = 0;
          //  else if (v > 63) v = 63;
          //}
          v = v + 10;
          synth.setVolume(id,v);
        }
        if (volCount++ >= (volConfig & 0x7F)) volCount = 0;
      }

      // Apply frequency slides
      if ( freqSlide != 0) {
        //if (!freqCount) {
          //uint16_t f = freq;
          //f += ch ->freqSlide;
          //if (!(freqConfig & 0x80)) {
            //if (f < 0) f = 0;
            //else if (f > 9397) f = 9397;
          //}
          //freq = f;
        //}
        //if (freqCount++ >= (freqConfig & 0x7F)) freqCount = 0;
      }

      // Apply Arpeggio
      if ((arpNotes != 0)  && (note != 0)) {
        if ((arpCount & 0x1F) < (arpTiming & 0x1F)) arpCount++;
        else {
          if ((arpCount & 0xE0) == 0) arpCount = 32;
          else if (((arpCount & 0xE0) == 32) && ((arpTiming & 0x40) == 0)) arpCount = 64;
          else arpCount = 0;
          var arpNote = note;
          if ((arpCount & 0xE0) != 0) arpNote += (arpNotes >> 4);
          if ((arpCount & 0xE0) == 64) arpNote += (arpNotes & 15);
          synth.setFrequency(id, noteTable[arpNote + tranConfig]);
        }
      }

      // Apply Tremolo or Vibrato
      if (treviDepth != 0) {
        //Tremolo (0) or Vibrato (1) ?
        //if (!(treviConfig & 0x40)) {
          //char v = vol;
          //if (treviCount & 0x80) v += treviDepth & 0x1F;
          //else v -= treviDepth & 0x1F;
          //if (v < 0) v = 0;
          //else if (v > 63) v = 63;
          //vol = v;
        //}
        //else {
          //int16_t f = freq;
          //if (treviCount & 0x80) f += treviDepth & 0x1F;
          //else f -= treviDepth & 0x1F;
          //if (f < 0) f = 0;
          //else if (f > 9397) f = 9397;
          //freq = f;
        //}
        //if ((treviCount & 0x1F) < (treviConfig & 0x1F)) treviCount++;
        //else {
          //if (treviCount & 0x80) treviCount = 0;
          //else (treviCount) = 0x80;
        //}
      }



      if(delay != 0) delay--;
      else {
        do {
          var cmd = readByte();
          if(cmd < 64) {
            // 0 … 63 : NOTE ON/OFF
            if ((note = cmd) != 0) note += tranConfig;
            synth.setFrequency(id, noteTable[note]);
            if ((arpTiming & 0x20) != 0) arpCount = 0; // ARP retriggering
            delay = 1;
          } else if(cmd < 160) {
            // 64 … 159 : SETUP FX
            var fx = cmd - 64;
            switch(fx) {
              case 0: // Set volume
                synth.setVolume(id, readByte());
                break;
              case 1: // Slide volume ON
                volSlide = readByte();
                break;
              case 2: // Slide volume ON advanced
                volSlide = readByte();
                volConfig = readByte();
                break;
              case 3: // Slide volume OFF (same as 0x01 0x00)
                volSlide = 0;
                break;
              case 4: // Slide frequency ON
                freqSlide = readByte();
                break;
              case 5: // Slide frequency ON advanced
                freqSlide = readByte();
                freqConfig = readByte();
                break;
              case 6: // Slide frequency OFF
                freqSlide = 0;
                break;
              case 7: // Set Arpeggio
                arpNotes = readByte();    // 0x40 + 0x03
                arpTiming = readByte();   // 0x80 + 0x40 + 0x20 + amount
                break;
              case 8: // Arpeggio off
                arpNotes = 0;
                break;
              case 9: // Set Retriggering (noise)
                reConfig = readByte();    // RETRIG: point = 1 (*4), speed = 0 (0 = fastest, 1 = faster , 2 = fast)
                break;
              case 10: // Retriggering (noise) OFF
                reConfig = 0;
                break;
              case 11: // ADD Transposition
                tranConfig += readByte();
                break;
              case 12: // SET Transposition
                tranConfig = readByte();
                break;
              case 13: // Transposition OFF
                tranConfig = 0;
                break;
              case 14: // SET Tremolo or Vibrato
                treviDepth = readByte();
                treviConfig = readByte();
                break;
              case 15: // Tremolo or Vibrato  OFF
                treviDepth = 0;
                break;
              case 16: // Glissando
                glisConfig = readByte();
                break;
              case 17: // glissando OFF
                glisConfig = 0;
                break;
            }
          } else if(cmd < 224) {
            // 160 … 223 : DELAY
            delay = cmd - 159;
          } else if(cmd == 224) {
            // 224: LONG DELAY
            delay = readVLE() + 65;
          } else if(cmd < 252) {
            // 225 … 251 : RESERVED
          } else if(cmd == 252 || cmd == 253) {
            // 252 (253) : CALL (REPEATEDLY)
            // Stack PUSH
            stackCounter[stackIndex] = counter;
            stackTrack[stackIndex] = track;
            counter = cmd == 252 ? 0 : readByte();
            track = readByte();
            stackPointer[stackIndex] = ptr;
            stackIndex++;
            ptr = trackAddress(track);
          } else if(cmd == 254) {
            // 254 : RETURN
            if(counter > 0) {
              // Repeat track
              counter--;
              ptr = trackAddress(track);
            } else {
              // Check stack depth
              if(stackIndex == 0) {
                // End-Of-File
                delay = Infinity;
              } else {
                // Stack POP
                stackIndex--;
                ptr = stackPointer[stackIndex];
                counter = stackCounter[stackIndex];
                track = stackTrack[stackIndex]; // note 1
              }
            }
          } else if(cmd == 255) {
            // 255 : EMBEDDED DATA
            ptr += readVLE();
          }
        } while(delay == 0);
        // Apply volume slides
        delay--;

      }
    }
    
    // References
    this.id = _id;
    this.jumpTo = _jumpTo;
    this.play = _play;
  }

  // Define playback channels
  var channel = [
    new Channel(0),
    new Channel(1),
    new Channel(2),
    new Channel(3)
  ];

  // == INTERFACE == //
  
  // Called by synth when connecting graph
  // Provides reference to synthesizer
  function _setup(squawkSynth) {
    synth = squawkSynth;
    synth.setTick(sampleRate / 40); // Default to 40 ticks per second 
  };

  // Retrieve tick counter
  function _getTickCount() {
    return tickCount;
  }

  // Called by synthesizer each tick
  function _tick() {
    // Count ticks for no technical reason whatsoever
    tickCount++;
    // Run all channels
    if(expriette.checked) {
      synth.setVolume(0, 0);
      synth.setVolume(1, 0);
      synth.setVolume(2, 0);
      synth.setVolume(3, 63);
    } else {
      channel.forEach(function(e) { e.play(); });
    }
    // Run oscillator testing function (make insanity sounds)
    // DMEP();
  }

  // Set source of music data to either a callback function or an array
  function _setSource(source) {
    if(typeof source == "function") {
      // Source from callback function (useful for interfacing with tracker).
      // Jumping and looping must be handled by callback function.
      
      data = source;
    } else {
      // Source from array.
      // Mimics playback on embedded device.

      // Auto-fill algorithm (for track addresses)
      var temp = new Array();
      for(var n = 0, a = 0, b = 0; n < source.length; n++) {
        if(typeof source[n] == "string") {
          temp[(a << 1) + 1] = b >> 8;
          temp[(a << 1) + 2] = b & 0xFF;
          a++;
        } else {
          temp.push(source[n]);
          b++;
        }
      }
      
      // Copy to typed array
      data = new Uint8Array(new ArrayBuffer(b));
      for(var n = 0; n < b; n++) data[n] = temp[n];

      // Set up entry tracks for each channel
      var entryBase = (readByte(0) << 1) + 1;
      channel.forEach(function(e) { e.jumpTo(trackAddress(readByte(entryBase + e.id()))); });
    }
  }

  // References
  this.setup = _setup;
  this.getTickCount = _getTickCount;
  this.tick = _tick;
  this.setSource = _setSource;
}