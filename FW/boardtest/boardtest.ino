// ATtiny861 outputs
const int ROW1 = 0; // PA0
const int ROW2 = 1; // PA1
const int ROW3 = 2; // PA2
const int ROW4 = 14; // PA3

const int EN_RGB1 = 10; // PA4
const int EN_RGB2 = 11; // PA5
const int EN_RGB3 = 12; // PA6
const int EN_RGB4 = 13; // PA7
const int R_PWM = 8; // PB1
const int G_PWM = 6; // PB3
const int B_PWM = 4; // PB5

const int SAO_SDA = 9; // PB0
const int SAO_SCL = 7; // PB2

const int SPI_MOSI = 9; // PB0
const int SPI_MISO = 8; // PB1
const int SPI_SCK = 7; // PB2

const int DM = 5; // PB4
const int DP = 4; // PB5

const int L_SWITCH = 5; // PB4
const int R_SWITCH = 3; // PB6

const int RESET = 15; // PB7

void setup() {
  pinMode(EN_RGB1, OUTPUT);
  pinMode(EN_RGB2, OUTPUT);
  pinMode(EN_RGB3, OUTPUT);
  pinMode(EN_RGB4, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(G_PWM, OUTPUT);
  pinMode(B_PWM, OUTPUT);
  digitalWrite(EN_RGB1, 0);
  digitalWrite(EN_RGB2, 0);
  digitalWrite(EN_RGB3, 0);
  digitalWrite(EN_RGB4, 0);
  analogWrite(R_PWM, 255);
  analogWrite(G_PWM, 255);
  analogWrite(B_PWM, 255);

}

void configRows(int highrow,int lowrow,int z1,int z2)
{
  pinMode(z1,INPUT);
  pinMode(z2,INPUT);
  pinMode(highrow,OUTPUT);
  digitalWrite(highrow,1);
  pinMode(lowrow,OUTPUT);
  digitalWrite(lowrow,0);
}

void setLED(int ledNum)
{
  switch(ledNum){
    case 1: configRows(ROW4,ROW1,ROW2,ROW3);  break;
    case 2: configRows(ROW1,ROW4,ROW2,ROW3);  break;
    case 3: configRows(ROW3,ROW1,ROW2,ROW4);  break;
    case 4: configRows(ROW1,ROW3,ROW2,ROW4);  break;
    case 5: configRows(ROW2,ROW1,ROW3,ROW4);  break;
    case 6: configRows(ROW1,ROW2,ROW3,ROW4);  break;
    case 7: configRows(ROW3,ROW2,ROW1,ROW4);  break;
    case 8: configRows(ROW2,ROW3,ROW1,ROW4);  break;
    case 9: configRows(ROW4,ROW3,ROW1,ROW2);  break;
    case 10: configRows(ROW3,ROW4,ROW1,ROW2);  break;
    case 11: configRows(ROW4,ROW2,ROW1,ROW3);  break;
    case 12: configRows(ROW2,ROW4,ROW1,ROW3);  break;
    default:
      pinMode(ROW1,INPUT);
      pinMode(ROW2,INPUT);
      pinMode(ROW3,INPUT);
      pinMode(ROW4,INPUT);
  }
}

void RGBCycle(int delayVal=25)
{
  analogWrite(R_PWM, 255);
  analogWrite(G_PWM, 255);
  analogWrite(B_PWM, 255);
  for (int i=0; i <= 255; i++) {
    analogWrite(G_PWM, 255-i);
    delay(delayVal);
  }
  for (int i=0; i <= 255; i++) {
    analogWrite(G_PWM, i);
    analogWrite(B_PWM, 255-i);
    delay(delayVal);
    }
  for (int i=0; i <= 255; i++) {
    analogWrite(B_PWM, i);
    analogWrite(R_PWM, 255-i);
    delay(delayVal);
    }
  for (int i=0; i <= 255; i++) {
    analogWrite(R_PWM, i);
    delay(delayVal);
    }
  analogWrite(R_PWM, 255);
  analogWrite(G_PWM, 255);
  analogWrite(B_PWM, 255);
}

void cyclePWM()
{
    digitalWrite(R_PWM, 0);
    delay(1000);
    digitalWrite(R_PWM, 1);
    digitalWrite(G_PWM, 0);
    delay(1000);
    digitalWrite(G_PWM, 1);
    digitalWrite(B_PWM, 0);
    delay(1000);
    digitalWrite(B_PWM, 1);
}

void loop() {
  digitalWrite(EN_RGB1, 1);
  cyclePWM();  
  digitalWrite(EN_RGB1, 0);
  digitalWrite(EN_RGB2, 1);
  cyclePWM();  
  digitalWrite(EN_RGB2, 0);
  digitalWrite(EN_RGB3, 1);
  cyclePWM();  
  digitalWrite(EN_RGB3, 0);
  digitalWrite(EN_RGB4, 1);
  cyclePWM();  
  digitalWrite(EN_RGB4, 0);
     
  for (int i=1; i<=13; i++){
      setLED(i);
      delay(1000);
  }
  digitalWrite(EN_RGB1, 1);
  RGBCycle(10);    
  digitalWrite(EN_RGB1, 0);
  digitalWrite(EN_RGB2, 1);
  RGBCycle(10);    
  digitalWrite(EN_RGB2, 0);
  digitalWrite(EN_RGB3, 1);
  RGBCycle(10);    
  digitalWrite(EN_RGB3, 0);
  digitalWrite(EN_RGB4, 1);
  RGBCycle(10);   
  digitalWrite(EN_RGB4, 0);
}
