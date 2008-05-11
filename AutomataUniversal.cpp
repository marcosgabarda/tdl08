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

  
  /**
   * Conjunto de conjunto de estados. Cada conjunto de estados corresponde a 
   * una de las intersecciones posibles.
   */
  std::set<std::set<int> > EstadosInciales;
  std::set<std::set<int> > Intersecciones;

  /** 
   * Mapa que representa la tabla para la construccion del Automata Universal.
   */ 
  std::map<std::set<int>, std::vector<bool> > Mapa;
  std::map<std::vector<bool>, std::set<int> > MapaInverso;


  /**
   * En primer lugar se iuntroducen los estados correspondinetes a cada uno de
   * los estados del automata original.
   */
  for (unsigned int i = 0; i < nEstados; i++) {
    std::set<int> Estado;
    Estado.insert(i);
    EstadosInciales.insert(Estado);
  }

  /**
   * Una vez estan los estados originales, se introduce crea el vector de
   * "binarios" que se construye con los estados que resultan del reverso
   * hecho determinista.
   */
  std::set<std::set<int> >::iterator it_i;
  std::set<std::set<int> >::iterator it_j;
  for(it_i = EstadosInciales.begin(); it_i != EstadosInciales.end(); it_i++) {
    std::vector<bool> vCol(nEstadosDR);
    for(it_j = EstadosDR.begin(); it_j != EstadosDR.end(); it_j++) {
      if (it_j->find(*it_i->begin()) != it_j->end()) 
	vCol.push_back(true);
      else 
	vCol.push_back(false);
    } // for it_j
    Mapa[*it_i] = vCol;
    if (MapaInverso.find(vCol) == MapaInverso.end()) // Solo insertamos en el inverso si es nuevo el elemento.
      MapaInverso[vCol] = *it_i;
  } // for it_i


  /**
   * Para generar todas las intersecciones, se generan todas las posibles combinaciones
   * de estados, de forma incremental, admitiendo repeticiones y importando el orden, y se 
   * introducen en un conjunto que elimina los elementos que estan ya repetidos
   * y donde no importa el orden.
   */
  int nLimit = pow(static_cast<float>(nEstados),static_cast<float>(nEstados) );
  for (int i = 0; i < nLimit; i++) {
    std::set<int> Estado;
    if (i == 0)
      Estado.insert(i);
    for (int j = i; j > 0; j/=nEstados) {
      Estado.insert(j%nEstados);
    }
    if (Estado.size() > 1) { // Solo introducimos los estados que no son los iniciales.
      Intersecciones.insert(Estado);
    }
  }

  /**
   * NOTA: No se si te tiene que tener en cuenta un vector con todo a "false".
   */
  std::vector<bool> vColIni(nEstadosDR);
  for (int i = 0; i < nEstadosDR; i++) vColIni[i] = true;

  std::set<int> it_k;
  for(it_i = Intersecciones.begin(); it_i != Intersecciones.end(); it_i++) {
    std::vector<bool> vCol(nEstadosDR);
    vCol = vColIni;
    for (it_k = it_i->begin(); it_k != it_i->end(); it_k++) {
      std::vector<bool> vColNuevo(nEstadosDR);       
      vColNuevo = Mapa[*it_k];
      for (int i = 0; i < nEstadosDR; i++) vCol[i] = vCol[i] && vColNuevo[i];
    } //for it_k
    Mapa[*it_i] = vCol;
    if (MapaInverso.find(vCol) == MapaInverso.end()) // Solo insertamos en el inverso si es nuevo el elemento.
      MapaInverso[vCol] = *it_i;
  } // for it_i

  /**
   * Definimos las relaciones de inclusion. Si hay una transición que llega a 
   * un estado, tambien llega a todos los que esten incluidos.
   *
   * Tenemos que empezar con las transiciones originales del automata.   
   */
  std::map<std::set<int>, std::set<std::set<int> > > RelacionesDeInclusion;
  std::set<std::set<int> > EstadosFinales;
  std::map<std::vector<bool>, std::set<int> >::iterator it_z;
  for(it_z = MapaInverso.begin(); it_z != MapaInverso.end(); it_z++) {
    EstadosFinales.insert(*it_z);
  }

  for(it_i = EstadosFinales.begin(); it_i != EstadosFinales.end(); it_i++) {
    std::set<std::set<int> > SubEstados;
    for(it_j = EstadosFinales.begin(); it_j != EstadosFinales.end(); it_j++) {
      if (it_i != it_j) { 
	/**
	 * Si los elementos son diferentes, entonces comprobar si it_j esta
	 * incluido en it_i. Si es asi, se introduce en la relacion de
	 * inclusion (se añade a SubEstados).
	 */
	std::vecotr<bool> vColI = Mapa[*it_i];
	std::vecotr<bool> vColJ = Mapa[*it_j];
	bool bTF = false;
	bool bIncluido = true;
	for (unsigned int i = 0; i < nEstadosDR; i++) {
	  if(bTF) {
	    // No esta incluido
	    bIncludio = false;
	    break;
	  }
	  // Seguimos mirando
	  bTF = !( (vColI[i] && !vColJ[i]) || (vColI[i] == vColJ[i]) ) ;
	} // for i
	if (bIncluido) {
	  SubEstados.insert(*it_j)
	}
      } // if
    } // for it_j
    RelacionesDeInclusion[*it_i] = SubEstados;
  } // for it_i
  
  
  /**
   * Construimos el automata universal.
   */
  std::map<Par,int> lTransiciones;
  
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
