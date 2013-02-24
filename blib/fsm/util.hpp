//  util.hpp
//
//  Copyright (c) 2003 Eugene Gladyshev
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef __BLIB_fsm_util__hpp
#define __BLIB_fsm_util__hpp


namespace blib
{
namespace fsm
{
	typedef void (*log_function)( const char* context, const char* format, ... );

	// base event
	struct event
	{
		std::string name_;

		event( const std::string& n, const std::string& log_context, log_function lf  ) 
			:name_(n)
		{
			lf( log_context.c_str(), "event: (%s)", name_.c_str() );
		};

		event( const std::string& n )
			:name_(n)
		{
		}
	};

	struct state
	{
		std::string name_;

		template< typename Event >
		state( const std::string& n, const Event& e, const std::string& log_context, log_function lf )
			:name_(n)
		{
			lf( log_context.c_str(), "transition: (%s) --> [%s]",  e.name_.c_str(), name_.c_str() );
		}

		state( const std::string& n, const std::string& log_context, log_function lf )
			:name_(n)
		{
			lf( log_context.c_str(), "transition: --> [%s]",  name_.c_str() );
		}

		template< typename Event >
		state( const std::string& n, const Event& e )
			:name_(n)
		{
		}

		state( const std::string& n )
			:name_(n)
		{
		}
	};

}; //fsm
}; //ttl

#endif //__BLIB_fsm_util__hpp

