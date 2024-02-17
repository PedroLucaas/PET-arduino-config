#include <thermistor.h>
#include <PID_v1.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define colorblau 0x1b1b
#define colorverd 0x4747
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

#define DRV8825_M2 A0 // MOTOR SPEED PIN
#define PIN_ROTURA A7 // LIMIT SWITCH PIN
#define SelectorSW 4 // MODE PIN
#define SelectorDT 3 // ENCODER PIN
#define SelectorCLK 2 // CLK PIN

//INPUTS
#define HAB_CAL_PIN 8  // OUTPUT HEATING PIN
#define PIN_TERMISTOR A6 // NTC 3950 100K Pullut 4K7 TEMPERATURE PIN
#define PIN_HAB_MOTOR 7 // OUTPUT SPEED MOTOR PIN

//OUTPUTS
#define PIN_CALEFACCIO 9 // PWM PIN
#define PIN_LED_CAL 10 // HEATING LED PIN
#define PIN_MARXA_MOTOR 6 // ENABLE SPEED MOTOR PIN
#define PIN_VELOCITAT 5 // OUTPUT SPEED PIN

// BITMAP
const unsigned char IconaTemperatura [] PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x40, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x18, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
   0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x60, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x10,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x20, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x17, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x20, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20,
   0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x17, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x20, 0x17, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x17, 0x00, 0x00,
   0x00, 0x00, 0x00, 0xb0, 0x37, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x6f,
   0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x4f, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xe8, 0x5f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe8, 0x5f, 0x00, 0x00, 0x00,
   0x00, 0x00, 0xe8, 0x5f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc8, 0x6f, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x90, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
   0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00 };

const uint8_t IconaVelocitat [] PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0xf8, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc,
   0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xc0, 0x03, 0x00, 0x00, 0x00,
   0x80, 0x03, 0x00, 0x07, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x00, 0x0e, 0x00,
   0x00, 0x00, 0xc0, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00,
   0x18, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x30,
   0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x30, 0x00, 0x00,
   0x00, 0x30, 0x00, 0x20, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x30, 0x70,
   0x00, 0x00, 0x00, 0x10, 0x00, 0x10, 0x60, 0x00, 0x00, 0x00, 0x10, 0x00,
   0x18, 0x60, 0x00, 0x00, 0x00, 0x30, 0x00, 0x1c, 0x70, 0x00, 0x00, 0x00,
   0x30, 0x00, 0x0e, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x0f, 0x30, 0x00,
   0x00, 0x00, 0x30, 0x80, 0x0f, 0x30, 0x00, 0x00, 0x00, 0x70, 0x80, 0x04,
   0x38, 0x00, 0x00, 0x00, 0x60, 0x80, 0x07, 0x18, 0x00, 0x00, 0x00, 0xc0,
   0x00, 0x03, 0x0c, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x0c, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00 };

const uint8_t Fulles [] PROGMEM = {
   0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0xd0, 0x00, 0x00, 0xd8, 0x01, 0x00,
   0xb8, 0x03, 0x00, 0xb8, 0x07, 0x00, 0xbc, 0x0f, 0x00, 0xbc, 0x1f, 0x00,
   0xbc, 0x1f, 0x00, 0x7e, 0x3f, 0x00, 0x7e, 0x3f, 0x38, 0x7e, 0x3f, 0x5e,
   0x7e, 0x3f, 0x6f, 0x7e, 0xbf, 0x77, 0xfe, 0xde, 0x7b, 0xfc, 0xee, 0x7d,
   0xfc, 0xf6, 0x7d, 0xf8, 0xf6, 0x3e, 0xf0, 0x7a, 0x3f, 0xe0, 0xbd, 0x1f,
   0xc0, 0xdd, 0x1f, 0x80, 0xed, 0x0f, 0x00, 0xed, 0x07, 0x00, 0xf4, 0x01 };

// VARIABLES
bool PolsadorMotor=0,PolsadorSelector=0,dif3=0,dif4=0,MarxaMotor=0,MarxaCalefaccio=0,AlarmaRotura=0;
double ValorAnterior=0,Constant=15,ValorCalculat=0;
int status;
int CorreccioTemperatura=3,RoturaCinta=0;
int VelocitatMotor=5000,Mode=0;
double Temperatura,SortidaCalefaccio=0,ConsignaCalefaccio=220;
char Temp [30], Consigna[30],Vel[30];
const int Retras=100, RetrasDisplay=1000;
long Temps=0, TempsDisplay=0;
float S, R;
long TempsPolsadorPremut = 0;
int Divisor = 1;
int ValorAnalogic;   
int TempTermistor;
int currentStateCLK;
int lastStateCLK;
uint32_t start, stop;
int buzzerstate = 0;
long previousmillis = 0;
long intervalon = 500;
long intervaloff = 1500;
int c = 0;

// PID
thermistor therm1(PIN_TERMISTOR,0); // llibreria termistor
#define TFT_CS     A4         // TFT CS pin is connected to arduino pin A4
#define TFT_RST    -1         // TFT RST pin is connected to arduino pin A3
#define TFT_DC     A2        // TFT DC pin is connected to arduino pin A2
double agrKp=9,agrKi=0.0,agrKd=0;
double conKp=8,conKi=0.2,conKd=0;
PID pid(&ValorCalculat,&SortidaCalefaccio,&ConsignaCalefaccio, conKp, conKi, conKd,DIRECT);

// TFT
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


void setup() {
pid.SetMode(AUTOMATIC);
pinMode(A3, OUTPUT); // ALARM
  Serial.begin(9600);
  Serial.println(__FILE__);
  Serial.println();

  Temperatura = therm1.analog2temp();
  ValorCalculat=Temperatura;
  ValorAnterior=Temperatura;

  attachInterrupt(0, LecturaSelector, CHANGE);

// PIN CONFIGURATION
  pinMode(PIN_CALEFACCIO,OUTPUT);
  pinMode(HAB_CAL_PIN,INPUT);
  pinMode(PIN_LED_CAL,OUTPUT);  
  pinMode(PIN_MARXA_MOTOR,OUTPUT);
  pinMode(PIN_VELOCITAT,OUTPUT);
  pinMode(PIN_HAB_MOTOR,INPUT);
  pinMode(DRV8825_M2,OUTPUT);
  pinMode(PIN_ROTURA,INPUT);
  pinMode(SelectorCLK,INPUT);
  pinMode(SelectorDT,INPUT);
  pinMode(SelectorSW,INPUT);

  lastStateCLK = digitalRead(SelectorCLK);  // Read the initial state of CLK
  setPWMPrescaler(PIN_CALEFACCIO, 1024);

// DISPLAY LOGO
 tft.initR(INITR_BLACKTAB);      
 tft.setRotation (4);
 tft.fillScreen(ST7735_BLACK); 
 delay(2500); 
 tft.fillScreen(ST7735_BLACK);
 tft.setTextColor(WHITE,BLACK);
 tft.setCursor(10, 30);
 tft.setTextSize(2.5);
 tft.setCursor(18, 80);
 tft.print("ECODECAT");
 tft.drawXBitmap( 35, 50, Fulles,24,24, colorverd);
 delay(2000);
 tft.fillScreen(ST7735_BLACK);
}

void loop() {
  Serial.print("\t Temperatura : ");
  Serial.print(ValorCalculat);
  Serial.print("\t Consigna : ");
  Serial.print(ConsignaCalefaccio);
  Serial.print("\t Sortida : ");
  Serial.println(SortidaCalefaccio);

  SeleccioEntrada();
  LecturaSelector();
  LecturaTemperatura();
  ControlCalefaccio();
  ControlMotor();
  ControlRotura();
  Main_tft();
}

// CLK
void LecturaSelector()
{
  currentStateCLK = digitalRead(SelectorCLK);  // Read the current state of CLK
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {
    if (digitalRead(SelectorDT) != currentStateCLK) {
        switch(Mode)// Mode=1 modificacio temperatura Mode=2 Modificacio Velocitat
        {
          case 1:
            ConsignaCalefaccio++;
            if (ConsignaCalefaccio > 300) {ConsignaCalefaccio=300;}
          break;
          case 2:
            VelocitatMotor=VelocitatMotor+100;
            if (VelocitatMotor > 10000) {VelocitatMotor=10000;}
          break;
          case 3:
            Divisor++;
            if (Divisor>2){Divisor=2;}
          break;
        }
    } else {
        switch(Mode)// Mode=1 Temperature Mode=2 Speed
        {
          case 1:
            ConsignaCalefaccio--;
            if (ConsignaCalefaccio <= 0) {ConsignaCalefaccio=0;}
          break;
          case 2:
            VelocitatMotor=VelocitatMotor-100;
            if (VelocitatMotor <= 0) {VelocitatMotor=0;}
          break;
          case 3:
            Divisor--;
            if (Divisor<1){Divisor=1;}
          break;
        }      
    }
  }
  lastStateCLK = currentStateCLK;
}

// TFT 
void Main_tft ()
  {
  //tft.drawXBitmap( 10, 20, IconaTemperatura,50,50, WHITE);
  tft.setTextSize(2);
  tft.setCursor (65,27);
  tft.setTextColor(WHITE,BLACK);
  sprintf(Temp,"%d",TempTermistor);
  if (TempTermistor<100) {tft.print(" ");tft.print(Temp);tft.println((char)247);}else{tft.print(Temp);tft.println((char)247);}
  if (Mode==1) {tft.setTextColor(colorverd,BLACK);}else {tft.setTextColor(WHITE,BLACK);}
  tft.setCursor (65,53);
  sprintf(Consigna,"%d",int(ConsignaCalefaccio));
  if (ConsignaCalefaccio<100) {tft.print(" ");tft.print(Consigna);tft.println((char)247);}else{tft.print(Consigna);tft.println((char)247);}

  //tft.drawXBitmap( 10, 100, IconaVelocitat,50,50, WHITE);
  if (Mode==2) {tft.setTextColor(colorverd,BLACK);}else {tft.setTextColor(WHITE,BLACK);}
  sprintf(Vel,"%d",(VelocitatMotor/100));
  tft.setCursor (60,120);
  if (VelocitatMotor/100 <100) {tft.print(" ");tft.print(Vel);}else{tft.print(Vel);}
  tft.print("%  ");
  if (Mode==3) 
  { 
    tft.setTextColor(WHITE,BLACK);
    tft.print(" ");
    tft.print(Divisor);
  }else{
    tft.print("   ");   
  }

  tft.setCursor (30,80);
  tft.setTextColor(RED,BLACK);
  if (AlarmaRotura)
    {
    tft.print("!");
    unsigned long currentmillis = millis();
    if (buzzerstate == 0) 
      {
      if (currentmillis - previousmillis > intervaloff) 
        {
        previousmillis = currentmillis;
        buzzerstate = 1023;
        c = c + 1;
        if (c < 2) 
          {
          analogWrite(A3, buzzerstate);
          }
        else 
          {
          tft.print("   ");
          analogWrite(A3, 0);
          buzzerstate = 0;
          AlarmaRotura = 0;
          c = 0;
          }
        }
      }
    else 
      {
      if (currentmillis - previousmillis > intervalon) 
        {  
      previousmillis = currentmillis;
      buzzerstate = 0;
      c = c;
      if (c < 2) 
          {
          analogWrite(A3, buzzerstate);
          }
        else 
          {
          tft.print("   ");
          analogWrite(A3, 0);
          buzzerstate = 0;
          AlarmaRotura = 0;
          c = 0;
          }
        }
      }
    
    }
  else
    {
    tft.print("   ");
    analogWrite(A3, 0);
    buzzerstate = 0;    
    }
}

// TEMPERATURE READING
void LecturaTemperatura()
  {
     if (millis() > Temps + Retras)
     {
        ValorAnalogic=analogRead(PIN_TERMISTOR);
        Temperatura = therm1.analog2temp();
        ValorCalculat=(ValorAnterior*((Constant - 1)/Constant)+(Temperatura/Constant));
        ValorAnterior=ValorCalculat;
        TempTermistor = ValorCalculat;
        Temps = millis();
     }
  }

 // TEMPERATURE CONTROL
 void ControlCalefaccio ()
  {
      double gap = abs(ConsignaCalefaccio-Temperatura); //distance away from setpoint
  if (gap < 40)
  {  //we're close to setpoint, use conservative tuning parameters
    pid.SetTunings(conKp, conKi, conKd);
  }
  else
  {
     //we're far from setpoint, use aggressive tuning parameters
     pid.SetTunings(agrKp, agrKi, agrKd);
  }
        // Marxa calefaccio per polsador
    if (digitalRead(HAB_CAL_PIN)& dif3==0)
      {
        MarxaCalefaccio=MarxaCalefaccio^1;
      }
    dif3=digitalRead(HAB_CAL_PIN);
      
  if (MarxaCalefaccio==1)
    {
       pid.Compute();
       if (SortidaCalefaccio<0) {SortidaCalefaccio=0;}
       analogWrite(PIN_CALEFACCIO,SortidaCalefaccio);
       //digitalWrite(PIN_LED_CAL,HIGH);
       tft.drawXBitmap( 10, 20, IconaTemperatura,50,50, RED);
    }
   else
    {
      analogWrite(PIN_CALEFACCIO, 0);
      SortidaCalefaccio = 0;
      //mdigitalWrite(PIN_LED_CAL,LOW);
      tft.drawXBitmap( 10, 20, IconaTemperatura,50,50, WHITE);
    }
  }

// SPEED
void ControlMotor ()
  {
    if (digitalRead(PIN_HAB_MOTOR)& PolsadorMotor==0)
      {
        MarxaMotor=MarxaMotor^1;
      }
    PolsadorMotor=digitalRead(PIN_HAB_MOTOR);
    if (MarxaMotor==1)
    {
      digitalWrite(PIN_MARXA_MOTOR,HIGH);
       tft.drawXBitmap( 10, 100, IconaVelocitat,50,50, RED);
     if (VelocitatMotor==0){noTone(PIN_VELOCITAT); tft.drawXBitmap( 10, 100, IconaVelocitat,50,50, WHITE);} else {tone(PIN_VELOCITAT,VelocitatMotor);}
    }
    else
    {
      digitalWrite(PIN_MARXA_MOTOR,LOW);
      noTone(PIN_VELOCITAT);
      tft.drawXBitmap( 10, 100, IconaVelocitat,50,50, WHITE);
    }
    switch(Divisor)
    {
      case 1:
        digitalWrite(DRV8825_M2,HIGH);
      break;
      case 2:
        digitalWrite(DRV8825_M2,LOW);
      break;
    }
  }
  
// LIMIT SWITCH
void ControlRotura ()
  {
//    RoturaCinta=digitalRead(PIN_ROTURA);
    RoturaCinta=analogRead(PIN_ROTURA);
    if (RoturaCinta<=500 & MarxaMotor==HIGH & PolsadorMotor==LOW){
      AlarmaRotura = HIGH;
      MarxaMotor=LOW;
      Mode = Mode-1;
    }
    if ( MarxaMotor==HIGH & PolsadorMotor==HIGH){
      AlarmaRotura=LOW;
      Mode = 0;
    }
  }

// MODE
void SeleccioEntrada ()
    {
      if (digitalRead(SelectorSW)==0 & PolsadorSelector==1)
        {
          if (Mode<2)
          {
            Mode=Mode+1;  
          }
          else
          {
            Mode=0;
          }
          AlarmaRotura=LOW;
        }
      PolsadorSelector=digitalRead(SelectorSW);
      if (PolsadorSelector==LOW)
        {
          Mode = Mode;
        }
    }

void setPWMPrescaler(uint8_t pin, uint16_t prescale) {
  
  byte mode;
  
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(prescale) {
      case    1: mode = 0b001; break;
      case    8: mode = 0b010; break;
      case   64: mode = 0b011; break; 
      case  256: mode = 0b100; break;
      case 1024: mode = 0b101; break;
      default: return;
    }
    
  } else if(pin == 3 || pin == 11) {
    switch(prescale) {
      case    1: mode = 0b001; break;
      case    8: mode = 0b010; break;
      case   32: mode = 0b011; break; 
      case   64: mode = 0b100; break; 
      case  128: mode = 0b101; break;
      case  256: mode = 0b110; break;
      case 1024: mode = 0b111; break;
      default: return;
    }
  }
  
  if(pin==5 || pin==6) {
    TCCR0B = TCCR0B & 0b11111000 | mode;
  } else if (pin==9 || pin==10) {
    TCCR1B = TCCR1B & 0b11111000 | mode;
  } else if (pin==3 || pin==11) {
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
