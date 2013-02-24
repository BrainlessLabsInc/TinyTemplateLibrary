//  collection.hpp
//
//  Copyright (c) 2004 Eugene Gladyshev
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef __BLIB_collection__hpp
#define __BLIB_collection__hpp

namespace blib
{

#define BLIB_COLLECTION_ARG(n,t) const T& t##n,
#define BLIB_COLLECTION_ARG_END(n,t) const T& t##n

#define BLIB_COLLECTION_ITEM(n,t) push_back(t##n);
#define BLIB_COLLECTION_ITEM_END(n,t) push_back(t##n);

#define BLIB_COLLECTION_ARGS(n) BLIB_REPEAT(n, BLIB_COLLECTION_ARG, BLIB_COLLECTION_ARG_END, p)
#define BLIB_COLLECTION_ITEMS(n) BLIB_REPEAT(n, BLIB_COLLECTION_ITEM, BLIB_COLLECTION_ITEM_END, p)

	template< typename T, typename C = std::vector<T> >
	struct collection : C
	{
		typedef C container;
				
		collection( ) {}
		
		collection( BLIB_COLLECTION_ARGS(1) ) 
		{
			reserve(1);
			BLIB_COLLECTION_ITEMS(1)
		}
		collection( BLIB_COLLECTION_ARGS(2) ) 
		{
			reserve(2);
			BLIB_COLLECTION_ITEMS(2)
		}
		collection( BLIB_COLLECTION_ARGS(3) ) 
		{
			reserve(3);
			BLIB_COLLECTION_ITEMS(3)
		}
		collection( BLIB_COLLECTION_ARGS(4) ) 
		{
			reserve(4);
			BLIB_COLLECTION_ITEMS(4)
		}
		collection( BLIB_COLLECTION_ARGS(5) ) 
		{
			reserve(5);
			BLIB_COLLECTION_ITEMS(5)
		}
		collection( BLIB_COLLECTION_ARGS(6) ) 
		{
			reserve(6);
			BLIB_COLLECTION_ITEMS(6)
		}
		collection( BLIB_COLLECTION_ARGS(7) ) 
		{
			reserve(7);
			BLIB_COLLECTION_ITEMS(7)
		}
		collection( BLIB_COLLECTION_ARGS(8) ) 
		{
			reserve(8);
			BLIB_COLLECTION_ITEMS(8)
		}
		collection( BLIB_COLLECTION_ARGS(9) ) 
		{
			reserve(9);
			BLIB_COLLECTION_ITEMS(9)
		}
		collection( BLIB_COLLECTION_ARGS(10) ) 
		{
			reserve(10);
			BLIB_COLLECTION_ITEMS(10)
		}
		collection( BLIB_COLLECTION_ARGS(11) ) 
		{
			reserve(11);
			BLIB_COLLECTION_ITEMS(11)
		}
		collection( BLIB_COLLECTION_ARGS(12) ) 
		{
			reserve(12);
			BLIB_COLLECTION_ITEMS(12)
		}
		collection( BLIB_COLLECTION_ARGS(13) ) 
		{
			reserve(13);
			BLIB_COLLECTION_ITEMS(13)
		}
		collection( BLIB_COLLECTION_ARGS(14) ) 
		{
			reserve(14);
			BLIB_COLLECTION_ITEMS(14)
		}
		collection( BLIB_COLLECTION_ARGS(15) ) 
		{
			reserve(15);
			BLIB_COLLECTION_ITEMS(15)
		}
	};
};

#endif //__BLIB_collection__hpp
