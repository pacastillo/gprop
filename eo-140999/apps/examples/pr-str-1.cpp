// to avoid warning with long names in MSVC++
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <iostream>

using namespace std;

// EO classes
#include <generic/EOGOps.h>
#include <specific/EOString.h>	// for EOString

main() {
  typedef EOString<float> EOStr;  // Short name
  
  // Declare and print using default fitness = float
  EOStr EOSA( "12345678"), EOSA2("abcdefgh");
  cout << "EOSA " <<  EOSA << endl << EOSA2 << endl;
  
  // Assignment operator
  EOStr strEo1;
  strEo1 = EOSA;
  cout << "strEo1 = EOSA " << strEo1 << endl << EOSA << endl;

  // apply transposer
  EOTranspose<EOStr> transposer;
  EOStr strEo2 = EOSA;
  transposer( strEo2 );
  cout << "strEo2 = transpose ( EOSA )" << strEo2 << endl;
  
   // apply xover
   EOXOver2<EOStr> xOver; 
   EOStr strEo3 = EOSA;
   xOver( strEo3, EOSA2 );
   cout << "strEo3 , EOSA2 " << strEo3 
        << endl << EOSA2 << endl;
  
   // Test exceptions
   try {
     cout << EOSA.readGene( EOSA.length() + 1 );
   } catch ( exception& _e ) {
     cout << _e.what() << endl;
   }
  
   return 0;
}
	
