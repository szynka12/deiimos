#ifndef SRC_XMLHANDLER_H
#define SRC_XMLHANDLER_H

#include "deal.II/base/exceptions.h"
// #include "msgstream.h"
#include "tinyxml2.h"

#include <iostream>
#include <memory>
#include <stdexcept>

namespace deiimos
{
namespace xmlhandler
{
class Node
{
  private:
    tinyxml2::XMLHandle h_;
    std::string         path_;

    void        safe_check( tinyxml2::XMLError e );
    inline void show_me( )
    {
        std::cout << "In xml path:" << path_ << std::endl;
    };

  public:
    Node( tinyxml2::XMLHandle h, std::string path );
    Node get_child( const char* name );

    template < typename T >
    T value( );

    ~Node( );
};

// Exceptions
DeclException1( XMLNoNodeException,
                std::string,
                << "There is no node on the path: " << arg1 );

DeclException0( XML_NO_ATTRIBUTE );
DeclException0( XML_WRONG_ATTRIBUTE_TYPE );
DeclException0( XML_ERROR_FILE_NOT_FOUND );
DeclException0( XML_ERROR_FILE_COULD_NOT_BE_OPENED );
DeclException0( XML_ERROR_FILE_READ_ERROR );
DeclException0( XML_ERROR_PARSING_ELEMENT );
DeclException0( XML_ERROR_PARSING_ATTRIBUTE );
DeclException0( XML_ERROR_PARSING_TEXT );
DeclException0( XML_ERROR_PARSING_CDATA );
DeclException0( XML_ERROR_PARSING_COMMENT );
DeclException0( XML_ERROR_PARSING_DECLARATION );
DeclException0( XML_ERROR_PARSING_UNKNOWN );
DeclException0( XML_ERROR_EMPTY_DOCUMENT );
DeclException0( XML_ERROR_MISMATCHED_ELEMENT );
DeclException0( XML_ERROR_PARSING );
DeclException0( XML_CAN_NOT_CONVERT_TEXT );
DeclException0( XML_NO_TEXT_NODE );
DeclException0( XML_ELEMENT_DEPTH_EXCEEDED );
DeclException0( XML_ERROR_COUNT );

// Declarations
template <>
double Node::value< double >( );

} // namespace xmlhandler
} // namespace deiimos

#endif // SRC_XMLHANDLER_H