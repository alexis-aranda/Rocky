# Servos

ServoCinta: 3 estaciones
- Recepción ---> RECEPCION_SC
- Sensor de Color --->COLOR_SC
- Punto de caida ---> CAIDA_ST

ServoTobogan: 6 estaciones
- ST_1, ST_2, ST_3, ST_4, ST_5, ST_6 

Colores: Rojo, Celeste, Naranja, Verde, Marrón, Amarillo

# Objetos
- 2 servos (pueden ser de la misma clase)
- Barrera infrarroja
- Sensor color
- Pulsador
- Potenciómetro
- LED

# Funciones
- Barrera infrarroja: dice si hay rocklets para clasificar
- Pulsador:
  - Modo manual: toque tira rocklet y pulso largo vuelve a automatico
  - Modo automatico: toque pasa a manual
- Potenciometro:
  - Modo manual: maneja tobogan
- LED:
   - Durante todo el recorrido, excepto cuando muevo el tobogán (manual): prende y apaga
   - Al mover el tobogán (manual): intensidad según valor del potenciómetro
   - Esperando rocklets (auto): sube y baja intensidad suavemente
   - Al reconocer color (auto): se enciende
   - Al tirar rocklets (auto): apago

# Estados
1. Esperando rocklet
2. Yendo a buscar rocklet
3. Recibiendo rocklet
4. Llevando rocklet a sensar
5. Sensando color de rocklet
6. Decidiendo
7. Acomodando tobogan
8. 
