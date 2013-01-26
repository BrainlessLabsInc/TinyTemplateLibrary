#ifndef __TTL_VALUE_TO_TYPE_INCLUDED__
#define __TTL_VALUE_TO_TYPE_INCLUDED__

namespace ttl{namespace meta{

   struct int_to_type_tag;
   template<int Val>
   struct int_
   {
      const static int value = Val;
      typedef int value_type;
      typedef int_to_type_tag tag;
   };

   struct bool_to_type_tag;
   template<bool Val>
   struct bool_
   {
      const static bool value = Val;
      typedef bool value_type;
      typedef bool_to_type_tag tag;
   };

   struct long_to_type_tag;
   template<long Val>
   struct long_
   {
      const static long value = Val;
      typedef long value_type;
      typedef long_to_type_tag tag;
   };

   struct char_to_type_tag;
   template<char Val>
   struct char_
   {
      const static char value = Val;
      typedef char value_type;
      typedef char_to_type_tag tag;
   };

   namespace type_traits{
      struct integral_c_type_tag;
      template<typename Type,Type Val>
      struct integral_constant
      {
         const static Type value = Val;
         typedef Type value_type;
         typedef integral_c_type_tag tag;
      };
      typedef integral_constant<bool,true> true_type;
      typedef integral_constant<bool,false> false_type;
   }// namespace type_traits
}
}

#endif // __TTL_VALUE_TO_TYPE_INCLUDED__
