#ifndef BITMAP_H
#define BITMAP_H

#include "../types.hpp"

namespace Containers {
    struct bitmap {
        bitmap(byte* buffer, usize _size);
        bitmap();

        bool set(usize index, bool value);
        bool get(usize index) const;

        usize size;
        byte* pBuffer;
    };
}

#endif // BITMAP_H
