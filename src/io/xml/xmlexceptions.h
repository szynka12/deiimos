#ifndef SRC_IO_XML_XMLEXCEPTIONS_H
#define SRC_IO_XML_XMLEXCEPTIONS_H

#include "deal.II/base/exceptions.h"
#include "tinyxml2.h"

namespace deiimos
{
namespace io
{
DeclException1( XMLNoNodeException,
                std::string,
                << "There is no node on the path: " << arg1 );

DeclException2( XML_NO_ATTRIBUTE,
                std::string,
                std::string,
                << "No attribute named <" << arg2 << "> found in node <" << arg1
                << ">." );
DeclException0( XML_WRONG_ATTRIBUTE_TYPE );
DeclException1( XML_ERROR_FILE_NOT_FOUND,
                std::string,
                << "No file found on path <" << arg1 << ">." );
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
DeclException2( XML_CAN_NOT_CONVERT_TEXT,
                std::string,
                std::string,
                << "Unable to convert text in node <" << arg1 << "> to type <"
                << arg2 << ">." );

DeclException1( XML_NO_TEXT_NODE,
                std::string,
                << "No text present in node <" << arg1 << ">." );
DeclException0( XML_ELEMENT_DEPTH_EXCEEDED );
DeclException0( XML_ERROR_COUNT );

void xml_check( tinyxml2::XMLError e,
                std::string        arg1 = std::string( "" ),
                std::string        arg2 = std::string( "" ),
                std::string        arg3 = std::string( "" ) );

} // namespace io
} // namespace deiimos

#endif // SRC_IO_XML_XMLEXCEPTIONS_H