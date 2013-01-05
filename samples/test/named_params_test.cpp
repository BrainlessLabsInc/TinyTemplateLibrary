//  named_params_test.cpp
//
//  Copyright (c) 2004 Eugene Gladyshev
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//


//INCLUDES
//========
#include "boost/bind.hpp"
#include "ttl/func/named_params_function.hpp"


//LOCAL DECLARATIONS
//==================

//define argument names

struct widget
{
    //define a functor with named parameters
    typedef ttl::func::named_params_function< 
        int //the function returns 'int'
        (
            //'argument name',  'argument type'
             struct title,       const char*
            ,struct style,       ttl::func::numeric_argument<int, 45> //the default is 45
        ) 
    > create_functor;
    
    //
    widget()
    {
        //initialize the functor with create_impl
        create = boost::bind( &widget::create_impl, this, _1, _2 );
    }
    
    create_functor create;
    
private:
    
    int create_impl( const char* title, int style )
    {
        if( !title )
        {
            printf( "create arguments:\n"
                    "title=%s, style=%d\n\n", "*nill*", style );
        }
        else
        {
            printf( "create arguments:\n"
                    "title=%s, style=%d\n\n", title, style );
        }
        return 2;
    }
    
};


//IMPLEMENTATION
//==============
void named_params_test()
{
    widget w;
    
    w.create(
         w.create.arg<title>("custom style")
        ,w.create.arg<style>(3)
    );
    w.create( w.create.arg<title>("default style") );
    int res = w.create( w.create.arg<style>(10) );
         
    printf( "the last widget::create result = %d\n", res );
}
