#include "XmlHandler.h"
#include "XmlNode.h"
#include "merge.h"
#include "mesh.h"
#include "msgstream.h"
#include "tinyxml2.h"

#include <array>
#include <chrono>
#include <iostream>
#include <tuple>
#include <utility>

int main( )
{
    deiimos::io::XmlHandler safe_doc( "../data/configs/simple_mesh.xml" );
    safe_doc.load( );

    auto vertices = deiimos::mesh::get_vertices< 3 >( safe_doc );
    auto hexes    = deiimos::mesh::get_hexes( safe_doc );

    size_t n_tria = hexes.size( );

    std::vector< dealii::Triangulation< 3 > > triangulations( n_tria );

    for ( size_t i = 0; i < n_tria; i++ )
    {
        dealii::GridGenerator::general_cell(
          triangulations[i],
          deiimos::mesh::to_vertices< 3 >( hexes[i], vertices ),
          true );
        dealii::print_mesh_info( triangulations[i], "test.vtu" );
    }

    dealii::Triangulation< 3 > output_tria;
    for ( size_t i = 0; i < n_tria; i++ )
    {
        dealii::merge_triangulations_mine(
          output_tria, triangulations[i], output_tria, 1e-12 );
    }

    for ( auto& cell : output_tria.active_cell_iterators( ) )
    {
        for ( size_t i = 0; i < 6; i++ )
        {
            if ( cell->face( i )->at_boundary( ) )
            {
                std::cout << cell->face( i )->vertex( 0 ) << "; ";
                std::cout << cell->face( i )->vertex( 1 ) << "; ";
                std::cout << cell->face( i )->vertex( 2 ) << "; ";
                std::cout << cell->face( i )->vertex( 3 ) << std::endl;
            }
        }
    }

    dealii::print_mesh_info( output_tria, "test.vtu" );

    return 0;
}