#include "../../types.hpp"
#include "map_indexer.hpp"

namespace Memory {
    pageMapIndexer::pageMapIndexer(usize _virtualAddress)
    {
        virtualAddress = -_virtualAddress;

        _virtualAddress >>= 12;
        pi   = _virtualAddress & 0x1ff;
        _virtualAddress >>= 9;
        pti  = _virtualAddress & 0x1ff;
        _virtualAddress >>= 9;
        pdi  = _virtualAddress & 0x1ff;
        _virtualAddress >>= 9;
        pdpi = _virtualAddress & 0x1ff;
    }
}