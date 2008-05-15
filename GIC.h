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

  std::set<std::string> producciones_varios_auxiliares (char A) const;
  std::set<std::string> union_no_unitarias (char A, std::set<char> CNoTerm) const;

public:

  GIC(std::set<char> noTerminales,
      std::set<char> terminales,
      char simboloInicial,
      std::map<char,std::vector<std::string> > producciones);
  

  std::set<char> simbolosGenerativos() const;
  GIC eliminacionNoGenerativos() const;

  std::set<char> simbolosAlcanzables() const;
  GIC eliminacionNoAlcanzables() const;

  std::set<char> simbolosAnulables() const;
  GIC eliminacionProcuccionesVacias() const;

  std::set<char> produccionesUnitarias(char A) const;
  GIC eliminacionProduccionesUnitarioas() const;

  GIC gramaticaSimplificada() const;

  GIC formaNormalChomsky() const;
};

#endif /*GIC_H_*/
