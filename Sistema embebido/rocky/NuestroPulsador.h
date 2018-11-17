/* Definición del controlador del pulsador */

class NuestroPulsador {
  public:
    NuestroPulsador(const int pin); //Pin del pulsador
    bool detectaCorto(); //Detecta si hay rocklet (entre MINSHORT y MAXSHORT segundos)
    bool detectaLargo(); //Detecta si hay un pulso largo (mas de MAXSHORT)
    void chequear();

  private:
    static const unsigned long MINSHORT = 50;
    static const unsigned long MAXSHORT = 500;
    static const unsigned int LARGO = 2;
    static const unsigned int CORTO = 1;
    static const unsigned int NADA = 0;

    int estado = NADA;
    unsigned long tmillis = 0; //Contador de espera para saber se activó el pulsador
    int pin; //Pin del pulsador
    bool counting = false;
};
