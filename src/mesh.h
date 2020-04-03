#ifndef SRC_MESH_H
#define SRC_MESH_H

#include "XmlHandler.h"

#include <array>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_in.h>
#include <deal.II/grid/grid_out.h>
#include <deal.II/grid/grid_tools.h>
#include <deal.II/grid/manifold_lib.h>
#include <deal.II/grid/tria.h>
#include <deal.II/grid/tria_accessor.h>
#include <deal.II/grid/tria_iterator.h>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
namespace dealii
{
template < int dim, int spacedim = dim >
void print_mesh_info( const Triangulation< dim, spacedim >& triangulation,
                      const std::string&                    filename )
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
    std::ofstream out_eps( filename + ".eps" );
    std::ofstream out_vtk( filename + ".vtu" );
    GridOut       grid_out;
    // grid_out.write_eps( triangulation, out_eps );
    grid_out.write_vtu< dim, spacedim >( triangulation, out_vtk );
    std::cout << " written to " << filename << std::endl << std::endl;
}

} // namespace dealii

namespace deiimos
{
namespace mesh
{

std::vector< std::array< size_t, 8 > >
  get_hexes( deiimos::io::XmlHandler& config )
{
    auto h_xml = config["deiimos_config"]["hexes"].get_siblings( "h" );

    std::vector< std::array< size_t, 8 > > hexes( h_xml.size( ) );

    size_t i = 0;
    for ( auto x : h_xml )
    {
        std::tie( hexes[i][0],
                  hexes[i][1],
                  hexes[i][2],
                  hexes[i][3],
                  hexes[i][4],
                  hexes[i][5],
                  hexes[i][6],
                  hexes[i][7] ) = x.tuple_value< size_t,
                                                 size_t,
                                                 size_t,
                                                 size_t,
                                                 size_t,
                                                 size_t,
                                                 size_t,
                                                 size_t >( );
        i++;
    }
    return hexes;
}

template < int dim >
std::vector< dealii::Point< dim > >
  to_vertices( std::array< size_t, 8 >             hex,
               std::vector< dealii::Point< dim > > vertices )
{
    return {vertices[hex[0]],
            vertices[hex[1]],
            vertices[hex[2]],
            vertices[hex[3]],
            vertices[hex[4]],
            vertices[hex[5]],
            vertices[hex[6]],
            vertices[hex[7]]};
}

template < int dim >
std::vector< dealii::Point< dim > >
  get_vertices( deiimos::io::XmlHandler& config );

template <>
std::vector< dealii::Point< 3 > >
  get_vertices( deiimos::io::XmlHandler& config )
{
    auto v_xml = config["deiimos_config"]["vertices"].get_siblings( "v" );

    std::vector< dealii::Point< 3 > > vertices( v_xml.size( ) );

    size_t i = 0;
    for ( auto x : v_xml )
    {
        std::tie( vertices[i][0], vertices[i][1], vertices[i][2] ) =
          x.tuple_value< double, double, double >( );
        i++;
    }
    return vertices;
}

} // namespace mesh
} // namespace deiimos

#endif // SRC_MESH_H