#ifndef __BLIB_ENABLE_IF_INCLUDED__
#define __BLIB_ENABLE_IF_INCLUDED__

namespace blib{namespace meta{namespace type_traits{
   template<bool Cond,class Type=void>
   struct enable_if
   {
   };

   template<class Type>
   struct enable_if<true,Type>
   {
      typedef Type type;
   };
}
}
}
#endif// __BLIB_ENABLE_IF_INCLUDED__
