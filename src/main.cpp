#include "XMLHandler.h"
#include "msgstream.h"
#include "tinyxml2.h"

#include <chrono>
#include <iostream>

int main( void )
{

    tinyxml2::XMLDocument doc;
    doc.LoadFile( "../data/configs/test1.xml" );

    tinyxml2::XMLHandle docHandle( &doc );

    // deiimos::xmlhandler::Node n( docHandle, "" );
    tinyxml2::XMLHandle h = docHandle.FirstChildElement( "deiimos_config" );

    deiimos::xmlhandler::Node n( h, std::string( "deiimos_config" ) );
    deiimos::msg( )
      << n.get_child( "section" ).get_child( "double" ).value< double >( )
      << std::endl;

    return ( 0 );
}