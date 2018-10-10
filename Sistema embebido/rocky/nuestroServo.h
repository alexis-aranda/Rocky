/*
 * nuestroServo.h - Biblioteca para inicializar y mover los servos
 */

#ifndef nuestroServo_h
#define nuestroServo_h

#include <Arduino.h>

class NuestroServo
{
  public:
    //Estaciones para el servo negro
    static const int RECEPCION_ST;
    static const int COLOR_ST;
    static const int CAIDA_ST;

    //Estaciones para el servo azul
    static const int ST_1;
    static const int ST_2;
    static const int ST_3;
    static const int ST_4;
    static const int ST_5;
    static const int ST_6;

    //Funciones para ambos servos
    /** Inicializa un servo en el pin indicado y setea entre que valores PWM puede trabajar */
    NuestroServo(int pin, int limiteInf, int limiteSup);
    /* 
     * Indica al servo que se mueva hasta la posicion recibida por parametro, 
     * siempre que esta se encuentre entre los limites setados.
     * La posicion debe pasarse en valores PWM.
     */
    void irA(int posicion);

  private:
    int pin;
    int limInf;
    int limSup;
};

#endif
