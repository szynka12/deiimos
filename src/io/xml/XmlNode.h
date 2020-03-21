#ifndef SRC_IO_XML_XMLNODE_H
#define SRC_IO_XML_XMLNODE_H

#include "deal.II/base/exceptions.h"
#include "msgstream.h"
#include "tinyxml2.h"
#include "xmlexceptions.h"

#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

namespace deiimos
{
namespace io
{
class XmlNode
{
  private:
    // read one element from the space delimited string and convert it to one
    // element of the Type T
    template < typename T >
    bool read_element( std::istream& in, T& value );

    // write several elements from space delimited string into the Tuple T
    template < typename Tuple, std::size_t... I >
    void read_tuple_elements( std::istream& in,
                              Tuple&        value,
                              std::index_sequence< I... > );

    // convert space delimited string to tuple
    template < typename... Targs >
    std::tuple< Targs... > to_tuple( std::string str_ );

    template < typename T, int N >
    std::array< T, N > to_array( std::string str_ );

  protected:
    // handle wrapping a node pointer
    tinyxml2::XMLHandle h_;

    // path to node in xml tree
    std::string path_;

    XmlNode( ) : h_( nullptr ){};

  public:
    XmlNode( tinyxml2::XMLHandle h, std::string path );

    // get child node
    XmlNode get_child( const char* name );

    bool has_child( const char* name );

    std::vector< XmlNode > get_siblings( const char* name );

    inline XmlNode operator[]( const char* name ) { return get_child( name ); }

    // get value
    template < typename T >
    T value( );

    // get tuple of values
    template < typename... Targs >
    std::tuple< Targs... > tuple_value( );

    // get array of values
    template < typename T, int N >
    std::array< T, N > array_value( );

    // get attribute value
    template < typename T >
    T attribute( const char* name );

    // get tuple of attribute values
    template < typename... Targs >
    std::tuple< Targs... > tuple_attribute( const char* name );

    // get attribute value or default value
    template < typename T >
    T attribute_or_default( const char* name, T default_value );

    ~XmlNode( );
};

template < typename T >
T XmlNode::attribute_or_default( const char* name, T default_value )
{
    T value;
    try
    {
        value = attribute< T >( name );
    }
    catch ( const XML_NO_ATTRIBUTE& e )
    {
        value = default_value;
    }
    return value;
}

template < typename T >
bool XmlNode::read_element( std::istream& in, T& value )
{
    in >> value;
    return true;
}

template < typename Tuple, std::size_t... I >
void XmlNode::read_tuple_elements( std::istream& in,
                                   Tuple&        value,
                                   std::index_sequence< I... > )
{
    std::initializer_list< bool >{
      read_element( in, std::get< I >( value ) )...};
}

template < typename... Targs >
std::tuple< Targs... > XmlNode::to_tuple( std::string str_ )
{
    std::istringstream in( str_ );

    std::tuple< Targs... > t;
    read_tuple_elements(
      in, t, std::make_index_sequence< sizeof...( Targs ) >{} );
    if ( in && in.eof( ) ) { return t; }
    else
    {
        throw XML_CAN_NOT_CONVERT_TEXT( path_, std::string( "tuple" ) );
    }
}

template < typename T, int N >
std::array< T, N > XmlNode::to_array( std::string str_ )
{
    std::istringstream in( str_ );
    std::array< T, N > a;
    for ( int i = 0; i < N; i++ ) { read_element( in, a[i] ); }
    if ( in && in.eof( ) ) { return a; }
    else
    {
        throw XML_CAN_NOT_CONVERT_TEXT(
          path_, std::string( "array with N = " + std::to_string( N ) ) );
    }
}

template < typename... Targs >
std::tuple< Targs... > XmlNode::tuple_value( )
{
    return to_tuple< Targs... >( value< std::string >( ) );
}

template < typename T, int N >
std::array< T, N > XmlNode::array_value( )
{
    return to_array< T, N >( value< std::string >( ) );
}

template < typename... Targs >
std::tuple< Targs... > XmlNode::tuple_attribute( const char* name )
{
    return to_tuple< Targs... >( attribute< std::string >( name ) );
}
// Declarations
template <>
double XmlNode::value< double >( );

template <>
std::string XmlNode::value< std::string >( );

template <>
int XmlNode::value< int >( );

template <>
unsigned long int XmlNode::value< unsigned long int >( );

template <>
bool XmlNode::value< bool >( );

template <>
double XmlNode::attribute< double >( const char* name );

template <>
std::string XmlNode::attribute< std::string >( const char* name );

template <>
int XmlNode::attribute< int >( const char* name );

template <>
unsigned long int XmlNode::attribute< unsigned long int >( const char* name );

template <>
bool XmlNode::attribute< bool >( const char* name );

} // namespace io
} // namespace deiimos

#endif // SRC_IO_XML_XMLNODE_H