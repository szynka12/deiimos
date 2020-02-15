#ifndef SRC_IO_XML_XMLHANDLER_H
#define SRC_IO_XML_XMLHANDLER_H

#include "XmlNode.h"

namespace deiimos
{
namespace io
{

class XmlHandler : public XmlNode
{
  private:
    const char*           path_to_xml_;
    tinyxml2::XMLDocument document;

  public:
    XmlHandler( const char* path_to_xml );

    ~XmlHandler( );

    void load( );
};

XmlHandler::XmlHandler( const char* path_to_xml ) : path_to_xml_( path_to_xml )
{
}

void XmlHandler::load( )
{
    xml_check( document.LoadFile( path_to_xml_ ), std::string( path_to_xml_ ) );
    h_    = tinyxml2::XMLHandle( document );
    path_ = "";
}

XmlHandler::~XmlHandler( ) {}
} // namespace io
} // namespace deiimos

#endif // SRC_IO_XML_XMLHANDLER_H