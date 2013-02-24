//  typelist.hpp
//
//  Copyright (c) 2003 Eugene Gladyshev
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef __BLIB_typelist__hpp
#define __BLIB_typelist__hpp

#include "blib/config.hpp"
#include "blib/data_holder.hpp"
#include "blib/exception.hpp"
#include "blib/macro_params.hpp"
#include "blib/equivalent_types.hpp"
#include "blib/meta/is_same.hpp"
#include "blib/selector.hpp"


namespace blib
{
namespace meta
{	
struct exception : blib::exception
{
	exception() : blib::exception("typelist error") {}
};
//internal implementation
namespace impl
{
	////////////////////////////////////////////////////////////
	template< BLIB_TPARAMS_DEF(BLIB_MAX_TYPELIST_PARAMS, empty_type) >
	struct typelist_traits
	{
		typedef typelist_traits< BLIB_ARGS_S(BLIB_DEC(BLIB_MAX_TYPELIST_PARAMS)) > tail;

		enum
		{
			length = 1 + tail::length,
		};

		typedef typename selector< sizeof(blib::data_holder<T1>) >=sizeof(blib::data_holder<typename tail::largest_type>),
			T1,
			typename tail::largest_type
			>::type largest_type;
	};

	template<>
	struct typelist_traits< BLIB_LIST_ITEMS(BLIB_MAX_TYPELIST_PARAMS,empty_type) >
	{
		typedef empty_type tail;
		enum
		{
			length = 0,
		};
		typedef empty_type largest_type;
	};
	
	////////////////////////////////////////////////////////////
	//
	//Instantiate BLIB_MAX_TYPELIST_PARAMS get<> templates
	//	template<typename T>  struct get<T, 0>
	//	{ 
	//		enum { index = 0 };			
	//		typedef typename T::type1 type; 
	//	};
	//	
	//	template<typename T>  struct get<T, 1>
	//	{ 
	//		enum { index = 1 };			
	//		typedef typename T::type2 type; 
	//	};
	//	...
	//
	
	template< typename T, int N > struct get;
	#define BLIB_META_TYPELIST_GET(n, t) template<typename T>  struct get<T, BLIB_CNTDEC_##n>  \
	{ enum {index = n-1}; typedef typename T::t##n type; };
	
	BLIB_REPEAT( BLIB_MAX_TYPELIST_PARAMS, BLIB_META_TYPELIST_GET, BLIB_META_TYPELIST_GET, type )
	
	#undef BLIB_META_TYPELIST_GET
};

	template < BLIB_TPARAMS_DEF(BLIB_MAX_TYPELIST_PARAMS, empty_type) >
	struct typelist
	{
		BLIB_TYPEDEFS(BLIB_MAX_TYPELIST_PARAMS)
		
		typedef impl::typelist_traits< BLIB_ARGS(BLIB_MAX_TYPELIST_PARAMS) > list_traits;
	
		enum{ length = list_traits::length };
		typedef typename list_traits::largest_type largest_type;
	};
	
	////////////////////////////////////////////////////////////
	template < typename L >
	struct length
	{
		enum { value = L::length };
	};
	
	///////////////////////////////////////////////////////////
	template< typename L, int N, bool Ok = (N < length<L>::value) >
	struct get
	{
		typedef typename impl::get<L,N>::type type;
		enum{ index = N };
	};
	
	template< typename L, int N >
	struct get<L,N,false>
	{
		//index is out of range
	};

	////////////////////////////////////////////////////////////
	//	run-time type switch
	template <typename L, int N = 0, bool Stop=(N==length<L>::value) > struct type_switch;

	template <typename L, int N, bool Stop> 
	struct type_switch
	{
		template< typename F >
		void operator()( size_t i, F& f )
		{
			if( i == N ) 
			{
				f.operator()<typename impl::get<L,N>::type>();
			}
			else
			{
				type_switch<L, N+1> next;
				next(i, f);
			}
		}
	};

	template <typename L, int N> 
	struct type_switch<L, N, true>
	{
		template< typename F >
		void operator()( 
			size_t //i
			,F& //f 
		)
		{
			throw meta::exception();
		}
	};


	//////////////////////////////////////////////////////////////
	template< typename T, typename L, int N = 0, bool Stop=(N>=length<L>::value) >
	struct find_equivalent_type
	{
	private:
		typedef impl::get<L,N> get_type;
		
		typedef typename selector< equivalent_types<typename get_type::type, T>::value,
			get_type,
			find_equivalent_type<T,L,N+1> 
			>::type found;
			
	public:
		typedef typename found::type type;
		enum {index = found::index};
	};

	template<typename T, typename L, int N>
	struct find_equivalent_type<T, L, N, true>
	{
	};


	//////////////////////////////////////////////
	template< typename T, typename L, int N = 0, bool Stop=(N>=length<L>::value) >
	struct find
	{
	private:
		typedef impl::get<L,N> get_type;
		
		typedef typename selector< is_same<typename get_type::type, T>::value,
			get_type,
			find<T,L,N+1> 
			>::type found;
			
	public:
		typedef typename found::type type;
		enum {index = found::index};
	};

	template<typename T, typename L, int N>
	struct find<T, L, N, true>
	{
	};

};
};

#endif //__typelist__hpp
