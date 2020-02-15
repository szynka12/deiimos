#include "XMLHandler.h"

namespace deiimos
{
namespace io
{

XmlNode::XmlNode( tinyxml2::XMLHandle h, std::string path )
  : h_( h ), path_( path )
{
    // check if underlying node is not nullptr
    AssertThrow( h_.ToNode( ), XMLNoNodeException( path ) );
}

XmlNode::~XmlNode( ) {}

XmlNode XmlNode::get_child( const char* name )
{
    return XmlNode( h_.FirstChildElement( name ), path_ + "/" + name );
}

// Node value getters for various data types

template <>
double XmlNode::value< double >( )
{
    double value;
    xml_check( h_.ToElement( )->QueryDoubleText( &value ),
               path_,
               std::string( "double" ) );
    return value;
}

template <>
unsigned long int XmlNode::value< unsigned long int >( )
{
    unsigned long int value;
    xml_check( h_.ToElement( )->QueryUnsigned64Text( &value ),
               path_,
               std::string( "unsigned long int" ) );
    return value;
}

template <>
int XmlNode::value< int >( )
{
    int value;
    xml_check(
      h_.ToElement( )->QueryIntText( &value ), path_, std::string( "int" ) );
    return value;
}

template <>
std::string XmlNode::value< std::string >( )
{

    if ( !h_.ToElement( )->GetText( ) ) { throw XML_NO_TEXT_NODE( path_ ); }
    return std::string( h_.ToElement( )->GetText( ) );
}

template <>
bool XmlNode::value< bool >( )
{
    bool value;
    xml_check(
      h_.ToElement( )->QueryBoolText( &value ), path_, std::string( "bool" ) );
    return value;
}

// Attribute getters for various data types

template <>
double XmlNode::attribute< double >( const char* name )
{
    double value;
    xml_check( h_.ToElement( )->QueryDoubleAttribute( name, &value ),
               path_,
               std::string( name ) );
    return value;
}

template <>
unsigned long int XmlNode::attribute< unsigned long int >( const char* name )
{
    unsigned long int value;
    xml_check( h_.ToElement( )->QueryUnsigned64Attribute( name, &value ),
               path_,
               std::string( name ) );
    return value;
}

template <>
int XmlNode::attribute< int >( const char* name )
{
    int value;
    xml_check( h_.ToElement( )->QueryIntAttribute( name, &value ),
               path_,
               std::string( name ) );
    return value;
}

template <>
std::string XmlNode::attribute< std::string >( const char* name )
{
    const char* value;
    xml_check( h_.ToElement( )->QueryStringAttribute( name, &value ),
               path_,
               std::string( name ) );

    return std::string( value );
}

template <>
bool XmlNode::attribute< bool >( const char* name )
{
    bool value;
    xml_check( h_.ToElement( )->QueryBoolAttribute( name, &value ),
               path_,
               std::string( name ) );
    return value;
}

} // namespace io
} // namespace deiimos
