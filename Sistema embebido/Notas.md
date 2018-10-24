# Servos

ServoCinta: 3 estaciones
- Recepción ---> RECEPCION_ST
- Sensor de Color --->COLOR_ST
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
| Estados | Sensores a chequear | Si... | ... hacer | Accion
| --- | --- | --- | --- | --- |
| 1. En espera | Infrarrojo | Detecta  | Pasar a Buscando | Nada |
| 2. Buscando |          |             |             | 1. ServoCinta a RECEPCION_ST |
|             |          |             |             | 2. Esperar TBuscar (varios loops) |
|             |          |             |             | 3. Pasar a Llevando |
| 3. Llevando |          |             |             | 1. ServoCinta a COLOR_ST |
|             |          |             |             | 2. Esperar TLlevar (varios loops) |
|             |          |             |             | 3. Pasar a Sensando |
| 4. Sensando |          |             |             | 1. Identificar color (varios loops?) |
|             |          |             |             | 2. Informar color a app (proximamente) |
|             |          |             |             | 3. Si M: pasar a Tobogan M |
|             |          |             |             | 4. Si A: pasar a Tobogan A |
| 5. Tobogan A| Pulsador | *Pulso largo* | 1. *Switch M/A*    | 1. Decidir estacion |
|             |          |             | 2. Pasar a tobogan M | 2. ServoTobogan a estacion decidida|
|             |          |             |                      | 3. Esperar TAcomodar (varios loops) |
|             |          |             |                      | 4. Pasar a Despachando |
| 6. Tobogan M| Pulsador | *Pulso largo* | 1. *Switch M/A*    | 1. Setear LED en modo INTENSIDAD VARIABLE|
|             |          |             | 2. Pasar a tobogan A | 2. Chequear potenciometro |
|             |          | Pulso corto | Pasar a Despachando  | 3. Convertir valor|
|             |          |             |                      | 4. ServoTobogan a valor 
|             |          |             |                      | 5. *Activar LED* con valor
|             |          |             |                      | Repetir 2, 3, 4, 5 en cada loop |
| 7. Despachando|          |             |             | 1. ServoCinta a CAIDA_ST |
|               |          |             |             | 2. Esperar TDespacho (varios loops) |
|               |          |             |             | 3. Pasar a En espera |
| **Todos** | **Pulsador** | **Pulso largo** | **Switch M/A**  | **Activar LED** (en cada loop)


Switch M/A
- Si estaba en Manual, setea modo Automatico y viceversa
- Setear modo LED

Setear modo LED
- Si Tobogan M: INTENSIDAD_VARIABLE; return;
- Si Manual: PRENDE_APAGA
- Si Automatico
  - Si En espera: SOFT_PWM
  - Si Tobogan A o Despachando: SIEMPRE_PRENDIDO
  - Si Buscando o Llevando o Sensando: SIEMPRE_APAGADO
  
Pasar a Buscando:
- si Automatico: LED en SIEMPRE_APAGADO

Pasar a Tobogan M:
- LED en INTENSIDAD_VARIABLE

Pasar a Tobogan A:
- LED en SIEMPRE_PRENDIDO

Pasar a Despachando:
- si Manual: LED en PRENDE_APAGA

Pasar a En espera:
- si Automatico: LED en SOFT_PWM

# Alimentacion
Notas de los voltajes necesarios para cada componente

| Componente | Modelo | Voltaje |
| ---------- | ------ | ------- |
| ServoTobogan (azul) | TowerPro SG90 Micro Servo | 4.8 - 6 |
| ServoCinta (negro) | Small servo del Arduino Starter Kit | 5 |
| Sensor color | TCS3200 | 2.7 - 5.5 |

# PWM
Los pines digitales que pueden trabajar con PWM son: 3, 5, 6, 9, 10 y 11.
Casi todos trabajan a 490 Hz, salvo 5 y 6 que trabajan a 980 Hz.
5 y 6 no los usamos con servos, porque el valor PWM lo testeamos con 490 Hz.

# Pines

| Componente | Pata | Pin | Modo |
| --- | --- | --- | --- |
| Laser | S | D2 | OUTPUT |
| Detector de flama | D0 | D3 | INPUT |
| Sensor color | S0 | D4 | OUTPUT |
|              | S1 | D5 | OUTPUT |
|              | S2 | D6 | OUTPUT |
|              | S3 | D7 | OUTPUT |
|              | OUT | D8 | INPUT |
| LED          | larga | D9 | OUTPUT |
| ServoTobogán | sig | D10 | OUTPUT |
| ServoCinta    | sig | D11 | OUTPUT |
| Potenciómetro | sig | A0 | INPUT |
