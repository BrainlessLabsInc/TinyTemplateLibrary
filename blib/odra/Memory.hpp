#ifndef __BLIB_MEMORY_INCLUDED__
#define __BLIB_MEMORY_INCLUDED__

#ifndef __STD_STRING_INCLUDED__
#define __STD_STRING_INCLUDED__
#include <string>
#endif
#ifndef BOOST_TYPE_TRAITS_HPP
#include <boost/type_traits.hpp>
#endif
#ifndef BOOST_POOL_ALLOC_HPP
#include <boost/pool/pool_alloc.hpp>
#endif

namespace blib{namespace odra{namespace memory{

   struct ExeMemAllocator
   {
      typedef boost::uint8_t RetType;
      typedef std::size_t size_type;
      typedef std::ptrdiff_t difference_type;
      static RetType* malloc(const size_type)
      {
      }
      static void free(RetType* const)
      {
      }
   };// struct ExeMemAllocator

   typedef std::basic_string<ExeMemAllocator::RetType
      ,std::char_traits<ExeMemAllocator::RetType>
      ,boost::pool_allocator<ExeMemAllocator::RetType, ExeMemAllocator> > ExeBuffer;

}// namespace utils
}// namespace odra
}// namespace blib

#endif// __BLIB_MEMORY_INCLUDED__