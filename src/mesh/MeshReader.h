#ifndef SRC_MESH_MESHREADER_H
#define SRC_MESH_MESHREADER_H

#include "XmlHandler.h"
#include "deal.II/base/exceptions.h"

#include <array>
#include <deal.II/base/utilities.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_reordering.h>
#include <deal.II/grid/grid_tools.h>
#include <deal.II/grid/tria.h>
#include <string>
#include <vector>

namespace deiimos
{
namespace mesh
{

template < int dim, int spacedim = dim >
class MeshReader
{
  private:
    constexpr static int coords_number_   = spacedim;
    constexpr static int points_per_cell_ = dealii::Utilities::pow( 2, dim );

  public:
    using Vert        = dealii::Point< coords_number_, double >;
    using CellIndices = std::array< size_t, points_per_cell_ >;
    using MeshType    = dealii::Triangulation< dim, spacedim >;

  private:
    std::vector< Vert >        verts_;
    std::vector< CellIndices > cells_;
    std::vector< MeshType >    sub_mesh_;
    MeshType&                  result_;

    void generate_meshes( );

  public:
    MeshReader( MeshType& result ) : result_( result ){};
    ~MeshReader( ){};

    void read_verts( deiimos::io::XmlHandler& config );
    void read_cells( deiimos::io::XmlHandler& config );

    std::vector< Vert > cell_indices_to_coords( size_t cell_id );

    // same as dealii::merge_triangulations but always copies bc ids and
    // manifolds
    void
      attach_mesh( const std::initializer_list< const MeshType* const >& source,
                   MeshType&                                             target,
                   const double duplicated_vertex_tolerance = 1e-12 );

    void combine_meshes( );
};

template < int dim, int spacedim >
void MeshReader< dim, spacedim >::read_verts( deiimos::io::XmlHandler& config )
{
    deiimos::msg( ) << "Reading vertices... ";

    auto v_config = config["deiimos_config"]["vertices"].get_siblings( "v" );
    verts_        = std::vector< Vert >( v_config.size( ) );

    for ( size_t i = 0; i < v_config.size( ); i++ )
    {
        auto tmp = v_config[i].array_value< double, coords_number_ >( );
        for ( size_t j = 0; j < coords_number_; j++ ) { verts_[i][j] = tmp[j]; }
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
    { cells_[i] = c_config[i].array_value< size_t, points_per_cell_ >( ); }

    deiimos::msg( ) << "Done." << std::endl;
    deiimos::msg( 2 ) << "  # of cells: " << cells_.size( ) << std::endl;
}

template < int dim, int spacedim >
std::vector< typename MeshReader< dim, spacedim >::Vert >
  MeshReader< dim, spacedim >::cell_indices_to_coords( size_t cell_id )
{
    std::vector< Vert > out( points_per_cell_ );
    for ( size_t i = 0; i < points_per_cell_; i++ )
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
void MeshReader< dim, spacedim >::attach_mesh(
  const std::initializer_list< const MeshType* const >& source,
  MeshType&                                             result,
  const double duplicated_vertex_tolerance )
{
    std::vector< dealii::Point< spacedim > > vertices;
    std::vector< dealii::CellData< dim > >   cells;
    dealii::SubCellData                      subcell_data;

    unsigned int n_accumulated_vertices = 0;
    for ( const auto triangulation : source )
    {
        // Assert(
        //   triangulation->n_levels( ) == 1,
        //   dealii::ExcMessage( "The input triangulations must be non-empty "
        //                       "and must not be refined." ) );
        if ( triangulation->n_levels( ) != 0 )
        {
            std::vector< dealii::Point< spacedim > > tria_vertices;
            std::vector< dealii::CellData< dim > >   tria_cells;
            dealii::SubCellData                      tria_subcell_data;
            std::tie( tria_vertices, tria_cells, tria_subcell_data ) =
              dealii::GridTools::get_coarse_mesh_description( *triangulation );

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
            n_accumulated_vertices += triangulation->n_vertices( );
        }
    }
    // throw out duplicated vertices
    std::vector< unsigned int > considered_vertices;
    dealii::GridTools::delete_duplicated_vertices(
      vertices,
      cells,
      subcell_data,
      considered_vertices,
      duplicated_vertex_tolerance );

    // reorder the cells to ensure that they satisfy the convention for
    // edge and face directions
    dealii::GridReordering< dim, spacedim >::reorder_cells( cells, true );
    result.clear( );
    result.create_triangulation( vertices, cells, subcell_data );
}

template < int dim, int spacedim >
void MeshReader< dim, spacedim >::combine_meshes( )
{
    generate_meshes( );

    deiimos::msg( ) << "Combining meshes... ";
    for ( size_t i = 0; i < sub_mesh_.size( ); i++ )
    { attach_mesh( {&sub_mesh_[i], &result_}, result_ ); }
    deiimos::msg( ) << "Done." << std::endl;
}

} // namespace mesh
} // namespace deiimos

#endif // SRC_MESH_MESHREADER_H
