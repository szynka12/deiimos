#include "XMLHandler.h"

namespace deiimos
{
namespace xmlhandler
{

Node::Node( tinyxml2::XMLHandle h, std::string path ) : h_( h ), path_( path )
{
    // check if underlying node is not nullptr
    AssertThrow( h_.ToNode( ), XMLNoNodeException( path ) );
}

Node Node::get_child( const char* name )
{
    return Node( h_.FirstChildElement( name ), path_ + "/" + name );
}

void Node::safe_check( tinyxml2::XMLError e )
{
    switch ( e )
    {
        case tinyxml2::XML_SUCCESS: break;
        case tinyxml2::XML_NO_ATTRIBUTE: throw XML_NO_ATTRIBUTE( );
        case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
            throw XML_WRONG_ATTRIBUTE_TYPE( );
        case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
            throw XML_ERROR_FILE_NOT_FOUND( );
        case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
            throw XML_ERROR_FILE_COULD_NOT_BE_OPENED( );
        case tinyxml2::XML_ERROR_FILE_READ_ERROR:
            throw XML_ERROR_FILE_READ_ERROR( );
        case tinyxml2::XML_ERROR_PARSING_ELEMENT:
            throw XML_ERROR_PARSING_ELEMENT( );
        case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE:
            throw XML_ERROR_PARSING_ATTRIBUTE( );
        case tinyxml2::XML_ERROR_PARSING_TEXT: throw XML_ERROR_PARSING_TEXT( );
        case tinyxml2::XML_ERROR_PARSING_CDATA:
            throw XML_ERROR_PARSING_CDATA( );
        case tinyxml2::XML_ERROR_PARSING_COMMENT:
            throw XML_ERROR_PARSING_COMMENT( );
        case tinyxml2::XML_ERROR_PARSING_DECLARATION:
            throw XML_ERROR_PARSING_DECLARATION( );
        case tinyxml2::XML_ERROR_PARSING_UNKNOWN:
            throw XML_ERROR_PARSING_UNKNOWN( );
        case tinyxml2::XML_ERROR_EMPTY_DOCUMENT:
            throw XML_ERROR_EMPTY_DOCUMENT( );
        case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT:
            throw XML_ERROR_MISMATCHED_ELEMENT( );
        case tinyxml2::XML_ERROR_PARSING: throw XML_ERROR_PARSING( );
        case tinyxml2::XML_CAN_NOT_CONVERT_TEXT:
            throw XML_CAN_NOT_CONVERT_TEXT( );
        case tinyxml2::XML_NO_TEXT_NODE: throw XML_NO_TEXT_NODE( );
        case tinyxml2::XML_ELEMENT_DEPTH_EXCEEDED:
            throw XML_ELEMENT_DEPTH_EXCEEDED( );
        case tinyxml2::XML_ERROR_COUNT: throw XML_ERROR_COUNT( );
    }
}

template <>
double Node::value< double >( )
{
    double value;
    try
    {
        safe_check( h_.ToElement( )->QueryDoubleText( &value ) );
    }
    catch ( ... )
    {
        show_me( );
        throw;
    }
    return value;
}

Node::~Node( ) {}

} // namespace xmlhandler
} // namespace deiimos
