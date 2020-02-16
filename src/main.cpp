#include "MeshReader.h"
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

    deiimos::set_verbosity( 3 );

    dealii::Triangulation< 1, 3 > output_tria;

    deiimos::io::XmlHandler safe_doc( "../data/configs/test_1_1.xml" );
    safe_doc.load( );

    deiimos::mesh::MeshReader< 1, 3 > m( output_tria );
    m.read_verts( safe_doc );
    m.read_cells( safe_doc );

    m.combine_meshes( );

    dealii::print_mesh_info( output_tria, "test_1_1" );

    return 0;
}