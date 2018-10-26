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

//esta opción actúa con una sola lectura
int ColorRocklet::getColor(){

      if(inicio_lectura_loop){
        
        this->tmillis = millis();
        inicio_lectura_loop = false;
        
      }else if (millis() - this->tmillis >= T_ROJO && verdeLeido){
        
        digitalWrite(this->pin_s2, LOW);  
        digitalWrite(this->pin_s3, LOW);   
        this->rojo = pulseIn(this->pin_out, LOW);
        //this->rojo = pulseIn(this->pin_out, digitalRead(this->pin_out) == HIGH ? LOW : HIGH);  
        this->tmillis = millis();
        rojoLeido = true;
        
      }else if(millis() - this->tmillis >= T_AZUL && rojoLeido){
        
        digitalWrite(this->pin_s3, HIGH);
        this->azul = pulseIn(this->pin_out, LOW);
        //this->azul = pulseIn(this->pin_out, digitalRead(this->pin_out) == HIGH ? LOW : HIGH);
        this->tmillis = millis();
        azulLeido = true;
        
      }else if(millis() - this->tmillis >= T_VERDE && azulLeido){
        
        digitalWrite(this->pin_s2, HIGH);
        this->verde = pulseIn(this->pin_out, LOW);
        //this->verde = pulseIn(this->pin_out, digitalRead(this->pin_out) == HIGH ? LOW : HIGH);  
        this->tmillis = millis();
        verdeLeido = true;
        
        rojoLeido = false;
        azulLeido = false;
        inicio_lectura_loop = true;
      }    
}
  
int ColorRocklet::identificarColor(){
    getColor();
    
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
    
    //si no esta dentro de ningun rango, queda como NO_IDENTIFICADO por defecto
    return this->idColor;
}
//fin 2da alternativa

/*
// para la 3er alternativa de resolución que actúa con un contador de N lecturas
int ColorRocklet::getColor(){

    if(inicio_lectura_loop){
      this->tmillis = millis();
      inicio_lectura_loop = false;
    }else if (millis() - this->tmillis >= T_ROJO && verdeLeido){
      
      digitalWrite(this->pin_s2, LOW);  
      digitalWrite(this->pin_s3, LOW);   
      this->rojo = this->rojo + pulseIn(this->pin_out, LOW);
      this->tmillis = millis();
      rojoLeido = true;
      
    }else if(millis() - this->tmillis >= T_AZUL && rojoLeido){
      
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

int ColorRocklet::identificarColor(){
    if(this->cantLoops < LOOPS_COLOR){
      getColor();
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
    return this->idColor;
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
    getColor();
    
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
