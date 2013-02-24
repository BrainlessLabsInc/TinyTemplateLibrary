//  signal.hpp
//
//  Copyright (c) 2003 Eugene Gladyshev
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef __BLIB_signal__hpp
#define __BLIB_signal__hpp

#include <list>
#include "blib/func/function.hpp"

namespace blib
{
namespace sig
{
	struct exception : blib::exception
	{
		exception() : blib::exception("signal error") {}
	};
	
	struct connection;

/////////////////////////////////////////////////////////////
namespace impl
{
	struct connection_holder_base
	{
		typedef blib::mem::memtraits::allocator<connection_holder_base> allocator;
		
		connection_holder_base() : ref_(0) {}
		virtual ~connection_holder_base() {}
		
		virtual void deactivate() = 0;
		
		int ref() const { return ref_; }
		
	protected:
		int ref_;
		
		friend struct blib::sig::connection;
		
	 private:
		//protect from copy
		connection_holder_base( const connection_holder_base& );
		const connection_holder_base& operator=( const connection_holder_base& );
	};
}; //impl

/////////////////////////////////////////////////////////////////////
	struct connection
	{
		typedef impl::connection_holder_base holder;
		connection() : c_(0) {}
		connection( holder* c ) : c_(c) { if(c_) ++c_->ref_; }
		connection( const connection& c ) : c_(c.c_) 
		{ 
			if(c_) ++c_->ref_; 
		}
		~connection() 
		{ 
			if(c_) --c_->ref_; 
		}
		
		inline void disconnect()
		{
			if( !c_ )return;
			
			//last connection
			if( !--c_->ref_ )
				blib::mem::destroy<holder::allocator>(c_);
			else
				c_->deactivate();
			c_ = 0;
		}
		inline bool is_active() const { return c_?true:false; }
		connection& operator=( const connection& c )
		{
			if( &c == this ) return *this;
			if(c_) --c_->ref_;
			c_ = c.c_;
			++c_->ref_;
			return *this;
		}
		
	private:
		holder* c_;
	};
	
namespace impl
{
	template< typename F >
	struct connection_holder : connection_holder_base
	{
		typedef std::list< connection_holder<F>* > list;
		
		list& l_;
		typename list::iterator it_;
		F f_;
		bool active_;
		
		connection_holder( list& l, const F& f ) : l_(l), f_(f), active_(true) 
		{
			it_ = l_.insert( l_.end(), this );
		}
		virtual ~connection_holder() 
		{  
			deactivate();
		}
		
		void deactivate()
		{
			if( !active_ ) return;
			l_.erase(it_);
			active_ = false;
		}
	};
	
	template< typename F >
	struct signal_base
	{
		typedef connection_holder<F> holder;
		typedef typename holder::list list;
		typedef blib::mem::memtraits::allocator<holder> allocator;
		
			
		signal_base() {}
		virtual ~signal_base() 
		{ 
			typename list::const_iterator it;
			for( it = con_.begin(); it != con_.end(); it = con_.begin() )
			{
				(*it)->deactivate();
			}
		}
		
		blib::sig::connection connect( const F& f )
		{
			connection_holder<F> *c = blib::mem::create<allocator, list&, const F&>( con_, f );
			if( !c ) throw blib::sig::exception();
			return blib::sig::connection(c);;
		}
		
		inline size_t size() const { return con_.size(); }
		
	protected:
		list con_;
		
	private:
		//protect from copy
		signal_base( const signal_base& );
		const signal_base& operator=( const signal_base& );
	};
	
};  //impl


	template < typename R, BLIB_TPARAMS_DEF(BLIB_MAX_FUNCTION_PARAMS, empty_type) >
	struct signal : impl::signal_base< func::function<R,BLIB_ARGS(BLIB_MAX_FUNCTION_PARAMS) > >
	{
		typedef func::function< R,BLIB_ARGS(BLIB_MAX_FUNCTION_PARAMS) > slot_type;
		typedef impl::signal_base<slot_type> base_t;
		typedef typename base_t::list list;
		
//		inline blib::sig::connection first() const { return blib::sig::connection(*con_.begin()); }  
//		inline blib::sig::connection last() const { return blib::sig::connection(*--con_.end()); }  
	
		void operator()()
		{
//			call( con_.begin(), con_.end() );
			for( typename list::const_iterator it = base_t::con_.begin(); it != base_t::con_.end(); ++it )
			{
				(*it)->f_();
			}
		}
		
		/*
		void operator()( const blib::sig::connection& first, const blib::sig::connection& end )
		{
			const_iterator it_first( static_cast<const connection_holder*>(first.get_holder())->it_ );
			const_iterator it_end( static_cast<const connection_holder*>(end.get_holder())->it_ );
			call( it_first, ++it_end );
		}
		*/
		
	protected:
		/*
		void call( const const_iterator& first, const const_iterator& end )
		{
			for( const_iterator it = fist; it != end; ++it )
			{
				(*it)->f_();
			}
		}
		*/
	};

#define BLIB_SIG_BUILD_SIGNAL(n) \
		typedef impl::signal_base<slot_type> base_t; \
		typedef typename base_t::list list; \
		void operator()(BLIB_FUNC_PARAMS(n,p)) \
		{ \
			typename list::const_iterator it;  \
			for( it = base_t::con_.begin(); it != base_t::con_.end(); ++it ) \
			{ \
				(*it)->f_(BLIB_ENUM_ITEMS(n,p)); \
			} \
		} 

/*
#define BLIB_SIG_BUILD_SIGNAL(n) \
		typedef impl::signal_base<slot_type> base_t; \
		typedef typename base_t::list list; \
		typedef typename base_t::connection_holder connection_holder;  \
		typedef typename list::const_iterator const_iterator;  \
		inline blib::sig::connection first() const { return blib::sig::connection(*con_.begin()); }  \
		inline blib::sig::connection last() const { return blib::sig::connection(*--con_.end()); }  \
		void operator()(BLIB_FUNC_PARAMS(n,p)) \
		{ \
			call( con_.begin(), con_.end(), BLIB_ENUM_ITEMS(n,p) );  \
		}  \
		void operator()( const blib::sig::connection& first, const blib::sig::connection& end, BLIB_FUNC_PARAMS(n,p) )  \
		{  \
			const_iterator it_first( static_cast<const connection_holder*>(first.get_holder())->it_ );  \
			const_iterator it_end( static_cast<const connection_holder*>(end.get_holder())->it_ );  \
			call( it_first, ++it_end, BLIB_ENUM_ITEMS(n,p) );  \
		}  \
		protected:  \
		void call( const const_iterator& first, const const_iterator& end, BLIB_FUNC_PARAMS(n,p) )  \
		{ \
			for( const_iterator it = first; it != end; ++it ) \
			{ \
				(*it)->f_(BLIB_ENUM_ITEMS(n,p)); \
			} \
		}
*/
		
	
	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(1)), T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15 > : 
		impl::signal_base< func::function<R (BLIB_ARGS(1)), T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> >
	{
		typedef func::function<R (BLIB_ARGS(1)), T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> slot_type;
		BLIB_SIG_BUILD_SIGNAL(1)
	};
	
	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(2)), T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15 > : 
		impl::signal_base< func::function<R (BLIB_ARGS(2)), T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> >
	{
		typedef func::function<R (BLIB_ARGS(2)), T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> slot_type;
		BLIB_SIG_BUILD_SIGNAL(2)
	};
	
	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(3)), T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15 > : 
		impl::signal_base< func::function<R (BLIB_ARGS(3)), T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> >
	{
		typedef func::function<R (BLIB_ARGS(3)), T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> slot_type;
		BLIB_SIG_BUILD_SIGNAL(3)
	};
	
	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(4)), T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15 > : 
		impl::signal_base< func::function<R (BLIB_ARGS(4)), T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> >
	{
		typedef func::function<R (BLIB_ARGS(4)), T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> slot_type;
		BLIB_SIG_BUILD_SIGNAL(4)
	};
	
	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(5)), T6, T7, T8, T9, T10, T11, T12, T13, T14, T15 > : 
		impl::signal_base< func::function<R (BLIB_ARGS(5)), T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> >
	{
		typedef func::function<R (BLIB_ARGS(5)), T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> slot_type;
		BLIB_SIG_BUILD_SIGNAL(5)
	};
	
	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(6)), T7, T8, T9, T10, T11, T12, T13, T14, T15 > : 
		impl::signal_base< func::function<R (BLIB_ARGS(6)), T7, T8, T9, T10, T11, T12, T13, T14, T15> >
	{
		typedef func::function<R (BLIB_ARGS(6)), T7, T8, T9, T10, T11, T12, T13, T14, T15> slot_type;
		BLIB_SIG_BUILD_SIGNAL(6)
	};
	
	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(7)), T8, T9, T10, T11, T12, T13, T14, T15 > : 
		impl::signal_base< func::function<R (BLIB_ARGS(7)), T8, T9, T10, T11, T12, T13, T14, T15> >
	{
		typedef func::function<R (BLIB_ARGS(7)), T8, T9, T10, T11, T12, T13, T14, T15> slot_type;
		BLIB_SIG_BUILD_SIGNAL(7)
	};

	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(8)), T9, T10, T11, T12, T13, T14, T15 > : 
		impl::signal_base< func::function<R (BLIB_ARGS(8)), T9, T10, T11, T12, T13, T14, T15> >
	{
		typedef func::function<R (BLIB_ARGS(8)), T9, T10, T11, T12, T13, T14, T15> slot_type;
		BLIB_SIG_BUILD_SIGNAL(8)
	};
	
	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(9)), T10, T11, T12, T13, T14, T15 > : 
		impl::signal_base< func::function<R (BLIB_ARGS(9)), T10, T11, T12, T13, T14, T15> >
	{
		typedef func::function<R (BLIB_ARGS(9)), T10, T11, T12, T13, T14, T15> slot_type;
		BLIB_SIG_BUILD_SIGNAL(9)
	};

	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(10)), T11, T12, T13, T14, T15 > : 
		impl::signal_base< func::function<R (BLIB_ARGS(10)), T11, T12, T13, T14, T15 > >
	{
		typedef func::function< R (BLIB_ARGS(10)), T11, T12, T13, T14, T15 > slot_type;
		BLIB_SIG_BUILD_SIGNAL(10)
	};
	
	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(11)), T12, T13, T14, T15 > : 
		impl::signal_base< func::function<R (BLIB_ARGS(11)), T12, T13, T14, T15 > >
	{
		typedef func::function< R (BLIB_ARGS(11)), T12, T13, T14, T15 > slot_type;
		BLIB_SIG_BUILD_SIGNAL(11)
	};
	
	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(12)), T13, T14, T15 > : 
		impl::signal_base< func::function<R (BLIB_ARGS(12)), T13, T14, T15 > >
	{
		typedef func::function< R (BLIB_ARGS(12)), T13, T14, T15 > slot_type;
		BLIB_SIG_BUILD_SIGNAL(12)
	};
	
	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(13)), T14, T15 > : 
		impl::signal_base< func::function<R (BLIB_ARGS(13)), T14, T15 > >
	{
		typedef func::function< R (BLIB_ARGS(13)), T14, T15 > slot_type;
		BLIB_SIG_BUILD_SIGNAL(13)
	};
	
	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(14)), T15 > : 
		impl::signal_base< func::function<R (BLIB_ARGS(14)), T15 > >
	{
		typedef func::function< R (BLIB_ARGS(14)), T15 > slot_type;
		BLIB_SIG_BUILD_SIGNAL(14)
	};
	
	template < typename R, BLIB_TPARAMS(BLIB_MAX_FUNCTION_PARAMS) >
	struct signal<R (BLIB_ARGS(15)) > : 
		impl::signal_base< func::function<R (BLIB_ARGS(15)) > >
	{
		typedef func::function< R (BLIB_ARGS(15)) > slot_type;
		BLIB_SIG_BUILD_SIGNAL(15)
	};
	
}; //sig
}; //ttl

#endif //__BLIB_signal__hpp
