const int _xp=6,_xm=A1,_yp=A2,_ym=7; //240x400 ID=0x7793 - PINS

typedef struct {
  uint8_t x;
  uint16_t y;
  bool touched;
} Point;

// Modified from Adafruit TouchScreen.h
uint8_t getXP(void){
  int samples[2];
  
  pinMode(_yp, INPUT);
  pinMode(_ym, INPUT);
  pinMode(_xp, OUTPUT);
  pinMode(_xm, OUTPUT);
  digitalWrite(_xp, HIGH);
  digitalWrite(_xm, LOW);

  for (int i = 0; i < 2; i++) {
    samples[i] = analogRead(_yp);
  }
  
  // Allow small amount of measurement noise, because capacitive
  // coupling to a TFT display's signals can induce some noise.
  samples[1] = (samples[0] + samples[1]) >> 1; // average 2 samples

  return map((1023 - samples[1]), 153, 938, 0, WIDTH); // values from screen calibration
}
// Modified from Adafruit TouchScreen.h
bool touched(void) {
  // Set X+ to ground
  pinMode(_xp, OUTPUT);
  digitalWrite(_xp, LOW);

  // Set Y- to VCC
  pinMode(_ym, OUTPUT);
  digitalWrite(_ym, HIGH);

  // Hi-Z X- and Y+
  digitalWrite(_xm, LOW);
  pinMode(_xm, INPUT);
  digitalWrite(_yp, LOW);
  pinMode(_yp, INPUT);

  int z1 = analogRead(_xm);
  int z2 = analogRead(_yp);

  return (1023 - (z2 - z1) > 200) ? true : false;
}

Point getPoint(void) {
  Point point;
  point.x = getXP();
  point.touched = touched();
  pinMode(_yp, OUTPUT);      // restore shared pins
  pinMode(_xm, OUTPUT);      // program was hanging without this
  return point;
}
