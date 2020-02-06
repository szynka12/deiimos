#include "msgstream.h"

#include <chrono>
#include <iostream>

int main( void )
{

    deiimos::verbosity( 1 );
    deiimos::set_warning( true );

    deiimos::msg( ) << "SILENT" << std::endl;
    deiimos::msg( 1 ) << "INFO1" << std::endl;
    deiimos::msg( 2 ) << "INFO2" << std::endl;
    deiimos::msg( 3 ) << "INFO3" << std::endl;
    deiimos::err( ) << "Error!" << std::endl;

    deiimos::warn( ) << "Warning" << std::endl;

    return ( 0 );
}