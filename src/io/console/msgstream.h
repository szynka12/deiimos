#ifndef SRC_IO_CONSOLE_MSGSTREAM_H
#define SRC_IO_CONSOLE_MSGSTREAM_H

#include <iostream>
#include <memory>
#include <utility>

namespace deiimos
{

class MsgObject
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
    MsgObject( );

    std::ostream& standard( );

    std::ostream& warning( );

    std::ostream& null( );

    std::ostream& error( );

    std::ostream& msg( v_level l );

    void set_verobisity( v_level l );
    void set_warning( bool flag );
};

namespace INTERNAL
{

extern MsgObject msg_;

}

inline void verbosity( MsgObject::v_level l )
{
    INTERNAL::msg_.set_verobisity( l );
}
inline void verbosity( int l )
{
    INTERNAL::msg_.set_verobisity(
      static_cast< deiimos::MsgObject::v_level >( l ) );
}

inline void set_warning( bool flag ) { INTERNAL::msg_.set_warning( flag ); }

inline std::ostream& msg( MsgObject::v_level l = MsgObject::INFO1 )
{
    return INTERNAL::msg_.msg( l );
}
inline std::ostream& msg( int l )
{
    return msg( static_cast< deiimos::MsgObject::v_level >( l ) );
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
#endif // SRC_IO_CONSOLE_MSGSTREAM_H