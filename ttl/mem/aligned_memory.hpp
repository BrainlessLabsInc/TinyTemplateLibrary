#ifndef __TTL_ALIGNED_MEMORY_INCLUDED__
#define __TTL_ALIGNED_MEMORY_INCLUDED__

#ifndef __TTL_IF_INCLUDED__
#include <ttl/meta/if.hpp>
#endif

#ifndef __TTL_COMPILER_CONFIG_INCLUDED__
#include <ttl/config/compiler_config.hpp>
#endif

#ifndef __TTL_PROPERTY_QUERIES_INCLUDED__
#include <ttl/meta/property_queries.hpp>
#endif

// http://msdn.microsoft.com/en-us/library/83ythb65%28v=vs.90%29.aspx
// http://stackoverflow.com/questions/7895869/cross-platform-alignx-macro
// Credited to Sebastian Cabot
#if defined(TTL_MSVC)
#define _ALIGNED(x) __declspec(align(x))
#elif defined(TTL_GCC)
#define _ALIGNED(x) __attribute__ ((aligned(x)))
#endif
#define _ALIGNED_TYPE(t,x) typedef t _ALIGNED(x)

// http://stackoverflow.com/questions/6959261/how-can-i-simulate-alignast
// and boost aligned_storage implementation
// http://gustedt.wordpress.com/2012/01/02/emulating-c11-compiler-features-with-gcc-_generic/
namespace ttl{namespace mem{namespace type_traits{

   namespace ttl_ns_impl{
      class alignment_dummy;
      typedef void (*function_ptr)();
      typedef int (alignment_dummy::*member_ptr);
      typedef int (alignment_dummy::*member_function_ptr)();

      template <bool found, unsigned int target, typename TestType>
      struct lower_alignment_helper
      {
         static const bool value = true;
         typedef char type;
      };

      template <unsigned int target, typename TestType>
      struct lower_alignment_helper<false,target,TestType>
      {
         static const bool value = (ttl::meta::type_traits::alignment_of<TestType>::value == target) ;
         typedef typename ttl::meta::if_c<value, TestType, char>::type type;
      };

      template <typename Type>
      struct has_one_T
      {
         Type data;
      };

      union max_align
      {
         char c;
         short s;
         int i;
         long l;
         float f;
         double d;
         long long ll;
         long double ld;
         void* vp;
         member_function_ptr mfp;
         function_ptr fp;
         member_ptr mp;
         has_one_T<char> hc;
         has_one_T<short> hs;
         has_one_T<int> hi;
         has_one_T<long> hl;
         has_one_T<float> hf;
         has_one_T<double> hd;
         has_one_T<long long> hll;
         has_one_T<long double> hld;
         has_one_T<void*> hvp;
         has_one_T<member_function_ptr> hmfp;
         has_one_T<function_ptr> hfp;
         has_one_T<member_ptr> hmp;
      };

      template <unsigned int target>
      union lower_alignment
      {
         static const bool found0 = false;

         typedef lower_alignment_helper< found0,target,char > T0;
         typename T0::type t0;
         static const bool found1 = T0::value;

         typedef lower_alignment_helper<found1,target,short > T1;
         typename T1::type t1;
         static const bool found2 = T1::value;

         typedef lower_alignment_helper< found2,target,int > T2;
         typename T2::type t2;
         static const bool found3 = T2::value;

         typedef lower_alignment_helper< found3,target,long > T3;
         typename T3::type t3;
         static const bool found4 = T3::value;

         typedef lower_alignment_helper< found4,target,long long > T4;
         typename T4::type t4;
         static const bool found5 = T4::value;

         typedef lower_alignment_helper< found5,target,float > T5;
         typename T5::type t5;
         static const bool found6 = T5::value;

         typedef lower_alignment_helper< found6,target,double > T6;
         typename T6::type t6;
         static const bool found7 = T6::value;

         typedef lower_alignment_helper< found7,target,long double > T7;
         typename T7::type t7;
         static const bool found8 = T7::value;

         typedef lower_alignment_helper< found8,target,void* > T8;
         typename T8::type t8;
         static const bool found9 = T8::value;

         typedef lower_alignment_helper< found9,target,function_ptr > T9;
         typename T9::type t9;
         static const bool found10 = T9::value;

         typedef lower_alignment_helper< found10,target,member_ptr > T10;
         typename T10::type t10;
         static const bool found11 = T10::value;

         typedef lower_alignment_helper< found11,target,member_function_ptr > T11;
         typename T11::type t11;
         static const bool found12 = T11::value;

         typedef lower_alignment_helper< found12,target,has_one_T< char > > T12;
         typename T12::type t12;
         static const bool found13 = T12::value;

         typedef lower_alignment_helper< found13,target,has_one_T< short > > T13;
         typename T13::type t13;
         static const bool found14 = T13::value;

         typedef lower_alignment_helper< found14,target,has_one_T< int > > T14;
         typename T14::type t14;
         static const bool found15 = T14::value;

         typedef lower_alignment_helper< found15,target,has_one_T< long > > T15;
         typename T15::type t15;
         static const bool found16 = T15::value;

         typedef lower_alignment_helper< found16,target,has_one_T< long long > > T16;
         typename T16::type t16;
         static const bool found17 = T16::value;

         typedef lower_alignment_helper< found17,target,has_one_T< float > > T17;
         typename T17::type t17;
         static const bool found18 = T17::value;

         typedef lower_alignment_helper< found18,target,has_one_T< double > > T18;
         typename T18::type t18;
         static const bool found19 = T18::value;

         typedef lower_alignment_helper< found19,target,has_one_T< long double > > T19;
         typename T19::type t19;
         static const bool found20 = T19::value;

         typedef lower_alignment_helper< found20,target,has_one_T< void* > > T20;
         typename T20::type t20;
         static const bool found21 = T20::value;

         typedef lower_alignment_helper< found21,target,has_one_T< function_ptr > > T21;
         typename T21::type t21;
         static const bool found22 = T21::value;

         typedef lower_alignment_helper< found22,target,has_one_T< member_ptr > > T22;
         typename T22::type t22;
         static const bool found23 = T22::value;

         typedef lower_alignment_helper< found23,target,has_one_T< member_function_ptr > > T23;
         typename T23::type t23;
         static const bool found24 = T23::value;
      };

      //struct is_aligned -------------------------------
      template<unsigned int TypeForAlign, unsigned int Align>
      struct is_aligned
      {
         static const bool value = (TypeForAlign >= Align) & (TypeForAlign % Align == 0);
      };
      //struct is_aligned -------------------------------

      template <unsigned int Align>
      class type_with_alignment_imp
      {
         typedef lower_alignment<Align> t1;
         typedef typename ::ttl::meta::if_c<
            is_aligned< ::ttl::meta::type_traits::alignment_of<t1>::value,Align >::value
            , t1
            , max_align
         >::type align_t;

         static const unsigned int found = ::ttl::meta::type_traits::alignment_of<align_t>::value;
      public:
         typedef align_t type;
      };

      template <std::size_t Align>
      class type_with_alignment
         : public type_with_alignment_imp<Align>
      {/**/};

      static const unsigned int alignment_of_max_align = ::ttl::meta::type_traits::alignment_of<max_align>::value;

      template <unsigned int size, unsigned int alignment>
      struct aligned_storage_imp
      {
         union data_t
         {
            char buf[size];

            typename ::ttl::meta::if_c<
              (alignment == (unsigned int)-1)
            , max_align
            , typename type_with_alignment<alignment>::type
            >::type align_;
         } data_;
         void* address() const { return const_cast<aligned_storage_imp*>(this); }
      };

      template< unsigned int alignment >
      struct aligned_storage_imp<0u,alignment>
      {
         void* address() const { return 0; }
      };

      template <unsigned int size_, unsigned int alignment_ = (unsigned int)-1>
      class aligned_storage_ : private aligned_storage_imp<size_, alignment_>
      {
      public:
         typedef aligned_storage_imp<size_, alignment_> type;
         static const unsigned int size = size_;
         static const unsigned int alignment = ( alignment_ == (unsigned int)-1 ? alignment_of_max_align : alignment_ );
      public:
         aligned_storage_(const aligned_storage_&);
         aligned_storage_& operator=(const aligned_storage_&);
      public:
         aligned_storage_()
         {
         }
         ~aligned_storage_()
         {
         }
      public:
         void* address()
         {
            return static_cast<type*>(this)->address();
         }
         const void* address() const
         {
            return static_cast<const type*>(this)->address();
         }
      };
   }// namespace ttl_ns_impl

   template<unsigned int Len,unsigned int Align = (unsigned int)-1>
   struct aligned_storage : public ttl_ns_impl::aligned_storage_<Len,Align>
   {/**/};
}
}
}

#endif// __TTL_ALIGNED_MEMORY_INCLUDED__
