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
  * Ademas, la Activity pone en ejecucón el hilo que permite escuchar mensajes desde Arduino. Los mensajes deben comenzar con el simbolo "#", terminar con "\r\n" y los atributos pasados deben estar separados por "-"

# Otras clases
* Tools: Funciones varias que no nos parecía que fueran en las Activities
* SingletonColorPantalla: Clase tipo Singleton que controla el color de la pantalla según el brillo que el dispositivo detecte
 * La clase controla que se posea o no un sensor de luz en el dispositivo. Informa de esto al abrir la aplicación, y de no poseerlo, el fondo de pantalla de la aplicación se mantendrá blanco durante el resto de su ejecución
 * La clase solo tiene dos métodos públicos: Uno que sirve para indicarle la activity sobre la que tiene que operar, y otro para informar que la app está en pausa. El primero se usa cada vez que se ejecuta onResume(), y el segundo cada vez que se ejecuta onPause()
 * La clase implementa SensorEventListener por lo que escucha el sensor de luz del dispositivo de forma paralela a la actividad en ejecución
 * La clase cambia el color a cada vez mas oscuro cuanto menos luz detecte (Para proteger la vista) hasta llegar al negro cuando no se detecte luz. A su vez tiende al blanco conforme sube la cantidad de luz hasta los 128 Lux (Unidad de medida de luz), a partir de donde se muestra en blanco para que sea visible a pesar de la luz.
* ConnectedThread: Clase encargada de los datos de la conexión Bluetooth, es la que informa y recibe mensajes con el dispositivo
