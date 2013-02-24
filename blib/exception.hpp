//  exception.hpp
//
//  Copyright (c) 2003 Eugene Gladyshev
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef __BLIB_EXCEPTION_INCLUDED__
#define __BLIB_EXCEPTION_INCLUDED__

#ifndef __BLIB_CONFIG_INCLUDED__
#include <blib/config.hpp>
#endif

namespace blib
{
	struct exception : std::runtime_error
	{
		exception() : std::runtime_error("ttl error") {}
		exception( char* msg ) :  std::runtime_error(msg) {}
	};
};

#endif //__BLIB_EXCEPTION_INCLUDED__
