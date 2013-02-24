#ifndef __BLIB_TYPE_TRAIT_INCLUDED__
#define __BLIB_TYPE_TRAIT_INCLUDED__

#ifndef __STD_CSTDDEF_INCLUDED__
#define __STD_CSTDDEF_INCLUDED__
#include <cstddef>
#endif
#ifndef __BLIB_ENABLE_IF_INCLUDED__
#include <blib/meta/enable_if.hpp>
#endif
#ifndef __BLIB_VALUE_TO_TYPE_INCLUDED__
#include <blib/meta/value_to_type.hpp>
#endif
#ifndef __BLIB_TYPE_RELATIONSHIP_INCLUDED__
#include <blib/meta/type_relationship.hpp>
#endif
#ifndef __BLIB_PROPERTY_QUERIES_INCLUDED__
#include <blib/meta/property_queries.hpp>
#endif
#ifndef __BLIB_ALIGNED_MEMORY_INCLUDED__
#include <blib/mem/aligned_memory.hpp>
#endif
#ifndef __BLIB_MODIFY_PROPERTY_INCLUDED__
#include <blib/meta/modify_property.hpp>
#endif

namespace std{
   using namespace ::blib::meta::type_traits;
}

#endif// __BLIB_TYPE_TRAIT_INCLUDED__
