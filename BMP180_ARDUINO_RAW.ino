/*Written By: Er. Dapinder Singh Virk
  Email ID: brillosolutions@gmail.com
  Version: 1.0.0
  Date: October 21, 2018
  Outputs: Temperature in °C, °F and Kelvin units.*/
		   
#include <Wire.h>
#include <math.h>
uint8_t DEV_ADDR = 0x77;
short AC1, AC2, AC3;
unsigned short AC4, AC5, AC6;
short AB1, AB2;
short MB, MC, MD;
double T;
long UT, UP, UT_MSB, UT_LSB, X1, X2;
long UP_MSB, UP_LSB, UP_XLSB, AB5;
void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  getCoeffData();
}

void loop() 
{
  T = calcTemperature();
  Serial.print(T / 10);
  Serial.print("°C\n");
  Serial.print((T / 10 * 1.8) + 32);
  Serial.print("°F\n");
  Serial.print(Temp + 273.15);
  Serial.print("K\n\n");
  delay(500);
}

double calcTemperature()
{
  double TT;
  beginConversion(0xF4, 0x2E);
  delay(5);
  
  UT_MSB = getTargetData(0xF6) << 8;
  UT_LSB = getTargetData(0xF7);
  UT = UT_MSB + UT_LSB;

  X1 = (UT - AC6) * AC5 / pow(2, 15);
  X2 = MC * pow(2, 11) / (X1 + MD);
  AB5 = X1 + X2;
  TT = (AB5 + 8) / pow(2, 4);
  return TT;
}

void getCoeffData()
{
  AC1 = getCoeff(0xAA);
  AC2 = getCoeff(0xAC);
  AC3 = getCoeff(0xAE);
  AC4 = getCoeff(0xB0);
  AC5 = getCoeff(0xB2);
  AC6 = getCoeff(0xB4);
  AB1 = getCoeff(0xB6);
  AB2 = getCoeff(0xB8);
   MB = getCoeff(0xBA);
   MC = getCoeff(0xBC);
   MD = getCoeff(0xBE);
}

short getCoeff(uint8_t mLoc)
{
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(mLoc);
  Wire.endTransmission();
  Wire.requestFrom(DEV_ADDR, 2); 
  return Wire.read() << 8 | Wire.read();
}

void beginConversion(uint8_t aByte, uint8_t dByte)
{
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(aByte);
  Wire.write(dByte);
  Wire.endTransmission();
}

uint8_t getTargetData(uint8_t mLoc)
{
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(mLoc);
  Wire.endTransmission();
  Wire.requestFrom(DEV_ADDR, 1); 
  return Wire.read();
}