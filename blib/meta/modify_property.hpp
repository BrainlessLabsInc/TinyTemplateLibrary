#ifndef __TTL_MODIFY_PROPERTY_INCLUDED__
#define __TTL_MODIFY_PROPERTY_INCLUDED__

namespace blib{namespace meta{namespace type_traits{

   //add qualifiers
   template<typename T>
   struct add_const
   {
      typedef const T type;
   };

   template<typename T>
   struct add_const<const T>
   {
      typedef T type;
   };

   template<typename T> 
   struct add_volatile
   {
      typedef volatile T type; 
   };

   template<typename T> 
   struct add_volatile<volatile T>
   {
      typedef T type; 
   };

   template<typename T>
   struct add_cv 
   {
      typedef typename ::blib::meta::type_traits::add_volatile<
         typename ::blib::meta::type_traits::add_const<T>::type >::type type;
   };

   //remove qualifiers
   template<typename T>
   struct remove_const
   {
      typedef T type;
   };

   template<typename T>
   struct remove_const<const T>
   {
      typedef T type;
   };

   template<typename T>
   struct remove_volatile
   {
      typedef T type;
   };

   template<typename T>
   struct remove_volatile<volatile T>
   {
      typedef T type;
   };

   template<typename T>
   struct remove_cv
   {
      typedef typename ::blib::meta::type_traits::remove_volatile<
         typename ::blib::meta::type_traits::remove_const<T>::type>::type type;
   };

   //remove_pointer
   template<typename T>
   struct remove_pointer
   {
      typedef T type;
   };
   template<typename T>
   struct remove_pointer<T*>
   {
      typedef T type;
   };
   template<typename T>
   struct remove_pointer<T* const>
   {
      typedef T type;
   };
   template<typename T >
   struct remove_pointer<T* volatile>
   {
      typedef T type;
   };
   template<typename T >
   struct remove_pointer<T* const volatile>
   {
      typedef T type;
   };

   //remove_reference
   template<typename T>
   struct remove_reference
   {
      typedef T type;
   };
   template<typename T>
   struct remove_reference<T&> 
   {
      typedef T type;
   };
   //add_pointer
   template<typename T>
   struct add_pointer
   {
      typedef typename ::blib::meta::type_traits::remove_reference<T>::type* type;
   };

   //add_reference
   template<typename T>
   struct add_reference
   {
      typedef T& type;
   };

   template<typename T>
   struct add_reference<T&>
   {
      typedef T type;
   };

   template<>
   struct add_reference<void>
   {
      typedef void type;
   };
   template<>
   struct add_reference<const void>
   {
      typedef const void type;
   };
   template<>
   struct add_reference<volatile void>
   {
      typedef const void type;
   };
   template<>
   struct add_reference<const volatile void>
   {
      typedef const volatile void type;
   };
}
}
}

#endif// __TTL_MODIFY_PROPERTY_INCLUDED__

