#ifndef AFD_H_
#define AFD_H_

#define WINDOWS

#include <list>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cstdlib>

typedef std::pair<int,int> Par;
typedef std::pair<int,char> cPar;

/*
 * Clase que implementa un Automata Finito Determinista. 
 */
class AFD
{
 public:

   int getNumEstados() {return m_cEstados;}

  /*
   * El constructor aleatorio requiere de cSimbolos="nº de simbolos" y de cEstados="nº de estados"
   */
 AFD(int cSimbolos, int cEstados):
  m_cSimbolos(cSimbolos),
    m_cEstados(cEstados),
    m_vbEstadosFinales(cEstados),
    m_strName("")
    {
      do {
        inicializar();
      } while (m_lTransiciones.empty());
    }

  /*
   * El constructor definido requiere:
   *  cSimbolos="nº de simbolos",
   *  cEstados="nº de estados"
   *  lTransiciones="definicion de las transiciones"
   *  vbEstadosFinales="definición de estados finales"
   *  
   */
 AFD(int cSimbolos, int cEstados, std::map<Par,int> lTransiciones, std::vector<bool> vbEstadosFinales, int iEstadoInicial):
  m_cSimbolos(cSimbolos),
    m_cEstados(cEstados),
    m_vbEstadosFinales(vbEstadosFinales),
    m_lTransiciones(lTransiciones),
    m_iEstadoInicial(iEstadoInicial),
    m_strName("")
    {
    }

AFD(std::set<char> lSimbolos, int cEstados, std::map<cPar,int> lTransiciones, std::vector<bool> vbEstadosFinales, int iEstadoInicial):
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
      std::map<cPar,int>::iterator itM;
      for (itM = lTransiciones.begin(); itM != lTransiciones.end(); itM++) {
        m_lTransiciones[Par((itM->first).first, m_lciAlfabeto[(itM->first).second])] = itM->second;
      }
  }


 AFD(std::list<char> lSimbolos, int cEstados):
   m_cSimbolos(static_cast<int>(lSimbolos.size())),
   m_cEstados(cEstados),
   m_vbEstadosFinales(cEstados),
   m_vcAlfabeto(static_cast<int>(lSimbolos.size())),
   m_strName("")
   {
     std::list<char>::iterator it = lSimbolos.begin();
	   for (int i = 0; i < m_cSimbolos; i++, it++) {
		   m_lciAlfabeto[*it] = i;
		   m_vcAlfabeto[i] = *it;
	   }
     do {
	    inicializar();
     } while (m_lTransiciones.empty());
   }

 
  /*
   * minimizar -> devuelve el AFD mínimo asociado a este AFD
   */
  AFD minimizar() const;
  AFD minimizar2() const;

  /*
   *
   */
  bool evaluar(std::string strCadena) const;
  
  /*
   * Metodo para introducir un alfabeto del automata
   */
  void setAlfabeto (std::list<char> lAlfabeto);

  /*
   * Metodo que devuelbe el afabeto del automata
   */
  std::list<char> getAlfabeto () const;

  /*
   * Permite salvar el AFD a un fichero ASCII
   */
  bool salvar(const std::string& strFileName) const;

  std::string getName() const { return m_strName; };
  void setName(const std::string& strName) { m_strName = strName; }
  
  /*
   * Calcula el automata unversal.
   */
  AFD automataUniversal();

 private:
	
  std::string m_strName;
  int m_cSimbolos; 
  int m_cEstados; 

  /* 
   * Función auxiliar que nos permite calcular las intersecciones entre lenguajes por la derecha para
   * poder calcular el autómata universal
   */


	
  /*
   * Mapa de transiciones. Para obtener el destino de una transicion desde un origen, con un
   * simbolo, hay que acceder al mapa con el par (origen,simbolo) y nos devolvera el destino
   */
  std::map<Par, int> m_lTransiciones;
	
  /* 
   * Vector de booleanos, donde final[e] == true indica que el estado e es final,
   * y no en caso contrario.
   */
  std::vector<bool> m_vbEstadosFinales; 

  // Estado inicial del automata.
  int m_iEstadoInicial;
  
  /*
   * Mapa para traducir al alfabeto de entrada a un entero identificador.
   */
  std::map<char, int> m_lciAlfabeto;
  
  /*
   * Mapa para traducir de entero identifcador a simbolo del alfabeto.
   */
  std::vector<char> m_vcAlfabeto;

  /*
   * inicializar -> inicializa el AFD de forma aleatoria.
   */
  void inicializar();
  
  /*
   * Comprueba si hay estados no alcanzables
   */
  bool hayEstadosNoAlcanzables() const;
 
  std::set<std::set<int> > calculaEstadosDR() const;

  /*
   * Funciones auxiliares para la minimizacion
   */
  class Particion {
  private:
    typedef std::map<std::list<int>,std::list<int> > t_Particion;
    t_Particion m_Particion;

  public:
    typedef std::list<int> t_IDCompartimento;
    typedef std::list<int> t_ListaEstados;
    typedef t_Particion::const_iterator const_iterator;
    typedef t_Particion::iterator iterator;

    void insertarEnCompartimento(const t_IDCompartimento& lIDCompartimento, const int estado);
    t_IDCompartimento obtenerCompartimento(const int iEstado) const;
    int obtenerIndice(const int iEstado) const;
    bool operator ==(const Particion& rhs) const;
    t_ListaEstados& operator [](const t_IDCompartimento& lIDCompartimento) {
      return m_Particion[lIDCompartimento];
    }
    
    size_t getSize() const
    {
      return m_Particion.size();
    }

    iterator begin() {
      return m_Particion.begin();
    }

    const_iterator begin() const {
      return m_Particion.begin();
    }

    iterator end() {
      return m_Particion.end();
    }

    const_iterator end() const {
      return m_Particion.end();
    }
  };
	
};

#endif /*AFD_H_*/
