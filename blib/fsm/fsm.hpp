//  fsm.hpp
//
//  Copyright (c) 2003 Eugene Gladyshev
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef __BLIB_fsm__hpp
#define __BLIB_fsm__hpp

#include "blib/config.hpp"
//#include "blib/meta/typelist.hpp"
#include "blib/sys/sync.hpp"
#include "blib/sys/fifo.hpp"

#include "boost/mpl/vector.hpp"
#include "boost/mpl/insert_range.hpp"
#include "boost/mpl/at.hpp"
#include "boost/mpl/size.hpp"
#include "boost/mpl/begin.hpp"

namespace blib
{
namespace fsm
{

	namespace impl
	{
		template< typename Sys >
		struct node : noncopyable
		{
			typedef node this_t;
			typedef Sys system;

			virtual ~node() {}


			virtual this_t* find_node( int ev_id ) = 0;
			virtual this_t* make_transition(system* sys, void *pe ) = 0;
			virtual void exit( ) = 0;
			virtual bool on_event( system* sys, int ev_id, void *pe ) = 0;
		};

		template< typename State >
		struct state_instance
		{
			typedef State state;

			state_instance() : s_(0) {}
			state_instance( state* s ) : s_(s) {}
			~state_instance( )
			{
				exit();
			}

			template< typename Sys, typename E >
			void enter( Sys* sys, const E* pe )
			{
				assert( !s_ );
				s_ = new state(sys, *pe);
			}

			void exit()
			{
				if( !s_ ) return;
				delete s_;
				s_ = 0;
			}

			template< typename Sys >
			bool on_event( Sys* sys, int ev_id, void* pe )
			{
				assert( s_ );
				return find_map_event< Sys, state >::find( sys, s_, ev_id, pe );
			}

		private:
			state *s_;
		};

		template< 
			typename Sys
			,typename S
			,int N = 0
			,bool Stop = (N>=boost::mpl::size<typename S::transitions>::value) 
		>
		struct state_node : node<Sys>
		{
			typedef Sys system;
			typedef S state;
			typedef typename state::transitions transitions;
			typedef typename boost::mpl::at_c<transitions,N>::type transition;
			typedef typename transition::event event;
			typedef typename transition::target_state target_state;

			state_node() 
			{}
			state_node( system *s ) 
				:s_( new state(s) ) 
			{}

			virtual ~state_node()
			{}

			node<system>* find_node( int ev_id )
			{
				if( event::id == ev_id )
					return this;

				//continue to the next sibling
				return n_.find_node( ev_id );
			}

			virtual node<system>* make_transition( system* sys, void *pe )
			{
				state_node<system,target_state>* t = new state_node<system,target_state>();
				t->enter(sys, static_cast<event*>(pe));
				return t;
			}

			void exit()
			{
				s_.exit();
			}

			virtual bool on_event( system *sys, int ev_id, void *pe )
			{
				return s_.on_event( sys, ev_id, pe );
			}

			template< typename E >
			void enter( system* sys, const E* pe )
			{
				s_.enter( sys, pe );
			}

		private:
			state_instance<state> s_;

			state_node<system, state, N+1> n_; // siblings
		};


		template< typename Sys, typename S, int N >
		struct state_node< Sys, S, N, true > : node<Sys>
		{
			typedef S state;

			state_node() : s_(0) {}
			state_node( system* sys ) : s_( new state(sys) ) {}

			virtual ~state_node()
			{}

			node<system>* find_node( int ev_id )
			{
				return 0;
			}

			this_t* make_transition(system* sys, void *pe )
			{
				assert(0);
				throw std::runtime_error( "blib::fsm - internal error (bug)" );
			}

			void exit()
			{
				s_.exit();
			}

			bool on_event( system *sys, int ev_id, void *pe )
			{
				return s_.on_event( sys, ev_id, pe );
			}

			template< typename E >
			void enter( system* sys, const E* pe )
			{
				s_.enter( sys, pe );
			}

		private:
			state_instance<state> s_;
		};


		template< 
			typename Sys
			,typename S
			,int N = 0
			,bool Stop = (N>=boost::mpl::size<typename S::event_map>::value) 
		>
		struct find_map_event
		{
			typedef Sys system;
			typedef S state;
			typedef typename S::event_map events;
			typedef typename boost::mpl::at_c<events,N>::type event;

			static bool find( system *sys, state* s, int id, void* pe )
			{
				if( id == event::id )
				{
					s->on_event( sys, *static_cast<event*>(pe) );
					return true;
				}
				return find_map_event<Sys, S,N+1>::find(sys, s, id, pe);
			}
		};

		template< typename Sys, typename S, int N >
		struct find_map_event<Sys, S, N, true>
		{
			typedef Sys system;
			typedef S state;

			static bool find( system *sys, state* s, int id, void* pe )
			{
				return false;
			}
		};


		struct event_holder_base : noncopyable
		{
			int id_;
			void* pe_;
			event_holder_base( int id, void* pe ) 
				:id_(id), pe_(pe)
			{}
			virtual ~event_holder_base() {}
		};

		template< typename E >
		struct event_holder : event_holder_base
		{
			event_holder( const E& e ) 
				:event_holder_base( E::id, new E(e) )
			{}
			~event_holder()
			{
				assert( pe_ );
				delete static_cast<E*>(pe_);
			}
		};

	}; //impl


	// transition statement
	template< typename E, typename S >
	struct transition
	{
		typedef E event;
		typedef S target_state;
	};

	// the default policies for unexpected events
	struct ignore_unexpected_events_policy
	{
		void operator()( void* sys, int id, void* pe )
		{
		}
	};

	struct throw_unexpected_events_policy
	{
		void operator()( int id )
		{
			throw std::runtime_error( "blib::fsm - unexpected event" );
		}
	};

	// default exception policies
	struct ignore_exceptions_policy
	{
		bool ignore( int id )
		{
			return true; //ignore
		}
	};

	struct propagate_exceptions_policy
	{
		bool ignore( int id )
		{
			return false; //don't ignore
		}
	};

	// the default event queue interface
	template< typename SyncTraits >
	struct event_queue : noncopyable
	{
		typedef sys::fifo<impl::event_holder_base*, SyncTraits> fifo;

		//event deleter
		struct deleter
		{
			void operator()( impl::event_holder_base *e ) const
			{
				delete e;
			}
		};

		event_queue() : ce_(0)
		{}

		~event_queue()
		{
			clear();
		}


		template< typename E >
		void post( const E& e )
		{
			f_.post( new impl::event_holder<E>(e) );
		}

		void exit()
		{
			f_.exit();
		}

		// !!! note: NOT THREAD SAFE
		// this should be called from the state machine thread only
		// the same thread that is calling next() or is_empty()
		template< typename E >
		void post_critical( const E& e )
		{
			assert( !ce_ );
			ce_ = new impl::event_holder<E>(e);
		}

		bool is_exit()
		{
			return f_.is_exit();
		}

		bool is_empty()
		{
			// not exit event, critical event or fifo event
			return !is_exit() && !ce_ && f_.is_empty();
		}

		impl::event_holder_base* next()
		{
			impl::event_holder_base* e;

			if( ce_ )
			{
				//the caller is responsible for deleting it
				impl::event_holder_base* e = ce_; 
				ce_ = 0;
				return e;
			}

			if( !f_.next(e) )
				return 0;
			return e;
		}


	private:
		impl::event_holder_base *ce_; //high priority internal event
		fifo f_;

		void clear()
		{
			clear_internal();
			f_.clear( deleter() );
		}

		void clear_internal()
		{
			if( !ce_ )
				return;
			delete ce_;
			ce_ = 0;
		}
	};

	struct state_machine_base : noncopyable
	{
		virtual ~state_machine_base()
		{}

		virtual void start() = 0;
		virtual void stop() = 0;
		virtual bool transit() = 0;
	};
	
	template< 
		typename System
		,typename RootState
		,typename EventQ = event_queue<sys::single_thread_traits>
		,typename ExceptionPolicy = propagate_exceptions_policy
		,typename UnEvPolicy = ignore_unexpected_events_policy
	>
	struct state_machine : state_machine_base
	{
		typedef System system;
		typedef RootState root_state;
		typedef ExceptionPolicy exceptions_policy;
		typedef UnEvPolicy unexpected_events_policy;
		typedef EventQ event_queue;

		system* sys_;
		event_queue& q_;
		exceptions_policy ep_;
		unexpected_events_policy ue_;

		state_machine( 
			system* sys
			,event_queue& q
			,const exceptions_policy& ep = exceptions_policy()
			,const unexpected_events_policy ue = unexpected_events_policy()
		) 
			:sys_(sys)
			,q_(q)
			,ep_(ep)
			,ue_(ue)
			,cs_(0)
			,ues_(0)
		{
		}

		~state_machine()
		{
			delete cs_;
			delete ues_;
		}

		void start()
		{
			cs_ = new impl::state_node<system, root_state>(sys_);
		}

		void stop()
		{
			q_.exit();
		}


		// unhandled events state
		template< typename UES >
		void set_ues()
		{
			delete ues_;
			ues_ = 0;
			ues_ = new impl::state_node<system, UES>(sys_);
		}

		bool transit()  //return false if stopped
		{
			if( q_.is_exit() )
				return false;

			impl::event_holder_base* e = q_.next();
			if( !e )
			{
				if( !q_.is_exit() )
					return true;  //the queue is empty
				return true;
			}

			typename event_queue::deleter d;
			try
			{
				make_transition( sys_, e );
				d(e);
			}
			catch( const std::exception& )
			{
				int id = e->id_;
				d(e);
				if( !ep_.ignore(id) ) //don't ignore
					throw;
			}
			return true;
		}

		event_queue& get_queue() { return q_; }

	private:
		impl::node<system> *cs_; //current state
		impl::node<system> *ues_; //unhandled event state

		void make_transition( system* sys, impl::event_holder_base* e )
		{
			assert( cs_ );


			impl::node<system> *ns = cs_->find_node( e->id_ );
			if( ns )
			{
				//exit the current state
				cs_->exit();

				//enter the next state
				impl::node<system> *n = ns->make_transition( sys, e->pe_ );

				//reset the current state holder (node)
				delete cs_;
				cs_ = n;
			}
			else //try the map
			{
				if( !cs_->on_event( sys, e->id_, e->pe_ ) ) //not in the map
				{
					if( !ues_ || !ues_->on_event(sys, e->id_, e->pe_) )
						ue_( sys, e->id_, e->pe_ );
				}
			}
		}
	};


}; //fsm

}; //ttl

#define FSM_BEGIN_STATE_MAP() typedef boost::mpl::vector<
#define FSM_END_STATE_MAP() > transitions;

#define FSM_BEGIN_STATE_MAP_EX(base) typedef boost::mpl::vector<
#define FSM_END_STATE_MAP_EX(base) > local_transitions; typedef boost::mpl::insert_range<local_transitions, boost::mpl::end<local_transitions>::type, base::transitions>::type transitions;

#define FSM_EMPTY_STATE_MAP() typedef boost::mpl::vector< > transitions;

#define FSM_BEGIN_EVENT_MAP() typedef boost::mpl::vector<
#define FSM_END_EVENT_MAP() > event_map;

#define FSM_BEGIN_EVENT_MAP_EX( base )  using base::on_event; typedef boost::mpl::vector<
#define FSM_END_EVENT_MAP_EX( base ) >event_map_local; typedef boost::mpl::insert_range<event_map_local, boost::mpl::end<event_map_local>::type, base::event_map>::type event_map;

#define FSM_USE_BASE_EVENT_MAP( bs ) using bs::event_map; using bs::on_event;

#define FSM_EMPTY_EVENT_MAP() typedef boost::mpl::vector< > event_map;


#endif //__BLIB_fsm__hpp

