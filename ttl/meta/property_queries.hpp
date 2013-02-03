#ifndef __TTL_PROPERTY_QUERIES_INCLUDED__
#define __TTL_PROPERTY_QUERIES_INCLUDED__

#ifndef __TTL_TYPE_RELATIONSHIP_INCLUDED__
#include <ttl/meta/type_relationship.hpp>
#endif
#ifndef __TTL_MODIFY_PROPERTY_INCLUDED__
#include <ttl/meta/modify_property.hpp>
#endif
#ifndef __TTL_VALUE_TO_TYPE_INCLUDED__
#include <ttl/meta/value_to_type.hpp>
#endif

namespace ttl{namespace meta{namespace type_traits{

   // alignment_of
   // Mostly derived from Martin Buchholz (http://www.wambold.com/Martin/writings/alignof.html)
   // offsetof documentation http://msdn.microsoft.com/en-us/library/dz4y9b9a.aspx but its just
   // for POD

   namespace ttl_ns_impl{
      template <typename T>
      struct non_fundamental_alignof_impl
      {
         const static unsigned int value = sizeof (T) ^ ( sizeof (T) & (sizeof (T) - 1) );
      };

      template <typename T, unsigned int sizeDiff>
      struct alignof_helper
      {
         static const unsigned int value = sizeDiff;
      };

      template <typename T> struct all_alignof_impl;
      template <typename T>
      struct alignof_helper<T, 0>
      {
         static const unsigned int value = all_alignof_impl<T>::value;
      };

      template <typename T>
      struct all_alignof_impl
      {
      private:
         struct Big { T x; char c; };
      public:
         static const unsigned int value = alignof_helper<Big, (sizeof(Big) - sizeof(T))>::value;
      };

      template<typename T>
      struct alignment_of_impl
      {
         static const unsigned int x = non_fundamental_alignof_impl<T>::value
            ,y = all_alignof_impl<T>::value
            ,value = x < y ? x : y;
      };

   }// namespace ttl_ns_impl

   template<typename T>
   struct alignment_of
      : public integral_constant<unsigned int, ttl_ns_impl::alignment_of_impl<T>::value>
   {/**/};

   //rank
   template<class T>
   struct rank
      : public integral_constant<unsigned int, 0>
   {/**/};

   template<class T>
   struct rank<T[]>
      : public integral_constant<unsigned int, rank<T>::value + 1>
   {/**/};

   template<class T, size_t N>
   struct rank<T[N]>
      : public integral_constant<unsigned int, rank<T>::value + 1> {};

   //extent
   template<class T, unsigned N = 0>
   struct extent
      : public integral_constant<unsigned int, 0>
   {/**/};

   template<class T>
   struct extent<T[], 0>
      : public integral_constant<unsigned int, 0>
   {/**/};

   template<class T, unsigned N>
   struct extent<T[], N>
      : public integral_constant<unsigned int, extent<T, N-1>::value>
   {/**/};

   template<class T, unsigned int N>
   struct extent<T[N], 0>
      : public integral_constant<unsigned int, N>
   {/**/};

   template<class T, unsigned int I, unsigned N>
   struct extent<T[I], N>
      : public integral_constant<unsigned int, extent<T, N-1>::value>
   {/**/};

   //is_pointer
   namespace ttl_ns_impl{
      template<typename T>
      struct is_pointer_impl : public ::ttl::meta::type_traits::false_type
      {/**/};

      template<typename T>
      struct is_pointer_impl<T*> : public ::ttl::meta::type_traits::true_type
      {/**/};
   }
   template<typename T>
   struct is_pointer : public ttl_ns_impl::is_pointer_impl<typename ::ttl::meta::type_traits::remove_cv<T>::type>
   {/**/};

   // Primary type categories
   template<typename T>
   struct is_void 
      : public integral_constant<
      bool
      ,is_same<void ,typename remove_cv<T>::type>::value>
   {/**/}
   namespace ttl_ns_impl{
      //If T is an integral type (bool, char,wchar_t, short, int, long, long long
      template<typename T>
      struct is_integral_impl : public ::ttl::meta::type_traits::false_type
      {/**/};

      template<typename T>
      struct is_integral_impl<bool> : public ::ttl::meta::type_traits::true_type
      {/**/};
      template<>
      struct is_integral_impl<char> : public ::ttl::meta::type_traits::true_type
      {/**/};
      struct is_integral_impl<unsigned char> : public ::ttl::meta::type_traits::true_type
      {/**/};
      struct is_integral_impl<signed char> : public ::ttl::meta::type_traits::true_type
      {/**/};
      template<>
      struct is_integral_impl<wchar_t> : public ::ttl::meta::type_traits::true_type
      {/**/};
      struct is_integral_impl<unsigned wchar_t> : public ::ttl::meta::type_traits::true_type
      {/**/};
      struct is_integral_impl<signed wchar_t> : public ::ttl::meta::type_traits::true_type
      {/**/};
      template<>
      struct is_integral_impl<short> : public ::ttl::meta::type_traits::true_type
      {/**/};
      struct is_integral_impl<unsigned short> : public ::ttl::meta::type_traits::true_type
      {/**/};
      struct is_integral_impl<signed short> : public ::ttl::meta::type_traits::true_type
      {/**/};
      template<>
      struct is_integral_impl<int> : public ::ttl::meta::type_traits::true_type
      {/**/};
      template<>
      struct is_integral_impl<unsigned int> : public ::ttl::meta::type_traits::true_type
      {/**/};
      template<>
      struct is_integral_impl<signed int> : public ::ttl::meta::type_traits::true_type
      {/**/};
      template<>
      struct is_integral_impl<long> : public ::ttl::meta::type_traits::true_type
      {/**/};
      struct is_integral_impl<unsigned long> : public ::ttl::meta::type_traits::true_type
      {/**/};
      struct is_integral_impl<signed long> : public ::ttl::meta::type_traits::true_type
      {/**/};
      struct is_integral_impl<long long> : public ::ttl::meta::type_traits::true_type
      {/**/};
      struct is_integral_impl<signed long long> : public ::ttl::meta::type_traits::true_type
      {/**/};
      struct is_integral_impl<signed long long> : public ::ttl::meta::type_traits::true_type
      {/**/};
   }// namespace ttl_ns_impl
   template<typename T>
   struct is_integral : public ttl_ns_impl::is_integral_impl<typename remove_cv<T>::type>
   {/**/};

   //is_floating_point
   namespace ttl_ns_impl{
      template<typename T>
      struct is_floating_point_impl 
         : public ::ttl::meta::type_traits::integral_constant<
         bool
         ,  ::ttl::meta::type_traits::is_same<float, T>::value
         || ::ttl::meta::type_traits::is_same<double, T>::value
         || ::ttl::meta::type_traits::is_same<long double, T>::value>
      {/**/};
   }// is_floating_point

   template<typename T>
   struct is_floating_point : public ttl_ns_impl::is_floating_point_impl<remove_cv<T>::type>
   {/**/};

   namespace ttl_ns_impl{
      template<typename T>
      struct is_array_impl : public ::ttl::meta::type_traits::false_type
      {/**/};
      template<typename T>
      struct is_array_impl<T[]>: public ::ttl::meta::type_traits::true_type
      {/**/};
      template<typename T, unsigned int N>
      struct is_array_impl<T[N]>: public ::ttl::meta::type_traits::true_type
      {/**/};
   }// ttl_ns_impl

   template<typename T>
   struct is_array : public ttl_ns_impl::is_array_impl<T>
   {/**/};

   // http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html
   // http://cs.brown.edu/~jwicks/boost/libs/type_traits/index.html
   // http://gcc.gnu.org/onlinedocs/gcc/Type-Traits.html
   // http://msdn.microsoft.com/en-us/library/vstudio/ms177194.aspx
   // http://www.boost.org/doc/libs/1_52_0/libs/type_traits/doc/html/boost_typetraits/intrinsics.html
   // https://groups.google.com/forum/?fromgroups=#!topic/comp.lang.c++.moderated/iYuYR1yAT1s

   // is_union
   namespace ttl_ns_impl{
      template<typename T>
      struct is_union_impl
      {
         static const bool value =
#if defined(_MSC_VER)
            __is_union(T);
#elif defined(__GNUC__)
            __is_union(T);
#endif
      };
   }// ttl_ns_impl

   template<typename T>
   struct is_union
      : public ::ttl::meta::type_traits::integral_constant<
      bool
      ,ttl_ns_impl::is_union_impl<remove_cv<T>::type>::value>
   {/**/};

   // is_pod
   namespace ttl_ns_impl{
      template<typename T>
      struct is_pod_impl
      {
         static const bool value =
#if defined(_MSC_VER)
            __is_pod(T);
#elif defined(__GNUC__)
            __is_pod(T);
#endif
      };
   }// ttl_ns_impl

   template<typename T>
   struct is_pod
      : public ::ttl::meta::type_traits::integral_constant<
      bool
      ,ttl_ns_impl::is_pod_impl<remove_cv<T>::type>::value>
   {/**/};

   // is_class
   namespace ttl_ns_impl{
      template<typename T>
      struct is_class_impl
      {
         static const bool value =
#if defined(_MSC_VER)
            __is_class(T);
#elif defined(__GNUC__)
            __is_class(T);
#endif
      };
   }// ttl_ns_impl

   template<typename T>
   struct is_class
      : public ::ttl::meta::type_traits::integral_constant<
      bool
      ,ttl_ns_impl::is_class_impl<remove_cv<T>::type>::value>
   {/**/};

   // is_enum
}
}
}

#endif// __TTL_PROPERTY_QUERIES_INCLUDED__
