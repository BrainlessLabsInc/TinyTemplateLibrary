//  fifo.hpp
//
//  Copyright (c) 2003 Eugene Gladyshev
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef __BLIB_fifo__hpp
#define __BLIB_fifo__hpp

#include "blib/config.hpp"

namespace blib
{
namespace sys
{
	template< typename E, typename SyncPolicy >
	struct fifo : noncopyable
	{
		typedef E event;
		typedef SyncPolicy sync_policy;
		typedef typename sync_policy::mutex mutex;
		typedef typename sync_policy::scoped_lock scoped_lock;
		typedef typename sync_policy::condition condition;

		typedef std::list<event> queue;

		fifo() 
			:exit_(false)
			,max_size_(0)
		{}

		fifo( size_t max_size )
			:exit_(false)
			,max_size_(max_size)
		{}

		~fifo()
		{}

		void exit()
		{
			scoped_lock l(m_);
			exit_ = true;
			c_.notify_one();
		}

		void post( const event& e )
		{
			scoped_lock l(m_);
			while( max_size_ && q_.size() == max_size_ )
			{
				c_.wait(l);  //wait until there is enough space
				if( exit_ )
					return;
			}
			q_.push_back(e);
			c_.notify_one();
		}

		bool next( event& e )
		{
			scoped_lock l(m_);

			if( exit_ ) 
				return false;

			while( !q_.size() ) //no events
			{
				c_.wait(l);
				if( exit_ ) 
					return false;
			}

			// next event in the queue
			queue::iterator it = q_.begin();
			assert( it != q_.end() );
			e = *it;
			q_.erase(it);
			if( max_size_ )
				c_.notify_one();
			return true;
		}

		bool is_exit()
		{
			scoped_lock l(m_);
			return exit_;
		}

		bool is_empty()
		{
			scoped_lock l(m_);
			return q_.size() == 0;
		}

		template< typename Deleter >
		void clear( const Deleter& d )
		{
			scoped_lock l(m_);
			queue::const_iterator it;
			for( it = q_.begin(); it != q_.end(); ++it )
			{
				d(*it);
			}
			q_.clear();
		}
		void clear()
		{
			q_.clear();
		}

		size_t get_size()
		{
			scoped_lock l(m_);
			return q_.size();
		}

	private:
		bool exit_;
		size_t max_size_;

		queue q_;
		mutex m_; 
		condition c_;
	};

}; //sys
}; //ttl

#endif //__BLIB_fifo__hpp


