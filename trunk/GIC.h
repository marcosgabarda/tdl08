#ifndef GIC_H_
#define GIC_H_
 

#include <map>
#include <vector>
#include <set>
#include <string>
 

/*
 * Clase que implementa una Gramática Incontextual. 
 */

class GIC
{
 
  std::set<char> m_noTerminales;
  std::set<char> m_terminales;
  char m_simboloInicial;
  std::map<char,std::vector<std::string> > m_producciones;

};

 

#endif/*GIC_H_*/
