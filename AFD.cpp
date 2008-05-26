#include "AFD.h"
#include <list>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>
#include <queue>
#include <fstream>
#include <cmath>

#ifdef WINDOWS
#define SRANDOM srand
#define RANDOM rand
#else
#define SRANDOM srandom
#define RANDOM random
#endif

using namespace std;

bool TodoFalse(std::vector<bool> v) {
  int n = static_cast<int>(v.size());
  for (int i = 0; i < n; i++) {
    if (v[i]) return false;
  }
  return true;
}

bool fileExists(const std::string& fileName)
{
  std::fstream fin;
  fin.open(fileName.c_str(),std::ios::in);
  if( fin.is_open() )
    {
      fin.close();
      return true;
    }
  fin.close();
  return false;
}

bool AFD::salvar(const std::string& strFileName) const {
  std::ofstream osSalida;
  
  if (fileExists(strFileName))
    {
      // Si el archivo existe, entonces sólo se pueden añadir AFDs con nombre
      if(m_strName.empty())
	return false;

      osSalida.open(strFileName.c_str(),std::ios_base::app);
    }
  else
    {
      osSalida.open(strFileName.c_str());
    }

  if(!m_strName.empty())
    osSalida << "Name " << m_strName << std::endl;
  
  osSalida << "NumStates " << m_cEstados << std::endl;
  
  for(int iEstado=0; iEstado<m_cEstados; iEstado++)
    osSalida << "State " << iEstado << " initial=" << (m_iEstadoInicial==iEstado) << " final=" << (m_vbEstadosFinales[iEstado] ? 1:0) << std::endl;
  
  //  for(int iEstadoOrigen=0; iEstadoOrigen<m_cEstados; iEstadoOrigen++)
  //    for(int iSimbolo=0; iSimbolo<m_cSimbolos; iSimbolo++)
  //      osSalida << iEstadoOrigen << " " << (m_lTransiciones.find(Par(iEstadoOrigen,iSimbolo)))->second << " " << m_vcAlfabeto[iSimbolo] << std::endl;

  std::map<Par,int>  lTransiciones(m_lTransiciones);
  std::cout << "N. Transiciones guardadas: "<< lTransiciones.size() << std::endl;
  for(std::map<Par,int>::iterator it = lTransiciones.begin();
      it != lTransiciones.end();
      it++) {
    osSalida << it->first.first << " " << it->second << " " << m_vcAlfabeto[it->first.second] << std::endl;
  }

  osSalida.close();

  return true;
}

bool AFD::hayEstadosNoAlcanzables() const {

  if (estadosAlcanzables(m_cSimbolos,m_lTransiciones,m_iEstadoInicial).size() != m_cEstados) 
    return true;
  return false;
}

std::set<int> AFD::estadosAlcanzables(int cSimbolos,
                                      const std::map<Par,int>& lTransiciones,
                                      int iEstadoInicial) const {
  int iEstadoActual = iEstadoInicial;

  std::queue<int> lEstados;
  std::set<int> lEstadosVisitados;

  lEstados.push(iEstadoActual);
  lEstadosVisitados.insert(iEstadoActual);

  while (!lEstados.empty()) {

    iEstadoActual = lEstados.front();
    lEstados.pop();

    for (int i = 0; i < cSimbolos; i++) {
      if (lTransiciones.find(Par(iEstadoActual,i)) != lTransiciones.end()) {
        int iEstadoDestino = lTransiciones.find(Par(iEstadoActual,i))->second;
	      if (lEstadosVisitados.find(iEstadoDestino) == lEstadosVisitados.end()) {
	        lEstados.push(iEstadoDestino);
	        lEstadosVisitados.insert(iEstadoDestino);
	      }
      }
    }    
  }

  return lEstadosVisitados;
}

std::set<int> AFD::estadosGenerativos(int cSimbolos,
                                      const std::map<Par,int>& lTransiciones,
                                      const std::set<int>& lEstadosFinales) const {
  std::queue<int> lEstados;
  std::set<int> lEstadosGenerativos(lEstadosFinales);

  for(std::set<int>::const_iterator itEstadosFinales = lEstadosFinales.begin();
      itEstadosFinales != lEstadosFinales.end();
      itEstadosFinales++)
        lEstados.push(*itEstadosFinales);

  while (!lEstados.empty()) {

    int iEstadoActual = lEstados.front();
    lEstados.pop();

    // TODO
    for(std::map<Par,int>::const_iterator itTransiciones = lTransiciones.begin();
        itTransiciones != lTransiciones.end();
        itTransiciones++)
    {
      if(itTransiciones->second==iEstadoActual &&
         lEstadosGenerativos.find(itTransiciones->first.first)==lEstadosGenerativos.end())
      {
        lEstados.push(itTransiciones->first.first);
        lEstadosGenerativos.insert(itTransiciones->first.first);
      }
    }
  }

  return lEstadosGenerativos;
}

void AFD::inicializar() {

  /*
   * Inicializamos la semilla aleatroria.
   */

  SRANDOM(static_cast<unsigned int>(time(0)));

  for (int iEstado = 0; iEstado < m_cEstados; iEstado++) {
    for (int iSimbolo = 0; iSimbolo < m_cSimbolos; iSimbolo++) {
      int iEstadoDestino = RANDOM()%m_cEstados;
      m_lTransiciones[Par(iEstado,iSimbolo)]=iEstadoDestino;
    }
  }

  /*
   * Inicializamos la semilla aleatroria.
   */
  SRANDOM(static_cast<unsigned int>(time(0)));

  for (int i = 0; i < m_cEstados; i++) {
    if(RANDOM()%2 == 0) {
      m_vbEstadosFinales[i] = true;
    } else {
      m_vbEstadosFinales[i] = false;
    }
  }

  /* 
   * Queremos garantizar que haya al menos un estado final
   */
  m_vbEstadosFinales[RANDOM()%m_cEstados]=true;

  /*
   * Inicializamos la semilla aleatroria.
   */
  SRANDOM(static_cast<unsigned int>(time(0)));

  m_iEstadoInicial = RANDOM()%m_cEstados;

  if (hayEstadosNoAlcanzables()) { // || minimizar().getNumEstados()!=m_cEstados) {
    // std::cerr << "Warning - Se encontraron estados no alcanzables." << std::endl;
    m_lTransiciones.clear();
  }
}

AFD AFD::minimizar() const {
  std::vector<std::set<int> > P(2);
  std::vector<std::set<int> > P2;
  std::set<std::set<int> > L;
  std::set<int> noFinales;

  for(int iEstado=0; iEstado<m_cEstados; iEstado++)
  {
    P[m_vbEstadosFinales[iEstado] ? 0 : 1].insert(iEstado);
    if(!m_vbEstadosFinales[iEstado])
      noFinales.insert(iEstado);
  }
  L.insert(noFinales);

  while(!L.empty())
  {
    std::set<int> S = *(L.begin());
    L.erase(L.begin());

    for(int iSimbolo=0; iSimbolo<m_cSimbolos; iSimbolo++)
    {
      P2.clear();
      for(std::vector<std::set<int> >::iterator B = P.begin();
          B != P.end();
          B++)
      {
        std::set<int> B1, B2;
        // Split
        for(std::set<int>::iterator b = B->begin();
            b != B->end();
            b++)
        {
          if(S.find(m_lTransiciones.find(Par(*b,iSimbolo))->second)!=S.end())
            B1.insert(*b);
          else
            B2.insert(*b);
        } // endfor b

        if(!B1.empty())
          P2.push_back(B1);
        if(!B2.empty())
          P2.push_back(B2);

        if(!B1.empty() && !B2.empty())
        {
          if(B1.size() < B2.size())
            L.insert(B1);
          else
            L.insert(B2);
        }
      } // endfor B
      P=P2;
    } // endfor iSimbolo
  } //endwhile

  // En este punto la partición está refinada al máximo

  // Eliminamos estados no alcanzables
  std::set<int> alcanzables = estadosAlcanzables(m_cSimbolos, m_lTransiciones, m_iEstadoInicial);

  for(unsigned int iPos=0; iPos < P.size(); iPos++)
  {
    bool bHayQueBorrar = true;
    for(std::set<int>::const_iterator itEstados = P[iPos].begin();
        itEstados != P[iPos].end();
        itEstados++)
    {
      if(alcanzables.find(*itEstados)!=alcanzables.end()) {
        bHayQueBorrar=false;
        break;
      }
    } // endfor itEstados

    if(bHayQueBorrar) {
      P.erase(P.begin()+iPos);
      iPos--;
    }

  } // endfor itP


  // Eliminamos estados no generativos
  std::set<int> lEstadosFinales;
  for(int iEstado=0; iEstado<m_cEstados; iEstado++)
    if(m_vbEstadosFinales[iEstado])
      lEstadosFinales.insert(iEstado);

  std::set<int> generativos = estadosGenerativos(m_cSimbolos, m_lTransiciones, lEstadosFinales);

  for(unsigned int iPos=0; iPos < P.size(); iPos++)
  {
    bool bHayQueBorrar = true;
    for(std::set<int>::const_iterator itEstados = P[iPos].begin();
        itEstados != P[iPos].end();
        itEstados++)
    {
      if(generativos.find(*itEstados)!=generativos.end()) {
        bHayQueBorrar=false;
        break;
      }
    } // endfor itEstados

    if(bHayQueBorrar) {
      P.erase(P.begin()+iPos);
      iPos--;
    }

  } // endfor itP

  std::vector<int> vIndices(m_cEstados,-1);
  int iAux=0;
  for(std::vector<std::set<int> >::const_iterator itCompartimento=P.begin();
      itCompartimento != P.end();
      itCompartimento++, iAux++)
  {
    for(std::set<int>::const_iterator itElem = itCompartimento->begin();
        itElem != itCompartimento->end();
        itElem++)
    {
      vIndices[*itElem]=iAux;
    }
  }

  std::map<Par,int> lTransicionesMinimas;
  std::vector<bool> vbEstadosMinimosFinales(P.size());
  int iNuevoEstado = 0;

  for(std::vector<std::set<int> >::const_iterator itCompartimento=P.begin();
      itCompartimento != P.end();
      itCompartimento++, iNuevoEstado++) {
    for(int iSimbolo = 0; iSimbolo < m_cSimbolos; iSimbolo++) {
      int iDestino = vIndices[m_lTransiciones.find(Par(*(itCompartimento->begin()),iSimbolo))->second];
      if(iDestino != -1)
        lTransicionesMinimas[Par(iNuevoEstado,iSimbolo)] = iDestino;
    }
    vbEstadosMinimosFinales[iNuevoEstado] = m_vbEstadosFinales[*(itCompartimento->begin())];
  }


  AFD afdMinimo(m_cSimbolos, static_cast<int>(P.size()), lTransicionesMinimas, vbEstadosMinimosFinales, vIndices[m_iEstadoInicial]);


  return afdMinimo;  

}

std::set<std::set<int> > AFD::calculaEstadosDR() const {
  std::map<Par,std::set<int> > transicionesR;
  std::map<std::pair<std::set<int>,int>,std::set<int> > transicionesDR;
  std::set<std::set<int> > estadosDR;
  std::list<std::set<int> > porProcesar;

  for(std::map<Par,int>::const_iterator it = m_lTransiciones.begin();
      it != m_lTransiciones.end();
      it++)
    {
      transicionesR[Par(it->second,it->first.second)].insert(it->first.first);
    }

  {
    std::set<int> estadoInicialDR;
    for(int i=0; i<m_cEstados; i++)
      if(m_vbEstadosFinales[i])
        estadoInicialDR.insert(i);

    porProcesar.push_back(estadoInicialDR);
  }

  while(!porProcesar.empty()) {
    std::set<int> estadoActual(porProcesar.front());
    porProcesar.pop_front();

    if(estadosDR.find(estadoActual) != estadosDR.end())
      continue;

    for(int iSimbolo=0; iSimbolo < m_cSimbolos; iSimbolo++) {
      std::set<int> destino;
      for(std::set<int>::const_iterator itEstadoActual = estadoActual.begin();
          itEstadoActual != estadoActual.end();
          itEstadoActual++)
	{
	  std::set<int> aux = transicionesR[Par(*itEstadoActual,iSimbolo)];
	  for(std::set<int>::const_iterator itAux = aux.begin();
	      itAux != aux.end();
	      itAux++)
	    {
	      destino.insert(*itAux);
	    } // endfor aux
	} // endfor estadoActual
      transicionesDR[std::pair<std::set<int>,int>(estadoActual,iSimbolo)] = destino;
      if(!destino.empty() && destino != estadoActual && estadosDR.find(destino)==estadosDR.end())
        porProcesar.push_back(destino);
    } // endfor simbolos
    estadosDR.insert(estadoActual);
  } // endwhile

  return estadosDR;
}

void AFD::setAlfabeto(std::list<char> lAlfabeto) {
  /*
   * El método solo cogera de la lista de simbolos los indicados por el numero de simbolos
   * que se haya definido en el constructor. Se usará para definir el alfabeto si no se ha 
   * definido en el constructor.
   */
  std::vector<char> vcAlfabeto(static_cast<int>(m_vcAlfabeto.size()));
  m_lciAlfabeto.clear();
  m_vcAlfabeto.clear();
  std::list<char>::iterator it = lAlfabeto.begin();
  for (int i = 0; i < m_cSimbolos; i++, it++) {
    m_lciAlfabeto[*it] = i;
    vcAlfabeto[i] = *it;
  } 
  m_vcAlfabeto = vcAlfabeto;
}

std::list<char> AFD::getAlfabeto () const {
  std::list<char> lAlfabeto;
  int nSize =  static_cast<int>( m_vcAlfabeto.size() );
  for (unsigned int i = 0; i < nSize; i++) {
    lAlfabeto.push_back(m_vcAlfabeto[i]);
  }
  return lAlfabeto;  
}

bool AFD::evaluar (std::string strCadena) const {
  
  int iEstadoActual = m_iEstadoInicial;
  int nSimbolos = strCadena.size();

  for(int iSimbolo = 0; iSimbolo < nSimbolos; iSimbolo++) {
    if (m_lciAlfabeto.find(strCadena[iSimbolo]) == m_lciAlfabeto.end()) return false;
    if (m_lTransiciones.find(Par(iEstadoActual, (m_lciAlfabeto.find(strCadena[iSimbolo]))->second)) == m_lTransiciones.end()) return false;
    iEstadoActual = m_lTransiciones.find(Par(iEstadoActual, (m_lciAlfabeto.find(strCadena[iSimbolo]))->second))->second;
  }

  return m_vbEstadosFinales[iEstadoActual];
}

/**
 * Calcula el automata unversal.
 */
AFN AFD::AutomataUniversal() {


  unsigned int nEstados = this->getNumEstados(); // N. de estados del automata
  std::set<std::set<int> > EstadosDR = this->calculaEstadosDR();
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
    std::vector<bool> vCol;
    for(it_j = EstadosDR.begin(); it_j != EstadosDR.end(); it_j++) {
      int nEI = *(it_i->begin());
      if (it_j->find(nEI) != it_j->end()) {
	vCol.push_back(true);
      } else {
	vCol.push_back(false);
      }
    } // for it_j
    Mapa[*it_i] = vCol;
    if (MapaInverso.find(vCol) == MapaInverso.end()) // Solo insertamos en el inverso si es nuevo el elemento.
      MapaInverso[vCol] = *it_i;
  } // for it_i


  /**
   * Para generar todas las intersecciones, se generan todas las posibles combinaciones
   * de estados, de forma incremental, admitiendo repeticiones y importando el orden, y se 
   * introducen en un conjunto que elimina los elementos que estan ya repetidos
   * y donde no importa el orden. COSTE EXPONENCIAL.
   */
  int nLimit = static_cast<int>(pow(static_cast<float>(nEstados),static_cast<float>(nEstados) ));;
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

  std::vector<bool> vColIni(nEstadosDR);
  for (unsigned int i = 0; i < nEstadosDR; i++) vColIni[i] = true;

  std::set<int>::iterator it_k;
  for(it_i = Intersecciones.begin(); it_i != Intersecciones.end(); it_i++) {
    std::vector<bool> vCol(vColIni);
    for (it_k = it_i->begin(); it_k != it_i->end(); it_k++) {

      std::set<int> aux;
      aux.insert(*it_k);

      std::vector<bool> vColNuevo(Mapa[aux]);

      for (unsigned int i = 0; i < nEstadosDR; i++) vCol[i] = vCol[i] && vColNuevo[i];
    } //for it_k
    if (!TodoFalse(vCol)) {
      Mapa[*it_i] = vCol;
      if (MapaInverso.find(vCol) == MapaInverso.end()) // Solo insertamos en el inverso si es nuevo el elemento.
        MapaInverso[vCol] = *it_i;
    }
  } // for it_i

  /**
   * Definimos las relaciones de inclusion.
   */
  std::map<std::set<int>, std::set<std::set<int> > > RelacionesDeInclusion;
  std::set<std::set<int> > EstadosFinales;
  std::map<std::vector<bool>, std::set<int> >::iterator it_z;
  for(it_z = MapaInverso.begin(); it_z != MapaInverso.end(); it_z++) {
    EstadosFinales.insert(it_z->second);
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
	std::vector<bool> vColI = Mapa[*it_i];
	std::vector<bool> vColJ = Mapa[*it_j];
	bool bIncluido = true;
	for (unsigned int i = 0; i < nEstadosDR; i++) {
          if (vColJ[i] && !vColI[i]) {
	    bIncluido = false;
	    break;
	  }
	} // for i
	if (bIncluido) {
	  SubEstados.insert(*it_j);
	}
      } // if
    } // for it_j
    RelacionesDeInclusion[*it_i] = SubEstados;
  } // for it_i
  
  // INI DEBUGGING
  /*for (std::map<std::set<int>, std::set<std::set<int> > >::iterator it = RelacionesDeInclusion.begin();
       it != RelacionesDeInclusion.end();
       it++) {
    std::cout << "Estado: { ";
    for (std::set<int>::iterator it2 = it->first.begin();
	 it2 != it->first.end();
	 it2++) {
      std::cout << *it2 << " " ;
    }
    std::cout << "}" << std::endl;
    std::cout << "Tiene por abajo: ";
    for (std::set<std::set<int> >::iterator it2 = it->second.begin();
	 it2 != it->second.end();
	 it2++) {
      std::cout <<  "{ " ;
      for (std::set<int>::iterator it3 = it2->begin();
	   it3 != it2->end();
	   it3++) {
	std::cout << *it3 << " " ;
      }
      std::cout <<  "} " ;
    }
    std::cout << std::endl;
  }*/
  // FIN DEBUGGING
  
  /**
   * Construimos el automata universal.
   */
  std::map<std::set<int>, int> TraduceEstados;
  int nEstado = nEstados;
  for (std::set<std::set<int> >::iterator it = EstadosFinales.begin();
       it != EstadosFinales.end();
       it++) {
    if (it->size() == 1) {
      TraduceEstados[*it] = *(it->begin());
    } else {
      TraduceEstados[*it] = nEstado;
      nEstado++;
    }
  }

  /**
   * Si hay una transición que llega a un estado, tambien llega a todos los
   * que esten incluidos.
   *
   * Tenemos que empezar con las transiciones originales del automata.
   */
  
  std::map<Par,int> lTransiciones(m_lTransiciones);
  std::map<Par,int> lTransicionesTmp(m_lTransiciones);  
  std::set<Transicion> lTransicionesFinales;

  // TODO: Solo añadir las transiciones comunes a los estados que forman la interseccion.
  for ( std::set<std::set<int> >::iterator it = EstadosFinales.begin();
	it != EstadosFinales.end();
	it++) {
    std::set<int> st = *it;
    if (st.size() > 1) { // Si se añaden estados, estos serán fruto de las intersecciones.
      for (std::set<int>::iterator it2 = st.begin();
	   it2 != st.end();
	   it2++) { // it2 => Estados de la interseccion.
	for (std::set<int>::iterator it3 = st.begin();
	     it3 != st.end();
	     it3++) { // it3 => Estados de la interseccion.
	  if (*it2 != *it3 ) {
	    for (std::map<Par,int>::iterator it4 = lTransicionesTmp.begin();
		 it4 != lTransicionesTmp.end();
		 it4++) { // it4 => Cada una de las transiciones originales
	      Par origen1(*it2, it4->first.second);
	      Par origen2(*it3, it4->first.second);
	      if (lTransicionesTmp.find(origen1) != lTransicionesTmp.end() && lTransicionesTmp.find(origen2) != lTransicionesTmp.end()) {
		if (lTransicionesTmp[origen1] == lTransicionesTmp[origen2]){
		  lTransiciones[Par(TraduceEstados[st], it4->first.second)] = it4->second;
		}
	      }
	    } // for it4
	  } // if
	} // for it3
      } // for it2
    } // if
  } // for it

  for (std::map<Par, int>::iterator it = lTransiciones.begin();
       it != lTransiciones.end();
       it++) {
    lTransicionesFinales.insert(Transicion(it->first, it->second));
  }

  for ( std::map<Par,int>::iterator it = lTransiciones.begin();
	it != lTransiciones.end();
	it++ ) {
    // Si llega a un estado, tambien tiene que llegar a todos los que estan incluidos
    // std::map<std::set<int>, std::set<std::set<int> > > RelacionesDeInclusion;
    std::set<int> st;
    st.insert(it->second);
    Par orig = it->first;
    std::set<std::set<int> > incl = RelacionesDeInclusion[st];
    for (std::set<std::set<int> >::iterator it2 = incl.begin();
	 it2 != incl.end();
	 it2++) {	
      lTransicionesFinales.insert(Transicion(orig, TraduceEstados[*it2]));
    }
  }


  // AFD(int cSimbolos, int cEstados, std::map<Par,int> lTransiciones, std::vector<bool> vbEstadosFinales, int iEstadoInicial):
  
  std::list<char> lAlfabeto(this->getAlfabeto());
  int cSimbolos =  static_cast<int>(getAlfabeto().size());
  int cEstados = static_cast<int>(EstadosFinales.size());
  std::vector<bool> vbEstadosFin(m_vbEstadosFinales);
  int iEstadoInicial = m_iEstadoInicial;
  
  AFN AFDUniversal(cSimbolos, cEstados, lTransicionesFinales, vbEstadosFin, iEstadoInicial);
  AFDUniversal.setAlfabeto (lAlfabeto);
  
  //TODO: No recuedo si los finales eran los mismos que en el automata original, o si tambien se añaden
  // los estados untersección en los que TODOS son finales... ¿me lo estare imaginando? No te lo imaginas

  return AFDUniversal;
}
