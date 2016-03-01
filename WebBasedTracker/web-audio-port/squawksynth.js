/*
 * Emulates the four Squawk oscillators
 * Exposes:
 *   setFrequency(oscillator, frequency)
 *   setVolume(oscillator, frequency)
 *   sample()
 *   setTick(numSamples)
 *   connect(playRoutine)
 */

function SquawkSynth() {
  
  // Oscillator class
  function Oscillator(waveshaper, freq) {
    this.freq   = freq === undefined ? 0 : freq;
    this.vol    = freq === undefined ? 64 : 0;
    this.phase  = 0.0;
    this.sample = waveshaper;
  }

  // Define our four oscillators
  var osc = new Array(
    new Oscillator(function() { // Pulse wave (25%) oscillator
      this.phase += this.freq / 65536.0;
      if(this.phase > 1.0) this.phase -= 1.0;
      return (this.phase < 0.25 ? +0.5 : -0.5) * this.vol / 256.0;
    }),
    new Oscillator(function() { // Square wave oscillator
      this.phase += this.freq / 65536.0;
      if(this.phase > 1.0) this.phase -= 1.0;
      return (this.phase < 0.5 ? +0.5 : -0.5) * this.vol / 256.0;
    }),
    new Oscillator(function() { // Triangle wave oscillator
      this.phase += this.freq / 65536.0;
      if(this.phase > 1.0) this.phase -= 1.0;
      return (this.phase < 0.5 ? (this.phase * 4.0) - 1.0 : 3.0 - (this.phase * 4.0)) * this.vol / 256.0;
    }),
    new Oscillator(function() { // Noise oscillator (LFSR)
      this.freq <<= 1;
      if((0 != (this.freq & 0x8000)) != (0 != (this.freq & 0x4000))) this.freq |= 1;
      return this.vol / (this.freq & 1 ? +256.0 : -256.0);
    }, 1)
  );
  var samplesPerTick = 160;
  var tickCount = 1;
  var source;

  // == INTERFACE == //
  
  // Set frequency (UWORD)
  function _setFrequency(oscillator, freq) {
    osc[oscillator].freq = freq;
  }
  
  // Set volume (UBYTE)
  function _setVolume(oscillator, vol) {
    osc[oscillator].vol = vol;
  }
  
  // Set tick time in samples (UWORD)
  function _setTick(sampleCount) {
    samplesPerTick = sampleCount;
  }
  
  // Connects to a playroutine (object providing .tick() and .setup(synth))
  function _connect(player) {
    source = player;
    source.setup(this);
  }
  
  // Grinds out a single sample (run all oscillators, mix, clip and output)
  function _sample() {
    // Run and mix all oscillators
    var mix = 0.0;
    for(var n = 0; n < osc.length; n++) {
      mix += osc[n].sample();
    }
    // CPU style overflow... well... close enough!
    while(Math.abs(mix) > 1.0) mix -= Math.sign(mix) * 2.0;
    // Tick counter
    if(--tickCount == 0) {
      source.tick(); // Call the playroutine
      tickCount = samplesPerTick;
    }
    // One sample, ready to ship!
    return mix;
  };
  
  // References
  this.setFrequency = _setFrequency;
  this.setVolume = _setVolume;
  this.setTick = _setTick;
  this.connect = _connect;
  this.sample = _sample;
}