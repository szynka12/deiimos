#ifndef SRC_MERGE_H
#define SRC_MERGE_H

#include <cmath>
#include <deal.II/distributed/shared_tria.h>
#include <deal.II/distributed/tria.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_reordering.h>
#include <deal.II/grid/grid_tools.h>
#include <deal.II/grid/intergrid_map.h>
#include <deal.II/grid/manifold_lib.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/tria_accessor.h>
#include <deal.II/grid/tria_iterator.h>
#include <limits>

namespace dealii
{

template < int dim, int spacedim >
void merge_triangulations_mine(
  const std::initializer_list< const Triangulation< dim, spacedim >* const >&
                                  triangulations,
  Triangulation< dim, spacedim >& result,
  const double                    duplicated_vertex_tolerance )
{
    std::vector< Point< spacedim > > vertices;
    std::vector< CellData< dim > >   cells;
    SubCellData                      subcell_data;

    unsigned int n_accumulated_vertices = 0;
    for ( const auto triangulation : triangulations )
    {
        Assert( triangulation->n_levels( ) == 1,
                ExcMessage( "The input triangulations must be non-empty "
                            "and must not be refined." ) );

        std::vector< Point< spacedim > > tria_vertices;
        std::vector< CellData< dim > >   tria_cells;
        SubCellData                      tria_subcell_data;
        std::tie( tria_vertices, tria_cells, tria_subcell_data ) =
          GridTools::get_coarse_mesh_description( *triangulation );

        vertices.insert(
          vertices.end( ), tria_vertices.begin( ), tria_vertices.end( ) );
        for ( CellData< dim >& cell_data : tria_cells )
        {
            for ( unsigned int& vertex_n : cell_data.vertices )
                vertex_n += n_accumulated_vertices;
            cells.push_back( cell_data );
        }

        std::cout << "Copying manifolds" << std::endl;
        for ( CellData< 1 >& line_data : tria_subcell_data.boundary_lines )
        {
            for ( unsigned int& vertex_n : line_data.vertices )
                vertex_n += n_accumulated_vertices;

            subcell_data.boundary_lines.push_back( line_data );
        }

        for ( CellData< 2 >& quad_data : tria_subcell_data.boundary_quads )
        {
            std::cout << quad_data.boundary_id << std::endl;

            for ( unsigned int& vertex_n : quad_data.vertices )
                vertex_n += n_accumulated_vertices;

            subcell_data.boundary_quads.push_back( quad_data );
        }

        n_accumulated_vertices += triangulation->n_vertices( );
    }

    // throw out duplicated vertices
    std::vector< unsigned int > considered_vertices;
    GridTools::delete_duplicated_vertices( vertices,
                                           cells,
                                           subcell_data,
                                           considered_vertices,
                                           duplicated_vertex_tolerance );

    // reorder the cells to ensure that they satisfy the convention for
    // edge and face directions
    GridReordering< dim, spacedim >::reorder_cells( cells, true );
    result.clear( );
    result.create_triangulation( vertices, cells, subcell_data );
}

template < int dim, int spacedim >
void merge_triangulations_mine(
  const Triangulation< dim, spacedim >& triangulation_1,
  const Triangulation< dim, spacedim >& triangulation_2,
  Triangulation< dim, spacedim >&       result,
  const double                          duplicated_vertex_tolerance )
{
    // if either Triangulation is empty then merging is just a copy.
    if ( triangulation_1.n_cells( ) == 0 )
    {
        result.copy_triangulation( triangulation_2 );
        return;
    }
    if ( triangulation_2.n_cells( ) == 0 )
    {
        result.copy_triangulation( triangulation_1 );
        return;
    }
    merge_triangulations_mine( {&triangulation_1, &triangulation_2},
                               result,
                               duplicated_vertex_tolerance );
}

} // namespace dealii
#endif