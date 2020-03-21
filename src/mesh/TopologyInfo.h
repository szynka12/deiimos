#ifndef SRC_MESH_TOPOLOGYINFO_H
#define SRC_MESH_TOPOLOGYINFO_H

#include "TopologySet.h"
#include "deal.II/base/exceptions.h"
#include "msgstream.h"

#include <array>
#include <map>
#include <tuple>

namespace deiimos
{
namespace mesh
{

DeclException1(
  INERIOR_FACE_WITH_MULTIPLE_NEIGHGBOURS,
  std::string,
  << "The face " << arg1
  << " has more than two adiacent cells (impossible if dim=spacedim)" );

template < int dim, int spacedim, int... structdims >
class TopologyInfo
{
  public:
    enum PATCH_TYPE
    {
        PATCH    = 0,
        INTERNAL = 1
    };

    template < int dim_ >
    using TopologyMap =
      std::map< TopologySet< dim_ >,
                std::pair< dealii::TriaIterator<
                             dealii::TriaAccessor< dim_, dim, spacedim > >,
                           PATCH_TYPE > >;

  private:
    std::tuple< TopologyMap< structdims >... > topo_map_;

  public:
    TopologyInfo( ){};
    ~TopologyInfo( ){};

    template < int dim_ >
    void insert( TopologySet< dim_ >     set,
                 std::pair< dealii::TriaAccessor< dim_, dim, spacedim >,
                            PATCH_TYPE > pointer_type_pair )
    {
        // Check if the face is present in the topology map of correct dim. If
        // it is change its type to <INTERNAL>. If it was already of type
        // <INTERNAL> and dimension conditions are met, throw an exception.

        auto& map_ref = std::get< dim_ >( topo_map_ );

        auto map_it = map_ref.find( set );
        if ( map_it == map_ref.end( ) )
        {
            map_ref[set] = pointer_type_pair;
            deiimos::msg( 2 ) << "Adding to FaceTopologyMap." << std::endl;
        }
        else
        {
            deiimos::msg( 2 ) << "Repeated entry... ";
            if ( map_it->second.second == PATCH_TYPE::PATCH )
            {
                map_it->second.second = PATCH_TYPE::INTERNAL;
                deiimos::msg( 2 ) << "Changing type to INTERNAL." << std::endl;
            }
            else
            {
                Assert( !( spacedim == dim_ &&
                           map_it->second.second == PATCH_TYPE::INTERNAL ),
                        INERIOR_FACE_WITH_MULTIPLE_NEIGHGBOURS( set.str( ) ) );

                deiimos::msg( 2 ) << "Ok." << std::endl;

                deiimos::warn( )
                  << "Face with multiple adiacent cells created!";
            }
        }
    }
};

template < int dim, int spacedim, size_t... dims >
auto get_topology_info_imp_( std::integer_sequence< size_t, dims... > )
{
    return TopologyInfo< dim, spacedim, dims... >( );
}

template < int dim, int spacedim, int max_topo_dim_ >
auto get_topology_info( )
{
    return get_topology_info_imp_< dim, spacedim >(
      std::make_index_sequence< max_topo_dim_ >{} );
}

} // namespace mesh
} // namespace deiimos

#endif // SRC_MESH_TOPOLOGYINFO_H