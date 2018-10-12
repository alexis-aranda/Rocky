/* Definición del LED */

class NuestroLED{
public:
  static const int NO_MANUAL=-1; //Usada para cuando no hay necesidad de enviar una intensidad manualmente
	static const int LIM_INF=0; //Creo que es este
	static const int LIM_SUP=255; //Necesita actualización, no es realmente esto, creo que era el 90%
	static const int PRENDE_APAGA=0; //Manual, todo el tiempo excepto cuando se usa el tobogan
	static const int INTENSIDAD_VARIABLE=1; // Manual, cuando se usa el tobogan
	static const int SOFT_PWM=2; //Auto, esperando rocklets
	static const int SIEMPRE_PRENDIDO=3; //Auto, al reconocer color
	static const int SIEMPRE_APAGADO=4; //Auto, al tirar

	NuestroLED(const int pin); //Incializo, por defecto en modo SOFT_PWM
	NuestroLED(const int pin,const int modo); //Inicializo, con modo
	void setModo(const int modo); //Seteo el modo
	int getModo(); //Obtengo el modo actual
	void activar(const int intensidadPWMManual); //Activo el LED, de forma generica, la idea es que sea transparente por cada ciclo, solo según el modo

private:
	int pin;
	int modo; //Modo actual
	int intensidad; //Intensidad actual
	bool enSubida; //Para el modo SOFT_PWM
	bool isON; //Para el modo PRENDE_APAGA
  unsigned long start; //Tiempo desde que inició o cambió de modo
  void setIntensidadPWM(); //Cambio de intensidad en el LED real, solo privada
  void setIntensidadBoolON(); //Cambio de intensidad en el LED real, solo privada
  void setIntensidadBoolOFF(); //Cambio de intensidad en el LED real, solo privada
  void onOff(); //Va prendiendo y apagando cada cierto tiempo
  void manual(const int intensidadPWMManual); //Modo manual, recibe de un potenciometro
  void soft(); //Varía subiendo y apagando el PWM
};
