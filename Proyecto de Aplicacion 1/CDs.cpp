#include "pch.h"
#include "CDs.h"
#include <fstream>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <iomanip>

using namespace std;

void CDs::LeerCarpetas(std::string direccion, std::list<CIDI>& lista, bool &exito)
{
		//Declaracion de Variables
		string elem,name,anterior;
		DIR* Directorio;
		CIDI CDS;
		Cancion Song;
		struct dirent *elementos;
		int cantidad = 0;


		//Verifica si hay un archivo o carpeta en la dirección ingresada 
		if(Directorio = opendir(direccion.c_str()))
		{
			//Bucle While que se ejecutará siempre que haya elementos por leer en el directorio
			while (elementos = readdir(Directorio))
			{
				//Ignora los puntos que aparecen al leer un directorio	
				if (string(elementos->d_name) != "." && string(elementos->d_name) != "..")
				{
					//Encuentra la posicion en la cadena de caracteres donde se ubica el .txt del nombre del archivo
					size_t pos = string(elementos->d_name).find_last_of(".");

					//Elimina el ".txt" del nombre del archivo
					CDS.Nombre = string(elementos->d_name).substr(0, pos);

					//Realiza un Reset de la estructura "CD"
					CDS.Cantcanciones = 0;
					CDS.Listado.clear();
					
					//Guarda como variable "string" la ruta de dirección del elemento para poder abrirlo
					string rutacompleta = direccion + "/" + elementos->d_name;
				
					//Abre el archivo .txt
					ifstream archivo(rutacompleta);

					//Verifica si el archivo esta vacio
					if (archivo.peek() != char_traits<char>::eof())
					{
						//Lee cada linea de texto
						while (getline(archivo, elem))
						{
							//Valida si la linea de codigo cumple el formato o si es una linea repetida
							if (VerificarFormato(elem) && anterior != elem)
							{
								anterior = elem;
								int separador = 0;
								string phrase = "";

								//Recorre cada letra de la linea de texto, obteniendo el nombre, artista y duracion de la cancion
								for (int i = 0; i < elem.length(); i++)
								{

									if (elem[i] != '|')
									{
										phrase += elem[i];

									}
									else
									{

										if (separador == 0)
										{
											Song.NombreC = phrase;


										}
										else if (separador == 2)
										{
											Song.Artista = phrase;


										}

										phrase = " ";
										separador++;
									}

									if (i == elem.length() - 1)
									{
										Song.duracion = phrase;

									}
								}

								Song.Album = CDS.Nombre;

								//Ingresa la canción en el CD
								CDS.Listado.push_back(Song);

								//Contador de las canciones en el CD
								cantidad++;

								//Imprime los errores que pueden presentarse
							}else if(anterior == elem)
							{
								
								cerr << "\x1b[31mERROR\x1b[0m, Archivo: " << "\x1b[31m" << CDS.Nombre << "\x1b[0m" << "; Tipo de Error: \x1b[31mLinea repetida\x1b[0m" << "; Numero de Linea: " << "\x1b[31m" << (cantidad + 1) << "\x1b[0m";
								exito = false;
								return;

							}else
							{
								cerr << "\x1b[31mERROR\x1b[0m, Archivo: " << "\x1b[31m" << CDS.Nombre << "\x1b[0m" <<"; Tipo de Error: \x1b[31mFormato de linea incorrecto\x1b[0m"<< "; Numero de Linea: " << "\x1b[31m" << (cantidad + 1) << "\x1b[0m";
								exito = false;
								return;
							}
						}
					}else
					{
						cerr << "ERROR, Archivo: " << "\x1b[31m"<<CDS.Nombre<<"\x1b[0m"<<"; Tipo de Error: \x1b[31mArchivo vacio\x1b[0m.";
						exito = false;
						return;
					
					}

					CDS.Cantcanciones = cantidad;
					cantidad = 0;

					//Por ultimo, Ingresa el "CD" a la lista de CD's global
					lista.push_back(CDS);
				}

				
			}
	
		}else
		{
			cerr << "ERROR,\x1b[31mNo se pudo abrir el directorio\x1b[0m";
			exito = false;
			return;
		}

		//Cierra el directorio
		closedir(Directorio);

		//Bool que declara que si se completo el proceso
		exito = true;
		return;

	}


	bool CDs::VerificarFormato(std::string elemento)
	{
		int separadores = 0;

		//Recorre la linea, verificando que solo se encuentren 4 separadores, y que haya alguna palabra antes o despues de los separadores
		for(size_t i = 0; i < elemento.length(); i++)
		{
			if(elemento[i] == '|')
			{
				separadores++;

				//Verifica si hay 3 separadores juntos o si detras del primer separador no hay nada
				if(separadores % 2 != 0)
				{
					if(elemento[i+2] == '|')
					{
						return false;

					}else if(elemento[i-1] == ' ')
					{
						return false;
					}
				
				}
			}
		}
	
		//Verifica si la duracion de la cancion, se tratan de numeros y no otro simbolos
		if(separadores == 4)
		{
			//Encuentra en que posición esta el último separador
			size_t pos = elemento.find_last_of('|');
			int contadorPuntos = 0;

			//Crea un string a partir del espacio siguiente del separador hasta el final de la linea de texto
			string duracion = elemento.substr(pos+1, elemento.length());

			//Recorre el String creado y valida el formato
			for (char c : duracion)
			{
				if(!isdigit(c))
				{
					if(c == ':')
					{
						contadorPuntos++;
						if(contadorPuntos > 1)
						{
							return false;
						}
					}else
					{
						return false;
					}	
				}
				
			}

		}else
		{
			return false;
		}


		//Si pasa todas las pruebas, significa que si sigue el formato estipulado
		return true;
	
	}

	void CDs::ListadoCDs(std::list<CIDI> &lista, std::queue<Cancion> &cola)
	{
		//Limpia la pantalla
		system("cls");

		//Declaracion de las variables
		int NoCD,songSelected;

		//Vector que contendrá la lista de CDs
		vector<CIDI> SeleccionCD;

		//Copia la lista del argumento al vector declarado
		SeleccionCD.assign(lista.begin(), lista.end());
	
		
		while (true)
		{
			//Muestra el mensaje para seleccionar un CD
			cout << "Escoja un CD" << "\n";

			//Imprime la lista de CDs
			for (int i = 0; i < SeleccionCD.size(); i++)
			{
				cout << (i+1) << ". " << SeleccionCD[i].Nombre << "\n";

			}

			//Limpia el buffer de entrada de la funcion cin
			cin.ignore(999, '\n');

			//Solicita al usuario que elija un CD
			cout << "CD: ";
			cin >> NoCD;
			cout << "\n";

			//Verifica si la entrada es válida
			if (cin.fail() || NoCD < 1 || NoCD > 13)
			{
				cin.clear();
				system("cls");
				cerr << "\x1b[31mOpcion invalida. Por favor, ingrese un numero entre 1 y 13.\x1b[0m" << endl;
				cout << "\n";
				continue;
			}
			else
			{
				system("cls");

				//Vector que almacena la lista de canciones del CD seleccionado
				vector<Cancion>SeleccionCancion;

				NoCD = NoCD - 1;

				while (true)
				{
					cout << "Del \033[34mCD\033[0m:" << SeleccionCD[NoCD].Nombre << "\n" << "\n";
					cout << "Escoja un cancion" << "\n";

					//Copia el listado de canciones del CD seleccionado al nuevo vector
					SeleccionCancion.assign(SeleccionCD[NoCD].Listado.begin(), SeleccionCD[NoCD].Listado.end());

					//Imprime el listado de canciones
					for (int i = 0; i < SeleccionCancion.size(); i++)
					{
						cout << (i + 1) << ". " << SeleccionCancion[i].NombreC << "\n";
					}

					//Se repite el mismo procedmiento en la seleccion del elemento
					cin.ignore(999,'\n');

					cout << "Cancion: ";
					cin >> songSelected;
					cout << "\n";

					if (cin.fail() || songSelected < 1 || songSelected > SeleccionCancion.size())
					{
						cin.clear();
						system("cls");
						cerr << "\x1b[31mOpcion	invalida. Por favor, ingrese un numero dentro del rango de canciones.\x1b[0m" << std::endl;
						cout << "\n";
					}
					else
					{
						songSelected = songSelected - 1;
						//Si logra seleccionar una cancion correctamente, la cancion se añade a la cola principal
						cola.push(SeleccionCancion[songSelected]);
						break;
					}
				}

				break;
			}
		}

	}


	void CDs::RevisarCola(std::queue<Cancion> cola)
	{
		//Limpia la pantalla
		system("cls");
		
		//Decalaracion de variable que almacena la opcion seleccionada por el usuario
		int opcion;

		//Comprueba si la cola esta vacia
		if(cola.empty())
		{
			cout << "No se han agregado canciones a la Cola de Reproduccion......";
		
		}else
		{
			while (true)
			{
				//Limpia el bufer de entrada de Cin
				cin.ignore(9999, '\n');

				//Imprime el menu
				cout << "¿Como desea ver la Cola de Reproduccion?" << endl;
				cout << "1. Por Nombre del artista" << endl;
				cout << "2. Por Nombre de la cancion" << endl;
				cout << "3. Por Duracion de la cancion" << endl;
				cout << "4. Sin ordenar" << endl;
				cout << "Opcion: ";
				cin >> opcion;

				//Validacion de la entrada del usuario
				if (cin.fail() || opcion < 1 || opcion > 4)
				{
					cin.clear();
					system("cls");
					cerr << "\x1b[31mOpcion	invalida. Por favor, ingrese un numero entre 1 y 4\x1b[0m" << std::endl;
					cout << "\n";
					continue;

				}
				else
				{
					//Segun la opcion escogida, ordena segun el artista, nombre, duracion o sin ordenar
					switch (opcion)
					{
					case 1:

						OrdenarArtista(cola, 1);

						break;

					case 2:

						OrdenarNombre(cola, 1);

						break;

					case 3:

						OrdenarDuracion(cola, 1);

						break;

					case 4:

						ImprimirCola(cola);

						break;
					}
					break;
				}
			}
		}

		return;
	}



	//Funcion booleana que comprueba el orden alfabetico del atributo
	bool Comparacion(const Cancion& pri, const Cancion& second)
	{
		return pri.Artista < second.Artista;

	}


	void CDs::OrdenarArtista(queue<Cancion> &cola, int formato)
	{
		//Vector para almacenar temporalmente las canciones
		vector<Cancion> Artista;

		//Transfiere los elementos de la cola a un vector
		while(!cola.empty())
		{
			Artista.push_back(cola.front());
			cola.pop();
		}

		//Ordena el vector en orden alfabetico según el nombre del artista
		sort(Artista.begin(), Artista.end(), &Comparacion);

		//Determinar si el orden es descendente o ascendente
		if(formato == 1)
		{
				for(const auto& Cancion:Artista)
				{
					//Si es orden ascendente (A a Z), agrega los elementos a la
					//cola de reproduccion
					cola.push(Cancion);

				}
		}
		else
		{
			//Si el orden es descendente (Z a A), invierte los elementos del vector 
			reverse(Artista.begin(), Artista.end());
				
			for(const auto& Cancion:Artista)
			{
				//Agrega los elementos del vector a la cola de reproduccion
				cola.push(Cancion);
			}
		}

		//Limpia el vector creado
		Artista.clear();

		//Imprime la cola de reproduccion
		ImprimirCola(cola);

		return;
	}



	//Funcion booleana que comprueba el orden alfabetico del atributo
	bool ComparacionNombre(const Cancion& pri, const Cancion& second)
	{
		return pri.NombreC < second.NombreC;

	}


	void CDs::OrdenarNombre(queue<Cancion> &cola, int formato)
	{
		//Vector temporal que almacena los elementos de la cola
		vector<Cancion>Nombre;

		while(!cola.empty())
		{
			//Se agregan los elementos al vector
			Nombre.push_back(cola.front());
			cola.pop();
		}

		//Se ordena alfabeticamente las canciones segun su nombre
		sort(Nombre.begin(), Nombre.end(), &ComparacionNombre);

		//Verifica si el orden elegido es ascendente o descendente
		if(formato == 1)
		{
			//Si el orden es ascendente (A o Z), solo agrega las canciones de nuevo a la cola
			for (const auto& Cancion : Nombre)
			{

				cola.push(Cancion);

			}
		
		}else
		{
			//Si el orden es descendete, invierte el vector
			reverse(Nombre.begin(), Nombre.end());

			for (const auto& Cancion : Nombre)
			{
				//Agrega los elementos del vector a la cola de reproduccion
				cola.push(Cancion);

			}
		
		}

		//Limpia el vector
		Nombre.clear();

		//Imprime la cola de reproduccion
		ImprimirCola(cola);

		return;

	
	}

	//Funcion booleana que comprueba que se ordene de menor a mayor
	bool ComparacionNumero(const Cancion& pri, const Cancion& second)
	{
		return pri.duracion < second.duracion;

	}


	void CDs::OrdenarDuracion(queue<Cancion>& cola, int formato)
	{
		//Vector temporal que almacena las canciones de la cola de reproduccion
		vector<Cancion> Duracion;

		//Agrega los elementos de la cola al vector
		while(!cola.empty())
		{
			Duracion.push_back(cola.front());
			cola.pop();
		}

		//Se ordenan los elementos de menor a mayor
		sort(Duracion.begin(), Duracion.end(), &ComparacionNumero);

		if(formato == 1)
		{
			//Si el orden es ascendente, solo se agregan los elementos de nuevo a la cola
			for (const auto Cancion : Duracion)
			{
				cola.push(Cancion);
			}
		
		}else
		{
			//Si el orden es descedente, se invierte el orden de los elementos

			reverse(Duracion.begin(), Duracion.end());

			for (const auto Cancion : Duracion)
			{
				//Agrega los elementos de nuevo a la cola de reproduccion
				cola.push(Cancion);
			}

		}

		//Limpia el vector
		Duracion.clear();

		//Imprime la cola de reproduccion
		ImprimirCola(cola);
		
		return;
	}


	void CDs::OrdenarColaPer(queue<Cancion> &ColaRepro)
	{
		//Borra la pantalla
		system("cls");

		//Declaracion de variables
		int opc,orden;

		while (true)
		{
			//Limpia el buffer de entrada de Cin
			cin.ignore(9999, '\n');

			//Imprime el menu
			cout << "¿Como desea ordenar la Cola de Reproduccion?" << endl;
			cout << "1. Por Nombre del artista" << endl;
			cout << "2. Por Nombre de la cancion" << endl;
			cout << "3. Por Duracion de la cancion" << endl;
			cout << "Opcion: ";
			cin >> opc;

			//Validacion de la entrada del usuario
			if (cin.fail() || opc < 1 || opc > 3)
			{
				cin.clear();
				system("cls");
				cerr << "\x1b[31mOpcion	invalida. Por favor, ingrese un numero entre 1 y 3\x1b[0m" << std::endl;
				cout << "\n";
				continue;
			}
			break;
		}
		
		//guarda el valor de la opcion seleccionada
		int n = opc;

		while (true)
		{
			cin.ignore(9999, '\n');

			system("cls");

			//Imprime el menu secundario
			cout << "¿En que orden?" << endl;
			cout << "1. Ascendente" << endl;
			cout << "2. Descendente" << endl;
			cout << "Opcion: ";
			cin >> orden;

			if (cin.fail() || orden < 1 || orden > 2)
			{
				cin.clear();
				system("cls");
				cerr << "\x1b[31mOpcion	invalida. Por favor, ingrese un numero entre 1 y 2\x1b[0m" << std::endl;
				cout << "\n";
				continue;
			}

			break;
		}

		//Dependiendo del orden, manda como parametro el orden a seguir en la funcion ordenar
			switch(n)
			{
			 case 1:

				 OrdenarArtista(ColaRepro, orden);

				 break;

			 case 2:

				 OrdenarNombre(ColaRepro, orden);

				 break;

			 case 3:

				 OrdenarDuracion(ColaRepro, orden);

				 break;
			}
	
			return;
	}


	//Modifica el atributo privado de CancionActual
	void CDs::SetCancionActual(Cancion* song)
	{
		CancionActual = song;
	}

	//Llama al atributo privado CancionActual
	Cancion* CDs::GetCancionActual()
	{
		return CancionActual;
	}



	void CDs::ReproduccionAct(Cancion* Actual)
	{
		//Limpia la pantalla
		system("cls");

		//Comprueba si se está reproduciendo una cancion
		if(Actual == nullptr)
		{
			cout << "\n" << "\n" << "\n";

			cout<<"\t" <<"\t"<<"\t" <<"REPRODUCCION EN PAUSA";

		}else
		{
			//Si esta reproduciendo una cancion, imprime los atributos de la Cancion
			cout << "\n" << "\n" << "\n";

			cout << "\t" << "\t" << "\t" << "SE ESTA REPRODUCIENDO" << "\n";

			cout << "\t" << "\t" << "\t" << Actual->NombreC << "\n"; 

			cout << "\t" << "\t" << "Cancion: " << Actual->Artista << "\n";

			cout << "\t" << "\t" << "Artista:" << Actual->duracion << "\n";

			cout << "\t" << "\t" << "Duracion:" << Actual->NombreC << "\n";

			cout << "\t" << "\t" << "DEL CD: " << Actual->Album;
		
		}

		return;
	}

	void CDs::ReproduccionSig(std::queue<Cancion>& cola)
	{
		system("cls");

		//Verifica que la cola no esta vacia
		if(cola.empty())
		{
			cout << "la Cola De Reproduccion esta vacia";

		}else
		{
			//Si no se esta reproduciendo nada
			if(GetCancionActual() == nullptr)
			{
				//Reproduce la cancion que esta en al principio de la cola
				SetCancionActual(&cola.front());

			}else
			{
				//Guarda la cancion que se esta reproduciendo
				Cancion Cache = cola.front();

				//Borra la cancion que se reproduce de la cabeza
				cola.pop();

				//Inserta la cancion hasta al final de la cola
				cola.push(Cache);

				//Cambia la cancion actual por siguiente cancion en la cola de reproduccion
				SetCancionActual(&cola.front());

				
			}
			
			//Imprime los atributos de la CancionActual
			cout << "Reproduciendo la Cancion: " << GetCancionActual()->NombreC << " De: " << GetCancionActual()->Artista << "..............";
		}

		return;
	}

	
	void CDs:: ImprimirCola(queue<Cancion> colarepro)
	{
		//Limpia la pantalla
		system("cls");
		 
		//Imprime los encabezados de la cola de reproduccion
		cout << "\033[34mNombre\033[0m                                   \033[34mArtista\033[0m                                 \033[34mDuracion\033[0m" << endl;

		//Imprime las canciones de la cola de reproduccion
		while(!colarepro.empty())
		{
			cout << left << setw(40)<<colarepro.front().NombreC << setw(40) << colarepro.front().Artista << setw(40) << colarepro.front().duracion << "\n";
			colarepro.pop();
		}

		return;
	}


	void CDs::Reinicio(queue<Cancion> &Colareproduccion, list<CIDI> &ListadoCD)
	{
		//Vuelve el atributo CancionActual nulo
		SetCancionActual(nullptr);

		//Limpia la cola
		while(!Colareproduccion.empty())
		{
			Colareproduccion.pop();
		}

		//Limpia la lista de Cds
		ListadoCD.clear();

	}





	