#ifndef SRC_XMLHANDLER_H
#define SRC_XMLHANDLER_H

#include "deal.II/base/exceptions.h"
#include "msgstream.h"
#include "tinyxml2.h"
#include "xmlexceptions.h"

#include <iostream>
#include <locale>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <utility>

namespace deiimos
{
namespace io
{
class XmlNode
{
  private:
    tinyxml2::XMLHandle h_;
    std::string         path_;

    inline void show_me( )
    {
        deiimos::msg( ) << "In xml path:" << path_ << std::endl;
    };

    template < typename T >
    bool read_tuple_element( std::istream& in, T& value );

    template < typename Tuple, std::size_t... I >
    void read_tuple_elements( std::istream& in,
                              Tuple&        value,
                              std::index_sequence< I... > );

    template < typename... Targs >
    std::tuple< Targs... > to_tuple( std::string str_ );

  public:
    XmlNode( tinyxml2::XMLHandle h, std::string path );
    XmlNode get_child( const char* name );

    template < typename T >
    T value( );

    template < typename... Targs >
    std::tuple< Targs... > tuple_value( );

    template < typename T >
    T attribute( const char* name );

    template < typename... Targs >
    std::tuple< Targs... > tuple_attribute( const char* name );

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
bool XmlNode::read_tuple_element( std::istream& in, T& value )
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
      read_tuple_element( in, std::get< I >( value ) )...};
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

template < typename... Targs >
std::tuple< Targs... > XmlNode::tuple_value( )
{
    return to_tuple< Targs... >( value< std::string >( ) );
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

#endif // SRC_XMLHANDLER_H