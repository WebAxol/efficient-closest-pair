/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <bits/stdc++.h>
#include <stdio.h>

#include <iostream>
#include <sstream>

using namespace std;

// Modelos

struct Point2D {
    double x;
    double y;
};

struct Point3D {
    double x;
    double y;
    double z;
};

template<typename T>
struct Closest {
    double dist;
    T i;
    T j;
};

// Calcular distancias 
// Nota: se usa sobrecarga de funciones para aprovechar el polimorfismo all invocarlas con ambos tipos de punto

double dist(Point2D point1, Point2D point2){
    
    double dx = point1.x - point2.x;
    double dy = point1.y - point2.y;

    return sqrt(dx*dx + dy*dy);
}

double dist(Point3D point1, Point3D point2){
    
    double dx = point1.x - point2.x;
    double dy = point1.y - point2.y;
    double dz = point1.z - point2.z;

    return sqrt(dx*dx + dy*dy + dz*dz);
}

// Calcular par más cercano 
// Nota: se usan templates para poder pasar parámetros de tipo vector<Point2D> y vector<Point3D> sin reescribir toda la lógica

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

template<typename T>
Closest<T> closestPairHelper(vector<T> &points, int L = 0, int R = -1){

    const int N = R - L + 1;
  
    if(N == 2 || N == 3) return bruteClosestPair(points, L, R);
    
    int mid = L + (R - L) / 2;
    
    // Aquí se divide el problema y se resuelven las mitades por separado
    
    Closest<T> cl = closestPairHelper(points, L,     mid);
    Closest<T> cr = closestPairHelper(points, mid + 1, R);
    
    double dl = cl.dist;
    double dr = cr.dist;
    
    T ith = cl.i;
    T jth = cl.j;
    double d = dl;
    
    if(dr < dl){
        d = dr;
        ith = cr.i;
        jth = cr.j;
    }
    
    // Aquí se junta la solución, considerando la región sospechosa, para encontrar un par más cercano
    
    vector<T> strip;
    
    for(int i = L; i <= R; i++){
        if(abs(points[i].x - points[mid].x) < d){
            strip.push_back(points[i]);
        }
    }
    
    sort(strip.begin(),strip.end(), [](T point1, T point2){
       return point1.y < point2.y;
    });
 
    for(int i = 0; i < strip.size(); i++){
        for(int j = i + 1; j < min(i + 8, (int)strip.size()); j++){
             
             double newDist = dist(strip[i],strip[j]);
             
             if(d > newDist){
                d = newDist;
                ith = strip[i];
                jth = strip[j];
            }
        }
    }
    
    // TODO: Definir lógica de caja para evaluar los puntos cuya distancia a la frontera sea menor que d. Aprovechar template y sobrecarga para 
    // la compatibilidad con ambos tipos de punto
    
    return { d, ith, jth };
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
    
    cout << "\t" << "Veredicto: " << (bruteResult.dist == efficientResult.dist ? "PASA" : "FALLA") << "\n\n";

    //exportToFile(file, name, "brute", }, bruteResult);
    exportToFile(file, name, efficientResult);
}

void run2D(){
    
    fstream iss("in/input-2d.txt");
    ofstream oss("out/output-2d.csv");

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
    
    fstream iss("in/input-3d.txt");
    ofstream oss("out/output-3d.csv");

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