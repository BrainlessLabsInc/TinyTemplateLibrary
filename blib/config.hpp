//  config.hpp
//
//  Copyright (c) 2003 Eugene Gladyshev
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
// Modified by: Shakti Misra (2013-)
#ifndef __BLIB_CONFIG_INCLUDED__
#define __BLIB_CONFIG_INCLUDED__

#ifndef __BLIB_COMPILER_CONFIG_INCLUDED__
#include <blib/config/compiler_config.hpp>
#endif
#ifndef BOOST_CONFIG_HPP
#include <boost/config.hpp>
#endif
#ifndef __STD_NEW_INCLUDED__
#define __STD_NEW_INCLUDED__
#include <new>
#endif
#ifndef __STD_STDEXCEPT_INCLUDED__
#define __STD_STDEXCEPT_INCLUDED__
#include <stdexcept>
#endif
#ifndef __STD_FUNCTIONAL_INCLUDED__
#define __STD_FUNCTIONAL_INCLUDED__
#include <functional>
#endif
#ifndef __STD_MEMORY_INCLUDED__
#define __STD_MEMORY_INCLUDED__
#include <memory>
#endif
#ifndef __STD_STRING_INCLUDED__
#define __STD_STRING_INCLUDED__
#include <string>
#endif

#ifndef __STD_ASSERT_INCLUDED__
#define __STD_ASSERT_INCLUDED__
#include <assert.h>
#endif

#if defined(BLIB_MSVC)
#	define BLIB_MAX_TEMPLATE_PARAMS 25
#elif defined(BLIB_GCC)
#	define BLIB_MAX_TEMPLATE_PARAMS 25
#else
#	define BLIB_MAX_TEMPLATE_PARAMS 25
#endif

#define BLIB_MAX_TYPELIST_PARAMS BLIB_MAX_TEMPLATE_PARAMS
#define BLIB_MAX_TUPLE_PARAMS 15
#define BLIB_MAX_NAMED_PARAMS BLIB_MAX_TUPLE_PARAMS

namespace blib
{
#if defined(UNICODE)||defined(_UNICODE)
	typedef std::wstring string;
#define SC( txt ) L##txt
#else
	typedef std::string string;
	#define SC( txt ) txt
#endif

	typedef string::value_type tchar;


	struct empty_type {};


	struct noncopyable
	{
		noncopyable() {}
	private:
		noncopyable( const noncopyable& );
		noncopyable& operator=( const noncopyable& );
	};
};

#ifdef BOOST_NO_CXX11_NULLPTR
#define nullptr NULL
#endif
#ifndef BLIB_UNUSED
#define BLIB_UNUSED(var) ((void)var)
#endif

#endif// __BLIB_CONFIG_INCLUDED__
