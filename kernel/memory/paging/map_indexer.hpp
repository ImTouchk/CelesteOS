#ifndef PAGE_MAP_INDEXER_H
#define PAGE_MAP_INDEXER_H

#include "../../types.hpp"

namespace Memory {
    struct pageMapIndexer {
        pageMapIndexer(usize _virtualAddress);

        usize pi;
        usize pdi;
        usize pti;
        usize pdpi;
        usize virtualAddress;
    };
}

#endif // PAGE_MAP_INDEXER_H
