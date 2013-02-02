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
}
}
}

#endif// __TTL_PROPERTY_QUERIES_INCLUDED__
