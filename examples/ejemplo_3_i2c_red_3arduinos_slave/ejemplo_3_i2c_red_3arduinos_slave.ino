/*    ___
  _.-|   |          |\__/,|   (`\   Ejemplo 3: Esclavo
 {   |   |          |o o  |__ _) )
 "-.|___|        _.( T   )  `  /      Se recomienda al lector revisar la función
  .--'-`-.     _((_ `^--' /_<  \      tirar que se encuentra hasta abajo.
.+|______|__.-||__)`-'(((/  ((*/

#include <Gato.h>
#include <Wire.h>

Gato gato;                    // Objeto Gato para administrar el juego
int address = 8;              // Cambiar la dirección al 9, para el cliente 2
int mi_ficha = '-';           // Este valor se obtiene del maestro
int ganadas = 0;              // Lleva la cuenta de partidas gandas
int indice_rc[2] = {-1, -1};  // Nos ayuda a administrar las coordenadas r,c.

void setup() {
  Serial.begin(9600);
  Wire.begin(address);          // Iniciamos i2c como esclavo.
  Wire.onRequest(enviarTiro);       // Registramos las solicitudas
  Wire.onReceive(recibirDelMaster); // Registramos las recepciones
  gato.iniciarTurnoRandom();
}

void loop() {}

/**
 * Se encarga de recibir los mensajes del Maestro
 */
void recibirDelMaster(int numCaracteres) {
  if (numCaracteres == 1 && mi_ficha == '-' ) { // Inicio
    mi_ficha = Wire.read();     // Recibimos la ficha obtenida del Master
    Serial.print("Soy cliente: ");
    Serial.println(mi_ficha);
    return;
  }
  if (numCaracteres == 1) {           // Alguien gano.
    char ganador = Wire.read();     // Recibimos la ficha obtenida del Master
    if (mi_ficha == ganador ) {
      Serial.print("Bien ya vamos ganando: ");
      Serial.print(++ganadas);
      Serial.println(" Partidas");
    }
    gato.iniciarTurnoRandom();
    return;
  }
  if (numCaracteres != 2) { // Inicio
    return;
  }
  char r = Wire.read();
  char c = Wire.read();
  gato.tirarYCambiarTurno(r, c);
  gato.mostrarTablero();
}

/**
 * Se encarga de enviarle al Maestro el resultado de un tiro.
 */
void enviarTiro() {
  if (gato.turno != mi_ficha ) {
    gato.turno = mi_ficha;
  }
  tirar();                  // Obtenemos un tiro.
  Wire.write(indice_rc[0]); // Enviamos el R
  Wire.write(indice_rc[1]); // Enviamos el C
}

/**
 * Aqui se debe de implementar tú logica de juego :¬D
 */
bool tirar() {
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
