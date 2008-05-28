#include "AFN.h"
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


bool fileExists_(const std::string& fileName)
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

bool AFN::salvar(const std::string& strFileName) const {
  std::ofstream osSalida;
  
  if (fileExists_(strFileName))
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
    osSalida << "State " << iEstado << " initial=" << (m_iEstadoInicial.find(iEstado) != m_iEstadoInicial.end() ) << " final=" << (m_vbEstadosFinales[iEstado] ? 1:0) << std::endl;
  
//  for(int iEstadoOrigen=0; iEstadoOrigen<m_cEstados; iEstadoOrigen++)
//    for(int iSimbolo=0; iSimbolo<m_cSimbolos; iSimbolo++)
//      osSalida << iEstadoOrigen << " " << (m_lTransiciones.find(Par(iEstadoOrigen,iSimbolo)))->second << " " << m_vcAlfabeto[iSimbolo] << std::endl;

  std::set<Transicion>  lTransiciones(m_lTransiciones);
  std::cout << "N. Transiciones guardadas: "<< lTransiciones.size() << std::endl;
  for(std::set<Transicion>::iterator it = lTransiciones.begin();
      it != lTransiciones.end();
      it++) {
	osSalida << it->first.first << " " << it->second << " " << it->first.second << std::endl;
  }

  osSalida.close();

  return true;
}

void AFN::setAlfabeto(std::list<char> lAlfabeto) {
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

std::list<char> AFN::getAlfabeto () const {
  std::list<char> lAlfabeto;
  int nSize =  static_cast<int>( m_vcAlfabeto.size() );
  for (unsigned int i = 0; i < nSize; i++) {
    lAlfabeto.push_back(m_vcAlfabeto[i]);
  }
  return lAlfabeto;  
}
