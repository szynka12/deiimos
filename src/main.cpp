#include "ManifoldFactory.h"
#include "MeshReader.h"
#include "TopologyInfo.h"
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
#include <type_traits>
#include <typeinfo>
#include <utility>

// template < class T, std::size_t... I >
// using testA = std::tuple< std::array< T, I >... >;

// // using test = testA< int, std::make_index_sequence< 2 >{} >;

// template < size_t... ints >
// auto get_t( std::integer_sequence< size_t, ints... > int_seq )
// {
//     return testA< int, ints... >( );
// }

template < class T >
std::string type_name( )
{
    typedef typename std::remove_reference< T >::type TR;
    std::unique_ptr< char, void ( * )( void* ) >      own(
#ifndef _MSC_VER
      abi::__cxa_demangle( typeid( TR ).name( ), nullptr, nullptr, nullptr ),
#else
      nullptr,
#endif
      std::free );
    std::string r = own != nullptr ? own.get( ) : typeid( TR ).name( );
    if ( std::is_const< TR >::value ) r += " const";
    if ( std::is_volatile< TR >::value ) r += " volatile";
    if ( std::is_lvalue_reference< T >::value )
        r += "&";
    else if ( std::is_rvalue_reference< T >::value )
        r += "&&";
    return r;
}

int main( )
{

    deiimos::set_verbosity( 3 );

    dealii::Triangulation< 3, 3 > output_tria;

    deiimos::io::XmlHandler safe_doc( "../data/configs/simple_mesh.xml" );
    safe_doc.load( );

    deiimos::mesh::MeshReader< 3, 3 > m( output_tria );
    m.read_verts( safe_doc );
    m.read_cells( safe_doc );
    m.read_manifolds( safe_doc );

    m.generate_meshes( );
    m.generate_topology_data( );
    m.combine_meshes( );
    m.clear_meshes( );

    dealii::print_mesh_info( output_tria, "test_1_1" );

    auto t = deiimos::mesh::get_topology_info< 3, 3, 3 >( );

    std::cout << type_name< decltype( t ) >( ) << '\n';

    // static_assert( std::is_same< decltype( b ), testA< int, 1 > >::value, ""
    // );
    return 0;
}