#include "mesh.h"
#include "msgstream.h"

#include <vector>

using namespace dealii;

int main( void )
{
    using vert = Point< 3 >;

    Triangulation< 3 > tria1;
    Triangulation< 3 > tria2;
    Triangulation< 3 > tria;

    const Point< 3 >             center( {0.5, 0.5, 0.5} );
    const SphericalManifold< 3 > manifold( center );

    // GridGenerator::hyper_cube< 3 >( tria, 0.0, 1.0, true );

    vert a0 = {0, 0, 0};
    vert a1 = {1.5, 0, 0};
    vert a2 = {0, 1, 0};
    vert a3 = {1, 1, 0};
    vert a4 = {0, 0, 1};
    vert a5 = {1, 0, 1};
    vert a6 = {0, 1, 1};
    vert a7 = {1, 1, 1};

    vert b0 = a1;
    vert b1 = {2, 0, 0};
    vert b2 = a3;
    vert b3 = {2, 1, 0};
    vert b4 = a5;
    vert b5 = {2, 0, 1};
    vert b6 = a7;
    vert b7 = {2, 1, 1};

    GridGenerator::general_cell(
      tria1, {a0, a1, a2, a3, a4, a5, a6, a7}, true );

    GridGenerator::general_cell(
      tria2, {b0, b1, b2, b3, b4, b5, b6, b7}, true );

    for ( auto& cell : tria1.active_cell_iterators( ) )
    {
        for ( size_t line_id = 0; line_id < 12; line_id++ )
        {
            // for ( size_t vertex_id = 0; vertex_id < 2; vertex_id++ )
            // {
            //     std::cout << cell->line( line_id )->vertex( vertex_id )
            //               << std::endl;
            // }
            if ( cell->line( line_id )->vertex( 0 ) == Point< 3 >( 0, 0, 0 ) &&
                 cell->line( line_id )->vertex( 1 ) == Point< 3 >( 0, 0, 1 ) )
            {
                std::cout << "This is the line" << std::endl;
                cell->line( line_id )->set_manifold_id( 1 );
            }
        }
    }

    // tria.set_all_manifold_ids_on_boundary( 0, 0 );
    // tria1.set_manifold( 1, manifold );

    GridGenerator::merge_triangulations( {&tria1, &tria2}, tria );

    // for ( auto& cell : tria.active_cell_iterators( ) )
    // {
    //     for ( size_t line_id = 0; line_id < 12; line_id++ )
    //     {
    //         // for ( size_t vertex_id = 0; vertex_id < 2; vertex_id++ )
    //         // {
    //         //     std::cout << cell->line( line_id )->vertex( vertex_id )
    //         //               << std::endl;
    //         // }
    //         if ( cell->line( line_id )->vertex( 0 ) == Point< 3 >( 0, 0, 0 )
    //         &&
    //              cell->line( line_id )->vertex( 1 ) == Point< 3 >( 0, 0, 1 )
    //              )
    //         {
    //             std::cout << "This is the line" << std::endl;
    //             cell->line( line_id )->set_manifold_id( 1 );
    //         }
    //     }
    // }

    tria.set_manifold( 1, manifold );

    tria.refine_global( 3 );

    print_mesh_info( tria, "grid-2.eps" );

    return ( 0 );
}