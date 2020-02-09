#include "XMLHandler.h"
#include "msgstream.h"
#include "tinyxml2.h"

#include <chrono>
#include <iostream>

int main( void )
{

    tinyxml2::XMLDocument doc;
    doc.LoadFile( "../data/configs/test1.xml" );

    // tinyxml2::XMLHandle handle( &doc );

    double a = 1;

    // tinyxml2::XMLElement* el = handle.FirstChildElement( "deiimos_config" )
    //                              .FirstChildElement( "section" )
    //                              .FirstChildElement( "double" );

    tinyxml2::XMLHandle docHandle( &doc );

    deiimos::xmlhandler::Node n( docHandle );

    tinyxml2::XMLElement* child2 =
      docHandle.FirstChildElement( "Document" ).ToElement( );
    if ( !child2 ) { throw std::invalid_argument( "aaa" ); }

    deiimos::msg( ) << a << std::endl;

    return ( 0 );
}