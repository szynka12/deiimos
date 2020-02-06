#pragma once

#include <iostream>
#include <memory>
#include <utility>

namespace deiimos
{

class msgstream
{
  public:
    // verbosity
    enum v_level
    {
        SILENT = 0,
        INFO1  = 1,
        INFO2  = 2,
        INFO3  = 3
    };

  private:
    // buffers
    class NullBuffer : public std::streambuf
    {
      public:
        int overflow( int c ) { return c; }
    };
    NullBuffer null_buffer_;

    // streams
    std::ostream standard_ostream_;
    std::ostream null_ostream_;
    std::ostream error_ostream_;
    std::ostream warning_ostream_;

    // global verbosity
    v_level out_level_       = INFO1;
    bool    warning_enabled_ = true;

  public:
    msgstream( )
      : standard_ostream_( std::cout.rdbuf( ) ),
        null_ostream_( &null_buffer_ ),
        error_ostream_( std::cerr.rdbuf( ) ),
        warning_ostream_( std::cout.rdbuf( ) ){};

    std::ostream& standard( ) { return standard_ostream_; }

    std::ostream& warning( )
    {
        if ( warning_enabled_ ) { return warning_ostream_; }
        else
        {
            return null_ostream_;
        }
    }

    std::ostream& null( ) { return null_ostream_; }

    std::ostream& error( ) { return error_ostream_; }

    std::ostream& msg( v_level l )
    {
        if ( l <= out_level_ && out_level_ != SILENT )
        { return standard_ostream_; }
        else
        {
            return null_ostream_;
        }
    }

    void set_verobisity( v_level l ) { out_level_ = l; };
    void set_warning( bool flag ) { warning_enabled_ = flag; }
};

namespace INTERNAL
{

msgstream msg_;

}

inline void verbosity( msgstream::v_level l )
{
    INTERNAL::msg_.set_verobisity( l );
}
inline void verbosity( int l )
{
    INTERNAL::msg_.set_verobisity(
      static_cast< deiimos::msgstream::v_level >( l ) );
}

inline void set_warning( bool flag ) { INTERNAL::msg_.set_warning( flag ); }

inline std::ostream& msg( msgstream::v_level l = msgstream::INFO1 )
{
    return INTERNAL::msg_.msg( l );
}
inline std::ostream& msg( int l )
{
    return msg( static_cast< deiimos::msgstream::v_level >( l ) );
}

inline std::ostream& err( )
{
    return INTERNAL::msg_.error( ) << "*** Fatal error! ***\n";
}

inline std::ostream& warn( )
{
    return INTERNAL::msg_.warning( ) << "--- Warning ---\n";
}

} // namespace deiimos
