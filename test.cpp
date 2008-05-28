#include "AFD.h"
#include "AFN.h"
#include "GIC.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <ctime>

using namespace std;

int main() {

  cout << endl << endl << "### PRUEBAS DE GIC ###" << endl << endl;

  std::set<char> noTerminales;
  std::set<char> terminales;
  char simboloInicial;
  std::map<char, std::vector<std::string> > producciones;
  
  noTerminales.insert('S');
  noTerminales.insert('A');
  noTerminales.insert('B');
  noTerminales.insert('C');
  
  terminales.insert('a');
  terminales.insert('b');

  simboloInicial = 'S';

  vector<string> v;
  v.push_back(string("SS"));
  v.push_back(string("CA"));
  producciones['S'] = v;

  vector<string> v1;
  v1.push_back(string("bAA"));
  v1.push_back(string("aC"));
  v1.push_back(string("B"));
  producciones['A'] = v1;

  vector<string> v2;
  v2.push_back(string("aSS"));
  v2.push_back(string("BC"));
  producciones['B'] = v2;

  vector<string> v3;
  v3.push_back(string("CC"));
  v3.push_back(string(""));
  producciones['C'] = v3;

  GIC *G = new GIC(noTerminales, terminales, simboloInicial, producciones);
  
  GIC GSimplificada = G->gramaticaSimplificada();

  cout << endl << endl << "### PRUEBAS DE AFD ###" << endl << endl;

  int nEstados = 3;
  
  int nEstadoInicial = 0;

  set<char> Alfabeto;
  Alfabeto.insert('a');
  Alfabeto.insert('b');

  map<cPar, int> Transiciones;
  Transiciones[cPar(0,'a')] = 1;
  Transiciones[cPar(1,'b')] = 2;
  Transiciones[cPar(2,'a')] = 1;
  Transiciones[cPar(2,'b')] = 2;

  vector<bool> Finales(nEstados);
  Finales[0] = false;
  Finales[1] = true;
  Finales[2] = false;

  AFD Aut(Alfabeto, nEstados, Transiciones, Finales, nEstadoInicial);

  AFN AutU =  Aut.AutomataUniversal();
  
  cout << "Numero de estados del AU: " << AutU.getNumEstados() << endl;
  AutU.setName(string("foo"));
  AutU.salvar("automanta_universal.txt");

  /*cout << endl << endl << "### RENDIMIENTO DE AU ###" << endl << endl;



for(int n = 3; n < 10; n++) {
	AFD Ai(2,n);
	AFD A = Ai.minimizar();
	double t_ini = static_cast<double>(time(0));
	double nrep = 400.0;
	for (double r = 0.0; r < nrep; r+=1.0)
		A.AutomataUniversal();
	double t_fin = static_cast<double>(time(0));;
	double t = (t_fin - t_ini) / nrep;
	cout << n << "\t" << t << endl;
	}*/

  return 0;
  
}
