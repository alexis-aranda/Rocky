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
    static const int MAX_NARANJA_V = 200;
    static const int MIN_MARRON_R = 160;

    /* 
     * Inicializo con los pines que me asignen.
     * Inicializo como NO_IDENTIFICADO por defecto.
     * Inicializo parámetros de colores en 0.
    */
		ColorRocklet(const int pin_s0, const int pin_s1, const int pin_s2, const int pin_s3, const int pin_out);

    /*llama a getColor() e identifica el color con los parámetros leídos*/
		void identificarColor();

	private:
		int idColor; //id actual
		int rojo; //rojo medido actual
		int verde; //verde medido actual
		int azul; //azul medido actual

    /* pines del sensor de color */
    int pin_s0;
    int pin_s1;
    int pin_s2;
    int pin_s3;
    int pin_out;

    int getColor(); //lee los datos recibidos desde el sensor y los pone en los parámetros de los colores
    void setColor(const int idColor); //Seteo el id del color
    bool enRango(const int vecColor[]); /*verifica que los valores esten dentro de un
                                         *rango indicado (equivalente a un color)*/
};

#endif
