/*
    ColorRocklet.h - Biblioteca para identificar el color de un rocklet
*/
#include <Arduino.h>
#ifndef ColorRocklet_h
#define ColorRocklet_h

class ColorRocklet{
	public:
	    /* Definición de id para los Colores */
		static const int NO_IDENTIFICADO = -1; //para cuando no reconoce ningún color
        static const int MARRON = 0;
		static const int VERDE = 1;
		static const int AZUL = 2;
		static const int ROJO = 3;
		static const int NARANJA = 4;
		static const int AMARILLO = 5;
		
		static const int LEER_ROJO = 1;
		static const int LEER_AZUL = 2;
		static const int LEER_VERDE = 3;

        /* Defino topes para los colores en conflicto*/
        static const int MAX_NARANJA_R = 130;
        static const int MAX_NARANJA_V = 190;
        static const int MIN_MARRON_R = 160;

		ColorRocklet(const int pin_s0, const int pin_s1, const int pin_s2, const int pin_s3, const int pin_out);

        /*
         * Llama a identificarColor().
         * Devuelve el id del color identificado.
         */
		int getColor();
		bool hacerLectura();//realiza la lectura de datos recibidos desde el sensor y los pone en los parámetros de los colores.
		void prenderSensor();
		void apagarSensor();

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
    
        void identificarColor();//llama a hacerLectura identifica el color según los parámetros leídos.
        void setColor(const int idColor); //por si es necesario
    
};

#endif
