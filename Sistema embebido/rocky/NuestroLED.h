/* Definición del LED */

class NuestroLED{
public:
	static const int LIM_INF=0;
	static const int LIM_SUP=255;
	static const int PRENDE_APAGA=0; //Manual, todo el tiempo excepto cuando se usa el tobogan
	static const int INTENSIDAD_VARIABLE=1; // Manual, cuando se usa el tobogan
	static const int SOFT_PWM=2; //Auto, esperando rocklets
	static const int SIEMPRE_PRENDIDO=3; //Auto, al reconocer color
	static const int SIEMPRE_APAGADO=4; //Auto, al tirar

	NuestroLED(const int pin); //Incializo, por defecto en modo PRENDE_APAGA
	NuestroLED(const int pin,const int modo); //Inicializo, con modo
	void setModo(const int modo); //Seteo el modo
	int getModo(); //Obtengo el modo actual
	void setIntensidad(int intensidad); //Seteo la intensidad manualmente, talvez quede privada
	void activar(); //Activo el LED, de forma generica, la idea es que sea transparente por cada ciclo, solo según el modo

private:
	int pin;
	int modo; //Modo actual
	int intensidad; //Intensidad actual
	bool enSubida; //Para el modo SOFT_PWM
	bool isON; //Para el modo PRENDE_APAGA
};
