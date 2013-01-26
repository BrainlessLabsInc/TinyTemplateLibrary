#ifndef __TTL_TYPE_TRAIT_INCLUDED__
#define __TTL_TYPE_TRAIT_INCLUDED__

#ifndef __STD_CSTDDEF_INCLUDED__
#define __STD_CSTDDEF_INCLUDED__
#include <cstddef>
#endif
#ifndef __TTL_ENABLE_IF_INCLUDED__
#include <ttl/meta/enable_if.hpp>
#endif
#ifndef __TTL_VALUE_TO_TYPE_INCLUDED__
#include <ttl/meta/value_to_type.hpp>
#endif
#ifndef __TTL_TYPE_RELATIONSHIP_INCLUDED__
#include <ttl/meta/type_relationship.hpp>
#endif
#ifndef __TTL_PROPERTY_QUERIES_INCLUDED__
#include <ttl/meta/property_queries.hpp>
#endif
#ifndef __TTL_ALIGNED_MEMORY_INCLUDED__
#include <ttl/mem/aligned_memory.hpp>
#endif

namespace std{
   using namespace ::ttl::meta::type_traits;
}

#endif// __TTL_TYPE_TRAIT_INCLUDED__
