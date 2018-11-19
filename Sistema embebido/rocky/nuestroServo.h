/*
 * nuestroServo.h - Biblioteca para inicializar y mover los servos
 */

#ifndef nuestroServo_h
#define nuestroServo_h

class NuestroServo
{
  public:
    //Estaciones para el servo negro
    static const int RECEPCION_ST=190; //190
    static const int COLOR_ST=120; //128
    static const int CAIDA_ST=55; //55

    //Estaciones en valores PWM para el servo azul
    static const int ST_1=50;
    static const int ST_2=84;
    static const int ST_3=118;
    static const int ST_4=163;
    static const int ST_5=214;
    static const int ST_6=248;

    //Funciones para ambos servos
    NuestroServo(int pin, int limiteInf, int limiteSup);
    void irA(int posicion);
    void irAAnalogico(int posicion);

  private:
    int pin;
    int limInf;
    int limSup;
};

#endif
