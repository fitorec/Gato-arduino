#include "Arduino.h"
#include "Gato.h"

// Constructor para la clase Gato
Gato::Gato() {
	turno = 'x'; // por default.
	for(int i = 0; i<9; i++) {
		tablero[i] = '-';
	}
}

void Gato::iniciarTurnoRandom() {
	turno = (random(0, 2) == 1)? 'x': 'o';
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

bool Gato::serialRead(int* rc_data) {
	rc_data[0] = -1; rc_data[0] = -1;
	if (Serial.available() != 3) {
		return false;
	}
	rc_data[0] = Serial.parseInt();
	Serial.read();
	rc_data[1] = Serial.parseInt();
	return true;
}

// Se encarga de mostrar el tablero.

void Gato::mostrarTablero() {
	for(int r=0; r<3; r++) {
		for(int c=0; c<3; c++) {
			Serial.print("  ");
			Serial.print(tablero[3*r + c]);
		}
		Serial.print(" <- ");
		Serial.println(r+1);
	}
}


bool Gato::disponible(int* rc_data) {
	rc_data[0] = -1; rc_data[0] = -1;
	for (int i = 0; i<50; i++) {
		int index = random(0, 9);
		if (tablero[index] == '-' ) {
			index2RC(rc_data, index);
		}
	}
	for (int i = 0; i<9; i++) {
		if (tablero[i] == '-' ) {
			index2RC(rc_data, i);
		}
	}
}

bool Gato::index2RC(int* rc_data, int index) {
	rc_data[0] = -1; rc_data[0] = -1;
	if (index<0 || index>8) {
		return false;
	}
	rc_data[0] = index/3;
	rc_data[1] = index%3;
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
