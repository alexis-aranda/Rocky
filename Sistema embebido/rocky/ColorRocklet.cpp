#include <Arduino.h>
#include "ColorRocklet.h"

ColorRocklet::ColorRocklet(const int pin_s0, const int pin_s1, const int pin_s2, const int pin_s3, const int pin_out){
    //inicialización de ids y valores
    this->idColor = NO_IDENTIFICADO;
    this->rojo = 0;
    this->verde = 0;
    this->azul = 0;

    //inicialización de condiciones
    this->inicio_lectura_loop = true;
    this->rojoLeido = false;
    this->azulLeido = false;
    this->verdeLeido = true;
	
	this->nLectura = LEER_ROJO;
/*
    // para la 3er alternativa de resolución
    //inicialización de contador
    this->cantLoops = 0;
    // fin 3er alternativa
*/  
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

    digitalWrite(this->pin_s0,HIGH);
    digitalWrite(this->pin_s1,LOW);
}

void ColorRocklet::setColor(const int idColor){
    this->idColor=idColor;
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
  
void ColorRocklet::identificarColor(){
    //hacerLectura();
    
    if((this->verde < this->rojo) && (this->verde < this->azul) && (this->azul < this->rojo)){
        this->idColor = VERDE;
        Serial.println("Verde");
    }else if((this->azul < this->rojo) && (this->azul < this->verde) && (this->verde < this->rojo)){
        this->idColor = AZUL;
        Serial.println("Azul");
    }else if((this->rojo < this->verde) && (this->rojo < this->azul) && (this->verde < this->azul)){
        this->idColor = AMARILLO;
        Serial.println("Amarillo");
    }else if((this->rojo < this->verde) && (this->rojo < this->azul) && (this->azul < this->verde)){
        if(this->rojo > MIN_MARRON_R){
           this->idColor = MARRON;
           Serial.println("Marron");
        }else if(this->rojo < MAX_NARANJA_R && this->verde < MAX_NARANJA_V){
          this->idColor = NARANJA;
          Serial.println("Naranja");
        }else{
          this->idColor = ROJO;
          Serial.println("Rojo");
        }
    } else {
        this->idColor = NO_IDENTIFICADO;
    }
    
    //si no esta dentro de ningun rango, queda como NO_IDENTIFICADO por defecto
}

int ColorRocklet::getColor(){
  identificarColor();
  return this->idColor;
}

//fin 2da alternativa

/*
// para la 3er alternativa de resolución que actúa con un contador de N lecturas
void ColorRocklet::hacerLectura(){

    if(inicio_lectura_loop){
      this->tmillis = millis();
      inicio_lectura_loop = false;
    }else if (millis() - this->tmillis >= T_ROJO && verdeLeido){
      
      digitalWrite(this->pin_s2, LOW);  
      digitalWrite(this->pin_s3, LOW);   
      this->rojo = this->rojo + pulseIn(this->pin_out, LOW);
      this->tmillis = millis();
      rojoLeido = true;
      verdeLeido = false;
      
    }else if(millis() - this->tmillis >= T_AZUL && rojoLeido  && verdeLeido){
      
      digitalWrite(this->pin_s3, HIGH);
      this->azul = this->azul + pulseIn(this->pin_out, LOW);
      this->tmillis = millis();
      azulLeido = true;
      
    }else if(millis() - this->tmillis >= T_VERDE && azulLeido){
      
      digitalWrite(this->pin_s2, HIGH);
      this->verde = this->verde + pulseIn(this->pin_out, LOW);
      this->cantLoops += 1; //incremento loop de lectura
      this->tmillis = millis();
      verdeLeido = true;
      
      rojoLeido = false;
      azulLeido = false;
      inicio_lectura_loop = true;
    }
}

void ColorRocklet::obtenerPromedio(){
    this->rojo = this->rojo / this->cantLoops;
    this->verde = this->verde / this->cantLoops;
    this->azul = this->azul / this->cantLoops;
}

void ColorRocklet::identificarColor(){
    if(this->cantLoops < LOOPS_COLOR){
      hacerLectura();
    }else{
      obtenerPromedio();
      
      if((this->verde < this->rojo) && (this->verde < this->azul) && (this->azul < this->rojo)){
          this->idColor = VERDE;
      }else if((this->azul < this->rojo) && (this->azul < this->verde) && (this->verde < this->rojo)){
          this->idColor = AZUL;
      }else if((this->rojo < this->verde) && (this->rojo < this->azul) && (this->verde < this->azul)){
          this->idColor = AMARILLO;
      }else if((this->rojo < this->verde) && (this->rojo < this->azul) && (this->azul < this->verde)){
          if(this->rojo > MIN_MARRON_R){
             this->idColor = MARRON;
          }else if(this->rojo < MAX_NARANJA_R && this->verde < MAX_NARANJA_V){
            this->idColor = NARANJA;
          }else{
            this->idColor = ROJO;
          }
      }
      //después de N lecturas debería tener identificado el color
      this->cantLoops = 0;
    }
    
    //si no esta dentro de ningun rango, queda como NO_IDENTIFICADO por defecto
}
// fin 3er alternativa
*/

/*
//esta es la primer resolución que plantee con el uso de vecores con las medias por color
bool ColorRocklet::enRango(const int vecCol[]){
    return ((this->rojo>=vecCol[0] - DESVIO_COLOR && this->rojo<=vecCol[0] + DESVIO_COLOR)
    && (this->verde>=vecCol[1] - DESVIO_COLOR && this->verde<=vecCol[1] + DESVIO_COLOR)
    && (this->azul>=vecCol[2] - DESVIO_COLOR && this->azul<=vecCol[2] + DESVIO_COLOR));
}

void ColorRocklet::identificarColor(){
    hacerLectura();
    
    if(enRango(M_VERDE)){
        this->idColor = VERDE;
        return;
    }else if(enRango(M_AZUL)){
        this->idColor = AZUL;
        return;
    }else if(enRango(M_AMARILLO)){
        this->idColor = AMARILLO;
        return;
    }else if(enRango(M_ROJO)){
        this->idColor = ROJO;
        return;
    }else if(enRango(M_NARANJA)){
        this->idColor = NARANJA;
        return;
    }else if(enRango(M_MARRON)){
        this->idColor = MARRON;
        return;
    }
    //si no esta dentro de ningun rango, queda como NO_IDENTIFICADO por defecto
}
*/
