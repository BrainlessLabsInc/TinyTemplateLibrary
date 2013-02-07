//  sync.hpp
//
//  Copyright (c) 2003 Eugene Gladyshev
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef __ttl_sync__hpp
#define __ttl_sync__hpp

#include "blib/config.hpp"

#include "boost/thread/thread.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition.hpp"

namespace blib
{
namespace sys
{
	struct time_interval
	{
		enum
		{
			infinite = -1
		};

		int t_;
		time_interval( int t = 0 ) : t_(0)
		{}
	};

// the default synchronization policy (single-thread)
	template< typename M >
	struct scoped_lock
	{
		typedef M mutex;

		scoped_lock( mutex& m ) : m_(m) { m_.lock(); }
		~scoped_lock() { m_.unlock(); }

	private:
		mutex& m_;
	};

	struct single_thread_traits
	{
		typedef sys::time_interval time_interval;

		// fake mutex
		struct mutex : noncopyable
		{
			void lock() const {}
			void unlock() const {}
		};

		struct condition : noncopyable 
		{
			template< typename ScopedLock >
			void timed_wait( ScopedLock&, const time_interval& t )
			{
			}
			template< typename ScopedLock >
			void wait( ScopedLock& )
			{
			}
			void notify_one()
			{
			}
		};

		typedef scoped_lock<mutex> scoped_lock;
	};

	struct multi_thread_traits
	{
		typedef boost::thread thread;
		typedef boost::mutex mutex;
		typedef boost::condition condition;

		typedef mutex::scoped_lock scoped_lock;
	};


}; //sys
}; //ttl

#endif //ttl_sync_hpp
