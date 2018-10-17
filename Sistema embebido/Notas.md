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
| 1. En espera | Infrarrojo | Detecta     | Pasar a Buscando | Nada |
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
| 5. Tobogan A| Pulsador | Pulso largo | 1. Switch M/A        | 1. Decidir estacion |
|             |          |             | 2. Pasar a tobogan M | 2. ServoTobogan a estacion decidida|
|             |          |             |                      | 3. Esperar TAcomodar (varios loops) |
|             |          |             |                      | 4. Pasar a Despachando |
| 6. Tobogan M| Pulsador | Pulso largo | 1. Switch M/A        | 1. Chequear potenciometro |
|             |          |             | 2. Pasar a tobogan A | 2. Convertir valor |
|             |          | Pulso corto | Pasar a Despachando  | 3. ServoTobogan a valor |
|             |          |             |                      | Repetir 1, 2, 3 en cada loop |
| 7. Despachando|          |             |             | 1. ServoCinta a CAIDA_ST |
|               |          |             |             | 2. Esperar TDespacho (varios loops) |
|               |          |             |             | 3. Pasar a En espera |
| **Todos** | **Pulsador** | **Pulso largo** | **Switch M/A**  |
