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
	// Hacemos 27 iteraciones(aleatorias) lo que serian 3 vueltas.
	for (int i = 0; i<27; i++) {
		int index = random(0, 9);
		if (tablero[index] == '-' ) {
			index2RC(indice_rc, index);
			return true;
		}
	}
	if (random(0, 2) == 1 ) {
		for (int i = 0; i<9; i++) {
			if (tablero[i] == '-' ) {
				index2RC(indice_rc, i);
				return true;
			}
		}
	} else {
		for (int i = 8; i>=0; i--) {
			if (tablero[i] == '-' ) {
				index2RC(indice_rc, i);
				return true;
			}
		}
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

char Gato::fin() {
	int r, c;
	// Revisando los renglones
	for (r = 0; r<3; r++) {
		char anterior = r_c(r, 0);
		for(c = 1; c<3; c++) {
			char actual = r_c(r, c);
			if (actual == '-' || actual != anterior) {
				break;
			}
			anterior = actual;
		}
		if ( c == 3) {
			return anterior;
		}
	}
	for (c = 0; c<3; c++) {
		char anterior = r_c(0, c);
		for(r = 1; r<3; r++) {
			char actual = r_c(r, c);
			if (actual == '-' || actual != anterior) {
				break;
			}
			anterior = actual;
		}
		if ( c == 3) {
			return anterior;
		}
	}
	// Revisa las diagonales
	char centro = r_c(1, 1);
	if (r_c(0, 0) == centro && r_c(2, 2) == centro) {
		return centro;
	}
	if (r_c(0, 2) == centro && r_c(2, 0) == centro) {
		return centro;
	}
	return '-';
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
