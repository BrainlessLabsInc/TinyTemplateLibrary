//  function.hpp
//
//  Copyright (c) 2003 Eugene Gladyshev
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef __BLIB_func_function__hpp
#define __BLIB_func_function__hpp

#include "blib/config.hpp"
#include "blib/exception.hpp"
#include "blib/macro_repeat.hpp"
#include "blib/macro_params.hpp"
#include "blib/mem/memory.hpp"

//!!! don't change it w/o updating the definitions bellow
#define BLIB_MAX_FUNCTION_PARAMS 15

namespace blib
{
namespace func
{
struct exception : blib::exception
{
	exception() : blib::exception("blib::func::function error") {}
};
namespace impl
{
	template< typename R >
	struct functor_caller_base0
	{
		typedef functor_caller_base0 this_t;
		typedef R return_type;
		virtual ~functor_caller_base0() {}
		virtual return_type operator()() = 0;
		virtual this_t* clone() const = 0;
	};
	template< typename F, typename R >
	struct functor_caller0 : functor_caller_base0<R>
	{
		typedef functor_caller0 this_t;
		typedef functor_caller_base0<R> base_t;
		typedef R return_type;
		typedef typename blib::mem::memtraits::allocator<this_t> allocator;
		F f_;
		functor_caller0( F f ) : f_(f) {}
		virtual return_type operator()() { return f_(); }
		virtual base_t* clone() const { return blib::mem::create<allocator>(f_); } 
	};
	
#define BLIB_FUNC_BUILD_FUNCTOR_CALLER(n)	 \
	template< typename R, BLIB_TPARAMS(n) > \
	struct functor_caller_base##n \
	{ \
		typedef functor_caller_base##n this_t; \
		typedef R return_type; \
		virtual ~functor_caller_base##n() {} \
		virtual return_type operator()( BLIB_FUNC_PARAMS(n,p) ) = 0; \
		virtual this_t* clone() const = 0;  \
	}; \
	template< typename F=empty_type, typename R=empty_type, BLIB_TPARAMS_DEF(n,empty_type) > struct functor_caller##n;  \
	template< typename F, typename R, BLIB_TPARAMS(n) > \
	struct functor_caller##n< F, R (BLIB_ARGS(n)) > : functor_caller_base##n< R, BLIB_ARGS(n) > \
	{ \
		typedef functor_caller##n this_t; \
		typedef functor_caller_base##n< R, BLIB_ARGS(n) > base_t; \
		typedef typename blib::mem::memtraits::allocator<this_t> allocator;  \
		typedef R return_type; \
		F f_; \
		functor_caller##n( F f ) : f_(f) {} \
		virtual return_type operator()( BLIB_FUNC_PARAMS(n,p) ) { return f_( BLIB_ENUM_ITEMS(n,p) ); } \
		virtual base_t* clone() const { return blib::mem::create<allocator>(f_); }  \
	}; 
	
	//the parameter defines the arity
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(1)
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(2)
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(3)
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(4)
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(5)
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(6)
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(7)
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(8)
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(9)
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(10)
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(11)
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(12)
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(13)
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(14)
	BLIB_FUNC_BUILD_FUNCTOR_CALLER(15)
	
	#undef BLIB_FUNC_BUILD_FUNCTOR_CALLER
};

	template< typename R = empty_type, BLIB_TPARAMS_DEF(BLIB_MAX_FUNCTION_PARAMS, empty_type) > struct function;
	
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function
	{
		typedef function this_t;
		typedef impl::functor_caller_base0<R> caller;
		typedef R return_type;
		typedef R result_type; 
		typedef typename blib::mem::memtraits::allocator<caller> allocator;
		enum { arity = 0 };
		
		function() : fc_(0) {}
		virtual ~function() { destroy(); }
		
		template< typename F >
		function( F f ) : fc_(0)
		{
			typedef impl::functor_caller0<F,R> caller_spec;
			typedef typename blib::mem::memtraits::allocator<caller_spec> allocator;
			fc_ = blib::mem::create<allocator>(f);
		}
		function( const this_t& r ) : fc_(0)
		{
			operator=(r);
		}
		
		this_t& operator=( const this_t& r )
		{
			if( this == &r ) return *this;
			destroy();
			fc_ = r.fc_->clone();
			return *this;
		}
		
		return_type operator()() 
		{ 
			if( !fc_ ) throw func::exception();
			return (*fc_)(); 
		}
		
		inline bool is_valid() const { return fc_ != 0; }
	
	protected:
		caller *fc_;
		void destroy()
		{
			if(!is_valid()) return;
			blib::mem::destroy<allocator>(fc_);
			fc_ = 0;
		}
	};

#define BLIB_FUNC_BUILD_FUNCTION(n) \
		typedef function this_t; \
		typedef impl::functor_caller_base##n< R, BLIB_ARGS(n) > caller; \
		typedef typename blib::mem::memtraits::allocator<caller> allocator; \
		typedef R return_type; \
		typedef R result_type; \
		typedef T1 argument_type; \
		typedef T1 first_argument_type; \
		typedef T2 second_argument_type; \
		enum { arity = n };  \
		function() : fc_(0) {} \
		virtual ~function() { destroy(); } \
		template< typename F > \
		function( F f ) : fc_(0) \
		{ \
			typedef impl::functor_caller##n< F, R (BLIB_ARGS(n)) > caller_spec; \
			typedef typename blib::mem::memtraits::allocator<caller_spec> allocator; \
			fc_ = blib::mem::create<allocator>(f); \
		} \
		function( const this_t& r ) : fc_(0) \
		{ \
			operator=(r); \
		} \
		this_t& operator=( const this_t& r ) \
		{ \
			if( this == &r ) return *this; \
			destroy(); \
			fc_ = r.fc_->clone(); \
			return *this; \
		} \
		return_type operator()(BLIB_FUNC_PARAMS(n,p)) \
		{ \
			if( !fc_ ) throw func::exception(); \
			return (*fc_)( BLIB_ENUM_ITEMS(n,p) );  \
		} \
		inline bool is_valid() const { return fc_ != 0; } \
	protected: \
		caller *fc_; \
		void destroy() \
		{ \
			if(!is_valid()) return; \
			blib::mem::destroy<allocator>(fc_);  \
			fc_ = 0; \
		}

	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(1)), T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15 >
	{
		BLIB_FUNC_BUILD_FUNCTION(1)
	};
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(2)), T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15 >
	{
		BLIB_FUNC_BUILD_FUNCTION(2)
	};
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(3)), T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15 >
	{
		BLIB_FUNC_BUILD_FUNCTION(3)
	};
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(4)), T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15 >
	{
		BLIB_FUNC_BUILD_FUNCTION(4)
	};
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(5)), T6, T7, T8, T9, T10, T11, T12, T13, T14, T15 >
	{
		BLIB_FUNC_BUILD_FUNCTION(5)
	};
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(6)), T7, T8, T9, T10, T11, T12, T13, T14, T15 >
	{
		BLIB_FUNC_BUILD_FUNCTION(6)
	};
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(7)), T8, T9, T10, T11, T12, T13, T14, T15 >
	{
		BLIB_FUNC_BUILD_FUNCTION(7)
	};
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(8)), T9, T10, T11, T12, T13, T14, T15 >
	{
		BLIB_FUNC_BUILD_FUNCTION(8)
	};
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(9)), T10, T11, T12, T13, T14, T15 >
	{
		BLIB_FUNC_BUILD_FUNCTION(9)
	};
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(10)), T11, T12, T13, T14, T15 >
	{
		BLIB_FUNC_BUILD_FUNCTION(10)
	};
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(11)), T12, T13, T14, T15 >
	{
		BLIB_FUNC_BUILD_FUNCTION(11)
	};
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(12)), T13, T14, T15 >
	{
		BLIB_FUNC_BUILD_FUNCTION(12)
	};
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(13)), T14, T15 >
	{
		BLIB_FUNC_BUILD_FUNCTION(13)
	};
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(14)), T15 >
	{
		BLIB_FUNC_BUILD_FUNCTION(14)
	};
	template<  typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct function<R (BLIB_ARGS(15)) >
	{
		BLIB_FUNC_BUILD_FUNCTION(15)
	};
	
	
	#undef BLIB_FUNC_BUILD_FUNCTION
};
};

#endif //__function__hpp
