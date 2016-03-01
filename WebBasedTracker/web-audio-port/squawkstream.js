/*
 * Emulates the Squawk Stream player
 *   setSource(data)
 *   tick()
 *   getTickCount()
 */
// Note frequencies (scales with sample rate for now)
var noteTable = [
   262,  277,  294,  311,  330,  349,  370,  392,  415,  440,  466,  494,
   523,  554,  587,  622,  659,  698,  740,  784,  831,  880,  932,  988,
  1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
  2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951,
  4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902,
  8372, 8870, 9397,    0, 
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
    var ptr          = 0; // Pointer (into stream)
    var ctr          = 0; // Counter (for looping)
    var stackPointer = new Array();
    var stackCounter = new Array();
    var stackIndex   = 0;
    var delay        = 0;
    var track        = 0;
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
      if(delay > 0) {
        delay--;
      } else {
        do {
          var cmd = readByte();
          if(cmd < 64) {
            // 0 … 63 : NOTE ON/OFF
            synth.setFrequency(id, noteTable[cmd]);
            delay = 1;
          } else if(cmd < 160) {
            // 64 … 159 : SETUP FX
            var fx = cmd - 64;
            switch(fx) {
              case 0: // Set volume
                synth.setVolume(id, readByte());
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
            stackCounter[stackIndex] = ctr;
            ctr = cmd == 252 ? 1 : readByte();
            track = readByte();
            stackPointer[stackIndex] = ptr;
            stackIndex++;
            ptr = trackAddress(track);
          } else if(cmd == 254) {
            // 254 : RETURN
            if(ctr > 0) {
              // Repeat track
              ctr--;
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
                ctr = stackCounter[stackIndex];
              }
            }
          } else if(cmd == 255) {
            // 255 : EMBEDDED DATA
            ptr += readVLE();
          }
        } while(delay == 0);
        delay--;
      }
      // TODO: run effects here
      if(noise.checked) synth.setFrequency(3, 1);
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
    channel.forEach(function(e) { e.play(); });
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