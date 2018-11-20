/*                            Ejemplo 2: Arduino VS Arduino en una Tarjeta
  |\      _,,,,--,,_                    Se recomienda redefinir las funciones:
  /,`.-'`'    -,  \-;,                  - tirarX
 |,4-  ) ),,__ ,\ (  ;;                 - tirarY
'---''(.'--'  `-'`.)*/

#include <Gato.h>
// Creamos un objeto con el nombre gato para administrar el juego.
Gato gato;
// Arreglo de dos numeros los cuales seran las posiciones r,c
int coordenada_rc[2];

void setup() {
  Serial.begin(9600);
  Serial.println("Inicio del Juego");
  gato.iniciarTurnoRandom();    // Iniciamos de forma aleatoria
}

void loop() {
  gato.mostrarTablero();        // Mostramos el tablero
   if (gato.fin() != '-' ) {
      Serial.println("El juego a concluido, El ganador fue: ");
      Serial.println(gato.fin());
      delay(30000);             // esperamos 30 segundos.
      return;
   }
   Serial.print("Le toca al Arduino: ");
   Serial.println(gato.turno);
   if (gato.turno == 'x') {
      tirarX(coordenada_rc);
   } else {
      tirarY(coordenada_rc);
   }
   Serial.print("El arduino Selecciono: ");
   gato.mostrarIndiceRC(coordenada_rc);
   gato.tirarYCambiarTurno(coordenada_rc[0], coordenada_rc[1]);
   delay(random(1, 4) * 700); // Nos esperamos un rato de forma aleatoria
}

bool tirarX(int* indice_rc) {
    // Aqui implementar tú logica de juego.
    indice_rc[0] = -1;
    indice_rc[1] = -1;
    for (int i = 0; i<50; i++) {
      int index = random(0, 9);
      if (gato.value(index) == '-' ) {
        gato.index2RC(indice_rc, index);
        return true;
      }
  }
  return false;
}
bool tirarY(int* indice_rc) {
    return gato.disponible(indice_rc);     // Obtenemos un disponible random
}
