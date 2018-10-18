/* Definición del controlador del pulsador */

class NuestroPulsador{
public:
  NuestroPulsador(const int pin); //Pin del pulsador
  bool detectaCorto(); //Detecta si hay rocklet (entre MINSHORT y MAXSHORT segundos)
  bool detectaLargo(); //Detecta si hay un pulso largo (mas de MAXSHORT)
  
private:
  static const unsigned long MINSHORT = 500;
  static const unsigned long MAXSHORT = 1000;

  unsigned long tmillisC = 0; //Contador de espera para saber se activó el pulsador, para corto
  unsigned long tmillisL = 0; //Contador de espera para saber se activó el pulsador, para largo
  int pin; //Pin del pulsador
  bool countingC = false; //Si está contando segundos de detectaCorto
  bool countingL = false; //Si está contando segundos de detectaLargo
  bool posibleShort = false; //Sirve para decirme si es posible que sea un pulso corto, si se suelta el boton, sepa que es eso.
};
