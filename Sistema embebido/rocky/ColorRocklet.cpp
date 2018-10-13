#include <Arduino.h>
#include "ColorRocklet.h"

ColorRocklet::ColorRocklet(){
    this->idColor = NO_IDENTIFICADO;
    this->azul = 0;
    this->rojo = 0;
    this->verde = 0;
}

ColorRocklet::ColorRocklet(const int rojo, const int verde, const int azul){
    this->idColor = NO_IDENTIFICADO;
    this->rojo = rojo;
    this->verde = verde;
    this->azul = azul;
}

void ColorRocklet::setColor(const int idColor){
    this->idColor=idColor;
}

int ColorRocklet::getColor(){
    return this->idColor;
}

bool ColorRocklet::enRango(const int vecCol[]){
    return ((this->rojo>=vecCol[0] - 5 && this->rojo<=vecCol[0] + 5)
    && (this->verde>=vecCol[1] - 5 && this->verde<=vecCol[1] + 5)
    && (this->azul>=vecCol[2] - 5 && this->azul<=vecCol[2] + 5));
}

void ColorRocklet::identificarColor(){
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
