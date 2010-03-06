#ifndef PSTORE_EXTENT_H
#define PSTORE_EXTENT_H

#include "pstore/disk-format.h"
#include "pstore/compat.h"

#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>

struct pstore_column;
struct pstore_value;
struct mmap_window;
struct buffer;

struct pstore_extent {
	struct pstore_column	*parent;
	off_t			next_extent;
	uint64_t		size;

	/* read */
	struct mmap_window	*mmap;
	char			*start;

	/* write */
	struct buffer		*buffer;
	uint64_t		start_off;
	uint64_t		end_off;
};

struct pstore_extent *pstore_extent__new(struct pstore_column *parent);
void pstore_extent__delete(struct pstore_extent *self);
struct pstore_extent *pstore_extent__read(struct pstore_column *column, off_t offset, int fd);
void *pstore_extent__next_value(struct pstore_extent *self);
void pstore_extent__prepare_write(struct pstore_extent *self, int fd, uint64_t max_extent_len);
void pstore_extent__flush_write(struct pstore_extent *self, int fd);
void pstore_extent__finish_write(struct pstore_extent *self, off_t next_extent, int fd);
void pstore_extent__write_value(struct pstore_extent *self, struct pstore_value *value, int fd);
bool pstore_extent__has_room(struct pstore_extent *self, struct pstore_value *value);

static inline bool pstore_extent__is_last(struct pstore_extent *self)
{
	return self->next_extent == PSTORE_LAST_EXTENT;
}

#endif /* PSTORE_EXTENT_H */
