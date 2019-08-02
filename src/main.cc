#include <co.hpp>

int main( int argc, char **argv )
{
	co::run();

	co::spawn( [] {
		return co::Result<int, std::string>::Ok( 1 );
	} )
	  .and_then( []( int a ) {
		  return co::Result<double, std::string>::Ok( 0.5 );
	  } );
}