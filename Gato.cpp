#include "Arduino.h"
#include "Gato.h"

// Constructor para la clase Gato
Gato::Gato() {
	turno = 'x'; // por default.
	limpiarTablero();
}

void Gato::iniciarTurnoRandom() {
	turno = (random(0, 31)%2 == 1)? 'o': 'x';
	limpiarTablero();
}

void Gato::limpiarTablero() {
	for(int i = 0; i<9; i++) {
		tablero[i] = '-';
	}
}

bool Gato::r_c_val(int r, int c) {
	if (r<0 || r>2 || c<0 || c>2) {
		Serial.print("Error: fila_columna("); Serial.print(r);
		Serial.print(" ,"); Serial.print(c); Serial.print(");");
	return false;
	}
	return true;
}

char Gato::r_c(int r, int c) {
	if(!r_c_val(r, c)) {
		return '-';
	}
	return tablero[3*r + c];
}


// Intenta cambiar el valor(v) en las coordenas (r,c), si no se puede
// devuelve false.
bool Gato::tirar(int r, int c) {
	if(!r_c_val(r, c) || r_c(r, c) != '-') {
		return false;
	}
	tablero[3*r + c] = turno;
	return true;
}

bool Gato::tirarYCambiarTurno(int r, int c) {
	if (!tirar(r, c)) {
		return false;
	}
	turno = (turno == 'x')? 'o': 'x';
	return true;
}
/**
 * Muestre un indice par, un indice en coordenas renglón columnas:
 **/
void Gato::mostrarIndiceRC(int* indice_rc) {
	Serial.print("(");
	Serial.print(indice_rc[0]);
	Serial.print(" , ");
	Serial.print(indice_rc[1]);
	Serial.print(")\n");
}
/**
 * Lee del puerto Serial la columna y Renglón para tirar.
 * Revisa que estas coordenadas sean validas.
 **/
bool Gato::serialRead(int* indice_rc) {
	Serial.println("Inserte el valor del Renglón: ");
	while(Serial.available() == 0) {
	}
	indice_rc[0] = Serial.parseInt();
	Serial.println("Inserte el valor de la Columna: ");
	while(Serial.available() == 0) {
	}
	indice_rc[1] = Serial.parseInt();
	Serial.print("Caracteres leidos: ");
	mostrarIndiceRC(indice_rc);
	return true;
}

// Se encarga de mostrar el tablero.

void Gato::mostrarTablero() {
	Serial.print("\n============ Tablero Actual ==================\nC→");
	for(int r=0; r<3; r++) {
		Serial.print(r);
		Serial.print("  ");
	}
	Serial.println("R↓");
	for(int r=0; r<3; r++) {
		for(int c=0; c<3; c++) {
			Serial.print("  ");
			Serial.print(tablero[3*r + c]);
		}
		Serial.print(" ◄ ");
		Serial.println(r);
	}
}

/**
 * Intenta buscar aleatoriamente una ficha disponible.
 **/
bool Gato::disponible(int* indice_rc) {
	index2RC(indice_rc, -1); // hacemos -1 el renglon y columna
	int num_disponibles = 0;
	int disponibles[9];
	for (int i = 0; i<9; i++) {
		if (tablero[i] == '-' ) {
			disponibles[(num_disponibles++)] = i;
		}
	}
	if (num_disponibles > 0) {
		int randomIndex = random(0, num_disponibles);
		index2RC(indice_rc, disponibles[randomIndex]);
		return true;
	}
	return false;
}

/**
 * Convierte un indice de 0-9, a indice par (Renglon, Columna)
 **/
bool Gato::index2RC(int* indice_rc, int index) {
	indice_rc[0] = -1; indice_rc[0] = -1;
	if (index<0 || index>8) {
		return false;
	}
	indice_rc[0] = index/3;
	indice_rc[1] = index%3;
	return true;
}

/**
 * Regresa un caracter para indicar si la partida a concluido:
 *  x: Gano el jugador x
 *  o: Gano el jugador o
 *  e: No hay fichas disponibles, el juego concluyó en empate.
 */
char Gato::fin() {
	int r, c;
	char ch;
	// Revisando los renglones
	for (int i = 0; i<3; i++) {
		ch = r_c(i, 0);
		// Revisar el renglon
		if (ch == r_c(i, 1) && ch == r_c(i, 2) && ch != '-') {
			return ch;
		}
		// Revisando la columna
		ch = r_c(0, i);
		if (ch == r_c(1, i) && ch == r_c(2, i) && ch != '-') {
			return ch;
		}
	}
	// Revisa las diagonales
	ch = r_c(1, 1); // el elemento del centro.
	if (ch != '-') {
		if (r_c(0, 0) == ch && r_c(2, 2) == ch) {
			return ch;
		}
		if (r_c(0, 2) == ch && r_c(2, 0) == ch) {
			return ch;
		}
	}
	// Revisar si hay fichas disponibles
	for (int i = 0; i<9; i++) {
		if (tablero[i] == '-' ) {
			return '-'; // Hay disponibles.
		}
	}
	return 'e';
}

int Gato::fichasDisponibles() {
	int num_disponibles = 0;
	for (int i = 0; i<9; i++) {
		if (tablero[i] == '-' ) {
			num_disponibles++;
		}
	}
	return num_disponibles;
}

// Funciones de indice
bool Gato::valid(int index) {
	if (index<0 || index>8) {
		return true;
	}
	return false;
}

char Gato::value(int index) {
	if (index<0 || index>8) {
		return true;
	}
	return tablero[index];
}

bool Gato::set(int index, char c) {
	if (index<0 || index>8) {
		return false;
	}
	if (tablero[index] == '-' || (c != 'x' && c != 'o')) {
		return false;
	}
}
