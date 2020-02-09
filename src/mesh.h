#ifndef SRC_MESH_H
#define SRC_MESH_H

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_in.h>
#include <deal.II/grid/grid_out.h>
#include <deal.II/grid/grid_tools.h>
#include <deal.II/grid/manifold_lib.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/tria_accessor.h>
#include <deal.II/grid/tria_iterator.h>
#include <fstream>
#include <iostream>
#include <map>

namespace dealii
{
template < int dim >
void print_mesh_info( const Triangulation< dim >& triangulation,
                      const std::string&          filename )
{
    std::cout << "Mesh info:" << std::endl
              << " dimension: " << dim << std::endl
              << " no. of cells: " << triangulation.n_active_cells( )
              << std::endl;
    {
        std::map< types::boundary_id, unsigned int > boundary_count;
        for ( auto& cell : triangulation.active_cell_iterators( ) )
        {
            for ( unsigned int face = 0;
                  face < GeometryInfo< dim >::faces_per_cell;
                  ++face )
            {
                if ( cell->face( face )->at_boundary( ) )
                    boundary_count[cell->face( face )->boundary_id( )]++;
            }
        }

        std::cout << " boundary indicators: ";
        for ( const std::pair< const types::boundary_id, unsigned int >& pair :
              boundary_count )
        { std::cout << pair.first << "(" << pair.second << " times) "; }
        std::cout << std::endl;
    }

    // Finally, produce a graphical representation of the mesh to an output
    // file:
    std::ofstream out( filename );
    std::ofstream out_vtk( "aa.vtu" );
    GridOut       grid_out;
    grid_out.write_eps( triangulation, out );
    grid_out.write_vtu< 3 >( triangulation, out_vtk );
    std::cout << " written to " << filename << std::endl << std::endl;
}
} // namespace dealii

#endif // SRC_MESH_H