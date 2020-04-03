#ifndef SRC_MESH_TOPOLOGYSET_H
#define SRC_MESH_TOPOLOGYSET_H

#include "deal.II/base/exceptions.h"

#include <algorithm>
#include <array>

namespace deiimos
{
namespace mesh
{

DeclException2( BAD_TOPOLOGY_SET,
                int,
                std::string,
                << "Bad topology set of dim <" << arg1
                << ">, formed from points " << arg2 );

template < int dim >
class TopologySet
{
  private:
    constexpr static int points_per_cell_ = dealii::Utilities::pow( 2, dim );
    std::array< size_t, points_per_cell_ > point_ids_;
    std::string                            points_str;

  public:
    TopologySet( std::array< size_t, points_per_cell_ > points );
    ~TopologySet( ){};

    bool               operator<( const TopologySet< dim >& other_set ) const;
    inline std::string str( ) const;
};

template < int dim >
TopologySet< dim >::TopologySet( std::array< size_t, points_per_cell_ > points )
  : point_ids_( points )
{

    // generate string from the array, it will be used in case exception is
    // thrown
    points_str = "<";
    for ( auto p : points ) { points_str += std::to_string( p ) + " "; }
    points_str += ">";

    std::sort( point_ids_.begin( ), point_ids_.end( ) );
    for ( auto p = point_ids_.begin( ); p != point_ids_.end( ); p++ )
    { Assert( ( *p ) == ( *( p++ ) ), BAD_TOPOLOGY_SET( dim, points_str ) ) }
}

template < int dim >
bool TopologySet< dim >::operator<( const TopologySet< dim >& other_set ) const
{
    for ( size_t i = 0; i < points_per_cell_; i++ )
    {
        if ( this->point_ids_[i] < other_set.point_ids_[i] ) { return true; }
    }
    return false;
}

template < int dim >
std::string TopologySet< dim >::str( ) const
{
    return points_str;
}

} // namespace mesh
} // namespace deiimos

#endif // SRC_MESH_TOPOLOGYSET_H
