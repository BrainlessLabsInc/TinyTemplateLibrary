#ifndef __TTL_IF_INCLUDED__
#define __TTL_IF_INCLUDED__

namespace ttl{namespace meta{
   template<bool Cond,typename IfType,typename ElseType>
   struct if_c
   {
      typedef IfType type;
   };

   template<typename IfType,typename ElseType>
   struct if_c<false,IfType,ElseType>
   {
      typedef ElseType type;
   };
}// namespace meta
}// namespace ttl

#endif // __TTL_IF_INCLUDED__
