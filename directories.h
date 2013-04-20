/*
	libpe - the PE library

	Copyright (C) 2010 - 2013 libpe authors

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LIBPE_DIRECTORIES_H
#define LIBPE_DIRECTORIES_H

#include <inttypes.h>
#include "dir_resources.h"
#include "dir_security.h"

// directory Entries
typedef enum {
	IMAGE_DIRECTORY_ENTRY_EXPORT			= 0, // Export Table
	IMAGE_DIRECTORY_ENTRY_IMPORT			= 1, // Import Table
	IMAGE_DIRECTORY_ENTRY_RESOURCE			= 2, // Resource Table
	IMAGE_DIRECTORY_ENTRY_EXCEPTION			= 3, // Exception Table
	IMAGE_DIRECTORY_ENTRY_SECURITY			= 4, // Certificate Table
	IMAGE_DIRECTORY_ENTRY_BASERELOC			= 5, // Base Relocation Table
	IMAGE_DIRECTORY_ENTRY_DEBUG				= 6, // Debug
	//IMAGE_DIRECTORY_ENTRY_COPYRIGHT			= 7, // (X86 usage)
	IMAGE_DIRECTORY_ENTRY_ARCHITECTURE		= 7, // Architecture
	IMAGE_DIRECTORY_ENTRY_GLOBALPTR			= 8, // Global Ptr
	IMAGE_DIRECTORY_ENTRY_TLS				= 9, // TLS Table
	IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG		= 10, // Load Config Table
	IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT		= 11, // Bound Import
	IMAGE_DIRECTORY_ENTRY_IAT				= 12, // IAT
	IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT		= 13, // Delay Import Descriptor
	IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR	= 14, // CLR Runtime Header
	IMAGE_DIRECTORY_RESERVED				= 15  // Reserved, must be zero
} ImageDirectoryEntry;

typedef struct {
	uint32_t Characteristics;
	uint32_t TimeDateStamp;
	uint16_t MajorVersion;
	uint16_t MinorVersion;
	uint32_t Name;
	uint32_t Base;
	uint32_t NumberOfFunctions;
	uint32_t NumberOfNames;
	uint32_t AddressOfFunctions;
	uint32_t AddressOfNames;
	uint32_t AddressOfNameOrdinals;
} IMAGE_EXPORT_DIRECTORY;

typedef struct {
	union {
		uint32_t Characteristics; // 0 for terminating null import descriptor
		uint32_t OriginalFirstThunk; // RVA to original unbound IAT
	} u1;
	uint32_t TimeDateStamp;
	uint32_t ForwarderChain; // -1 if no forwarders
	uint32_t Name;
	// RVA to IAT (if bound this IAT has actual addresses)
	uint32_t FirstThunk;
} IMAGE_IMPORT_DESCRIPTOR;

// import name entry
typedef struct {
	uint16_t Hint;
	uint8_t Name;
} IMAGE_IMPORT_BY_NAME;

typedef struct {
	uint32_t StartAddressOfRawData;
	uint32_t EndAddressOfRawData;
	uint32_t AddressOfIndex;
	uint32_t AddressOfCallBacks; // PIMAGE_TLS_CALLBACK
	uint32_t SizeOfZeroFill;
	uint32_t Characteristics; // reserved for future use
} IMAGE_TLS_DIRECTORY32;

typedef struct {
	uint64_t StartAddressOfRawData;
	uint64_t EndAddressOfRawData;
	uint64_t AddressOfIndex;
	uint64_t AddressOfCallBacks;
	uint32_t SizeOfZeroFill;
	uint32_t Characteristics;
} IMAGE_TLS_DIRECTORY64;

typedef struct {
	uint32_t VirtualAddress;
	uint32_t Size;
} IMAGE_DATA_DIRECTORY;

const char *pe_directory_name(uint32_t dir_index);

#endif
