//  selector.hpp
//
//  Copyright (c) 2003 Eugene Gladyshev
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef __BLIB_selector__hpp
#define __BLIB_selector__hpp

namespace blib
{
	template <bool Cond, typename T1, typename T2 >
	struct selector
	{
		typedef T1 type;
	};

	template <typename T1, typename T2 >
	struct selector<false, T1, T2>
	{
		typedef T2 type;
	};
};

#endif //__selector__hpp
