#include <windows.h>
#include <vector>
#include "ttl/sccs/sccs.hpp"
#include "boost/bind.hpp"
#include <ios>

template< typename T >
struct buffer
{
	typedef T container;

	container d_;
	bool pending_get_;

	buffer() : pending_get_(false) {}
};


//message
struct get {};
struct in_item { int n_; };
struct out_item { int n_; };

// messages that could be generated in the producer/consumer system
typedef ttl::var::variant<
	get  //get the next item from the buffer
	,in_item //pushed item
	,out_item  //poped item
>message;

typedef ttl::sccs::agent<message> agent;
typedef agent::agent_pnt agent_pnt;


typedef buffer< std::vector<int> > my_buffer;

agent::message_pnt push( my_buffer* b, agent::message_pnt m )
{
	in_item d = ttl::var::get<in_item>(*m); //get the data

	//the buffer was empty and there is a pending get request
	//so we satisfy the get request immediately
	if( b->pending_get_ && !b->d_.size() )
	{
		b->pending_get_ = false;
		out_item dp = {d.n_};
		return agent::message_pnt( new message(dp) );
	}

	b->d_.push_back(d.n_);
	return agent::message_pnt(); //empty
}

agent::message_pnt pop( my_buffer* b, agent::message_pnt m )
{
	if( !b->d_.size() )
	{
		//buffer is empty, set the pending 'get' flag and return nothing
		b->pending_get_ = true;
		return agent::message_pnt();
	}

	my_buffer::container::iterator it = b->d_.begin();
	out_item d = {*it};
	b->d_.erase(it);
	return agent::message_pnt( new message(d) );
}

agent::message_pnt produce( agent::message_pnt m )
{
	static int counter = 0;
	::Sleep( 500 );
	in_item d = {++counter}; //produce the next item
	return agent::message_pnt( new message(d) );
}

agent::message_pnt consume( agent::message_pnt m )
{
	out_item d = ttl::var::get<out_item>(*m); //get the data
	std::cout<<d.n_<<"\n";
	::Sleep( 1000 );
	return agent::message_pnt(); //nothing
}

/*
buffer =
	in_item?:push:(out_item!:buffer+buffer)
	+get?:pop:(out_item!:buffer+buffer)

producer = produce:in_item!:producer

consumer = get!:out_item?:consume:consumer;

system = producer*consumer*buffer;
*/

void sccs_test()
{
	my_buffer b;  //buffer;

	//push branch of the buffer agent
	agent_pnt buffer_push(
		ttl::sccs::start<agent>()->in<in_item>()->act(boost::bind(push, &b, _1), "push")
		->attach(
			// post the input
			agent::agent_pnt( 
				ttl::sccs::start<agent>()->out<out_item>()
			)
			// OR do nothing
			+agent::agent_pnt(
				ttl::sccs::start<agent>()
			)
		)
	);

	//pop branch of the buffer agent
	agent_pnt buffer_pop(
		ttl::sccs::start<agent>()->in<get>()->act(boost::bind(pop, &b, _1), "pop")
		->attach(
			// post the input
			agent::agent_pnt( 
				ttl::sccs::start<agent>()->out<out_item>()
			)
			// OR do nothing
			+agent::agent_pnt(
				ttl::sccs::start<agent>()
			)
		)
	);

	// buffer agent is a choice between push and pop
	agent_pnt buffer( buffer_push + buffer_pop );

	// repeat forever
	buffer->repeat();

	//producer agent
	agent_pnt producer(
		ttl::sccs::start<agent>()->act(produce, "produce")->out<in_item>()->repeat()
	);

	//consumer agent
	agent_pnt consumer(
		ttl::sccs::start<agent>()->out<get>()->in<out_item>()->act(consume,"consume")->repeat()
	);

	//system is a product of buffer, producer and consumer agents
	agent_pnt system( producer * buffer * consumer );

	// print the the full agent's equation
	system->print( std::cout );

	// run the agent
	ttl::sccs::runtime<agent> rt( system );

	// let it run for sometime
	Sleep(5000);

	// ... and kill it
	rt.kill();
}


/*
struct done {};

// all messages
typedef ttl::var::variant<
	int
	,done
> messages;

typedef ttl::sccs::agent< messages > agent;  //our agent type

// agent action
agent::message_pnt cons_input( agent::message_pnt )
{
	int n;
	std::cin>>n;
	if( std::cin.fail() )
	{
		return agent::message_pnt( new messages(done()) );
	}
	return agent::message_pnt( new messages(n) );
}

struct print_visitor
{
	void operator()( int d )
	{
		std::cout<<d<<"\n";
	}
	template< typename T >
	void operator()( const T& d )
	{
	}
};

// agent action
agent::message_pnt cons_output( agent::message_pnt m )
{
	print_visitor v;
	ttl::var::apply_visitor(v,*m);
	return agent::message_pnt();
}


void sccs_test()
{
	//console input agent
	agent::agent_pnt input_agent(
		ttl::sccs::start<agent>()  //start
		->act( cons_input, "console_input" ) //agent action - next input (int)
		->attach(
			// post the input
			agent::agent_pnt( 
				ttl::sccs::start<agent>()->out<int>()
			)
			// OR post 'done'
			+agent::agent_pnt(
				ttl::sccs::start<agent>()->out<done>()->stop()
			)
		)
	);
	input_agent->repeat(); //repeat indefinitely

	//console output agent
	agent::agent_pnt output_agent(
		// output the input
		agent::agent_pnt(
			ttl::sccs::start<agent>()
			->in<int>()  //wait for the next input
			->act( cons_output, "console_output" )  //agent action - print the input
		)
		// OR wait for 'done' and stop
		+agent::agent_pnt(
			ttl::sccs::start<agent>()
			->in<done>()->stop()
		)
	);
	output_agent->repeat(); //repeat indefinitely

	// the full agent is a product of input_agent and output_agent
	// it means that cons_in and cons_out will run concurently 
	// (in different threads)
	agent::agent_pnt full( input_agent * output_agent );

	// print the the full agent's equation
	full->print( std::cout );

	// run the agent
	ttl::sccs::runtime<agent> rt( full );

	// wait for the agent to finish
	rt.join();
}

*/
