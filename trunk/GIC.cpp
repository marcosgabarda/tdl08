#include "GIC.h"

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

      for(std::string::iterator itSimbolo = itAlternativas->begin();
          itSimbolo != itAlternativas->end();
          itSimbolo++)
      {
        if(m_terminales.find(*itSimbolo)!=m_terminales.end()) {
          if(auxiliaresQueGeneranTerminales.find(*itSimbolo)==auxiliaresQueGeneranTerminales.end()) {
            char nuevoAuxiliar = nuevo_simbolo_auxiliar(noTerminales);
            auxiliaresQueGeneranTerminales[*itSimbolo]=nuevoAuxiliar;
            noTerminales.insert(nuevoAuxiliar);
          }
          *itSimbolo = auxiliaresQueGeneranTerminales[*itSimbolo]; // TODO: ¿Esto funciona?
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
          auxiliaresQueGeneranAuxiliares[std::pair<char,char>((itAlternativas->c_str()[0],itAlternativas->c_str()[1]))] = nuevoAuxiliar;
          noTerminales.insert(nuevoAuxiliar);
        }
        itAlternativas->replace(0,2,auxiliaresQueGeneranAuxiliares[std::pair<char,char>((itAlternativas->c_str()[0],itAlternativas->c_str()[1]))]);
      } // endwhile
    } // endfor itAlternativas
  } // endfor itProducciones


}