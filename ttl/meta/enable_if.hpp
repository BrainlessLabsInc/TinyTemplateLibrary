#ifndef __TTL_ENABLE_IF_INCLUDED__
#define __TTL_ENABLE_IF_INCLUDED__

namespace ttl{namespace meta{namespace type_traits{
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
#endif// __TTL_ENABLE_IF_INCLUDED__