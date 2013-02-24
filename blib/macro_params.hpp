//  macro_params.hpp
//
//  Copyright (c) 2003 Eugene Gladyshev
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#ifndef __BLIB_macro_params__hpp
#define __BLIB_macro_params__hpp

#include "blib/macro_repeat.hpp"


#define BLIB_TPARAM(n,t) typename t##n, 
#define BLIB_TPARAM_END(n,t) typename t##n
#define BLIB_TPARAM_DEF(n, p) typename T##n = p, 
#define BLIB_TPARAM_DEF_END(n, p) typename T##n = p

#define BLIB_TPARAMSX(n,t) BLIB_REPEAT(n, BLIB_TPARAM, BLIB_TPARAM_END, t)
#define BLIB_TPARAMS(n) BLIB_TPARAMSX(n,T)
#define BLIB_TPARAMS_DEF(n, def) BLIB_REPEAT(n, BLIB_TPARAM_DEF, BLIB_TPARAM_DEF_END, def)


#define BLIB_ARG(n,t) t##n, 
#define BLIB_ARG_END(n,t) t##n
#define BLIB_ARG_S(n,t) BLIB_APPEND(t, BLIB_CNTINC_##n),
#define BLIB_ARG_S_END(n,t) BLIB_APPEND(t, BLIB_CNTINC_##n)

#define BLIB_ARGS(n) BLIB_REPEAT(n, BLIB_ARG, BLIB_ARG_END, T)
#define BLIB_ARGSX(n,p) BLIB_REPEAT(n, BLIB_ARG, BLIB_ARG_END, p)
#define BLIB_ARGS_S(n) BLIB_REPEAT(n, BLIB_ARG_S, BLIB_ARG_S_END, T)
#define BLIB_ARGS_SX(n,p) BLIB_REPEAT(n, BLIB_ARG_S, BLIB_ARG_S_END, p)

#define BLIB_TYPEDEF(n, t) typedef t##n type##n;
#define BLIB_TYPEDEF_END(n, t) typedef t##n type##n;
#define BLIB_TYPEDEFS(n) BLIB_REPEAT(n, BLIB_TYPEDEF, BLIB_TYPEDEF_END, T)

#define BLIB_LIST_ITEM(n, t) t,
#define BLIB_LIST_ITEM_END(n, t) t
#define BLIB_LIST_ITEMS(n, t) BLIB_REPEAT(n,BLIB_LIST_ITEM,BLIB_LIST_ITEM_END,t)

#define BLIB_FUNC_PARAM(n,t) T##n t##n, 
#define BLIB_FUNC_PARAM_END(n,t) T##n t##n
#define BLIB_FUNC_PARAMS(n, p) BLIB_REPEAT(n, BLIB_FUNC_PARAM, BLIB_FUNC_PARAM_END, p)

#define BLIB_FUNC_PPARAM(n,t) P##n t##n, 
#define BLIB_FUNC_PPARAM_END(n,t) P##n t##n
#define BLIB_FUNC_PPARAMS(n, p) BLIB_REPEAT(n, BLIB_FUNC_PPARAM, BLIB_FUNC_PPARAM_END, p)

#define BLIB_ENUM_ITEMS(n, t) BLIB_REPEAT(n,BLIB_ARG,BLIB_ARG_END,t)

#endif //__macro_params__hpp
