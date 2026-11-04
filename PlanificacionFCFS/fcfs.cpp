#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <iomanip>

using namespace std;

struct Proceso {
    string id;
    int llegada;
    int servicio;
    int comienzo;
    int finalizacion;
    int espera;
    int estancia;
    double estanciaNormalizada;
    int ordenIngreso;
};

bool compararPorLlegada(const Proceso& a, const Proceso& b) {
    if (a.llegada != b.llegada)
        return a.llegada < b.llegada;
    return a.ordenIngreso < b.ordenIngreso;
}

int main() {
    int n;
    cout << "Ingrese el numero de procesos (entre 3 y 5): ";
    cin >> n;

    if (n < 3 || n > 5) {
        cout << "Error: el numero de procesos debe estar entre 3 y 5." << endl;
        return 1;
    }

    vector<Proceso> procesos(n);

    for (int i = 0; i < n; i++) {
        cout << "\nProceso " << (i + 1) << ":" << endl;
        cout << "Identificador: ";
        cin >> procesos[i].id;

        do {
            cout << "Tiempo de llegada: ";
            cin >> procesos[i].llegada;
            if (procesos[i].llegada < 0)
                cout << "  Error: el tiempo de llegada no puede ser negativo." << endl;
        } while (procesos[i].llegada < 0);

        do {
            cout << "Tiempo de servicio: ";
            cin >> procesos[i].servicio;
            if (procesos[i].servicio <= 0)
                cout << "  Error: el tiempo de servicio debe ser positivo." << endl;
        } while (procesos[i].servicio <= 0);

        procesos[i].ordenIngreso = i;
    }

    sort(procesos.begin(), procesos.end(), compararPorLlegada);

    int tiempoActual = 0;
    cout << "\nIntervalos de ociosidad del procesador:" << endl;

    bool huboOcio = false;
    for (int i = 0; i < n; i++) {
        if (tiempoActual < procesos[i].llegada) {
            cout << "  Procesador ocioso desde t=" << tiempoActual
                 << " hasta t=" << procesos[i].llegada
                 << " (duracion: " << (procesos[i].llegada - tiempoActual) << ")" << endl;
            tiempoActual = procesos[i].llegada;
            huboOcio = true;
        }

        procesos[i].comienzo = tiempoActual;
        procesos[i].finalizacion = tiempoActual + procesos[i].servicio;
        procesos[i].espera = procesos[i].comienzo - procesos[i].llegada;
        procesos[i].estancia = procesos[i].finalizacion - procesos[i].llegada;
        procesos[i].estanciaNormalizada = static_cast<double>(procesos[i].estancia) / procesos[i].servicio;

        tiempoActual = procesos[i].finalizacion;
    }

    if (!huboOcio) {
        cout << "  No hubo intervalos de ociosidad." << endl;
    }

    cout << "\nTabla de resultados:" << endl;
    cout << left
         << setw(10) << "Proceso"
         << setw(6)  << "TL"
         << setw(6)  << "Ts"
         << setw(6)  << "TC"
         << setw(6)  << "TF"
         << setw(8)  << "TE"
         << setw(6)  << "Tr"
         << setw(10) << "Tr/Ts" << endl;
    cout << string(58, '-') << endl;

    double sumaEspera = 0.0, sumaEstancia = 0.0;
    for (const auto& p : procesos) {
        cout << left
             << setw(10) << p.id
             << setw(6)  << p.llegada
             << setw(6)  << p.servicio
             << setw(6)  << p.comienzo
             << setw(6)  << p.finalizacion
             << setw(8)  << p.espera
             << setw(6)  << p.estancia
             << fixed << setprecision(2) << setw(10) << p.estanciaNormalizada
             << endl;
        sumaEspera += p.espera;
        sumaEstancia += p.estancia;
    }

    cout << string(58, '-') << endl;
    cout << fixed << setprecision(2);
    cout << "Promedio de espera: " << (sumaEspera / n) << endl;
    cout << "Promedio de estancia: " << (sumaEstancia / n) << endl;

    return 0;
}
