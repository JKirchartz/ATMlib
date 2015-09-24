// Generate a ~1000Hz + ~3000Hz sawtooth on PD7 (pin 6) and inverse on PD6 (pin 12) using ISR+PWM

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void setup() {
  pinMode(6, OUTPUT);
  pinMode(12, OUTPUT);

  // Set Timer 4 divider to 1 (0001)
  cbi(TCCR4B, CS43);
  cbi(TCCR4B, CS42);
  cbi(TCCR4B, CS41);
  sbi(TCCR4B, CS40);

  // Set COM4D1..0 to enable both OC4D and _OC4D (01)
  cbi(TCCR4C, COM4D1);
  sbi(TCCR4C, COM4D0);
  
  // Set counter mode for fast PWM (00)
  cbi(TCCR4D, WGM41);
  cbi(TCCR4D, WGM40);

  // Enable Timer 4 overflow interrupt
  sbi(TIMSK4, TOIE4);
  
  // Clear Timer 4 overflow flag just to be sure
  sbi(TIFR4, TOV4);
}

void loop() {
}

// Timer 4 overflow interrupt
byte f1, f2;
byte divider;
ISR(TIMER4_OVF_vect) {
  // Sample rate 1:3 prescaler
  if(++divider < 3) return;
  divider = 0;

  // Quick non fractional sawtooths :: fPWM/3/(256/x)
  // Typically one would use 16-bit registers to provide 8-bits of fractional frequency
  f1 += 13; // x=1015.625hz
  f2 += 37; // x=2890.625hz
  
  // Mix and output using PWM
  OCR4D = (byte)(f1 >> 1) + (byte)(f2 >> 1);
}
