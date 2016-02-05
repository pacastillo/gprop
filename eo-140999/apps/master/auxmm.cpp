// Funciones auxiliares para el mastermind

#include <getopt.h>			     // for getopt and optarg

#include "auxmm.hpp"

//------------------------------------------------------------
void gen2comb(char* _in, const chrom* _chrm,
	      unsigned _len, unsigned _sizeGenes, unsigned _colores ) 
//------------------------------------------------------------
// Pasa de cadenas empezadas en 0 a idem empezadas por 1
//-------------------------------------------------------------
{
    for ( unsigned i = 0; i < _len ; i ++ ) {
	_in[i] = '1'+ _chrm->getValue( _sizeGenes*i, _sizeGenes) % _colores; // adaptar al formato
    }
}
    

//-----------------------------------------------------------------
void procCL (int argc, char** argv, 
	     unsigned& _posiciones, 
	     unsigned& _popsize,
	     unsigned& _colores, 
	     float& _elitePerc, 
	     char* _acertada, 
	     mutRate_t& _transpRate ) {
//-----------------------------------------------------------------
// Procesa la linea de comandos. Aqui esta mejor, quitada de enmedio
//-----------------------------------------------------------------

    int opt;
    while (  (opt = getopt( argc, argv, "c:e:hn:p:s:t:") )!= -1 ){
	switch( opt ){
	  case 'c':
	    _colores = atoi( optarg );
	    break;
	  case 'e':
	    _elitePerc = atof( optarg );
	    break;
	  case 'h':
	    cout << "Uso: " << argv[0] <<" -c <colores> -g [sin graficos] \
-h [Ayuda] -n <poblacion> -p <posiciones> \
-s <cadena a averiguar> -t <porcentaje transposicion> \n";
	    exit(0);
	  case 'n' :
	    _popsize = atoi( optarg );
	    break;
	  case 'p':
	    _posiciones = atoi( optarg );
	    break;
	  case 's':
	    strcpy( _acertada, optarg );
	    break;
	  case 't':
	    _transpRate = atoi( optarg );
	    break;
	 
	}
    }
}


