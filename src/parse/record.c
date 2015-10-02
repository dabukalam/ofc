#include "parse.h"


static parse_record_t* parse_record(
	const sparse_t* src, const char* ptr,
	parse_debug_t* debug,
	unsigned* len)
{
	unsigned i = 0;
	if (ptr[i++] != '/')
		return 0;

	parse_record_t* record
		= (parse_record_t*)malloc(
			sizeof(parse_record_t));
	if (!record) return NULL;

	unsigned dpos = parse_debug_position(debug);

	unsigned l = parse_name(
		src, &ptr[i], debug,
		&record->structure);
	if (l == 0)
	{
		free(record);
		return NULL;
	}
	i += l;

	if (ptr[i++] != '/')
	{
		free(record);
		parse_debug_rewind(debug, dpos);
		return NULL;
	}

	l = parse_name(
		src, &ptr[i], debug,
		&record->name);
	if (l == 0)
	{
		free(record);
		parse_debug_rewind(debug, dpos);
		return NULL;
	}
	i += l;

	if (len) *len = i;
	return record;
}

static void parse_record_delete(
	parse_record_t* record)
{
	if (!record)
		return;

	free(record);
}

static bool parse_record_print(
	int fd, const parse_record_t* record)
{
	if (!record)
		return false;

	return (dprintf_bool(fd, "/")
		&& str_ref_print(fd, record->structure)
		&& dprintf_bool(fd, "/ ")
		&& str_ref_print(fd, record->name));
}



parse_record_list_t* parse_record_list(
	const sparse_t* src, const char* ptr,
	parse_debug_t* debug,
	unsigned* len)
{
	parse_record_list_t* list
		= (parse_record_list_t*)malloc(
			sizeof(parse_record_list_t));
	if (!list) return NULL;

	list->count = 0;
	list->record = NULL;

	unsigned i = parse_list(
		src, ptr, debug, ',',
		&list->count, (void***)&list->record,
		(void*)parse_record,
		(void*)parse_record_delete);
	if (i == 0)
	{
		free(list);
		return NULL;
	}

	if (len) *len = i;
	return list;
}

void parse_record_list_delete(
	parse_record_list_t* list)
{
	if (!list)
		return;

	parse_list_delete(
		list->count, (void**)list->record,
		(void*)parse_record_delete);
	free(list);
}

bool parse_record_list_print(
	int fd, const parse_record_list_t* list)
{
	if (!list)
		return false;

	return parse_list_print(fd,
		list->count, (const void**)list->record,
		(void*)parse_record_print);
}