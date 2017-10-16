#include <Wire.h>

#include <LiquidCrystal_I2C.h>

float Sensibilidad = 0.094; //sensibilidad en V/A para nuestro sensor
float offset = 0.100; // Equivale a la amplitud del ruido
const float POTENCIA_MAXIMA = 50;//Cambiar
int ledAlerta;//Iniciar estos leds
int ledNormal;
LiquidCrystal_I2C lcd(0x3F,16,2);


void setup() {  
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  pinMode(ledAlerta,OUTPUT);
  pinMode(ledNormal,OUTPUT);
}

void loop() {   
  float P=get_potencia(); // P=IV watts
  escribir_potencia();
  Serial.print("Potencia: ");
  Serial.print(P,3);  
  Serial.println("W");
  verificar(P);
  delay(500);     
}

void verificar(int potencia){
  if(potencia > POTENCIA_MAXIMA){
    alerta();
  }else{
    digitalWrite(ledAlerta,LOW);
    digitalWrite(ledNormal,HIGH);
  }
}

void alerta(){
  //TODO: Implementar este método
  //Es el método para cuando hay consumo excesivo, relés
}

float get_corriente()
{
  float voltajeSensor;
  float corriente=0;
  long tiempo=millis();
  float Imax=0;
  float Imin=0;
  while(millis()-tiempo<500)//realizamos mediciones durante 0.5 segundos
  { 
    voltajeSensor = analogRead(A0) * (5.0 / 1023.0);//lectura del sensor
    corriente=0.9*corriente+0.1*((voltajeSensor-2.515)/Sensibilidad); //Ecuación  para obtener la corriente
    if(corriente>Imax)Imax=corriente;
    if(corriente<Imin)Imin=corriente;
  }
  return(((Imax-Imin)/2)-offset);
}

float get_potencia(){
  return get_corriente() * 110 * 0.707;
}

void escribir_potencia(){
  lcd.setCursor(0,0);
  lcd.print("Potencia: ");
  lcd.print(get_potencia());
}

