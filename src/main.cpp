#include "XMLHandler.h"
#include "msgstream.h"
#include "tinyxml2.h"

#include <array>
#include <chrono>
#include <iostream>
#include <tuple>
#include <utility>

template < typename... Targs >
std::tuple< Targs... > f( Targs... args )
{
    return std::make_tuple( args... );
}

int main( void )
{

    tinyxml2::XMLDocument doc;
    doc.LoadFile( "../data/configs/test1.xml" );

    tinyxml2::XMLHandle docHandle( &doc );

    // deiimos::xmlhandler::Node n( docHandle, "" );
    tinyxml2::XMLHandle h = docHandle.FirstChildElement( "deiimos_config" );

    deiimos::io::XmlNode n( h, std::string( "deiimos_config" ) );
    deiimos::msg( )
      << n.get_child( "section" ).get_child( "double" ).value< double >( )
      << std::endl;

    deiimos::msg( )
      << n.get_child( "section" ).get_child( "text" ).value< std::string >( )
      << std::endl;

    deiimos::msg( )
      << n.get_child( "section" ).get_child( "int" ).value< int >( )
      << std::endl;

    deiimos::msg( )
      << n.get_child( "section" ).get_child( "label" ).value< size_t >( )
      << std::endl;

    deiimos::msg( )
      << n.get_child( "section" ).get_child( "bool" ).value< bool >( )
      << std::endl;

    deiimos::msg( ) << n.get_child( "section" )
                         .get_child( "doubleAttr" )
                         .attribute< double >( "a" )
                    << std::endl;

    deiimos::msg( ) << n.get_child( "section" )
                         .get_child( "doubleAttr" )
                         .attribute_or_default< double >( "b", 3.14 )
                    << std::endl;

    deiimos::msg( )
      << n.get_child( "section" ).get_child( "intAttr" ).attribute< int >( "a" )
      << std::endl;

    deiimos::msg( ) << n.get_child( "section" )
                         .get_child( "labelAttr" )
                         .attribute< size_t >( "a" )
                    << std::endl;

    deiimos::msg( ) << n.get_child( "section" )
                         .get_child( "textAttr" )
                         .attribute< std::string >( "a" )
                    << std::endl;

    deiimos::msg( ) << n.get_child( "section" )
                         .get_child( "boolAttr" )
                         .attribute< bool >( "a" )
                    << std::endl;

    auto t = n.get_child( "section" )
               .get_child( "tuple" )
               .tuple_attribute< int, int, std::string >( "a" );

    deiimos::msg( ) << std::get< 0 >( t ) << " " << std::get< 1 >( t ) << " "
                    << std::get< 2 >( t ) << std::endl;

    return ( 0 );
}