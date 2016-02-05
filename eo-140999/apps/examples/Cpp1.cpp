#include <strstream.h>
#include <iostream.h>
#include <string>

using namespace std;

main() {
	string s("1234");
	istrstream is( s.c_str() );
	unsigned u;

	is >> u;

	cout << u;
}
	