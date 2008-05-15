#include "GIC.h"

GIC::GIC(std::set<char> noTerminales,
         std::set<char> terminales,
         char simboloInicial,
         std::map<char,std::vector<std::string> > producciones) : m_noTerminales(noTerminales),
								  m_terminales(terminales),
								  m_simboloInicial(simboloInicial),
								  m_producciones(producciones)
{}


/**
 * Calcula los simbolos generativos de una gramática incontextual.
 */
std::set<char> GIC::simbolosGenerativos() const {

  std::set<char> gen;
  std::set<char> gen2 = m_terminales;

  do {
  
    /**
     * Gen <= No terminales tales que si A -> x, entonces x pertence a Gen2
     * std::set<char> m_noTerminales;
     * std::map<char,std::vector<std::string> > m_producciones;
     */
    std::set<char>::iterator itNoTerminales;
    for (itNoTerminales = m_noTerminales.begin(); itNoTerminales != itNoTerminales.end(); itNoTerminales++) {
      vProd = m_producciones[*itNoTerminales];
      int nProd = static_cast<int>(vProd.size());
      int i = 0;
      for (i = 0; i < nProd; i++) {
	if (gen2.find(vProd[i]) == gen2.end()) // Si hay un simbolo que no sea generativo, no se incluye.
	  break;
      }
      if ( i == nProd) { // Se ha saildo del for por terminar la cadena.
	gen.insert(*itNoTerminales);
      }
    }
    
  } while (gen.size() == 0)
  
  return gen; 
  
}

GIC GIC::formaNormalChomsky() const {
  std::map<char,char> auxiliaresQueGeneranTerminales;
  std::map<std::pair<char,char>,char> auxiliaresQueGeneranAuxiliares;
  std::map<char,std::vector<std::string> > producciones(m_producciones);
  std::set<char> noTerminales(m_noTerminales);

  for(std::map<char,std::vector<std::string> >::iterator itProducciones = producciones.begin();
      itProducciones != producciones.end();
      itProducciones++)
    {
      for(std::vector<std::string>::iterator itAlternativas = itProducciones->second.begin();
	  itAlternativas != itProducciones->second.end();
	  itAlternativas++)
	{
	  // Si la producción produce un único terminal
	  // (sabemos que es terminal porque la gramática está simplificada)
	  if(itAlternativas->length()== 1)
	    continue; // No hacemos nada

	  for(unsigned int uiChar = 0;
	      uiChar < itAlternativas->size();
	      uiChar++)
	    {
	      if(m_terminales.find(itAlternativas->at(uiChar))!=m_terminales.end()) {
		if(auxiliaresQueGeneranTerminales.find(itAlternativas->at(uiChar))==auxiliaresQueGeneranTerminales.end()) {
		  char nuevoAuxiliar = nuevo_simbolo_auxiliar(noTerminales);
		  auxiliaresQueGeneranTerminales[itAlternativas->at(uiChar)]=nuevoAuxiliar;
		  noTerminales.insert(nuevoAuxiliar);
		}
		(*itAlternativas)[uiChar] = auxiliaresQueGeneranTerminales[itAlternativas->at(uiChar)];
	      }
	    } // endfor iSimbolo
	} // endfor itAlternativas
    } // endfor itProducciones


  for(std::map<char,std::vector<std::string> >::iterator itProducciones = producciones.begin();
      itProducciones != producciones.end();
      itProducciones++)
    {
      for(std::vector<std::string>::iterator itAlternativas = itProducciones->second.begin();
	  itAlternativas != itProducciones->second.end();
	  itAlternativas++)
	{
	  // Si la producción produce un único terminal
	  // (sabemos que es terminal porque la gramática está simplificada)
	  if(itAlternativas->length()== 1)
	    continue; // No hacemos nada

	  // Si la producción produce dos símbolos auxiliares
	  if(itAlternativas->length()== 2)
	    continue; // No hacemos nada

	  while(itAlternativas->length()>2) {
	    if(auxiliaresQueGeneranAuxiliares.find(std::pair<char,char>(itAlternativas->c_str()[0],
									itAlternativas->c_str()[1]))
	       == auxiliaresQueGeneranAuxiliares.end())
	      {
		char nuevoAuxiliar = nuevo_simbolo_auxiliar(noTerminales);
		auxiliaresQueGeneranAuxiliares[std::pair<char,char>(itAlternativas->at(0),itAlternativas->at(1))] = nuevoAuxiliar;
		noTerminales.insert(nuevoAuxiliar);
	      }
	    itAlternativas->replace(0,2,1,auxiliaresQueGeneranAuxiliares[std::pair<char,char>(itAlternativas->at(0),itAlternativas->at(1))]);
	  } // endwhile
	} // endfor itAlternativas
    } // endfor itProducciones

  return GIC(noTerminales, m_terminales, m_simboloInicial, producciones);
}

char GIC::nuevo_simbolo_auxiliar(const std::set<char> &noTerminales)
{
  char nuevo;
  for(nuevo='a'; noTerminales.find(nuevo)!=noTerminales.end(); nuevo++);

  return nuevo;
}
