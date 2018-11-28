/* Definición del controlador de Laser y Flame Detector */

class NuestraBarreraLaser{
public:  
  NuestraBarreraLaser(const int pinLaser, const int pinFD); //Pin de datos de la barrera laser y pin de datos de el Flame Detector
  void activarBarrera(); //Prende la barrera laser
  void desactivarBarrera(); //Apaga la barrera laser
  bool detecta(); //Detecta si hay rocklet
  
private:
  int pinLaser;
  int pinFD;
  bool prendido = false;

  bool isOn(); //Devuelve si la barrera está prendida
};
