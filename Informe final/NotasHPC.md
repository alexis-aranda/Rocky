Vamos a reemplazar el sensor de color por una camarita 4K. Pero no queremos mas retrasos de lo que ya tenemos.
4K es muchos datos, y queremos definir el color del rocklet y su calidad (si esta deformado, manchado, etc). Este procesamiento debe ser paralelizable.
La camara puede estar conectada directamente a una PC o raspberry pi para no tener latencia de red, que le resuelva la imagen. Tambien podemos repartir procesamiento entre celular, arduino, etc.
Definir algoritmo de reconocimiento de color y forma. Marcar partes paralelizables y como las manejariamos.

El procesamiento podria hacerlo una raspberry pi con linux o una jetson.
Para reconocer formas y demas podemos usar Open CV
