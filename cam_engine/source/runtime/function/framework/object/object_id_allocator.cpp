#include "runtime/core/base/macro.h"
#include "object_id_allocator.h"

namespace ImCAM
{
    std::atomic<GObjectID> ObjectIDAllocator::m_next_id {0};

    GObjectID ObjectIDAllocator::alloc()
    {
        std::atomic<GObjectID> new_object_ret = m_next_id.load();
        m_next_id++;
        if (m_next_id >= k_invalid_gobject_id)
        {
            LOG_FATAL("gobject id overflow");
        }

        return new_object_ret;
    }

} 