#ifndef GIC_H_
#define GIC_H_

#include <map>
#include <vector>
#include <set>
#include <string>

#define WINDOWS

/*
 * Clase que implementa una Gramática Incontextual. 
 */
class GIC
{
private:
  std::set<char> m_noTerminales;
  std::set<char> m_terminales;
  char m_simboloInicial;
  std::map<char,std::vector<std::string> > m_producciones;

  static char nuevo_simbolo_auxiliar(const std::set<char> &noTerminales);

public:

  GIC(std::set<char> noTerminales,
      std::set<char> terminales,
      char simboloInicial,
      std::map<char,std::vector<std::string> > producciones);
  

  std::set<char> simbolosGenerativos() const;
  GIC formaNormalChomsky() const;
};

#endif /*GIC_H_*/
