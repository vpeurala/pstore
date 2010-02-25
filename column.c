#include "pstore/column.h"

#include "pstore/disk-format.h"
#include "pstore/read-write.h"
#include "pstore/buffer.h"
#include "pstore/extent.h"
#include "pstore/core.h"
#include "pstore/die.h"

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

struct pstore_column *pstore_column__new(const char *name, uint64_t column_id, uint8_t type)
{
	struct pstore_column *self = calloc(sizeof *self, 1);

	if (!self)
		die("out of memory");

	self->name	= strdup(name);
	self->column_id	= column_id;
	self->type	= type;

	return self;
}

void pstore_column__delete(struct pstore_column *self)
{
	free(self->name);
	free(self);
}

struct pstore_column *pstore_column__read(int fd)
{
	struct pstore_file_column f_column;
	struct pstore_column *self;

	read_or_die(fd, &f_column, sizeof(f_column));

	self = pstore_column__new(f_column.name, f_column.column_id, f_column.type);

	self->f_offset = f_column.f_offset;

	return self;
}

void pstore_column__write(struct pstore_column *self, int fd)
{
	struct pstore_file_column f_column;

	f_column = (struct pstore_file_column) {
		.column_id	= self->column_id,
		.type		= self->type,
		.f_offset	= self->f_offset,
	};
	strncpy(f_column.name, self->name, PSTORE_COLUMN_NAME_LEN);

	write_or_die(fd, &f_column, sizeof(f_column));
}

void pstore_column__import_values(struct pstore_column *self, int fd, struct pstore_iterator *iter, void *private)
{
	struct pstore_extent *extent;
	struct pstore_value value;

	extent = pstore_extent__new();
	extent->parent = self;

	pstore_extent__prepare_write(extent, fd);

	iter->begin(private);

	while (iter->next(self, private, &value))
		pstore_extent__write_value(extent, &value, fd);

	iter->end(private);

	pstore_extent__finish_write(extent, fd);
}
