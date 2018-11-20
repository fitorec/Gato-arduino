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
		void limpiarTablero();
		void iniciarTurnoRandom();
		// Acceso por Renglon y Columna
		bool r_c_val(int r, int c);
		char r_c(int r, int c);
		bool tirar(int r, int c);
		bool serialRead(int* indice_rc);
		void mostrarTablero();
		void mostrarIndiceRC(int* indice_rc);
		bool tirarYCambiarTurno(int r, int c);
		int fichasDisponibles();
		// Acceso por Index
		bool valid(int index);
		char value(int index);
		bool set(int index, char c);
		bool disponible(int* indice_rc);
		char fin();
		// Administracion del turno
		char turno;
		bool index2RC(int* indice_rc, int index);
	private:
		char tablero[9];
};
#endif
