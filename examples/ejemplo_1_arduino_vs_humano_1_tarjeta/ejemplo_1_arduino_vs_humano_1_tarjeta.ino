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
   if (gato.turno == 'x') {
     Serial.println("Le toca al Arduino");
     // Mostramos la ficha seleccionada por la tarjeta Arduino
     tirar(coordenada_rc);
     Serial.print("El arduino Selecciono: ");
     tirar(coordenada_rc);
     delay(random(1, 4) * 500); // Nos esperamos un rato de forma aleatoria
   } else {
    Serial.println("Le toca al Humano");
     gato.serialRead(coordenada_rc);     // Leemos los datos del usuario desde el puerto Serial (r,c).
     
   }
   gato.tirarYCambiarTurno(coordenada_rc[0], coordenada_rc[1]);
   delay(random(1, 4) * 500); // Nos esperamos un rato de forma aleatoria
}

bool tirar(int* indice_rc) {
    // Aqui implementar tú logica de juego.
    gato.disponible(indice_rc);     // Obtenemos un disponible random
}
