#ifndef PAGE_FRAME_ALLOCATOR_H
#define PAGE_FRAME_ALLOCATOR_H

#include "../../containers/bitmap.hpp"
#include "../../terminal.hpp"
#include "../../types.hpp"
#include "../../boot.hpp"

namespace Memory {
    class pageFrameAllocator {
    public:
        pageFrameAllocator(Boot::memoryData& memoryData);
        pageFrameAllocator();

        void* request();

        void lock(void* address);
        void free(void* address);
        void reserve(void* address);
        void unreserve(void* address);

        void lock(void* start, usize count);
        void free(void* start, usize count);
        void reserve(void* start, usize count);
        void unreserve(void* start, usize count);

        usize freeMemory() const;
        usize usedMemory() const;
        usize totalMemory() const;
        usize reservedMemory() const;
    private:
        usize m_FreeMemory;
        usize m_UsedMemory;
        usize m_TotalMemory;
        usize m_ReservedMemory;
        Containers::bitmap m_Bitmap;
    };
}

#endif // PAGE_FRAME_ALLOCATOR_H
