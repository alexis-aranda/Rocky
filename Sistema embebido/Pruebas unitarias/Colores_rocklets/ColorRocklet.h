/*
    ColorRocklet.h - Biblioteca para identificar el color de un rocklet
*/
#include <Arduino.h>
#ifndef ColorRocklet_h
#define ColorRocklet_h
/*
#ifndef LECTURAS_X_COLOR
#define LECTURAS_X_COLOR 3
#define DESVIO_COLOR 10
*/

class ColorRocklet{
	public:
	    /* Definición de id para los Colores */
		static const int NO_IDENTIFICADO = 0; //para cuando no reconoce ningún color
		static const int VERDE = 1;
		static const int AZUL = 2;
		static const int ROJO = 3;
		static const int NARANJA = 4;
		static const int AMARILLO = 5;
		static const int MARRON = 6;
		static const int LOOPS_COLOR = 10; // para la alternativa de resolución
		
		static const int LEER_ROJO = 1;
		static const int LEER_AZUL = 2;
		static const int LEER_VERDE = 3;

		/* Defino las medias de valores para cada color */
		/* MEDIA_COLOR[] = {rojo, verde, azul};*/
		/*
		static const int M_VERDE[LECTURAS_X_COLOR] = {155,142,154};
		static const int M_AZUL[LECTURAS_X_COLOR] = {169,156,111};
		static const int M_ROJO[LECTURAS_X_COLOR] = {135,231,183};
		static const int M_NARANJA[LECTURAS_X_COLOR] = {100,183,163};
		static const int M_AMARILLO[LECTURAS_X_COLOR] = {98,138,158};
		static const int M_MARRON[LECTURAS_X_COLOR] = {182,233,188};*/

    /* Defino topes para los colores en conflicto*/
    static const int MAX_NARANJA_R = 130;
    static const int MAX_NARANJA_V = 190;
    static const int MIN_MARRON_R = 155;

    /* 
     * Inicializo con los pines que me asignen.
     * Inicializo como NO_IDENTIFICADO por defecto.
     * Inicializo parámetros de colores en 0.
    */
		ColorRocklet(const int pin_s0, const int pin_s1, const int pin_s2, const int pin_s3, const int pin_out);

    /*
     * Llama a identificarColor().
     * Devuelve el id del color identificado.
     */
		int getColor();
		bool hacerLectura();//realiza la lectura de datos recibidos desde el sensor y los pone en los parámetros de los colores.
		void prenderSensor();
		void apagarSensor();
    bool isOn();

	private:
		int idColor; //id actual
		int rojo; //rojo medido actual
		int verde; //verde medido actual
		int azul; //azul medido actual
		int nLectura;

    /* pines del sensor de color */
    int pin_s0;
    int pin_s1;
    int pin_s2;
    int pin_s3;
    int pin_out;

    bool inicio_lectura_loop;
    bool rojoLeido;
    bool azulLeido;
    bool verdeLeido;
    static const unsigned long T_ROJO = 100; //Tiempo de espera entre lecturas
    static const unsigned long T_AZUL = 100; //Tiempo de espera entre lecturas
    static const unsigned long T_VERDE = 100; //Tiempo de espera entre lecturas
    unsigned long tmillis = 0; //Contador de espera entre lecturas
    /*
    // para la 3er alternativa de resolución con millis y un contador de loops para la identificación del color
    int cantLoops; //cuenta los loops de lectura de datos, vuelve a 0 cuando identifica el color
    
    void obtenerPromedio();// para la alternativa de resolución //saca el promedio de todas las lecturas
    //fin de la 3er alternativa
    */
    void identificarColor();//llama a hacerLectura identifica el color según los parámetros leídos.
    void setColor(const int idColor); //por si es necesario
    
    
    /*bool enRango(const int vecColor[]); *//*verifica que los valores esten dentro de un
                                         *rango indicado (equivalente a un color)*/
};

#endif
