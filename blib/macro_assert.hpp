//  macro_assert.hpp
//
//  Copyright (c) 2003 Eugene Gladyshev
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef __BLIB_macro_assert__hpp
#define __BLIB_macro_assert__hpp

namespace blib
{
	template< bool C > struct assert {};
	
	template<>
	struct assert<true> 
	{
		typedef int type;
	};
};


#define BLIB_ASSERT( c ) sizeof( blib::assert<c>::type );

#endif //__macro_assert__hpp
