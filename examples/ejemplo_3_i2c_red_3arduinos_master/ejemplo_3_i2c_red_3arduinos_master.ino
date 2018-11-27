/*
  |\      _,,,,--,,_
  /,`.-'`'    -,  \-;,     Ejemplo 3: Maestro que controla la partida y a los clientes
 |,4-  ) ),,__ ,\ (  ;;
'---''(.'--'  `-'`.)*/

#include <Gato.h>
#include <Wire.h>

// Creamos un objeto con el nombre gato para administrar el juego.
Gato gato;
// Arreglo de dos numeros los cuales seran las posiciones r,c
int coordenada_rc[2];
int ganadas_x = 0, ganadas_o = 0;
int limite_partidas = 5;

typedef struct {
  int address;
  char ficha;
  int ganadas;
} cliente;

cliente clients[2] = {{8, 'x', 0}, {9, 'o', 0}};


void setup() {
  Serial.begin(9600);
  Wire.begin();         // Iniciamos i2c como master
  delay(100);
  // Cambiamos la config. por defecto si generamos un número impar random.
  if (random(0, 31) % 2 == 1) {
    clients[0].address = 9;
    clients[1].address = 8;
  }
  // Les notificamos cual es la ficha que le toca a cada cliente
  for(int i = 0; i<2; i++) {
    Wire.beginTransmission(clients[i].address); // Iniciamos trasmisión a la direccion
    Wire.write(clients[i].ficha);               // Le enviamos el codigo de la ficha
    Wire.endTransmission();                     // Detenemos la trasmisión
    Serial.print("El jugador '");
    Serial.print(clients[i].ficha);
    Serial.print("' , tiene la dirección: ");
    Serial.println(clients[i].address);
  }
  gato.iniciarTurnoRandom();
}


void loop() {
  if (limite_partidas <= clients[0].ganadas || limite_partidas <= clients[1].ganadas ) {
    delay(5000);             // esperamos 5 segundos.
      return;
  }
  char fin = gato.fin();
  if (fin != '-') {
	  incrementarGanadas(fin);
      mostrarMarcador(fin);
      notificarResultadoALosClientes(fin);
      delay(3000);             // esperamos 3 segundos.
      gato.iniciarTurnoRandom();
      return;
  }
  pedirTiroAlClienteActual(coordenada_rc);
  Serial.print("El arduino '");
  Serial.print(gato.turno);
  Serial.print("', selecciono: ");
  gato.mostrarIndiceRC(coordenada_rc);
  if (gato.tirarYCambiarTurno(coordenada_rc[0], coordenada_rc[1])){
    notificarTiroAlRival(coordenada_rc);
    gato.mostrarTablero();
    delay(500);
  } else {
  Serial.print("Coordenada invalida");
  delay(200);
  }
}


char addressFichaActual(char ficha) {
  if(clients[0].ficha == ficha) {
    return clients[0].address;
  }
  return clients[1].address;
}
char addressFichaSiguiente(char ficha) {
  return addressFichaActual(ficha == 'x'? 'o' : 'x');
}

void mostrarMarcador(char fin) {
  Serial.print("El juego a concluido, El ganador fue: ");
    Serial.println(fin);
    Serial.print("Marcador Actual x = ");
    Serial.print(ganadas_x);
    Serial.print(", o = ");
    Serial.println(ganadas_o);
}

void pedirTiroAlClienteActual(int* indice_rc) {
  int dirActual = addressFichaActual(gato.turno);
  Wire.requestFrom(dirActual, 2);    // Solicitamos dos bytes al jugador actual
  while (!Wire.available()) {}       // Esperamos...
  indice_rc[0] = Wire.read();
  indice_rc[1] = Wire.read();
}

void notificarTiroAlRival(int* indice_rc) {
  int dirRival = addressFichaSiguiente(gato.turno);
  Wire.beginTransmission(dirRival);       // Iniciamos la trasmisión al rival
  Wire.write(indice_rc[0]);               // Enviamos la coordanada R
  Wire.write(indice_rc[1]);               // Enviamos la coordanada C
  Wire.endTransmission();                 // Detenemos la trasmisión
}

void notificarResultadoALosClientes(char fin) {
  for(int i = 0; i<2; i++) {
    Wire.beginTransmission(clients[i].address); // Iniciamos trasmisión a la direccion
    Wire.write(fin);                            // Le enviamos el codigo de la ficha
    Wire.endTransmission();                     // Detenemos la trasmisión
  }
}
void incrementarGanadas(char fin) {
	if (fin == 'e') {                           // caso de empate.
		return;
	}
   for(int i = 0; i<2; i++) {
	   if (clients[i].ficha == fin) {
		 clients[i].ganadas++;
		 return;
	   }
   }
}
