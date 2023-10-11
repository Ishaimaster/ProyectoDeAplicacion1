#include "pch.h"
#include "CDs.h"
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <stack>
#include <queue>



using namespace System;

std::list<CIDI> Albumes;
std::queue<Cancion> Cola;


int main(array<System::String^>^ args)
{
    
    std::string direccion;
    CDs Reproductor;
    int op,op1;
    bool Main = true;
    bool exitoso = false;

    while (Main)
    {
        bool Menu = true;

        do
        {
           //Imprime el menu de inicio de la aplicacion
             std::cout << "____________________________________________\n";
             std::cout << "||     Bienvenido al Reproductor de       ||\n";
             std::cout << "||               CD's de Musica           ||\n";
             std::cout << "--------------------------------------------\n";

            //Imprime el Menu de entrada
            std::cout << "Escoja una opcion" << "\n";
            std::cout << "1. Cargar respaldos" << "\n";
            std::cout << "2. Salir" << "\n";
            std::cout << "Opcion: ";
            std::cin >> op;
            
            //Verifica si la opcion ingresada es correcta
            if(std::cin.fail()|| (op <1) || (op > 2))
            {
                std::cerr << "Opción inválida. Por favor, ingrese un número entre 1 y 2." << std::endl;
                Console::ReadKey();
                Console::Clear();
            }else
            {
                break;
            }

        } while (true);
         
        //Limpia el bufer de la funcion "cin"
        std::cin.ignore();

        switch (op)
        {
        case 1:

            Console::Clear();
            Reproductor.Reinicio(Cola, Albumes);
            std::cout << "Ingrese la ruta de la carpeta donde se ubican sus " << "\x1b[31mCD's\x1b[0m\n";
            std::getline(std::cin, direccion);
            std::cout << "\n";
            

            Reproductor.LeerCarpetas(direccion, Albumes, exitoso);
            std::cout << "\n";
            
            
            if(exitoso)
            {
                while(Menu)
                {
                    Console::Clear();

                    std::cout << "--------------------- Reproductor de Canciones ---------------------" << "\n";
                    std::cout << "1. Agregar Cancion" << "\n";
                    std::cout << "2. Ver Cola de Reproduccion" << "\n";
                    std::cout << "3. Reproduccion Actual" << "\n";
                    std::cout << "4. Reproduccion Siguiente" << "\n";   
                    std::cout << "5. Ordenar" << "\n";
                    std::cout << "6. Salir" << "\n";
                    std::cout << "Opcion: ";
                    std::cin >> op1;

                    if(std::cin.fail()||op1 < 1|| op1 > 6)
                    {
                        std::cerr << "Opcion invalida. Por favor, ingrese un numero entre 1 y 6." << std::endl;
                        Console::ReadKey();
                        std::cin.clear();
                        std::cin.ignore(9999, '\n');
                        continue;
                    }
                    
                    switch(op1)
                    {
                     case 1:

                         Reproductor.ListadoCDs(Albumes, Cola);
                         Console::Clear();

                         break;

                     case 2:

                         Reproductor.RevisarCola(Cola);

                         std::cout << "\n";
                         Console::ReadKey();

                         break;

                     case 3:

                         Reproductor.ReproduccionAct(Reproductor.GetCancionActual());
                        
                         Console::ReadKey();

                         break;

                     case 4:

                         Reproductor.ReproduccionSig(Cola);

                        Console::ReadKey();

                         break;

                     case 5:
                         
                         Console::Clear();

                         if(!Cola.empty())
                         {
                             Reproductor.OrdenarColaPer(Cola);
                             Reproductor.SetCancionActual(nullptr);

                         }else
                         {
                             std::cout << "La cola de reproduccion esta vacia";
                             Console::ReadKey();
                         }


                         break;

                     case 6:

                         Menu = false;
                         break;
                    }
                    Console::Clear();


                }
            }else
            {
                Console::ReadKey();
                Console::Clear();
            }
            
            break;
            
        case 2:

            Console::Clear();
            std::cout << "Hasta la proxima..";
            Main = false;
            
            break;

        }


    }
    
        
    return 0;
}
