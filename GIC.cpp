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

std::set<char> GIC::simbolosAlcanzables() const {

  std::map<char,std::vector<std::string> > producciones(m_producciones);
  std::set<char> terminales(m_terminales);
  char simboloInicial = m_simboloInicial; 

  std::set<char> lAlcanzables;
  std::set<char> lAux;
  std::set<char> lSimbolos (m_noTerminales);

  for(std::set<char>::iterator it = terminales.begin();
      it != terminales.end();
      it++) {
    lSimbolos.insert(*it);
  }

  lAlcanzables.insert(simboloInicial);
  lAux.insert(simboloInicial);

  do {
    
    std::set<char> lAux2;
    for (std::set<char>::iterator itSimbolos = lSimbolos.begin(); 
	 itSimbolos != lSimbolos.end();
	 itSimbolos++) {
      char a = *itSimbolos;
      bool bOk = false;
      for (std::set<char>::iterator itAux = lAux.begin(); 
	   itAux != lAux.end();
	   itAux++) {
	std::vector<std::string> vProd = producciones[*itAux];
	int nProd = static_cast<int>(vProd.size());
	for (int i = 0; i < nProd; i++) {
	  std::string prod = vProd[i];
	  int nprod = static_cast<int>(prod.size());
	  for (int j = 0; j < nprod; j++) {
	    if (prod[j] == a) { // Entonces tiene que introducirse en simbolo en lAux2
	      bOk = true;
	      break;
	    } // if
	  } //for j
	  if (bOk) break;
	} //for i
	if (bOk) break;
      } // for itAux
       if (bOk) 
	 lAux2.insert(a);
    } // for itSimbolos

    // aux1 = aux2 - (alc U T)
    std::set<char> tmp;
    for (std::set<char>::iterator itAux2 = lAux2.begin(); 
	 itAux2 != lAux2.end();
	 itAux2++) {
      char a = *itAux2;
      if (lAlcanzables.find(a) == lAlcanzables.end() && terminales.find(a) == terminales.end()) {
	tmp.insert(a);
      }
    }
    lAux = tmp;


    // alc = alc U aux2
    for (std::set<char>::iterator itAux2 = lAux2.begin(); 
	 itAux2 != lAux2.end();
	 itAux2++) {
      lAlcanzables.insert(*itAux2); 
    }
  } while (lAux.size() != 0);
  
  return lAlcanzables;

}

GIC GIC::eliminacionNoAlcanzables() const {

  std::map<char,std::vector<std::string> > produccionesOrig(m_producciones);
  std::set<char> noTerminalesOrig(m_noTerminales);
  std::set<char> terminalesOrig(m_terminales);
  char simboloInicial = m_simboloInicial; 

  std::map<char,std::vector<std::string> > producciones;
  std::set<char> noTerminales;
  std::set<char> terminales;

  std::set<char> alcanzables = simbolosAlcanzables();

  // N1 = alc INT N
  // T1 = alc INT T
  for (std::set<char>::iterator it = alcanzables.begin(); 
       it != alcanzables.end();
       it++) {
    if (noTerminalesOrig.find(*it) != noTerminalesOrig.end())
      noTerminales.insert(*it);
    if (terminalesOrig.find(*it) != terminalesOrig.end())
      terminales.insert(*it);

  }

  for (std::set<char>::iterator it = noTerminales.begin(); 
       it != noTerminales.end();
       it++) {
    producciones[*it] = produccionesOrig[*it];
  }
  

  return GIC(noTerminales, terminales, simboloInicial, producciones);
}

std::set<char> GIC::simbolosAnulables() const {

  std::map<char,std::vector<std::string> > producciones(m_producciones);
  std::set<char> noTerminales(m_noTerminales);

  std::set<char> lAnulables;
  std::set<char> lAux;

  do {

    std::set<char> lSimbolos;
    std::set<char>::iterator it;
    for (it = noTerminales.begin(); it != noTerminales.end(); it++) {
      if (lAnulables.find(*it) == lAnulables.end())
	lSimbolos.insert(*it);
    }
    for (it = lSimbolos.begin(); it != lSimbolos.end(); it++) {
      std::vector<std::string> vProd = producciones[*it];
      bool bAnul = false;
      int nProd = static_cast<int>(vProd.size());
      for (int i = 0; i < nProd; i++) {
	std::string prod = vProd[i];
	int s, nprod = static_cast<int>(prod.size());
	for (s = 0; s < nprod; s++) {
	  if (lAnulables.find(prod[s]) == lAnulables.end())
	    break;
	}
	if (s == nprod) { // Hay una produccion que es anulable.
	  bAnul = true;
	  break;
	}
      }
      if (bAnul)
	lAux.insert(*it);
    }

    // Anulables = Anulables U aux
    for (it = lAux.begin(); it != lAux.end(); it++) {
      lAnulables.insert(*it);
    }

  } while (lAux.size() != 0);

}

std::set<std::string> concatena(std::set<std::string> A, std::set<std::string> B) {
  std::set<std::string> Resultado;
  for (std::set<std::string>::iterator it_i = A.begin();
       it_i != A.end();
       it_i++) {
    for (std::set<std::string>::iterator it_j = B.begin();
       it_j != B.end();
       it_j++) {
      std::string x;
      x.append(*it_i);
      x.append(*it_j);
      Resultado.insert(x);
    }
  }

  return Resultado;
}

std::set<std::string> GIC::sustitucion (std::string z, std::set<char> Anulables) const {

  std::set<char> lSimbolos (m_noTerminales);
  std::set<char> terminales(m_terminales);

  for(std::set<char>::iterator it = terminales.begin();
      it != terminales.end();
      it++) {
    lSimbolos.insert(*it);
  }

  std::vector<std::set<char> > vAux;
  int lenZ = static_cast<int>(z.size());
  for (int i = 0; i < lenZ; i++) {
    std::set<char> tmp;
    char x = z[i];
    tmp.insert(x);
    if (Anulables.find(x) != Anulables.end()) {
      tmp.insert('\0');
    }
    vAux.push_back(tmp);
  }
  
  std::set<std::string> Resultado;

  std::set<std::string> A;
  std::set<std::string> B;
  
  std::set<char> Atmp= vAux[0];
  std::set<char> Btmp= vAux[1];
  
  for (std::set<char>::iterator it = Atmp.begin();
       it != Atmp.end();
       it++) {
    std::string s;
    if (*it == '\0') s.append("");
    else s.push_back(*it);
    A.insert(s);
  }
  
  for (std::set<char>::iterator it = Btmp.begin();
       it != Btmp.end();
       it++) {
    std::string s;
    if (*it == '\0') s.append("");
    else s.push_back(*it);
    B.insert(s);
  }

  Resultado = concatena(A, B);

  int n = static_cast<int>(vAux.size());
  for (int i = 2; i < n; i++) {
    std::set<std::string> C;
    std::set<char> Ctmp= vAux[i];
    for (std::set<char>::iterator it = Ctmp.begin();
	 it != Ctmp.end();
	 it++) {
      std::string s;
      if (*it == '\0') s.append("");
      else s.push_back(*it);
      C.insert(s);
    }
    Resultado = concatena(Resultado, C);  
  }

  Resultado.erase(std::string(""));
  
  return Resultado;

}

GIC GIC::eliminacionProcuccionesVacias() const {

  std::map<char,std::vector<std::string> > produccionesOrig(m_producciones);
  std::set<char> noTerminales(m_noTerminales);
  std::set<char> terminales(m_terminales);
  char simboloInicial = m_simboloInicial; 

  std::map<char,std::vector<std::string> > producciones;
  
  std::set<char> anulables = simbolosAnulables();

  // producciones = {A -> x: Existe(A -> z) que pertenece a P, x pertenece a f(z)} 
  std::set<char>::iterator it;
  for (it = noTerminales.begin(); it != noTerminales.end(); it++) {
    char A = *it;
    std::vector<std::string> vProd = produccionesOrig[A];
    std::vector<std::string> vProdNuevas;
    std::set<std::string> nuevasProd;
    int nProd = static_cast<int> (vProd.size());
    for (int i = 0; i < nProd; i++) {
      std::string prod = vProd[i];
      std::set<std::string> tmp = sustitucion(prod, anulables);
      for (std::set<std::string>::iterator itTmp = tmp.begin();
	   itTmp != tmp.end();
	   itTmp++) {
	nuevasProd.insert(*itTmp);
      }
    }
    for (std::set<std::string>::iterator itTmp = nuevasProd.begin();
	 itTmp != nuevasProd.end();
	 itTmp++) {
      vProdNuevas.push_back(*itTmp);
    }

    producciones[A] = vProdNuevas;
  }
  
  return GIC(noTerminales, terminales, simboloInicial, producciones);

}

std::set<char> GIC::produccionesUnitarias(char A) const {

  std::map<char,std::vector<std::string> > producciones(m_producciones);
  std::set<char> noTerminales(m_noTerminales);

  std::set<char> alcanzablesUnitarias;
  std::set<char> lAux;

  alcanzablesUnitarias.insert(A);
  lAux.insert(A);

  do {
        
    std::set<char> lAux2;
    for (std::set<char>::iterator itAux = lAux.begin(); 
	 itAux != lAux.end(); 
	 itAux++) {
      char B = *itAux;
      for (std::set<char>::iterator itNoTerm = noTerminales.begin();
	   itNoTerm != noTerminales.end();
	   itNoTerm++) {
	char C = *itNoTerm;
	std::vector<std::string> vProd = producciones[B];
	int nProd = static_cast<int> (vProd.size());
	for (int i = 0; i < nProd; i++) {
	  std::string prod = vProd[i];
	  if (prod.size() == 1 && prod[0] == C) { // Si se cumple, se tiene que añadir B a lAux.
	    lAux2.insert(B);
	  }
	} // for i	
      } // for itNoTerm      
    } // for itAux


    // aux1 = aux2 - C(A)
    std::set<char> tmp;
    for (std::set<char>::iterator it = lAux2.begin(); 
	 it != lAux2.end();
	 it++) {
      if (alcanzablesUnitarias.find(*it) != alcanzablesUnitarias.end())
	tmp.insert(*it);
    }
    lAux = tmp;

    // C(A) = C(A) U aux1
    for (std::set<char>::iterator it = lAux.begin(); 
	 it != lAux.end();
	 it++) {
      alcanzablesUnitarias.insert(*it);
    }

  } while (lAux.size() != 0);
  
  return alcanzablesUnitarias;
}

std::set<std::string> GIC::producciones_varios_auxiliares (char A) const {
  
  std::map<char,std::vector<std::string> > producciones(m_producciones);
  std::set<char> terminales(m_terminales);

  std::vector<std::string> vProd = producciones[A];
  std::set<std::string> lProdNuevas;
  
  int nProd = static_cast<int> (vProd.size());
  for(int i = 0; i < nProd; i++) {    
    std::string prod = vProd[i];
    if (prod.size() > 1 || (prod.size() == 1 && terminales.find(prod[0]) != terminales.end())) // Solo los no terminales.
      lProdNuevas.insert(prod);
  }
  
  return lProdNuevas;
  
}

std::set<std::string> GIC::union_no_unitarias (char A, std::set<char> CNoTerm) const {  

  std::set<std::string> lAux;

  for (std::set<char>::iterator it = CNoTerm.begin();
       it != CNoTerm.end();
       it++) {
    char B = *it;
    std::set<std::string> tmp = producciones_varios_auxiliares (B);
    for (std::set<std::string>::iterator itTmp = tmp.begin();
	 itTmp != tmp.end();
	 itTmp++) {
      lAux.insert(*itTmp);
    }    
  }

  return lAux;

}

GIC GIC::eliminacionProduccionesUnitarioas() const {

  std::map<char,std::vector<std::string> > produccionesOrig(m_producciones);
  std::set<char> noTerminales(m_noTerminales);
  std::set<char> terminales(m_terminales);
  char simboloInicial = m_simboloInicial; 

  std::map<char,std::vector<std::string> > producciones;

  // Para todo simbolo no terminal
  for (std::set<char>::iterator it = noTerminales.begin();
       it != noTerminales.end();
       it++) {
    char A = *it;
    std::set<char> CNoTerm = produccionesUnitarias(A);

    std::set<std::string> tmp = union_no_unitarias ( A, CNoTerm);
    std::vector<std::string> vTmp;
    for (std::set<std::string>::iterator itTmp = tmp.begin();
	 itTmp != tmp.end();
	 itTmp++) {
      vTmp.push_back(*itTmp);
    }
    producciones[A] = vTmp;

  }

  return GIC(noTerminales, terminales, simboloInicial, producciones);

}

GIC GIC::gramaticaSimplificada() const {

  std::map<char,std::vector<std::string> > producciones(m_producciones);
  std::set<char> noTerminales(m_noTerminales);
  std::set<char> terminales(m_terminales);
  char simboloInicial = m_simboloInicial; 

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

bool GIC::accepts(const std::string& strCadena) const {
  const int n = strCadena.size();
  std::set<char>** V;
  V = new std::set<char>*[n];
  V--;
  for(int i=1; i<=n; i++) {
    V[i] = new std::set<char>[n];
    V[i]--;
  }

  for(int i=1; i<=n; i++)
  {
    for(std::map<char,std::vector<std::string> >::const_iterator itProducciones = m_producciones.begin();
        itProducciones != m_producciones.end();
        itProducciones++)
    {
      for(std::vector<std::string>::const_iterator itAlternativas = itProducciones->second.begin();
          itAlternativas != itProducciones->second.end();
          itAlternativas++)
      {
        if(strCadena.at(i-1)==itAlternativas->at(0)) {
          V[i][1].insert(itProducciones->first);
          break;
        }
      } // endfor itAlternativas
    } // endfor itProducciones
  } //endfor i

  for(int j=2; j<=n; j++)
  {
    for(int i=1; i<=n-j+1; i++)
    {
      for(int k=1; k<=j-1; k++)
      {
        for(std::map<char,std::vector<std::string> >::const_iterator itProducciones = m_producciones.begin();
            itProducciones != m_producciones.end();
            itProducciones++)
        {
          for(std::vector<std::string>::const_iterator itAlternativas = itProducciones->second.begin();
              itAlternativas != itProducciones->second.end();
              itAlternativas++)
          {
            if(itAlternativas->length()==2 &&
               V[i][k].find(itAlternativas->at(0))!=V[i][k].end() &&
               V[i+k][j-k].find(itAlternativas->at(1))!=V[i+k][j-k].end())
            {
              V[i][j].insert(itProducciones->first);
              break;
            }
          } // endfor itAlternativas
        } // endfor itProducciones
      } // endfor k
    } // endfor i
  } // endfor j

  // TODO: delete V

  return (V[1][n].find(m_simboloInicial)!=V[1][n].end());
}
