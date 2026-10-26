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
    
    double minDist = 1e8;
    T ith = points[L];
    T jth = points[R];
    
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

// Une el resultado de ambas mitades, considerando la región sospechosa que las separa
// Complejidad temporal: O(nlog(n))
template<typename T>
Closest<T> merge(
    vector<T> &points,
    Closest<T> cr,
    Closest<T> cl,
    int L,
    int mid,
    int R
){

    double dl = cl.dist, dr = cr.dist;
    T ith = cl.i, jth = cl.j;

    double d = dl;
    
    if(dr < dl){
        d = dr;
        ith = cr.i;
        jth = cr.j;
    }
        
    vector<T> strip;
    for(int i = L; i <= R; i++){
        if(abs(points[i].x - points[mid].x) < d){
            strip.push_back(points[i]);
        }
    }
    
    

    sort(strip.begin(),strip.end(), [](T point1, T point2){
       return point1.y < point2.y;
    });

    for(int i = 0; i < (int)strip.size(); i++){
        for(int j = i + 1; j < (int)strip.size() && strip[j].y - strip[i].y < d; j++){
    
             double newDist = dist(strip[i],strip[j]);
             
             if(d > newDist){
                d = newDist;
                ith = strip[i];
                jth = strip[j];
            }
        }
    }

    return { d, ith, jth };
}

template<typename T>
Closest<T> closestPairHelper(vector<T> &points, int L = 0, int R = -1){

    const int N = R - L + 1;
  
    if(N == 2 || N == 3) return bruteClosestPair(points, L, R);
    
    int mid = L + (R - L) / 2;
    
    // Aquí se divide el problema y se resuelven las mitades por separado
    
    Closest<T> cl = closestPairHelper(points, L,     mid);
    Closest<T> cr = closestPairHelper(points, mid + 1, R);
    
 
    // Nota: el límite de "7 vecinos" solo es válido en 2D. En 3D (y en general) se
    // recorre mientras la diferencia en y sea menor que d, lo cual es correcto en cualquier dimensión
       
    return merge(
        points, 
        cl, 
        cr,
        L,
        mid,
        R
    );
}

template<typename T>
Closest<T> efficientClosestPair(vector<T> points){
    
    sort(points.begin(),points.end(), [](T point1, T point2){
        return point1.x < point2.x;
    });

    int L = 0;
    int R = points.size() - 1;
    
    return closestPairHelper(points, L, R);
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

    run3D();
    

    return 0;
}