#include <bits/stdc++.h>

#include <iostream>
#include <sstream>
#include <filesystem>
#include "models.h"
#include "utils.h"

using namespace std;

// Modelos

// Calcular par más cercano 
// Nota: se usan templates para poder pasar parámetros de tipo vector<Point2D> y vector<Point3D> sin reescribir toda la lógica

// Algoritmo de referencia, usado para evaluar si el algoritmo D&C es óptimo
// Complejidad temporal: O(n²)
template<typename T>
Closest<T> bruteClosestPair(vector<T> &points, int L = 0, int R = -1){
    
    const int n = points.size();
    
    if(R == -1) R = n - 1;
    
    // Menos de 2 puntos: no existe par
    if(R - L < 1) return {1e8, T{}, T{} };

    double minDist = 1e8;
    T ith = points[L];
    T jth = points[L + 1];
    
    for(int i = L; i <= R; i++){
        for(int j = i + 1; j <= R; j++){
            
            double currDist = dist(points[i],points[j]);
            
            if(minDist > currDist){
                minDist = currDist;
                ith = points[i];
                jth = points[j];
            }
        }
    }
    
    return { minDist, ith, jth };
}

// Algoritmo D&C con presorting.
// P se ordena por x una sola vez y nunca se modifica. Y contiene los índices (rangos) de P[L..R]
// ordenados por y; se obtiene con un solo sort global y luego se particiona de forma estable en cada nivel.
// Así ni la división ni la franja requieren ordenar de nuevo: cada nivel cuesta O(n).
// Complejidad temporal: T(n) = 2T(n/2) + O(n) = O(nlog(n))
template<typename T>
Closest<T> closestPairHelper(const vector<T> &P, int L, int R, const vector<int> &Y){

    const int N = R - L + 1;

    if(N <= 3){
        // bruteClosestPair recibe referencia no constante, por eso se copia el rango (<= 3 puntos)
        vector<T> small(P.begin() + L, P.begin() + R + 1);
        return bruteClosestPair(small);
    }

    int mid = L + (R - L) / 2;
    double midX = P[mid].x;

    // Partición por rango, así los empates en x no desalinean las mitades
    vector<int> Yl, Yr;
    Yl.reserve(mid - L + 1);
    Yr.reserve(R - mid);
    for(int r : Y) (r <= mid ? Yl : Yr).push_back(r);

    Closest<T> cl = closestPairHelper(P, L,       mid, Yl);
    Closest<T> cr = closestPairHelper(P, mid + 1, R,   Yr);

    double d = cl.dist;
    T ith = cl.i, jth = cl.j;

    if(cr.dist < d){
        d = cr.dist;
        ith = cr.i;
        jth = cr.j;
    }

    vector<int> strip;
    for(int r : Y){
        if(abs(P[r].x - midX) < d) strip.push_back(r);
    }

    // Se recorre mientras la diferencia en y sea menor que d (en teoría, solo debería iterar 7 vecinos en 2d)
    for(int i = 0; i < (int)strip.size(); i++){
        for(int j = i + 1; j < (int)strip.size() && P[strip[j]].y - P[strip[i]].y < d; j++){

            cout << i << endl;

            double newDist = dist(P[strip[i]], P[strip[j]]);

            if(d > newDist){
                d = newDist;
                ith = P[strip[i]];
                jth = P[strip[j]];
            }
        }
    }

    return { d, ith, jth };
}

template<typename T>
Closest<T> efficientClosestPair(vector<T> points){

    if(points.size() < 2) return {1e8, T{}, T{} };

    // Único ordenamiento por x
    sort(points.begin(), points.end(), [](const T &a, const T &b){
        return a.x < b.x;
    });

    // Único ordenamiento por y (sobre rangos de points)
    // El vector Y contiene los índices al vector de puntos ordenado
    // dichos indices a su vez se encuentran ordenados por la posición y de cada punto
    // de modo que no es necesario reordenar en cada merge. Los índices son estables.
    vector<int> Y(points.size());
    iota(Y.begin(), Y.end(), 0);
    sort(Y.begin(), Y.end(), [&](int a, int b){
        return points[a].y < points[b].y;
    });

    return closestPairHelper(points, 0, (int)points.size() - 1, Y);
}

// Exportar resultado (para poder hacer las gráficas en Python)

void exportToFile(
    ofstream& file,
    string name,
    Closest<Point2D> result
) {
    file << name << ","
         << result.dist << ","
         << result.i.x << "," << result.i.y << ","
         << result.j.x << "," << result.j.y
         << "\n";
}

void exportToFile(
    ofstream& file,
    string name,
    Closest<Point3D> result
) {
    file << name << ","
         << result.dist << ","
         << result.i.x << "," << result.i.y << "," << result.i.z << ","
         << result.j.x << "," << result.j.y << "," << result.j.z
         << "\n";
}

// Ejecución

void printPoint(Point2D point){
    cout << "( " << point.x << " , " << point.y << " )" << endl;
}

void printPoint(Point3D point){
    cout << "( " << point.x << " , " << point.y << " , " << point.z << " )" << endl;
}


template<typename T>
void run(
    vector<T> points, 
    string name,
    ofstream& file
){
 
    // Ejecutamos el algoritmo bruto sabiendo que garantiza una respuesta correcta, 
    // con tal de validar que el algoritmo de fuerza bruta devuelve el mismo resultado
    cout << "Ejecutando caso de prueba '" << name << "'\n\n";
    
    cout << "\tNúmero de puntos a comparar: " << points.size() << endl;
    
    Closest<T> bruteResult     = bruteClosestPair(points);
    Closest<T> efficientResult = efficientClosestPair(points); 
    
    cout << "\t" << "Resultado bruto:" << endl;
    
    cout << "\t" << "- Distancia: " << bruteResult.dist << endl;
    cout << "\t" << "- Primer par:";
    
    printPoint(bruteResult.i);
    
    cout << "\t" << "- Segundo par: ";
    
    printPoint(bruteResult.j);
    cout << endl;

    
    cout << "\t" << "Resultado eficiente: " << endl;
    
    cout << "\t" << "- Distancia: " << efficientResult.dist << endl;
    cout << "\t" << "- Primer par:";
    
    printPoint(efficientResult.i);
    
    cout << "\t" << "- Segundo par: ";
    
    printPoint(efficientResult.j);
    
    cout << "\t" << "Veredicto: " << (fabs(bruteResult.dist - efficientResult.dist) < 1e-9 ? "PASA" : "FALLA") << "\n\n";

    exportToFile(file, name, efficientResult);
}

void run2D(){
    
    filesystem::create_directories("out");
    fstream iss("in/input-2d.txt");
    ofstream oss("out/output-2d.csv");

    if(!iss.is_open() || !oss.is_open()){
        cerr << "Error: no se pudo abrir in/input-2d.txt o out/output-2d.csv (revisa el directorio de ejecución)\n";
        return;
    }

    int T;
    iss >> T;

    cout << "========== CASOS DE PRUEBA 2D ========== \n\n\n";

    for(int t = 0; t < T; t++){
    
        string name;
        iss >> name;
    
        int N;
        iss >> N;
     
        vector<Point2D> points;
        for(int i = 0; i < N; i++){
            double x, y;
            iss >> x >> y;
            points.push_back({ x, y });
        }
        
        run(points, name, oss);
    }

    cout << "\n\n";
}

void run3D(){
    
    filesystem::create_directories("out");
    fstream iss("in/input-3d.txt");
    ofstream oss("out/output-3d.csv");

    if(!iss.is_open() || !oss.is_open()){
        cerr << "Error: no se pudo abrir in/input-3d.txt o out/output-3d.csv (revisa el directorio de ejecución)\n";
        return;
    }

    int T;
    iss >> T;

    cout << "========== CASOS DE PRUEBA 3D ========== \n\n\n";

    for(int t = 0; t < T; t++){
    
        string name;
        iss >> name;
    
        int N;
        iss >> N;
     
        vector<Point3D> points;
        for(int i = 0; i < N; i++){
            double x, y, z;
            iss >> x >> y >> z;
            points.push_back({ x, y, z });
        }
      
        run(points, name, oss);
    }
    
    cout << "\n\n";
}

int main(){

    // ===== Ejecutar casos 2D ========
    
    // Archivos originales (del Teams)
    
    // Caso 0: puntos-n10.txt 
    // Caso 1; puntos-n11.txt
    // Caso 2: puntos-n8.txt
    // Caso 3: puntos-n15.txt
    // Caso 4: puntos-n16.txt
    // Caso 5: puntos-n50.txt
    // Caso 6: puntos-n100.txt
    
    run2D();

    // ===== Ejecutar casos 3D ========
    
    // Archivos originales (del Teams)
    
    // Caso 0: puntos-n8-3d.txt
    // Caso 1: puntos-n16-3d.txt
    // Caso 2: puntos-n20-3d.txt
    // Caso 3: puntos-n50-3d.txt
    // Caso 4: puntos-n100-3d.txt

    //run3D();
    

    return 0;
}