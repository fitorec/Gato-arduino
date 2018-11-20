#ifndef Gato_h
#define Gato_h
#include "Arduino.h"

//      \    /\       __|___|__
//       )  ( ')        | X |
//      (  /  )       --|---|--
// Gato  \(__)| 
//--------------------------------------------------------

class Gato {
	public:
		Gato();
		// Acceso por Renglon y Columna
		bool r_c_val(int r, int c);
		char r_c(int r, int c);
		bool tirar(int r, int c);
		bool serialRead(int* rc_data);
		void mostrarTablero();
		bool tirarYCambiarTurno(int r, int c);
		// Acceso por Index
		bool valid(int index);
		char value(int index);
		bool set(int index, char c);
		bool disponible(int* rc_data);
		char fin();
		// Administracion del turno
		char turno;
		void iniciarTurnoRandom();
	private:
		bool index2RC(int* rc_data, int index);
		char tablero[9];
};
#endif
