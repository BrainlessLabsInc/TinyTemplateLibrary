#ifndef __BLIB_TYPE_RELATIONSHIP_INCLUDED__
#define __BLIB_TYPE_RELATIONSHIP_INCLUDED__

#ifndef __BLIB_VALUE_TO_TYPE_INCLUDED__
#include <value_to_type.hpp>
#endif

namespace blib{namespace meta{namespace type_traits{
   //is_same implementation
   template<typename,typename>
   struct is_same : public false_type{};
   template<typename Type>
   struct is_same<Type,Type> : public true_type{};

   //is_convertible and is_baseof implementation
   namespace blib_ns_impl{
      typedef char First;
      struct Second{char _[2];};
      template<typename ToType>
      First test(ToType);
      Second test(...);
      template<typename FromType>
      FromType makeType();
      template<typename FromType,typename ToType>
      struct is_convertible_impl
      {
      public:
         const static bool value = sizeof(test(makeType<FromType>())) == sizeof(First);
      };

      // is_base_of implementatio
      // Mostly derived from http://stackoverflow.com/questions/2910979/how-is-base-of-works
      template <typename BaseType, typename DerivedType>
      struct ConverterCast
      {
         operator BaseType*() const;
         operator DerivedType*();
      };

      template <typename BaseType, typename DerivedType>
      struct is_base_of_impl
      {
         static const bool value = sizeof(check(ConverterCast<BaseType,DerivedType>(), int())) == sizeof(First);
      private:
         template <typename T>
         First check(DerivedType*, T);
         Second check(BaseType*, int);
      };

      //another implementation for test http://ideone.com/T0C1V by Johannes Schaub (bloglitb.blogspot.com)
      /*
      namespace isbase_detail {
         template<typename T> struct identity { typedef T type; };

         template<typename,typename>
         struct tovoid { typedef void type; };
      }

      // detects private and ambiguous base classes, like boost::is_base_of,
      // but using a different technique.
      template<typename B, typename D, typename = void>
      struct isbase { static bool const value = false; };

      template<typename B, typename D>
      struct isbase<B, D,
         typename isbase_detail::tovoid<int B::*, int D::*>::type>
      {
         // will be used if B is *not* a base of D
         static isbase_detail::identity<char[1]>::type &c(B&, int);

         // will be used otherwise
         static isbase_detail::identity<char[2]>::type &c(int, long);

         struct inheritance : D {
            operator int();
            operator B&();
         };

         static inheritance irval();
         static bool const value = sizeof c(irval(), 0) == 2;
      };
      */
   }// namespace blib_ns_impl

   template<typename FromType,typename ToType>
   struct is_convertible
      :public integral_constant<bool,blib_ns_impl::is_convertible_impl<FromType,ToType>::value>
   {/**/};

   // is_base_of
   template <typename BaseType, typename DerivedType>
   struct is_base_of_impl
      :public integral_constant<bool,blib_ns_impl::is_base_of_impl<BaseType,DerivedType>::value>
   {/**/};
}
}
}
#endif// __BLIB_TYPE_RELATIONSHIP_INCLUDED__
