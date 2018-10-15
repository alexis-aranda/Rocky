#include <Arduino.h>
#include "ColorRocklet.h"

ColorRocklet::ColorRocklet(const int pin_s0, const int pin_s1, const int pin_s2, const int pin_s3, const int pin_out){
    this->idColor = NO_IDENTIFICADO;
    this->rojo = 0;
    this->verde = 0;
    this->azul = 0;
    this->pin_s0 = pin_s0;
    this->pin_s1 = pin_s1;
    this->pin_s2 = pin_s2;
    this->pin_s3 = pin_s3;
    this->pin_out = pin_out;
}

void ColorRocklet::setColor(const int idColor){
    this->idColor=idColor;
}

int ColorRocklet::getColor(){
    digitalWrite(this->pin_s2, LOW);  
    digitalWrite(this->pin_s3, LOW);   
    this->rojo = pulseIn(this->pin_out, digitalRead(this->pin_out) == HIGH ? LOW : HIGH);  
    digitalWrite(this->pin_s3, HIGH);   
    this->azul = pulseIn(this->pin_out, digitalRead(this->pin_out) == HIGH ? LOW : HIGH);  
    digitalWrite(this->pin_s2, HIGH);    
    this->verde = pulseIn(this->pin_out, digitalRead(this->pin_out) == HIGH ? LOW : HIGH);  
}

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
    }
    if(enRango(M_AZUL)){
        this->idColor = AZUL;
        return;
    }
    if(enRango(M_ROJO)){
        this->idColor = ROJO;
        return;
    }
    if(enRango(M_NARANJA)){
        this->idColor = NARANJA;
        return;
    }
    if(enRango(M_AMARILLO)){
        this->idColor = AMARILLO;
        return;
    }
    if(enRango(M_MARRON)){
        this->idColor = MARRON;
        return;
    }
    //si no esta dentro de ningun rango, queda como NO_IDENTIFICADO por defecto
}
