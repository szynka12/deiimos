#include "xmlexceptions.h"

namespace deiimos
{
namespace io
{
void xml_check( tinyxml2::XMLError e,
                std::string        arg1,
                std::string        arg2,
                std::string        arg3 )
{
    switch ( e )
    {
        case tinyxml2::XML_SUCCESS: break;
        case tinyxml2::XML_NO_ATTRIBUTE: throw XML_NO_ATTRIBUTE( arg1, arg2 );
        case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
            throw XML_WRONG_ATTRIBUTE_TYPE( );
        case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
            throw XML_ERROR_FILE_NOT_FOUND( arg1 );
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
            throw XML_CAN_NOT_CONVERT_TEXT( arg1, arg2 );
        case tinyxml2::XML_NO_TEXT_NODE: throw XML_NO_TEXT_NODE( arg1 );
        case tinyxml2::XML_ELEMENT_DEPTH_EXCEEDED:
            throw XML_ELEMENT_DEPTH_EXCEEDED( );
        case tinyxml2::XML_ERROR_COUNT: throw XML_ERROR_COUNT( );
    }
}
} // namespace io
} // namespace deiimos