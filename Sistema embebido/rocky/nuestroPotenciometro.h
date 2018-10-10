/* Potenciometro: Definición de la clase */

class nuestroPotenciometro{
public:
	static const int LIMITE_INFERIOR=0; //Solamente por si hay necesidad de usarlo
	static const int LIMITE_SUPERIOR=1023;

	nuestroPotenciometro(const int pin);
	int getPosicion();
	int getPosicion256();
private:
	int pin; //Le saco el const porque en el constructor intentamos asignarlo
}; //El punto y coma, Alexis, sino no compila
