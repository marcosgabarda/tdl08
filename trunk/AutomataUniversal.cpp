#include "AFD.h"
#include <set>
#include <vector>
#include <iostream>
#include <cmath>
#define _DEBUG_

void MuestraEstado (std::set<int> Estado) {
  for (std::set<int>::iterator it = Estado.begin();
       it != Estado.end();
       it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;  
}

/**
 * Calcula el automata unversal de uno dado.
 */
AFD AutomataUniversal(AFD A) {

  unsigned int nEstados = A.getNumEstados(); // N. de estados del automata
  std::set<std::set<int> > EstadosDR = A.calculaEstadosDR();
  unsigned int nEstadosDR = EstadosDR.size();


  std::set<std::set<int> > Intersecciones;
  int nLimit = pow(static_cast<float>(nEstados),static_cast<float>(nEstados) );
  for (int i = 0; i < nLimit; i++) {
    std::set<int> Estado;
    if (i == 0)
      Estado.insert(i);
    for (int j = i; j > 0; j/=nEstados) {
      Estado.insert(j%nEstados);
    }
    Intersecciones.insert(Estado);
  }

  std::set<std::set<int> >::iterator it_i;
  std::set<std::set<int> >::iterator it_j;

  std::map<std::set<int>, std::vector<bool> > Mapa;
  for(it_i = Intersecciones.begin(); it_i != Intersecciones.end(); it_i++) {
    std::vector<bool> vCol;
    for(it_j = Intersecciones.begin(); it_j != Intersecciones.end(); it_j++) {
      if (it_j->find(*it_i->begin()) != it_j->end()) 
	vCol.push_back(true);
      else 
	vCol.push_back(false);
    }
    Mapa[*it_i] = 
  }

  return;
  
}

#ifdef _DEBUG_

int main () {

  std::list<char> Sigma;
  Sigma.push_back('a');
  Sigma.push_back('b');
  AFD A(Sigma, 4);
  AFD AMinimo = A.minimizar2();

  AutomataUniversal(AMinimo);

  return 0;
}

#endif
