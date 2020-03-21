#ifndef SRC_MESH_MANIFOLDREADER_H
#define SRC_MESH_MANIFOLDREADER_H

#include "XmlHandler.h"

#include <deal.II/grid/manifold.h>
#include <map>
#include <memory>

namespace deiimos
{
namespace mesh
{

template < int dim, int spacedim = dim >
class ManifoldReader
{
  private:
    using ManifoldMap =
      std::map< std::string,
                std::unique_ptr< dealii::Manifold< dim, spacedim > > >;

  public:
    ManifoldReader( );
    ~ManifoldReader( );

    ManifoldMap read_manifolds( io::XmlHandler& config );
};

template < int dim, int spacedim >
ManifoldReader< dim, spacedim >::ManifoldReader( )
{
}

template < int dim, int spacedim >
ManifoldReader< dim, spacedim >::~ManifoldReader( )
{
}

template < int dim, int spacedim >
typename ManifoldReader< dim, spacedim >::ManifoldMap
  ManifoldReader< dim, spacedim >::read_manifolds( io::XmlHandler& config )
{
    deiimos::msg( ) << "Reading mainfolds... ";
    auto manifold_vec =
      config["deiimos_config"]["manifolds"].get_siblings( "m" );

    for ( auto& manifold : manifold_vec )
    {
        const std::string manifold_name = manifold.attribute( "name" );
        const char*       manifold_type = manifold.attribute( "type" ).c_str( );
    }
}

} // namespace mesh
} // namespace deiimos

#endif // SRC_MESH_MANIFOLDREADER_H
