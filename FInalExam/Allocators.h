#pragma once
#include <inttypes.h>
#include <malloc.h>

#include <stdio.h>

#include "FixedSizeAllocator.h"
#include "MemorySystem.h"

void* __cdecl Mymalloc(size_t i_size);

void __cdecl Myfree(void* i_ptr);

void* __CRTDECL operator new(size_t i_size);

void __CRTDECL operator delete(void* i_ptr) noexcept;

void* __CRTDECL operator new[](size_t i_size);

void __CRTDECL operator delete[](void* i_ptr) noexcept;

