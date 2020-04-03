#ifndef SRC_MESH_MESHREADER_H
#define SRC_MESH_MESHREADER_H

#include "ManifoldFactory.h"
#include "TopologyInfo.h"
#include "XmlHandler.h"
#include "deal.II/base/exceptions.h"

#include <array>
#include <deal.II/base/geometry_info.h>
#include <deal.II/base/utilities.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_reordering.h>
#include <deal.II/grid/grid_tools.h>
#include <deal.II/grid/manifold.h>
#include <deal.II/grid/tria.h>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace deiimos
{
namespace mesh
{

// DeclException1(
//   INERIOR_FACE_WITH_MULTIPLE_NEIGHGBOURS,
//   std::string,
//   << "The face " << arg1
//   << " has more than two adiacent cells (impossible if dim=spacedim)" );

template < int dim, int spacedim = dim >
class MeshReader
{
  private:
    constexpr static int c_coords_number_   = spacedim;
    constexpr static int c_points_per_cell_ = dealii::Utilities::pow( 2, dim );
    constexpr static int c_points_per_face_ =
      dealii::Utilities::pow( 2, dim - 1 );
    constexpr static int c_faces_per_cell_ = dim * 2;

  public:
    using Vert        = dealii::Point< c_coords_number_, double >;
    using CellIndices = std::array< size_t, c_points_per_cell_ >;
    using MeshType    = dealii::Triangulation< dim, spacedim >;
    using ManifoldMap =
      std::map< std::string,
                std::unique_ptr< dealii::Manifold< dim, spacedim > > >;

    enum PATCH_TYPE
    {
        PATCH    = 0,
        INTERNAL = 1
    };

    using FaceTopologyMap =
      std::map< TopologySet< dim - 1 >,
                std::pair< dealii::TriaIterator<
                             dealii::TriaAccessor< dim - 1, dim, spacedim > >,
                           PATCH_TYPE > >;

    auto t = deiimos::mesh::get_topology_info< dim, 3, 3 >( );

  private:
    std::vector< Vert >        verts_;
    std::vector< CellIndices > cells_;
    std::vector< MeshType >    sub_mesh_;
    MeshType&                  result_mesh_ref_;
    ManifoldMap                manifold_map_;

    // Map <TopologySet, TriaAccesor>
    //    Warning pointers are invalidated after mesh merging
    FaceTopologyMap boundary_map;
    double          duplicated_vertex_tolerance_ = 1e-12;

  public:
    MeshReader( MeshType& result_mesh_ref )
      : result_mesh_ref_( result_mesh_ref ){};
    ~MeshReader( ){};

    void read_verts( deiimos::io::XmlHandler& config );
    void read_cells( deiimos::io::XmlHandler& config );
    void generate_topology_data( );
    void read_manifolds( deiimos::io::XmlHandler& config );

    inline void set_tolerance( double duplicated_vertex_tolerance )
    {
        duplicated_vertex_tolerance_ = duplicated_vertex_tolerance;
    }
    inline double get_tolerance( ) { return duplicated_vertex_tolerance_; }

    std::vector< Vert > cell_indices_to_coords( size_t cell_id );

    // same as dealii::merge_triangulations but always copies bc ids and
    // manifolds
    void
      attach_mesh( const std::initializer_list< const MeshType* const >& source,
                   MeshType&                                             target,
                   const double duplicated_vertex_tolerance = 1e-12 );

    void generate_meshes( );

    void combine_meshes( );

    void clear_meshes( );
};

template < int dim, int spacedim >
void MeshReader< dim, spacedim >::read_verts( deiimos::io::XmlHandler& config )
{
    deiimos::msg( ) << "Reading vertices... ";

    auto v_config = config["deiimos_config"]["vertices"].get_siblings( "v" );
    verts_        = std::vector< Vert >( v_config.size( ) );

    for ( size_t i = 0; i < v_config.size( ); i++ )
    {
        auto tmp = v_config[i].array_value< double, c_coords_number_ >( );
        for ( size_t j = 0; j < c_coords_number_; j++ )
        { verts_[i][j] = tmp[j]; }
    }

    deiimos::msg( ) << "Done." << std::endl;
    deiimos::msg( 2 ) << "  # of vertices: " << verts_.size( ) << std::endl;
}

template < int dim, int spacedim >
void MeshReader< dim, spacedim >::read_cells( deiimos::io::XmlHandler& config )
{
    deiimos::msg( ) << "Reading cells... ";

    auto c_config = config["deiimos_config"]["cells"].get_siblings( "c" );
    cells_        = std::vector< CellIndices >( c_config.size( ) );

    for ( size_t i = 0; i < c_config.size( ); i++ )
    { cells_[i] = c_config[i].array_value< size_t, c_points_per_cell_ >( ); }

    deiimos::msg( ) << "Done." << std::endl;
    deiimos::msg( 2 ) << "  # of cells: " << cells_.size( ) << std::endl;
}

template < int dim, int spacedim >
void MeshReader< dim, spacedim >::generate_topology_data( )
{
    deiimos::msg( ) << "Reading mainfolds... ";
    deiimos::msg( 2 ) << std::endl;

    for ( size_t cell_i = 0; cell_i < cells_.size( ); cell_i++ )
    {
        const auto& cell_ref = cells_[cell_i];
        const auto& mesh_ref = sub_mesh_[cell_i].begin( );
        for ( size_t f = 0; f < c_faces_per_cell_; f++ )
        {
            std::array< size_t, c_points_per_face_ > tmp;
            for ( size_t v = 0; v < c_points_per_face_; v++ )
            {
                // Convert local number of a vertex to global one using
                // information from cell table. Than put it insite temporary
                // array used for TopologySet creation
                tmp[v] =
                  cell_ref[dealii::GeometryInfo< dim >::face_to_cell_vertices(
                    f, v )];
            }
            // create topology set
            TopologySet< dim - 1 > face_topo( tmp );
            deiimos::msg( 2 )
              << "Creating TopologySet with dim = " << ( dim - 1 )
              << " and point ids " << face_topo.str( ) << "... ";

            // get pointer to the face and make pair
            auto face_pair =
              std::make_pair( mesh_ref->face( f ), PATCH_TYPE::PATCH );

            // Check if the face is present in the topology map. If it is change
            // its type to <INTERNAL>. If it was of type <INTERNAL> already and
            // dimension conditions are met throw an exception.
            auto map_it = boundary_map.find( face_topo );
            if ( map_it == boundary_map.end( ) )
            {
                boundary_map[face_topo] = face_pair;
                deiimos::msg( 2 ) << "Adding to FaceTopologyMap." << std::endl;
            }
            else
            {
                deiimos::msg( 2 ) << "Repeated entry... ";
                if ( map_it->second.second == PATCH_TYPE::PATCH )
                {
                    map_it->second.second = PATCH_TYPE::INTERNAL;
                    deiimos::msg( 2 )
                      << "Changing type to INTERNAL." << std::endl;
                }
                else
                {
                    Assert( !( spacedim == dim &&
                               map_it->second.second == PATCH_TYPE::INTERNAL ),
                            INERIOR_FACE_WITH_MULTIPLE_NEIGHGBOURS(
                              face_topo.str( ) ) );

                    deiimos::msg( 2 )
                      << "No chenge of type needed." << std::endl;

                    deiimos::warn( )
                      << "Face with multiple adiacent cells created!";
                }
            }
        }
    }
    deiimos::msg( ) << "Done." << std::endl;
}

template < int dim, int spacedim >
void MeshReader< dim, spacedim >::read_manifolds( io::XmlHandler& config )
{
    deiimos::msg( ) << "Reading mainfolds... ";
    deiimos::msg( 2 ) << std::endl;

    auto manifold_vec =
      config["deiimos_config"]["manifolds"].get_siblings( "m" );

    for ( auto& manifold : manifold_vec )
    {
        const std::string manifold_name =
          manifold.attribute< std::string >( "name" );
        const std::string manifold_type =
          manifold.attribute< std::string >( "type" );
        manifold_map_[manifold_name] =
          ManifoldFactory< dim, spacedim >::create( manifold_type, manifold );

        deiimos::msg( 2 ) << "\t Name: " + manifold_name +
                               "\t| Type: " + manifold_type
                          << std::endl;
    }
    deiimos::msg( ) << "Done." << std::endl;
}

template < int dim, int spacedim >
std::vector< typename MeshReader< dim, spacedim >::Vert >
  MeshReader< dim, spacedim >::cell_indices_to_coords( size_t cell_id )
{
    std::vector< Vert > out( c_points_per_cell_ );
    for ( size_t i = 0; i < c_points_per_cell_; i++ )
    { out[i] = verts_[cells_[cell_id][i]]; }
    return out;
}

template < int dim, int spacedim >
void MeshReader< dim, spacedim >::generate_meshes( )
{
    deiimos::msg( ) << "Generating primitive cells... ";

    sub_mesh_ = std::vector< MeshType >( cells_.size( ) );

    for ( size_t i = 0; i < sub_mesh_.size( ); i++ )
    {
        dealii::GridGenerator::general_cell(
          sub_mesh_[i], cell_indices_to_coords( i ), true );
    }

    deiimos::msg( ) << "Done." << std::endl;
}

template < int dim, int spacedim >
void MeshReader< dim, spacedim >::combine_meshes( )
{
    deiimos::msg( ) << "Combining primitive cells... ";

    std::vector< dealii::Point< spacedim > > vertices;
    std::vector< dealii::CellData< dim > >   cells;
    dealii::SubCellData                      subcell_data;

    unsigned int n_accumulated_vertices = 0;
    for ( const auto& triangulation : sub_mesh_ )
    {
        Assert(
          triangulation.n_levels( ) == 1,
          dealii::ExcMessage( "The input triangulations must be non-empty "
                              "and must not be refined." ) );
        if ( triangulation.n_levels( ) != 0 )
        {
            std::vector< dealii::Point< spacedim > > tria_vertices;
            std::vector< dealii::CellData< dim > >   tria_cells;
            dealii::SubCellData                      tria_subcell_data;
            std::tie( tria_vertices, tria_cells, tria_subcell_data ) =
              dealii::GridTools::get_coarse_mesh_description( triangulation );

            vertices.insert(
              vertices.end( ), tria_vertices.begin( ), tria_vertices.end( ) );

            for ( dealii::CellData< dim >& cell_data : tria_cells )
            {
                for ( unsigned int& vertex_n : cell_data.vertices )
                    vertex_n += n_accumulated_vertices;
                cells.push_back( cell_data );
            }

            for ( dealii::CellData< 1 >& line_data :
                  tria_subcell_data.boundary_lines )
            {
                for ( unsigned int& vertex_n : line_data.vertices )
                    vertex_n += n_accumulated_vertices;

                subcell_data.boundary_lines.push_back( line_data );
            }

            for ( dealii::CellData< 2 >& quad_data :
                  tria_subcell_data.boundary_quads )
            {
                for ( unsigned int& vertex_n : quad_data.vertices )
                    vertex_n += n_accumulated_vertices;

                subcell_data.boundary_quads.push_back( quad_data );
            }
            n_accumulated_vertices += triangulation.n_vertices( );
        }
    }
    // throw out duplicated vertices
    std::vector< unsigned int > considered_vertices;
    dealii::GridTools::delete_duplicated_vertices(
      vertices,
      cells,
      subcell_data,
      considered_vertices,
      duplicated_vertex_tolerance_ );

    // reorder the cells to ensure that they satisfy the convention for
    // edge and face directions
    dealii::GridReordering< dim, spacedim >::reorder_cells( cells, true );
    result_mesh_ref_.clear( );
    result_mesh_ref_.create_triangulation( vertices, cells, subcell_data );

    deiimos::msg( ) << "Done." << std::endl;
}

template < int dim, int spacedim >
void MeshReader< dim, spacedim >::clear_meshes( )
{
    for ( auto& triangulation : sub_mesh_ ) { triangulation.clear( ); }
}

} // namespace mesh
} // namespace deiimos

#endif // SRC_MESH_MESHREADER_H
