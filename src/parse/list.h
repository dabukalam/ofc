#ifndef __parse_list_h__
#define __parse_list_h__

#include "../string.h"

unsigned parse_list(
	const sparse_t* sparse, const char* ptr,
	parse_debug_t* debug,
	char seperator, unsigned* elem_count, void*** elem,
	void* (*elem_parse)(const sparse_t*, const char*, parse_debug_t*, unsigned*),
	void (*elem_delete)(void*));

unsigned parse_list_seperator_optional(
	const sparse_t* sparse, const char* ptr,
	parse_debug_t* debug,
	char seperator, unsigned* elem_count, void*** elem,
	void* (*elem_parse)(const sparse_t*, const char*, parse_debug_t*, unsigned*),
	void (*elem_delete)(void*));

bool parse_list_copy(
	unsigned* dst_count, void*** dst,
	unsigned  src_count, const void** src,
	void* (*elem_copy)(const void*),
	void (*elem_delete)(void*));

void parse_list_delete(
	unsigned elem_count, void** elem,
	void (*elem_delete)(void*));

bool parse_list_print(
	string_t* tree_output,
	unsigned elem_count, const void** elem,
	bool (*elem_print)(string_t*, const void*));

#endif
