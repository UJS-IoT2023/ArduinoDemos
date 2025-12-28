/**
 * DIY Steering Wheel Project - Incremental Encoder Logic
 * Hardware: Arduino Leonardo + MBL3806 Rotary Encoder (600 PPR)
 */

// Pin 3 (INT.0) and Pin 2 (INT.1) on Leonardo support hardware interrupts.
// These are connected to Phase A and Phase B of the encoder.
const int PIN_A = 3; 
const int PIN_B = 2;

// 'volatile' ensures the variable is correctly read from RAM even when 
// changed inside an interrupt. 'long' prevents overflow during many rotations.
volatile long count = 0;

void setup() {
  Serial.begin(115200);
  
  // NPN Open Collector output requires internal pull-up resistors 
  // to hold the signal at 5V when the encoder switch is open.
  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);

  /**
   * attachInterrupt(..., CHANGE) triggers the function whenever Phase A 
   * switches from HIGH to LOW or LOW to HIGH. 
   * Since there are 600 pulses (slots) per revolution:
   * 600 (pulses) * 2 (CHANGE edges) = 1200 counts per revolution.
   */
  attachInterrupt(digitalPinToInterrupt(PIN_A), handleEncoder, CHANGE);
}

void loop() {
  static long lastCount = 0;
  
  // Only print when the position changes to save Serial bandwidth
  if (count != lastCount) {
    // Relationship: 
    // 1 Full Turn (360 degrees) = 1200 counts (in 2x Resolution Mode)
    // Resolution = 360 / 1200 = 0.3 degrees per count.
    float angle = count * 0.3; 

    Serial.print("Count: ");
    Serial.print(count);
    Serial.print(" | Angle: ");
    Serial.println(angle);
    
    lastCount = count;
  }
}

/**
 * Interrupt Service Routine (ISR)
 * This function determines the direction using "Quadrature Encoding" logic:
 * * If Phase A changes and is NOW EQUAL to Phase B, it moved in one direction.
 * If Phase A changes and is NOW DIFFERENT from Phase B, it moved in the other.
 */
void handleEncoder() {
  // Reading the state of both phases to determine direction (CW vs CCW)
  if (digitalRead(PIN_A) == digitalRead(PIN_B)) {
    count++; // Clockwise or Counter-Clockwise depending on wiring
  } else {
    count--; 
  }
}