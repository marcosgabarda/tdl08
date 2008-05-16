#include "GIC.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;

int main() {

  std::set<char> noTerminales;
  std::set<char> terminales;
  char simboloInicial;
  std::map<char, std::vector<std::string> > producciones;
  
  noTerminales.insert('S');
  noTerminales.insert('A');
  noTerminales.insert('B');
  noTerminales.insert('E');
  
  terminales.insert('a');
  terminales.insert('b');

  simboloInicial = 'S';

  vector<string> v;
  v.push_back(string("SA"));
  v.push_back(string("A"));
  producciones['S'] = v;

  vector<string> v1;
  v1.push_back(string("AbA"));
  v1.push_back(string("BB"));
  producciones['A'] = v1;

  vector<string> v2;
  v2.push_back(string("BBb"));
  v2.push_back(string("a"));
  producciones['B'] = v2;

  vector<string> v3;
  v3.push_back(string("a"));
  v3.push_back(string("Ea"));
  producciones['E'] = v3;


  GIC *G = new GIC(noTerminales, terminales, simboloInicial, producciones);
  
  set<char> sg = G->simbolosAlcanzables();
  
  set<char>::iterator it;
  for (it = sg.begin(); it != sg.end(); it++) {
    cout << *it << endl;
  }

  cout << "### Concatenacion ###" << endl;

  set<string> A;
  A.insert(string("Aa"));
  A.insert(string("a"));
  A.insert(string("AaA"));
  A.insert(string("aA"));
  set<string> B;
  B.insert(string("B"));

  set<string> res = concatena (A, B);
  set<string>::iterator it2;
  for (it2 = res.begin(); it2 != res.end(); it2++) {
    cout << *it2 << endl;
  }

  
  return 0;
  
}
