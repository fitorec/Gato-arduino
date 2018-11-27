/*   /\-/\      
    /a a  \                                 _
   =\ Y  =/-~~~~~~-,_______________________/ )
     '^--'          ________________________/
       \           /       Ejemplo 3: Esclavo
       ||  |---'\  \       Se recomienda al lector revisar la función
      (_(__|   ((__|       tirar que se encuentra hasta abajo.
*/
#include <Gato.h>
#include <Wire.h>

// Creamos un objeto con el nombre gato para administrar el juego.
Gato gato;
// Arreglo de dos numeros los cuales seran las posiciones r,c
int address = 8;
int ficha = 'z';         // este valor se obtiene del maestro
int ganadas = 0;
int indice_rc[2] = {-1, -1};

void setup() {
	Serial.begin(9600);
	Wire.begin(address);		      // Iniciamos i2c como esclavo.
	Wire.onRequest(enviarTiro);       // Registramos las solicitudas
	Wire.onReceive(recibirDelMaster); // Registramos las recepciones
	gato.iniciarTurnoRandom();
}

void loop() {
}
void recibirDelMaster(int numCaracteres) {
	if (numCaracteres == 1 && ficha == 'z' ) { // Inicio
		ficha = Wire.read();     // Recibimos la ficha obtenida del Master
		Serial.print("Yo soy cliente: ");
		Serial.println(ficha);
		return;
	}
	if (numCaracteres == 1) {           // Alguien gano.
		char ganador = Wire.read();     // Recibimos la ficha obtenida del Master
		if (ficha == ganador ) {
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

void enviarTiro() {
	if(gato.turno != ficha ) {
		gato.turno = ficha;
	}
	tirar();                  // Obtenemos un tiro.
    Wire.write(indice_rc[0]); // Enviamos el R
    Wire.write(indice_rc[1]); // Enviamos el C
}

// Aqui implementar tú logica de juego :¬D
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
