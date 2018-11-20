#include <Gato.h>
// Creamos un objeto con el nombre gato para administrar el juego.
Gato gato;
// Arreglo de dos numeros los cuales seran las posiciones
int tiro[2];

void setup() {
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
     gato.disponible(tiro);     // Obtenemos un disponible random
     delay(random(1, 4) * 500); // Nos esperamos un rato de forma aleatoria
   } else {
    Serial.println("Le toca al Humano");
     gato.serialRead(tiro);     // Leemos los datos del usuario desde el puerto Serial (r,c).
   }
   gato.tirarYCambiarTurno(tiro[0], tiro[1]);
}
