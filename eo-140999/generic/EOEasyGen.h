#ifndef _EOEASYGEN_H
#define _EOEASYGEN_H

#include <vector>				// for vector
#include <iterator>				// for iterator
#include <iostream>				// for cout

#include <ADT/EOAlgo.h>
#include <ADT/EOFitness.h>      // for Evaluator
#include <ADT/EOBreeder.h>     // for EOSelector and Breeder
#include <generic/EOselect/EOReplacers.h>      // for Replacement operators
#include <generic/EOselect/EOTournament.h>	// for Tournament


using namespace std;

/** Simple algorithm for a single generation in an evolutionary computation algorithms. Performs
 * everything within a generation, but somebody else must take care of termination
 */
template<class EOT>
class EOEasyGen: public EOAlgo<EOT> {
 public:
  
  /** The ctor takes all necessary object for an evolutionary algorithm
   * as pointers. These are used so that they can have default null values
   * Initial population can also be passed on. If not, it has to be
   * generated randomly
   */
  EOEasyGen( EOEvaluator<EOT>&  _eval,
	     EOSelector< EOT>& _select,
	     EOBreeder<EOT>& _breed,
	     EOReplace<EOT>& _rep,
	     bool _verbose = false )
    :EOAlgo<EOT>(), eval (_eval), select( _select),
    breed( _breed), replace( _rep),  verbose( _verbose ), evaluated( 0 ){};


  /** Copy ctor
   */
  EOEasyGen( const EOEasyGen<EOT>& _gen )
    :EOAlgo<EOT>(), eval (_gen.eval), select( _gen.select),
    breed( _gen.breed), replace( _gen.replace),  verbose( _gen.verbose ), evaluated( 0 ), generation( 0 ){};


  /// Ctor must ve virtual
  virtual ~EOEasyGen() {};
  
  /// Run the algorithm
  virtual void operator () ( EOPop<EOT>& _pop ) {
    
    EOPop<EOT> &oldGeneration = _pop;
    
    EOPop<EOT> pool;
    select( oldGeneration, pool );
    
    typename EOPop<EOT>::iterator l;		// assorted iterator
    unsigned j;		// Assorted index
    if ( verbose ) {
      // Print pool
      cout << "Pool ------------------------------\n";
	for ( l = pool.begin(), j = 0; l != pool.end(); l++, j ++ ) {
	  cout << "EO# " << j << " = " << *l 
	       << " - FITNESS - " << l->fitness( ) << endl;
	}
    }

    try {
      //Apply to new Generation
      EOPop<EOT> newPop;
      breed( pool, newPop );
      if ( verbose ) {	// And print if needed
      	 cout << "New Gen ---------------------------\n";	
	       for ( l = newPop.begin(), j = 0; 
	             l != newPop.end(); l++, j ++ ) {
	         cout << "EO# " << j << " = " << *l << endl;
	       }
      }
      
      // evaluate the new ones. Population is ranked
      eval( newPop );
      evaluated += newPop.size();	// increase the number of evaluated things.
      
      //***************************************************//
      // ponerle el ID para que se llame como la generación en que se generó...
      for ( j = 0; j < newPop.size(); j ++ ) {
	char cad[10]; sprintf(cad,"%d",generation);
	string id(cad);
	newPop[j].ID( id );
      }
      //***************************************************//

      // Replace after evaluation. 
      replace( newPop, oldGeneration );
      
      sort( oldGeneration.begin(), oldGeneration.end(), SortEO<EOT>() ); 
      
      if ( verbose ) {
	       cout << "Evaluated pop ----------------------\n";
	       for ( j = 0; j < oldGeneration.size(); j ++ ) {
	         cout << "Fitness " << j << " " 
	             << oldGeneration[j].fitness() << endl;
	       }

        // información en cada generación...
        double media2 = 0.0, media3 = 0.0;
        for(unsigned m=0; m<oldGeneration.size() ; m++) {
           media2 += oldGeneration[m].fitness().print_criterio2();
           media3 += oldGeneration[m].fitness().print_criterio3();
        }
        media2 = (double) ((double)media2/(double)oldGeneration.size());
        media3 = (double) ((double)media3/(double)oldGeneration.size());
      }

      /* SACAMOS LA INFO DE LA POBLACIÓN COMPLETA (LOS 10) */

      //***************************************************//
      cout << "\ng"<<generation;
      cout <<"\tID= "<< oldGeneration[0].ID()
	   <<"\tc1= "<< oldGeneration[0].fitness().print_criterio1()
	   <<"\tc2= "<< oldGeneration[0].fitness().print_criterio2()
	   <<"\tc3= "<< oldGeneration[0].fitness().print_criterio3()
/*	   << endl
	   << "\tm(distR)= " << media2
	   << "\tm(choques)= " << media3
  */
	   << endl;
      //***************************************************//
     /*
     if( generation == 1 ) {
        cout <<"\n<10_INIT>"<<endl;
        for (unsigned j = 0; j < oldGeneration.size(); j ++ ) {
          cout << j <<"\tID= "<< oldGeneration[j].ID()
          <<"\tc1= "<< oldGeneration[j].fitness().print_criterio1()
          //<<"\tc2= "<< oldGeneration[j].fitness().print_criterio2()
          //<<"\tc3= "<< oldGeneration[j].fitness().print_criterio3()
          <<endl;
          char tmpcad[10]; sprintf(tmpcad,"%d",j);
          string tmpid(tmpcad);
          tmpid = "x_"+tmpid+".mlp";
          oldGeneration[j].save( tmpid );
        }
        cout <<"\n</10_INIT>"<<endl;
     }
     if( generation == 50 ) {
        cout <<"\n<10_MIDDLE>"<<endl;
        for (unsigned j = 0; j < oldGeneration.size(); j ++ ) {
          cout << j <<"\tID= "<< oldGeneration[j].ID()
          <<"\tc1= "<< oldGeneration[j].fitness().print_criterio1()
          //<<"\tc2= "<< oldGeneration[j].fitness().print_criterio2()
          //<<"\tc3= "<< oldGeneration[j].fitness().print_criterio3()
          <<endl;
          char tmpcad[10]; sprintf(tmpcad,"%d",j);
          string tmpid(tmpcad);
          tmpid = "y_"+tmpid+".mlp";
          oldGeneration[j].save( tmpid );
        }
        cout <<"\n</10_MIDDLE>"<<endl;
     }
     if( generation == 100 ) {
        cout <<"\n<10_END>"<<endl;
        for (unsigned j = 0; j < oldGeneration.size(); j ++ ) {
          cout << j <<"\tID= "<< oldGeneration[j].ID()
          <<"\tc1= "<< oldGeneration[j].fitness().print_criterio1()
          //<<"\tc2= "<< oldGeneration[j].fitness().print_criterio2()
          //<<"\tc3= "<< oldGeneration[j].fitness().print_criterio3()
          <<endl;
          char tmpcad[10]; sprintf(tmpcad,"%d",j);
          string tmpid(tmpcad);
          tmpid = "z_"+tmpid+".mlp";
          oldGeneration[j].save( tmpid );
        }
        cout <<"\n</10_END>"<<endl;
     }
     */
      //***************************************************//
      /*
	cout <<"\t"<< mediaA
	<<"\t"<< mediaS
	<<"\t"<< oldGeneration[oldGeneration.size()-1].fitness().print_criterio1()
	<<"\t"<< oldGeneration[oldGeneration.size()-1].fitness().print_criterio2()
	<<"\t"<< oldGeneration[0].getLearning()
	<<endl;
      */

    } catch ( exception& _e  ) {
      throw _e;
    }
    
  };

  // indicarle la generación en que se encuentra...
  void setGenerationNumber(unsigned _g) { 
    generation = _g;
  };


	 // several ways of accesing inner variables
	 ///
	 EOEvaluator<EOT>& getEval() { return eval;};

	 ///
	 unsigned getEvaluated() const { return evaluated;};

 private:
  EOEvaluator<EOT>&  eval;
  EOSelector<EOT>& select;
  EOBreeder<EOT>& breed;
  EOReplace<EOT>& replace;
  
  /// Indicates if it is going to be "verbose": print all things, and so on
  bool verbose;

  /// Total number of evaluated EOs
  unsigned evaluated;
  unsigned generation;
};


#endif
