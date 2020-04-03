#ifndef SRC_MESH_MANIFOLDFACTORY_H
#define SRC_MESH_MANIFOLDFACTORY_H

#include "XmlHandler.h"
#include "deal.II/base/exceptions.h"

#include <deal.II/grid/manifold.h>
#include <deal.II/grid/manifold_lib.h>
#include <memory>
#include <set>

namespace deiimos
{
namespace mesh
{
template < int dim, int spacedim >
class ManifoldFactory
{
  public:
    using ManifoldPtr = std::unique_ptr< dealii::Manifold< dim, spacedim > >;

    static ManifoldPtr create( std::string type, io::XmlNode manifold_node );

  private:
    static ManifoldPtr create_spherical( io::XmlNode manifold_node );
};

template < int dim, int spacedim >
typename ManifoldFactory< dim, spacedim >::ManifoldPtr
  ManifoldFactory< dim, spacedim >::create( std::string type,
                                            io::XmlNode manifold_node )
{
    // std::set< std::string > supported_manifold_types_ = {"spherical"};

    // Assert( supported_manifold_types_.find( type ) !=
    //           supported_manifold_types_.end( ),
    //         dealii::ExcMessage(
    //           "Unsupported manifold type found in xml. "
    //           "Suported types are:\n"
    //           "\t <spherical> -> SphericalManifold< dim, spacedim >" ) );

    if ( type == "spherical" )
        return create_spherical( manifold_node );
    else
    {
        dealii::ExcMessage(
          "Unsupported manifold type found in xml. "
          "Suported types are:\n"
          "\t <spherical> -> SphericalManifold< dim, spacedim >" );
    }
    return nullptr;
}

template < int dim, int spacedim >
typename ManifoldFactory< dim, spacedim >::ManifoldPtr
  ManifoldFactory< dim, spacedim >::create_spherical(
    io::XmlNode manifold_node )
{
    Assert(
      manifold_node.has_child( "center" ) != manifold_node.has_child( "point" ),
      dealii::ExcMessage( "For SphericalManifolds <center> or <point> must "
                          "be specified in XML. " ) );

    if ( manifold_node.has_child( "center" ) )
    {
        auto center_array =
          manifold_node["center"].array_value< double, spacedim >( );
        dealii::Point< spacedim > center;
        for ( size_t j = 0; j < spacedim; j++ ) { center[j] = center_array[j]; }
        return std::make_unique< dealii::SphericalManifold< dim, spacedim > >(
          center );
    }
    if ( manifold_node.has_child( "point" ) ) { dealii::ExcNotImplemented( ); }
    return nullptr;
}
} // namespace mesh
} // namespace deiimos

#endif