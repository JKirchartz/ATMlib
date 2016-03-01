/*
 * Web Audio API streaming audio interface
 * Initializer:
 *   Throws error if Web Audio API is not available
 * Exposes:
 *   sampleRate
 *   connect(sampleSource)
 *   setVolume(volume)
 *   play(optional: !pause)
 *   pause(optional: !play)
 */
function WebAudioStream() {
  
  var bufferSize   = 1024; // Buffer size to use
  var preambleCount =  50; // Silent startup
  var globalVolume =  1.0;  // Default volume
  var source = function() { return 0; }
  var audioCtx, pcmNode;
  var paused = true;
  var lastSample = 0;
  
  // Let's do this by the book (copy-paste the chrome-devs code that is)
  try {
    window.AudioContext = window.AudioContext || window.webkitAudioContext;
    audioCtx = new AudioContext();
  } catch(e) {
    throw "Web Audio API is not available";
  }
  // Set up audio stream
  pcmNode = audioCtx.createScriptProcessor(bufferSize, 0, 1);
 
  // Fills out one full buffer of samples for the Web Audio API
  // ScriptProcessor Node (The WAASPN as we like toc all it).
  pcmNode.onaudioprocess = function(e) {
    var output = e.outputBuffer.getChannelData(0);
    if(preambleCount) {
      preambleCount--;
    } else {
      if(paused) {
        for(var i = 0; i < bufferSize; i++) {
          output[i] = lastSample;
        }
      } else {
        for(var i = 0; i < bufferSize; i++) {
          output[i] = source() * globalVolume;
        }
        lastSample = output[i - 1];
      }
    }
  }
  // Connect script processor directly to output
  pcmNode.connect(audioCtx.destination);
  // == INTERFACE == //
  // Retrieve system native samplerate
  function _getSampleRate() {
    return audioCtx.sampleRate;
  }
  
  // Connects to a sample source (object providing .sample())
  function _connect(sampleSource) {
    source = sampleSource.sample;
  }
  
  // Sets global volume
  function _setVolume(volume) {
    globalVolume = volume;
  }
  
  // Play (or pause)
  function _play(state) {
    paused = state == undefined ? false : !state;
  };
  
  // Pause (or play)
  function _pause(state) {
    paused = state == undefined ? true : state;
  };
  
  // References
  this.getSampleRate = _getSampleRate;
  this.connect = _connect;
  this.setVolume = _setVolume;
  this.play = _play;
  this.pause = _pause;
}
