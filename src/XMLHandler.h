#ifndef SRC_XMLHANDLER_H
#define SRC_XMLHANDLER_H

#include "tinyxml2.h"

#include <memory>

namespace deiimos
{
namespace xmlhandler
{
class Node
{
  private:
    tinyxml2::XMLHandle h_;

  public:
    Node( tinyxml2::XMLHandle h );
    ~Node( );
};

} // namespace xmlhandler
} // namespace deiimos

#endif // SRC_XMLHANDLER_H