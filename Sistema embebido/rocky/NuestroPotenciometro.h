/* Potenciometro: Definición de la clase */

class NuestroPotenciometro{
    public:    
    	NuestroPotenciometro(const int pin);
    	int getPosicion();
    private:
    	int pin;
};
