Vamos a reemplazar el sensor de color por una camarita 4K. Pero no queremos mas retrasos de lo que ya tenemos.
4K es muchos datos, y queremos definir el color del rocklet y su calidad (si esta deformado, manchado, etc). Este procesamiento debe ser paralelizable.
La camara puede estar conectada directamente a una PC o raspberry pi para no tener latencia de red, que le resuelva la imagen. Tambien podemos repartir procesamiento entre celular, arduino, etc.
Definir algoritmo de reconocimiento de color y forma. Marcar partes paralelizables y como las manejariamos.

El procesamiento podria hacerlo una raspberry pi con linux o una jetson.
Para reconocer formas y demas podemos usar Open CV

# Componentes
- Camara 4K para raspberry pi https://www.e-consystems.com/13mp-autofocus-usb-camera.asp
Necesita USB 3.1. Review: https://www.youtube.com/watch?v=qMDDPlWbY2Q
- Jetson de Nvidia https://www.nvidia.com/en-us/autonomous-machines/embedded-systems-dev-kits-modules/
  - Jetson TX1: US$366
  - Jetson TX2: US$468
  - Jetson TK1: descontinuada desde Abril 2018. Usada: US$206

# Sofware
- OpenCV JS: no nos sirve para el paper, pero es una muestra de lo que se puede hacer con OpenCV https://docs.opencv.org/3.4/df/df7/tutorial_js_table_of_contents_setup.html
- Funciones OpenCV GPU https://docs.opencv.org/2.4/modules/gpu/doc/image_processing.html

# Papers
- Arquitectura Pascal http://images.nvidia.com/content/pdf/tesla/whitepaper/pascal-architecture-whitepaper-v1.2.pdf
- Arquitectura Maxwell (info) https://developer.nvidia.com/maxwell-compute-architecture
