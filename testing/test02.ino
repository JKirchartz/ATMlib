// Generate a 62.5 kHz PWM on PD7 (pin 6) and inverse on PD6 (pin 12) using PWM

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

  // Enable timer 4 overflow interrupt
  sbi(TIMSK4, TOIE4);
  
  // Clear interrupt just to be sure
  sbi(TIFR4, TOV4);
}

void loop() {
}


word f1, f2, f3, f4;

byte divider;
ISR(TIMER4_OVF_vect) {
  // Sample rate 1:3 prescaler
  if(++divider < 3) return;
  divider = 0;

  // Quick non fractional sawtooths
  f1 += 3328;
  f2 += 3338;
  f3 += 3348;
  f4 += 3358;
  
  // Mix and output
  OCR4D = (byte)(f1 >> 10)
        + (byte)(f2 >> 10)
        + (byte)(f3 >> 10)
        + (byte)(f4 >> 10);
}
