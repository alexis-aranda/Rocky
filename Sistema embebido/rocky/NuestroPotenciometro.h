/* Potenciometro: Definición de la clase */

class NuestroPotenciometro{
public:
	static const int LIMITE_INFERIOR=0; //Solamente por si hay necesidad de usarlo
	static const int LIMITE_SUPERIOR=1023;

	NuestroPotenciometro(const int pin);
	int getPosicion();
	int getPosicion256();
private:
	int pin;
};
