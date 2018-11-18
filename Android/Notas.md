# Funcionalidades
* Pausa y reanudación del clasificador de rocklets
* Cuenta rocklets por vaso (Estadisticas, graficos de colores de rocklets si hay tiempo)
* Se informa el modo del embebido
* Nuevo modo para el embebido: ManualConGiroscopio
* En modo ManualConGiroscopio, mover el tobogan con el giroscopo. Para que no entre en conflicto con el potenciometro, va a ser un modo especifico con mas prioridad que los modos anteriores
* En modo ManualConGiroscopio se tira el rocklet acercando la mano a la pantalla
* La aplicación cambia la tonalidad del fondo de pantalla para ajustarse a la luz de la habitación, en todas sus pantallas

# Activities
* MainActvity: Activity principal, la cual sirve para conectarse al embebido. Posee tres botones:
  * Activar/Desactivar: Sirve para activar o desactivar el Bluetooth.
  * Dispositivos Emparejados: Sirve para conectarse al embebido una vez emparejado.
  * Buscar dispositivos: Sirve para buscar el embebido y emparejar con el, para posteriormente conectar.
* DeviceListActivity: Activity que sirve para conectar con un dispositivo emparejado, lee del SO cuantos otros dispositivos existen emparejados, y por cada uno muestra dos botones:
  * Iniciar: Inicia la conexión y abre la Activity para monitorear y operar el embebido. Si no recibe la comunicación esperada (Si el embebido falla o se intenta conectar a otro dispositivo) se lanza un mensaje de error de conexión.
  * Emparejar/Desemparejar: Literalmente eso
  * Buscar Dispositivos: Busca los dispositivos Bluetooth y permite emparejar.
* Funciones: Actividad central del programa, nos informa de la cantidad de rocklets en los vasos, por color y del modo actual del embebido (Manual o Automatico). Ademas tiene los siguientes botones:
  * Pausar/Reanudar: Permite pausar o reanudar el funcionamiento del embebido
  * Mover Tobogan: Permite al embebido entrar el en modo "ManualConGiroscopio" el cual nos permite: Mover el tobogan utilizando el groscopio del dispositivo, y soltar el rocklet acercando nuestra mano a la pantalla
