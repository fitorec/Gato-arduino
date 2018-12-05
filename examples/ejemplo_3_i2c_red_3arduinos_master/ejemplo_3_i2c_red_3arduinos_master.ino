/*  Ejemplo 3: "Master" - Juego del Cato en 3 Arduinos en I2C
   |\_._/|         |,\__/|           |\_._/|
   | o o |MASTER   |  o o|CLIENTE O  | O O | CLIENTE X
   (  Y  )         (   T )   dir9    (  T  )    dir8
  .^`-^-'^.       .^`--^'^.         .^`-^-'^.
   ___  \_         ___  \_          ___  \_
  [(_)] |=|-------[(_)] |=|--------[(_)] |=|
   '-`  |_| I2C    '-`  |_| I2C     '-`  |_|
  /mmm/           /mmm/            /mm*/

#include <Gato.h>
#include <Wire.h>

// Creamos un objeto con el nombre gato para administrar el juego.
Gato gato;
// Arreglo de dos numeros los cuales seran las posiciones r,c
int coordenada_rc[2];
int limite_partidas = 5;
int partidas_empatadas = 0;

typedef struct {
  int address;
  char ficha;
  int ganadas;
} cliente;

cliente clients[2] = {{8, 'x', 0}, {9, 'o', 0}};


void setup() {
  Serial.begin(9600);
  Wire.begin();         // Iniciamos i2c como master
  // Cambiamos la direccion por defecto, de manera aleatoria.
  randomSeed(analogRead(A0));
  if (random(0, 31) % 2 == 1) {
    clients[0].address = 9;
    clients[1].address = 8;
  }
  Serial.println("El servidor esta iniciando...");
  for(int i = 0; i<5; i++) {
    Serial.print("Esperando: ");
    Serial.print(i+1);
    Serial.println(" Segundos");
    delay(1000);
  }
  // Les notificamos cual es la ficha que le toca a cada cliente
  for(int i = 0; i<2; i++) {
    delay(1000);
    Wire.beginTransmission(clients[i].address); // Iniciamos trasmisión a la direccion
    Wire.write('x');               // Le enviamos el codigo de la ficha - clients[i].ficha
    Wire.endTransmission();                     // Detenemos la trasmisión
    Serial.print("El jugador '");
    Serial.print(clients[i].ficha);
    Serial.print("' , se le asigno la direccion: ");
    Serial.println(clients[i].address);
  }
  gato.iniciarTurnoRandom();
}


void loop() {
  if (limite_partidas <= clients[0].ganadas ||
      limite_partidas <= clients[1].ganadas ||
      limite_partidas <= partidas_empatadas ) {
    delay(5000);             // esperamos 5 seundos.
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
  if (ficha == 'x') {
    return clients[0].address;
  }
  return clients[1].address;
}

char addressFichaSiguiente(char fichaActual) {
  return addressFichaActual((fichaActual == 'x')? 'o' : 'x');
}

void mostrarMarcador(char fin) {
  Serial.print("Juego a concluido, El ganador fue: ");
  Serial.println(fin);
  Serial.print("Marcador Actual ");
  for(int i = 0; i<2; i++) {
    Serial.print(clients[i].ficha);
    Serial.print(" = ");
    Serial.print(clients[i].ganadas);
  }
  Serial.print(", empatados: ");
  Serial.println(partidas_empatadas);
}

void pedirTiroAlClienteActual(int* indice_rc) {
  int dirActual = addressFichaActual(gato.turno);
  Serial.print("Esperando respuesta del arduino ");
  Serial.print(gato.turno);
  Serial.print(" (direccion: ");
  Serial.print(dirActual);
  Serial.println(")");
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
  switch (fin) {
    case 'x':
      clients[0].ganadas++;
      break;
    case 'o':
      clients[1].ganadas++;
      break;
    case 'e':
      partidas_empatadas++;
      break;
  }
}
