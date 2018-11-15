Ideas:

* mover el tobogán usando el acelerómetro o el giróscopo. (modo manual)
* cambiar de modo cuando el celu está boca abajo, puede ser con el sensor de luz o con el sensor de distancia (con el acelerómetro sería complicado al pedo).
* tirar el rocklet en X vaso cuando se detecta que se sacudió el celu.
* variante de la anterior, shake para tirar el rocklet en modo manual
* cambiar colores segun luz ambiente (por ejemplo, colores brillantes y alto contraste cuando hay mucha luz, y colores oscuros cuando es de noche)

# Funcionalidades
* Pausa y play del clasificador de rocklets (boton activity)
* Muestra cuenta rocklets por vaso. La cuenta la hace el Arduino.
* Estadisticas, graficos de colores de rocklets
* Vasos tienen un limite. Lo cuenta el arduino y se lo informa a android. Pausa si un vaso se llena y muestra error, reanuda cuando le digo que lo solucione y reinicia la cuenta de ese vaso.
* En modo Manual, mover el tobogan con el giroscopo.
* En modo Manual, usar proximidad para tirar rocklet.
* Sensor de luz define el tono del fondo.
* Mostrar por pantalla si esta en modo Automatico, Manual o Celular.

Cuando pongo modo Celular, no importa si estaba Automatico o Manual, se maneja con el celu. Cuando salgo de modo Celular, usa el que tenga. Mientras estoy en modo Celular el boton puede cambiar entre Manual y Automatico, pero los cambios se van a ver recien cuando salga del celu.
