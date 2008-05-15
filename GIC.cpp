#include "GIC.h"
#include <iostream>

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

  std::set<char> terminales ( m_terminales );
  std::map<char,std::vector<std::string> > producciones ( m_producciones );
  std::set<char> noTerminales ( m_noTerminales );
  std::set<char> lGeneradores;
  std::set<char> lGeneradoresAux ( m_terminales );
  std::set<char>::iterator it;

  do {
  
    /**
     * lGeneradores <= No terminales tales que si A -> x, entonces x pertence a lGeneradoresAux
     * std::set<char> m_noTerminales;
     * std::map<char,std::vector<std::string> > m_producciones;
     */
    for (it = noTerminales.begin(); it != noTerminales.end(); it++) {
      char cNoTerminal = *it;
      std::vector<std::string> vProd = producciones[cNoTerminal];
      int nvProd = static_cast<int>(vProd.size());
      bool bTerGen = false;
      for (int i = 0; i < nvProd; i++) {
	std::string sProd ( vProd[i] );
	int nsProd = static_cast<int>(sProd.size());
	int s;
	for (s = 0; s < nsProd; s++) {
	  if (lGeneradoresAux.find(sProd[s]) == lGeneradoresAux.end()) 
	    break;
	}
	if (s == nsProd) // La produccion i es pertenece a lGeneradoresAux*
	  bTerGen = true;
	  
      }
      if (bTerGen) { // El no terminal tiene una produccion al menos que esta en lGeneradoresAux*
	lGeneradores.insert(cNoTerminal);	
      }
    }
    
    // gen = gen - gen2
    std::set<char> tmp;
    for(it = lGeneradores.begin(); it != lGeneradores.end(); it++) {
      if (lGeneradoresAux.find(*it) == lGeneradoresAux.end()) {
	tmp.insert(*it);
      }
    }
    lGeneradores = tmp;
    
    // gen2 = gen U gen2
    for (it = lGeneradores.begin(); it != lGeneradores.end(); it++) {
      lGeneradoresAux.insert(*it);
    }

    
  } while (lGeneradores.size() != 0);
  
  // gen = gen2 - T
  std::set<char> tmp;
  for (it = lGeneradoresAux.begin(); it != lGeneradoresAux.end(); it++) {
    if (terminales.find(*it) == terminales.end()) {
      lGeneradores.insert(*it);      
    }
  }
  
  
  return lGeneradores; 
  
}

GIC GIC::eliminacionNoGenerativos() const {
  
  std::map<char,std::vector<std::string> > produccionesOriginales(m_producciones);
  std::map<char,std::vector<std::string> > producciones;
  std::set<char> noTerminales;
  std::set<char> terminales(m_terminales);
  std::set<char> generativos = simbolosGenerativos();
  char simboloInicial = m_simboloInicial;

  noTerminales  = generativos;
  noTerminales.insert(simboloInicial);

  std::set<char>::iterator it;
  for (it = generativos.begin(); it != generativos.end(); it++) {
    std::vector<std::string> vProd = produccionesOriginales[*it];
    std::vector<std::string> vProdNuevas;
    int nProd = static_cast<int>(vProd.size());
    for (int i = 0; i < nProd; i++) {
      std::string prod = vProd[i];
      /**
       * Coprobar que x pertenece a (gen U T)*, y si es asi, se añade la produccion.
       */
      int nprod = static_cast<int>(prod.size());
      int s;
      for (s = 0; s < nprod; s++) {
	if (terminales.find(prod[s]) == terminales.end() && generativos.find(prod[s]) == generativos.end() )
	  break;
      }
      if (s == nprod) // Se añade a las producciones
	vProdNuevas.push_back(prod);
    }
    if (vProdNuevas.size() != 0) 
      producciones[*it] = vProdNuevas;
  }
  
  return GIC(noTerminales, terminales, simboloInicial, producciones);

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
