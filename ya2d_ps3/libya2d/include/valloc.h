/*
 * Helper for use with the PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed as 'free to use and modify as long as credited appropriately'
 *
 * valloc.c - Standard C library VRAM allocation routines.
 * 
 * modified for PS3 by xerpi
 *
 * Copyright (c) 2006 Alexander Berl <a.berl@gmx.de>
 *
 */
#ifndef _VALLOC_H
#define _VALLOC_H

#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


void init_valloc(u64 base_address, u64 size);

size_t vgetMemorySize(unsigned int width, unsigned int height, unsigned int format);

// Return a pointer relative to VRAM Base address useable by GU
void* vrelptr( void* ptr );
// Return an absolute pointer useable by CPU
void* vabsptr( void* ptr );

// Returns an absolute pointer useable by CPU
void* valloc( size_t size );
void  vfree( void* ptr );

size_t vmemavail();
size_t vlargestblock();


#ifdef __cplusplus
}
#endif

#endif	// _VALLOC_H
