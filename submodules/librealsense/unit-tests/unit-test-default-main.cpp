// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2021 Intel Corporation. All Rights Reserved.

/*
This file is automatically added to any unit-test project generated by unit-test-config.py.

It provides the default main() for our unit-tests so that they can receive any custom options (such
as 'context').

If you don't want this file included, use this cmake directive:
    //#cmake:custom-main
Catch2 will then include its own main() by default.
If you want to create your own main(), define CATCH_CONFIG_RUNNER before including catch.h so Catch2
doesn't. You may also want to use '//#test:flag custom-args'.

See below for each of the custom options...
*/

#include <unit-tests/test.h>

using namespace Catch::Clara;

int main( int argc, char * argv[] )
{
    Catch::Session session;
    bool rslog = false;
    auto cli = session.cli()
             /*
                 --context <context>

                 Define the context for the test. This is usually left blank but can be set if you
                 want to emulate test operation in the 'nightly' context, for example.

                 We write the value directly to 'test::context'.
             */
             | Opt( test::context, "context" )["--context"]( "Context in which to run the tests" )
             /*
                 --debug

                 Enable logging of "debug" messages (using test::log.d())
             */
             | Opt( test::debug )["--debug"]( "Enable debug logging via test::log.d(...)" )
             /*
                 --rslog

                 Enable logging of LibRS log messages (LOG_DEBUG, etc.) to console.
             */
             | Opt( rslog )["--rslog"]( "Enable LibRS logging (LOG_DEBUG, etc.) via rs2::log_to_console()" );
    
    // Use '|' to add additional options:
    //       | Opt( test::flag, "flag" )["-f"]["--flag"]("Switch something on")
    // (see https://github.com/catchorg/Clara)

    session.cli( cli );

    auto ret = session.applyCommandLine( argc, argv );
    if( ret )
        return ret;  // >0 == Error code

#ifdef LIBCI_DEPENDENCY_realsense2
    // rs2::log_to_console() is only available if realsense2 is a dependency of ours
    if( rslog )
        rs2::log_to_console( RS2_LOG_SEVERITY_DEBUG );
#endif

    return session.run();
}