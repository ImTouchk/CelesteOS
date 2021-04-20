#include "../../types.hpp"
#include "frame_allocator.hpp"

namespace Memory {
    struct descriptor {
        u32 type;
        u32 padding;
        void* pPhysicalAddress;
        void* pVirtualAddress;
        u64 pageCount;
        u64 attribute;
    };

    constexpr int EfiConventionalMemory = 7;
}

namespace Memory {
    pageFrameAllocator::pageFrameAllocator(Boot::memoryData& memoryData)
    {
        auto GetMemorySize = [&]() {
            descriptor* map     = static_cast<descriptor*>(memoryData.pMap);
            usize       entries = memoryData.size / memoryData.descriptorSize;
            usize       size    = 0;

            for(usize i = 0; i < entries; i++) {
                descriptor* current = (descriptor*)(
                    (usize)map + (i * memoryData.descriptorSize)
                );
                size += current->pageCount  * 4096;
            }

            return size;
        };

        auto StoreBitmap = [&](usize memorySize) {
            descriptor* map        = static_cast<descriptor*>(memoryData.pMap);
            descriptor* best       = map;
            usize       bitmapSize = memorySize / 4096 / 8 + 1;
            usize       entries    = memoryData.size / memoryData.descriptorSize;

            for(usize i = 0; i < entries; i++) {
                descriptor* current = (descriptor*)(
                    (usize)map + (i * memoryData.descriptorSize)
                );

                if(current->type != EfiConventionalMemory)
                    continue;

                if(current->pageCount > best->pageCount)
                    best = current;
            }

            m_Bitmap = Containers::bitmap((byte*)best->pPhysicalAddress, bitmapSize);
            lock(&m_Bitmap, m_Bitmap.size / 4096 + 1);
        };

        auto ReserveMemory = [&]() {
            descriptor* map     = static_cast<descriptor*>(memoryData.pMap);
            usize       entries = memoryData.size / memoryData.descriptorSize;

            for(usize i = 0; i < entries; i++) {
                descriptor* current = (descriptor*)(
                    (usize)map + (i * memoryData.descriptorSize)
                );

                if(current->type == EfiConventionalMemory)
                    continue;

                reserve(current->pPhysicalAddress, current->pageCount);
            }
        };

        m_TotalMemory = GetMemorySize();
        StoreBitmap(m_TotalMemory);
        ReserveMemory();
    }

    pageFrameAllocator::pageFrameAllocator()
    {
        
    }

    usize LAST_INDEX = 0;
    void* pageFrameAllocator::request()
    {
        for(; LAST_INDEX < m_Bitmap.size * 8; LAST_INDEX++) {
            if(m_Bitmap.get(LAST_INDEX) == true)
                continue;

            void* address = (void*)(LAST_INDEX * 4096);
            lock(address);
            return address;
        }

        /* TODO: Page frame swap */
        return nullptr;
    }

    void pageFrameAllocator::free(void* address)
    {
        const usize index = (uptr)address / 4096;
        if(m_Bitmap.get(index) == false)
            return;
        
        if(m_Bitmap.set(index, false)) {
            m_FreeMemory += 4096;
            m_UsedMemory -= 4096;
            if(LAST_INDEX > index) {
                LAST_INDEX = index;
            }
        }
    }

    void pageFrameAllocator::free(void* start, usize count)
    {
        for(usize i =  0; i < count; i++) {
            free((void*)((uptr)start + (i * 4096)));
        }
    }

    void pageFrameAllocator::lock(void* address)
    {
        const usize index = (uptr)address / 4096;
        if(m_Bitmap.get(index) == true)
            return;
        
        if(m_Bitmap.set(index, true)) {
            m_FreeMemory -= 4096;
            m_UsedMemory += 4096;
        }
    }

    void pageFrameAllocator::lock(void* start, usize count)
    {
        for(usize i = 0; i < count; i++) {
            lock((void*)((uptr)start + (i * 4096)));
        }
    }

    void pageFrameAllocator::unreserve(void* address)
    {
        const usize index = (uptr)address / 4096;
        if(m_Bitmap.get(index) == false)
            return;
        
        if(m_Bitmap.set(index, false)) {
            m_FreeMemory     += 4096;
            m_ReservedMemory -= 4096;
            if(LAST_INDEX > index) {
                LAST_INDEX = index;
            }
        }
    }

    void pageFrameAllocator::unreserve(void* start, usize count)
    {
        for(usize i = 0;  i < count; i++) {
            unreserve((void*)((uptr)start + (i * 4096)));
        }
    }

    void pageFrameAllocator::reserve(void* address)
    {
        const usize index = (uptr)address / 4096;
        if(m_Bitmap.get(index) == true)
            return;

        if(m_Bitmap.set(index, true)) {
            m_FreeMemory     -= 4096;
            m_ReservedMemory += 4096;
        }
    }

    void pageFrameAllocator::reserve(void* start, usize count)
    {
        for(usize i = 0; i < count; i++) {
            reserve((void*)((uptr)start + (i * 4096)));
        }
    }

    usize pageFrameAllocator::freeMemory() const
    {
        return m_FreeMemory;
    }

    usize pageFrameAllocator::usedMemory() const
    {
        return m_UsedMemory;
    }

    usize pageFrameAllocator::totalMemory() const
    {
        return m_TotalMemory;
    }

    usize pageFrameAllocator::reservedMemory() const
    {
        return m_ReservedMemory;
    }
}
