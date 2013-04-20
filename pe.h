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

#ifndef LIBPE_H
#define LIBPE_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "hdr_dos.h"
#include "hdr_coff.h"
#include "hdr_optional.h"
#include "directories.h"
#include "sections.h"

#define LIBPE_PTR_ADD(p, o)						((void *)((char *)p + o))
#define LIBPE_SIZEOF_ARRAY(array)				(sizeof(array) / sizeof(array[0]))
#define LIBPE_SIZEOF_MEMBER(type, member)		sizeof(((type *)0)->member)
#define LIBPE_IS_PAST_THE_END(ctx, ptr, type)	\
	((uintptr_t)((ptr) + sizeof(type)) > (ctx)->map_end)

#define MAGIC_MZ 0x5a4d // Belongs to the DOS header
#define MAX_DIRECTORIES 16
#define MAX_SECTIONS 96

#define SIGNATURE_NE 0x454E // NE\0\0 in little-endian
#define SIGNATURE_PE 0x4550 // PE\0\0 in little-endian

#pragma pack(push, 1)

typedef struct {
	// DOS header
	IMAGE_DOS_HEADER *dos_hdr;
	// Signature
	uint32_t signature;
	// COFF header
	IMAGE_COFF_HEADER *coff_hdr;
	// Optional header
	IMAGE_OPTIONAL_HEADER *optional_hdr;
	// Directories
	uint32_t num_directories;
	void *directories_ptr;
	IMAGE_DATA_DIRECTORY **directories; // array up to MAX_DIRECTORIES
	// Sections
	uint16_t num_sections;
	void *sections_ptr;
	IMAGE_SECTION_HEADER **sections; // array up to MAX_SECTIONS

#if 0
	uint16_t e_lfanew;
	uint16_t architecture;
	uint64_t entrypoint;
	uint64_t imagebase;
	uint64_t size;

	uint16_t num_rsrc_entries;
	uint16_t addr_rsrc_sec;
	uint16_t addr_rsrc_dir;

	// pointers (will be freed if needed)
	//IMAGE_TLS_DIRECTORY32 *tls_ptr;
	IMAGE_RESOURCE_DIRECTORY *rsrc_ptr;
	IMAGE_RESOURCE_DIRECTORY_ENTRY **rsrc_entries_ptr;
#endif
} PE_FILE;

#pragma pack(pop)

typedef struct {
	char *path;
	void *map_addr;
	off_t map_size;
	uintptr_t map_end;
	PE_FILE pe;
} pe_ctx_t;

typedef enum {
	LIBPE_E_OK = 0,
	LIBPE_E_ALLOCATION_FAILURE = -15,
	LIBPE_E_OPEN_FAILED,
	LIBPE_E_FSTAT_FAILED,
	LIBPE_E_NOT_A_FILE,
	LIBPE_E_NOT_A_PE_FILE,
	LIBPE_E_INVALID_LFANEW,
	LIBPE_E_MISSING_COFF_HEADER,
	LIBPE_E_MISSING_OPTIONAL_HEADER,
	LIBPE_E_INVALID_SIGNATURE,
	LIBPE_E_UNSUPPORTED_IMAGE,
	LIBPE_E_MMAP_FAILED,
	LIBPE_E_MUNMAP_FAILED,
	LIBPE_E_CLOSE_FAILED,
	LIBPE_E_TOO_MANY_DIRECTORIES,
	LIBPE_E_TOO_MANY_SECTIONS,
} pe_err_e;

// General functions
const char *pe_error_msg(pe_err_e error);
void pe_error_print(FILE *stream, pe_err_e error);
pe_err_e pe_load(pe_ctx_t *ctx, const char *path);
pe_err_e pe_unload(pe_ctx_t *ctx);
pe_err_e pe_parse(pe_ctx_t *ctx);
bool pe_is_pe(pe_ctx_t *ctx);
bool pe_is_dll(pe_ctx_t *ctx);
uint64_t pe_filesize(pe_ctx_t *ctx);
IMAGE_SECTION_HEADER *pe_rva2section(pe_ctx_t *ctx, uint64_t rva);
uint64_t pe_rva2ofs(pe_ctx_t *ctx, uint64_t rva);
uint32_t pe_ofs2rva(pe_ctx_t *ctx, uint32_t ofs);

// Header functions
IMAGE_DOS_HEADER *pe_dos(pe_ctx_t *ctx);
IMAGE_COFF_HEADER *pe_coff(pe_ctx_t *ctx);
IMAGE_OPTIONAL_HEADER *pe_optional(pe_ctx_t *ctx);
uint32_t pe_directories_count(pe_ctx_t *ctx);
IMAGE_DATA_DIRECTORY **pe_directories(pe_ctx_t *ctx);
IMAGE_DATA_DIRECTORY *pe_directory_by_entry(pe_ctx_t *ctx, ImageDirectoryEntry entry);
uint32_t pe_sections_count(pe_ctx_t *ctx);
IMAGE_SECTION_HEADER **pe_sections(pe_ctx_t *ctx);
IMAGE_SECTION_HEADER *pe_section_by_name(pe_ctx_t *ctx, const char *section_name);

const char *pe_machine_type_name(MachineType type);
const char *pe_image_characteristic_name(ImageCharacteristics characteristic);
const char *pe_windows_subsystem_name(WindowsSubsystem subsystem);
const char *pe_directory_name(ImageDirectoryEntry entry);

#endif
