const int EN_RGB1 = 10; // PA4
const int EN_RGB2 = 11; // PA5
const int EN_RGB3 = 12; // PA6
const int EN_RGB4 = 13; // PA7
const int R_PWM = 8; // PB1
const int G_PWM = 6; // PB3
const int B_PWM = 4; // PB5

byte red_pwm[10] = {26, 52, 78, 104, 130, 156, 182, 208, 234, 255};

void setup() {
  // put your setup code here, to run once:
  pinMode(EN_RGB1, OUTPUT);
  pinMode(EN_RGB2, OUTPUT);
  pinMode(EN_RGB3, OUTPUT);
  pinMode(EN_RGB4, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(G_PWM, OUTPUT);
  pinMode(B_PWM, OUTPUT);

  // Cleared on compare match, enable PWM on OC1A
  //TCCR1A = (1 << COM1A1) | (1 << PWM1A); 
  
  // Set presacle to CLK/ -> PWM Freq 3.926Khhz
  //TCCR1B = (1 << CS11) | (1 << CS10);
  OCR1A = 0;
  
  //digitalWrite(EN_RGB1, HIGH);
  digitalWrite(EN_RGB2, HIGH);
  //digitalWrite(B_PWM, HIGH);


}

void loop() {
  int i;

  for (i = 0; i < 255; i+=4) {
    OCR1A = i;
    delay(100);
  }
  OCR1A = 0;
}
