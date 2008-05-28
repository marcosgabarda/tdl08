#ifndef AFN_H_
#define AFN_H_

#define WINDOWS

#include <list>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cstdlib>
#include "types.h"

/*
 * Clase que implementa un Automata Finito Determinista. 
 */
class AFN
{
 private:
	
  int m_cSimbolos; 
  int m_cEstados; 

  // Estado inicial del automata.
  std::set<int> m_iEstadoInicial;
	
  /* 
   * Vector de booleanos, donde final[e] == true indica que el estado e es final,
   * y no en caso contrario.
   */
  std::vector<bool> m_vbEstadosFinales; 

  /*
   * Mapa de transiciones. Para obtener el destino de una transicion desde un origen, con un
   * simbolo, hay que acceder al mapa con el par (origen,simbolo) y nos devolvera el destino
   */
  std::set<Transicion> m_lTransiciones;

  /*
   * Mapa para traducir al alfabeto de entrada a un entero identificador.
   */
  std::map<char, int> m_lciAlfabeto;
  
  /*
   * Mapa para traducir de entero identifcador a simbolo del alfabeto.
   */
  std::vector<char> m_vcAlfabeto;

  /*
   * Nombre del autómata.
   */
  std::string m_strName;

 public:

  /*
   * El constructor definido requiere:
   *  cSimbolos="nº de simbolos",
   *  cEstados="nº de estados"
   *  lTransiciones="definicion de las transiciones"
   *  vbEstadosFinales="definición de estados finales"
   *  
   */
 AFN(int cSimbolos, int cEstados, std::set<Transicion> lTransiciones, std::vector<bool> vbEstadosFinales, std::set<int> iEstadoInicial):
  m_cSimbolos(cSimbolos),
    m_cEstados(cEstados),
    m_vbEstadosFinales(vbEstadosFinales),
    m_lTransiciones(lTransiciones),
    m_iEstadoInicial(iEstadoInicial),
    m_strName("")
    {
    }

 AFN(std::set<char> lSimbolos, int cEstados, std::set<cTransicion> lTransiciones, std::vector<bool> vbEstadosFinales, std::set<int> iEstadoInicial):
  m_cSimbolos(static_cast<int>(lSimbolos.size())),
    m_cEstados(cEstados),
    m_vbEstadosFinales(vbEstadosFinales),
    m_iEstadoInicial(iEstadoInicial),
    m_vcAlfabeto(static_cast<int>(lSimbolos.size()))
    {
      std::set<char>::iterator it = lSimbolos.begin();
      for (int i = 0; i < m_cSimbolos; i++, it++) {
        m_lciAlfabeto[*it] = i;
        m_vcAlfabeto[i] = *it;
      }
      std::set<cTransicion>::iterator itM;
      for (itM = lTransiciones.begin(); itM != lTransiciones.end(); itM++) {
        m_lTransiciones.insert(Transicion(Par((itM->first).first, m_lciAlfabeto[(itM->first).second]), itM->second ));
      }
  }

   int getNumEstados() {return m_cEstados;}
 
  void setAlfabeto (std::list<char> lAlfabeto);
  std::list<char> getAlfabeto () const;

  /*
   * Permite salvar el AFD a un fichero ASCII
   */
  bool salvar(const std::string& strFileName) const;

  std::string getName() const { return m_strName; };
  void setName(const std::string& strName) { m_strName = strName; }

};

#endif /*AFN_H_*/
