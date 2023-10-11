#pragma once
#include <string>
#include <list>
#include "Cancion.h"
#include <queue>

struct CIDI
{

	std::string Nombre;

	int Cantcanciones;

	std::list<Cancion> Listado;

};

class CDs
{
	private:

		Cancion* CancionActual = nullptr;
	
	public:

	void LeerCarpetas(std::string direccion, std::list<CIDI> &lista, bool &exito);

	bool VerificarFormato(std::string elemento);

	void ListadoCDs(std::list<CIDI> &lista, std::queue<Cancion> &cola);

	void RevisarCola(std::queue<Cancion> cola);

	void OrdenarArtista(std::queue<Cancion> &cola, int formato);

	void OrdenarNombre(std::queue<Cancion>& cola, int formato);

	void OrdenarDuracion(std::queue<Cancion>& cola, int formato);

	void OrdenarColaPer(std::queue<Cancion>& cola);

	void ReproduccionAct(Cancion* Actual);

	void ReproduccionSig(std::queue<Cancion>& cola);

	void SetCancionActual(Cancion *song);

	Cancion* GetCancionActual();

	void ImprimirCola(std::queue<Cancion> colareproduccion);

	void Reinicio(std::queue<Cancion> &colareproduccion, std::list<CIDI> &Albumes);
};

