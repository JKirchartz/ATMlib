/*
 * Linear interpolation SRC (sample rate converter)
 * Initializer:
 *   SampleRateConverter(fromSampleRate, toSampleRate)
 * Exposes:
 *   connect(sampleSource)
 *   sample()
 */
function SampleRateConverter(fromSampleRate, toSampleRate) {
  var delta = fromSampleRate / toSampleRate
  var index = 0.0;
  var samples = new Array();
  // == INTERFACE == //
  
  // Connects to a sample source (object providing .sample())
  function _connect(newSource) {
    source = newSource.sample;
  }
  
  // Generates one sample at [toSampleRate] hz
  function _sample() {
    index += delta;
    while(index > 1.0) {
      samples[0] = samples[1];
      samples[1] = source();
      index -= 1.0;
    }
    return samples[0] * (1.0 - index) + samples[1] * index;          
  };
    
  // References
  this.connect = _connect;
  this.sample = _sample;
}
