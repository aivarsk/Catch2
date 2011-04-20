/*
 *  ExceptionTests.cpp
 *  Catch - Test
 *
 *  Created by Phil on 09/11/2010.
 *  Copyright 2010 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include "../catch.hpp"

#include <string>
#include <stdexcept>

namespace
{
    ATTRIBUTE_NORETURN
    int thisThrows();
    
    int thisThrows()
    {
        throw std::domain_error( "expected exception" );
        /*NOTREACHED*/    
    }

    int thisDoesntThrow()
    {
        return 0;
    }
}

TEST_CASE( "./succeeding/exceptions/explicit", "When checked exceptions are thrown they can be expected or unexpected" )
{
    REQUIRE_THROWS_AS( thisThrows(), std::domain_error );
    REQUIRE_NOTHROW( thisDoesntThrow() );
    REQUIRE_THROWS( thisThrows() );
}

TEST_CASE( "./failing/exceptions/explicit", "When checked exceptions are thrown they can be expected or unexpected" )
{
    CHECK_THROWS_AS( thisThrows(), std::string );
    CHECK_THROWS_AS( thisDoesntThrow(), std::domain_error );
    CHECK_NOTHROW( thisThrows() );
}

TEST_CASE_NORETURN( "./failing/exceptions/implicit", "When unchecked exceptions are thrown they are always failures" )
{
    throw std::domain_error( "unexpected exception" );
    /*NOTREACHED*/    
}

TEST_CASE( "./succeeding/exceptions/implicit", "When unchecked exceptions are thrown, but caught, they do not affect the test" )
{
    try
    {
        throw std::domain_error( "unexpected exception" );
    }
    catch(...)
    {
    }
}

class CustomException
{
public:
    CustomException( const std::string& msg )
    : m_msg( msg )
    {}
    
    std::string getMessage() const
    {
        return m_msg;
    }
    
private:
    std::string m_msg;
};

CATCH_TRANSLATE_EXCEPTION( CustomException& ex )
{
    return ex.getMessage();
}

CATCH_TRANSLATE_EXCEPTION( double& ex )
{
    return Catch::toString( ex );
}

TEST_CASE_NORETURN( "./failing/exceptions/custom", "Unexpected custom exceptions can be translated" )
{
    throw CustomException( "custom exception" );
}

TEST_CASE( "./failing/exceptions/custom/nothrow", "Custom exceptions can be translated when testing for nothrow" )
{
    REQUIRE_NOTHROW( throw CustomException( "unexpected custom exception" ) );
}

TEST_CASE( "./failing/exceptions/custom/throw", "Custom exceptions can be translated when testing for throwing as something else" )
{
    REQUIRE_THROWS_AS( throw CustomException( "custom exception - not std" ), std::exception );
}


TEST_CASE_NORETURN( "./failing/exceptions/custom/double", "Unexpected custom exceptions can be translated"  )
{
    throw double( 3.14 );
}
