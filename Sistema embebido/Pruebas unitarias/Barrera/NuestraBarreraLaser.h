/* Definición del controlador de Laser y Flame Detector */

class NuestraBarreraLaser{
public:
  static const int AUTO = 0; //Siempre prendida
  static const int MANUAL = 1; //EL prendido y apagado lo controla el usuario

  
  NuestraBarreraLaser(const int pinLaser, const int pinFD); //Pin de datos de la barrera laser y pin de datos de el Flame Detector
  void activarBarrera(); //Prende la barrera laser
  void desactivarBarrera(); //Apaga la barrera laser
  bool detecta(); //Detecta si hay rocklet
  bool isOn(); //Devuelve si la barrera está prendida
  
private:
  static const unsigned long INICIO = 0;
  static const unsigned long TRESPUESTA = 5;

  unsigned long tmillis = 0; //Contador de espera para saber si se cortó el laser
  int pinLaser;
  int pinFD;
  int modo = MANUAL; //Modo actual, todavía no lo armé
  bool prendido = false;
  bool counting = false;
};
