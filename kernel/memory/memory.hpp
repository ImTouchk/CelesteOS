#ifndef MEMORY_H
#define MEMORY_H

#define x86_64 1

#include "paging/map_indexer.hpp"
#include "paging/frame_allocator.hpp"
#include "paging/table_manager.hpp"
#include "utils.hpp"

#ifdef x86_64
#   include "x86_64/gdt.hpp"
#endif

#endif // MEMORY_H
