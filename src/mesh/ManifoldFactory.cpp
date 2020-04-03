template < int dim, int spacedim >
ManifoldFactory< dim, spacedim >::ManifoldPtr
  ManifoldFactory< dim, spacedim >::create( const char* type,
                                            io::XmlNode manifold_node )
{
    switch ( type )
    {
        case "spherical": break;
        default: break;
    }
}