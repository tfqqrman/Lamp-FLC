#include <RBDdimmer.h>
#include <Wire.h>
#include <BH1750.h>

#define outputPin 12
#define zerocross 5

dimmerLamp dimmer(outputPin);
BH1750 sensor;
//deklarasi variabel
int  dError,error,outputPersen,setPoint, nilaiSensor, lastError;
float keluaranFuzzy,pbE, psE, zE, nsE, nbE, pbDE, psDE, zDE, nsDE, nbDE,w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16,w17,w18,w19,w20,w21,w22,w23,w24,w25,z1,z2,z3,z4,z5,z6,z7,z8,z9,z10,z11,z12,z13,z14,z15,z16,z17,z18,z19,z20,z21,z22,z23,z24,z25;
const int n = -1, z = 0, ps = 1,pm = 8, pb=17;

//setup
void setup() {
  Serial.begin(9600);
  dimmer.begin(NORMAL_MODE, ON);
  Wire.begin();
  sensor.begin();
  outputPersen = 0;
  setPoint = 80; // set point yang dikehendaki dalam satuan lux
  error = 0;
  dError = 0;
  lastError = 0;
  readSensorLux();
  debugTabel_header();
  //Serial.print("nilaiSensor");
  //Serial.print("\t");
  //Serial.print("sp");
  //Serial.print("\t");
  
  delay(2000);
}

//loop
void loop() {
  readSensorLux();
  kontrol();
  fuzzyfikasi();
  basisAturan();
  defuzzyfikasi();
  //kontrol();
  //delay(1);
  outputPersen = round(keluaranFuzzy);
  if(outputPersen>42){
    outputPersen = 42;
  }else if(outputPersen<10){
    outputPersen = 10;
  }else{
    outputPersen = outputPersen;
  }
  dimmer.setPower(outputPersen);
  //Serial.println(outputPersen);
  //debugging();
  //plotter();
  debugTabel_isi();
  delay(500);
  //delay(180); 
}

/////////////////////////////////////////////////////////////////////////////////////////Function list/////////////////////////////////////////////////////////////////////////////////////////

void readSensorLux(){
  //delay(500);
  nilaiSensor = sensor.readLightLevel();
  }

void kontrolFuzzy(){
  kontrol();
  fuzzyfikasi();
  basisAturan();
  defuzzyfikasi();
}

void kontrol(){
  error = setPoint - nilaiSensor;
  dError = error - lastError;
  lastError = error;
  //outputPersen = outputPersen+round(keluaranFuzzy);
}

void fuzzyfikasi(){
  //Serial.print("fuzzyfikasi= ");
  //Serial.println(error);
  if(error>80){
    pbE = 1;
    psE = 0;
    zE = 0;
    nsE = 0;
    nbE = 0;
  }else if(error>40){
    pbE = derajat(1,0,80,40,error);//Pers.pb
    psE = 1-pbE;
    zE = 0;
    nsE = 0;
    nbE = 0;
  }else if(error>0){
    pbE = 0;//Pers.pb 
    psE = derajat(1,0,40,0,error);
    zE = 1-psE;
    nsE = 0;
    nbE = 0;
  }else if(error > -40){
    pbE = 0;
    psE = 0;
    zE = derajat(1,0,0,-40,error);
    nsE = 1-zE;
    nbE = 0;
  }else if(error > -80){
    pbE = 0; 
    psE = 0;
    zE = 0;
    nsE = derajat(1,0,-40,-80,error);
    nbE = 1-nsE;
  }else{
    pbE = 0; 
    psE = 0;
    zE = 0;
    nsE = 0;
    nbE = 1;
  }

  if(dError > 80){
    pbDE = 1; 
    psDE = 0;
    zDE = 0;
    nsDE = 0;
    nbDE = 0;
  }else if(dError > 40){
    pbDE = derajat(1,0,80,40,dError); 
    psDE = 1-pbDE;
    zDE = 0;
    nsDE = 0;
    nbDE = 0;
  }else if(dError > 0){
    pbDE = 0; 
    psDE = derajat(1,0,40,0,dError);
    zDE = 1-psDE;
    nsDE = 0;
    nbDE = 0;
  }else if(dError > -40){
    pbDE = 0; 
    psDE = 0;
    zDE = derajat(1,0,0,-40,dError);
    nsDE = 1-zDE;
    nbDE = 0;
  }else if(dError > -80){
    pbDE = 0; 
    psDE = 0;
    zDE = 0;
    nsDE = derajat(1,0,-40,-80,dError);
    nbDE = 1-nsDE;
  }else{
    pbDE = 0; 
    psDE = 0;
    zDE = 0;
    nsDE = 0;
    nbDE = 1;
  }
}

float derajat(float y2, float y1, float x2, float x1, float masukkan){
  //float hasil = 0;
  float hasil =(((y2-y1)/(x2-x1))*(masukkan-x1))+y1;
  return hasil;
}

void basisAturan(){
  
  w1 = min(nbE,nbDE);
  z1 = (0*error)+(0*dError)+((setPoint+26.77)/2.8);//
  w2 = min(nbE,nsDE);
  z2 = (0*error)+(0*dError)+((setPoint+26.77)/2.8);
  w3 = min(nbE,zDE);
  z3 = (0*error)+(0*dError)+((setPoint+26.77)/2.8);
  w4 = min(nbE,psDE);
  z4 = (0*error)+(0.11*dError)+((setPoint+26.77)/2.8);
  w5 = min(nbE,pbDE);
  z5 = (0.12*error)+(0.17*dError)+((setPoint+26.77)/2.8);

  w6 = min(nsE,nbDE);//kestabilan
  z6 = (0*error)+(0*dError)+((setPoint+26.77)/2.8);
  w7 = min(nsE,nsDE);
  z7 = (0*error)+(0*dError)+((setPoint+26.77)/2.8);
  w8 = min(nsE,zDE);
  z8 = (0*error)+(0*dError)+((setPoint+26.77)/2.8);
  w9 = min(nsE,psDE);
  z9 = (0.22*error)+(0.13*dError)+((setPoint+26.77)/2.8);
  w10 = min(nsE,pbDE);
  z10 = (0.14*error)+(0.1*dError)+((setPoint+26.77)/2.8);

  w11 = min(zE,nbDE);
  z11 = (0*error)+((-0.1)*dError)+((setPoint+26.77)/2.8);
  w12 = min(zE,nsDE);
  z12 = (0*error)+((-0.11)*dError)+((setPoint+26.77)/2.8);
  w13 = min(zE,zDE);
  z13 = (0*error)+(0*dError)+((setPoint+26.77)/2.8)+1;
  w14 = min(zE,psDE);
  z14 = (0*error)+(0.11*dError)+((setPoint+26.77)/2.8);
  w15 = min(zE,pbDE);
  z15 = (0*error)+(0.05*dError)+((setPoint+26.77)/2.8);

  w16 = min(psE,nbDE);//rem overshoot
  z16 = (0.36*error)+(0.1*dError)+((setPoint+26.77)/2.8);
  w17 = min(psE,nsDE);
  z17 = (0.31*error)+(0.2*dError)+((setPoint+26.77)/2.8);
  w18 = min(psE,zDE);
  z18 = (0.12*error)+(0.2*dError)+((setPoint+26.77)/2.8);
  w19 = min(psE,psDE);
  z19 = (0.01*error)+(0.1*dError)+((setPoint+26.77)/2.8);
  w20 = min(psE,pbDE);
  z20 = (0.1*error)+(0.01*dError)+((setPoint+26.77)/2.8);

  w21 = min(pbE,nbDE);
  z21 = (0.1*error)+(0.01*dError)+((setPoint+26.77)/2.8);
  w22 = min(pbE,nsDE);
  z22 = (0.1*error)+(0.15*dError)+((setPoint+26.77)/2.8);
  w23 = min(pbE,zDE);
  z23 = (0.05*error)+(0.15*dError)+((setPoint+26.77)/2.8);
  w24 = min(pbE,psDE);
  z24 = (0.01*error)+(0.08*dError)+((setPoint+26.77)/2.8);
  w25 = min(pbE,pbDE);
  z25 = (0.01*error)+(0.03*dError)+((setPoint+26.77)/2.8);
  
  //float w[25]={w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13,w14,w15,w16,w17,w18,w19,w20,w21,w22,w23,w24,w25};
  //int z[24]={z1,z2,z3,z4,z5,z6,z7,z8,z9,z10,z11,z12,z13,z14,z15,z16,z17,z18,z19,z20,z21,z22,z23,z24,z25};
}

void defuzzyfikasi(){
  keluaranFuzzy = ((w1*z1+w2*z2+w3*z3+w4*z4+w5*z5+w6*z6+w7*z7+w8*z8+w9*z9+w10*z10+w11*z11+w12*z12+w13*z13+w14*z14+w15*z15+w16*z16+w17*z17+w18*z18+w19*z19+w20*z20+w21*z21+w22*z22+w23*z23+w24*z24+w25*z25)/(w1+w2+w3+w4+w5+w6+w7+w8+w9+w10+w11+w12+w13+w14+w15+w16+w17+w18+w19+w20+w21+w22+w23+w24+w25));
}

void debugging(){
  Serial.print("SetPoint= ");
  Serial.println(setPoint);
  //Serial.print("fuzzy= ");
  //Serial.println(error);
  
  //==========================================================//
  Serial.print("nilai Sensor = "); Serial.println(nilaiSensor);
  Serial.print("error = "); Serial.println(error);
  Serial.print("dError= "); Serial.println(dError);
  Serial.print("lastError=");Serial.println(lastError);
  Serial.print("keluaran Fuzzy=");Serial.println(keluaranFuzzy);
  //Serial.print("pbE= ");Serial.println(pbE);
  //Serial.print("psE= ");Serial.println(psE);
  //Serial.print("zE= ");Serial.println(zE);
  //Serial.print("nbE= ");Serial.println(nbE);
  //Serial.print("nsE= ");Serial.println(nsE);
  Serial.println("==========================");
}

void debugTabel_header(){
  Serial.print("Sensor");
  Serial.print("\t");
  Serial.print("error");
  Serial.print("\t");
  Serial.print("dError");
  Serial.print("\t");
  Serial.print("fOut");
  Serial.print("\t");
  Serial.println("%out");
}
void debugTabel_isi(){
  Serial.print(nilaiSensor);
  Serial.print("\t");
  Serial.print(error);
  Serial.print("\t");
  Serial.print(dError);
  Serial.print("\t");
  Serial.print(keluaranFuzzy);
  Serial.print("\t");
  Serial.println(outputPersen);
}

void plotter(){
  Serial.print(nilaiSensor);
  Serial.print("\t");
  Serial.println(setPoint);
  }
