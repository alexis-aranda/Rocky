#include <Arduino.h>
#include "ColorRocklet.h"

/**
 * Instancia un sensor de color. Los pines para S0, S1, S2, S3 y OUT se pasan por parametros.
 */
ColorRocklet::ColorRocklet(const int pin_s0, const int pin_s1, const int pin_s2, const int pin_s3, const int pin_out){
    //inicialización de ids y valores
    this->idColor = NO_IDENTIFICADO;
    this->rojo = 0;
    this->verde = 0;
    this->azul = 0;

    //inicialización de condiciones
    this->inicio_lectura_loop = true;
	this->nLectura = LEER_ROJO;

  //inicialización de pines
    this->pin_s0 = pin_s0;
    this->pin_s1 = pin_s1;
    this->pin_s2 = pin_s2;
    this->pin_s3 = pin_s3;
    this->pin_out = pin_out;
    
    pinMode(this->pin_s0, OUTPUT);
    pinMode(this->pin_s1, OUTPUT);
    pinMode(this->pin_s2, OUTPUT);
    pinMode(this->pin_s3, OUTPUT);
    pinMode(this->pin_out, INPUT);

    apagarSensor();
}

/**
 * Enciende los leds para poder sensar el color.
 * Setea una frecuencia del 20%.
 * Se la puede llamar aunque ya est� prendido, sin problemas.
 */
void ColorRocklet::prenderSensor(){
	digitalWrite(this->pin_s0,HIGH);
	digitalWrite(this->pin_s1,LOW);
}

/**
 * Apaga los leds para ahorrar energia. No se puede sensar mientras este apagado.
 * Se la puede llamar aunque ya este apagado sin problemas.
 */
void ColorRocklet::apagarSensor(){
    digitalWrite(this->pin_s0,LOW);
	digitalWrite(this->pin_s1,LOW);
}

/**
 * A esta funcion se la llama varias veces.
 * En el primer llamado inicia la lectura.
 * En los siguientes chequea si ya paso un tiempo prudencial y si paso, lee el siguiente color.
 * Devuelve verdadero recien cuando ya leyo el ultimo color necesario para identificar al rocklet.
 * Todas las llamadas intermedias devuelven falso.
 */
bool ColorRocklet::hacerLectura(){
  
	//La espera entre inicio y leer rojo no siempre tiene sentido. Por ahi habria que comparar contra los millis en que termino la lectura anterior
      if(inicio_lectura_loop){
        this->tmillis = millis();
        inicio_lectura_loop = false;
        return false;
        
      }else if ( nLectura == LEER_ROJO && millis() - this->tmillis >= T_ROJO){ //Veo si puedo leer rojo
        //Leo rojo
        digitalWrite(this->pin_s2, LOW);  
        digitalWrite(this->pin_s3, LOW);   
        this->rojo = pulseIn(this->pin_out, LOW);
        //Preparo para leer azul en la proxima vuelta
        this->tmillis = millis();
		nLectura = LEER_AZUL;
		return false;
        
      }else if( nLectura == LEER_AZUL && millis() - this->tmillis >= T_AZUL){ //Veo si puedo leer azul
        //Leo azul
        digitalWrite(this->pin_s3, HIGH);
        this->azul = pulseIn(this->pin_out, LOW);
        //Preparo para leer verde en la proxima vuelta
        this->tmillis = millis();
		nLectura = LEER_VERDE;
        return false;
        
      }else if(nLectura == LEER_VERDE && millis() - this->tmillis >= T_VERDE){ //Veo si puedo leer verde
        //Leo verde
        digitalWrite(this->pin_s2, HIGH);
        this->verde = pulseIn(this->pin_out, LOW);
        //Preparo para leer rojo en la proxima vuelta
        this->tmillis = millis();
		nLectura = LEER_ROJO;
        inicio_lectura_loop = true;
        return true; //Termine de leer los tres colores, ya puedo identificar un rocklet
      }
      return false; //No puedo hacer nada en esta vuelta
}

/**
 * Identifica el color segun los valores de la ultima lectura.
 */
void ColorRocklet::identificarColor(){
    
    if((this->verde < this->rojo) && (this->verde < this->azul) && (this->azul < this->rojo))
        this->idColor = VERDE;
    else if((this->azul < this->rojo) && (this->azul < this->verde) && (this->verde < this->rojo))
        this->idColor = AZUL;
    else if((this->rojo < this->verde) && (this->rojo < this->azul) && (this->verde < this->azul))
        this->idColor = AMARILLO;
/*
    else if((this->rojo < this->verde) && (this->rojo < this->azul) && (this->azul < this->verde)){
        if(this->rojo > MIN_MARRON_R)
           this->idColor = MARRON;
        else if(this->rojo < MAX_NARANJA_R && this->verde < MAX_NARANJA_V)
          this->idColor = NARANJA;
        else
          this->idColor = ROJO;
 */
     else if((this->rojo < this->verde) && (this->azul < this->verde)){
        if(this->rojo >= this->azul)
          this->idColor = MARRON;
        else {
          //if(this->rojo > MIN_MARRON_R || this->verde > MIN_MARRON_V)
          //  this->idColor = MARRON;
          //else 
          if(this->rojo > MIN_NARANJA_R && this->verde < MAX_NARANJA_V)
            this->idColor = NARANJA;
          else
            this->idColor = ROJO;
        }
    } else 
      //si no esta dentro de ningun rango, queda como NO_IDENTIFICADO
        this->idColor = NO_IDENTIFICADO;
    
}

/**
 * Identifica el color segun los valores leidos y lo devuelve.
 */
int ColorRocklet::getColor(){
  identificarColor();
  return this->idColor;
}
