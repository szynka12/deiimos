#include "msgstream.h"
namespace deiimos
{

MsgObject::MsgObject( )
  : standard_ostream_( std::cout.rdbuf( ) ),
    null_ostream_( &null_buffer_ ),
    error_ostream_( std::cerr.rdbuf( ) ),
    warning_ostream_( std::cout.rdbuf( ) )
{
}

std::ostream& MsgObject::standard( ) { return standard_ostream_; }

std::ostream& MsgObject::warning( )
{
    if ( warning_enabled_ ) { return warning_ostream_; }
    else
    {
        return null_ostream_;
    }
}

std::ostream& MsgObject::error( ) { return error_ostream_; }

std::ostream& MsgObject::null( ) { return null_ostream_; }

std::ostream& MsgObject::msg( v_level l )
{
    if ( l <= out_level_ && out_level_ != SILENT ) { return standard_ostream_; }
    else
    {
        return null_ostream_;
    }
}

void MsgObject::set_verbosity( v_level l ) { out_level_ = l; }
void MsgObject::set_warning( bool flag ) { warning_enabled_ = flag; }

namespace INTERNAL
{

MsgObject msg_;

}

} // namespace deiimos