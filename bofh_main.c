/************************************************************************
 * Copyright (C) 2013, Felix Bytow <felix.bytow@googlemail.com>         *
 *                                                                      *
 * This file is part of bofh.                                           *
 *                                                                      *
 * bofh is free software: you can redistribute it and/or modify         *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * bofh is distributed in the hope that it will be useful,              *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with bofh.  If not, see <http://www.gnu.org/licenses/>.        *
 ************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/random.h>

/* the filename under /proc/ */
#define BOFH_FILENAME "excuse"

/* the BOFH exuses */
#include "bofh_excuses.h"

struct proc_dir_entry *bofh_proc_file;

/* here the file /proc/{BOFH_FILENAME} is filled with content */
int bofh_content(
	char *buffer,
	char **buffer_location,
	off_t offset,
	int buffer_length,
	int *eof,
	void *data
)
{
	char const *excuse;
	size_t len;

	/* the index of the chosen excuse... gets a random value */
	unsigned int index;

	int ret;

	get_random_bytes(&index, sizeof(unsigned int));
	index %= max_excuse;

	excuse = excuses[index];
	len = strlen(excuse);

	if (offset > 0)
		ret = 0;
	else {
		pr_alert("kmod_bofh: Return BOFH excuse #%d: %s\n",
			index, excuse);
		ret = snprintf(buffer, buffer_length, "%s\n", excuse + offset);
	}

	*eof = 1;
	return ret;
}

static int __init bofh_init(void)
{
	bofh_proc_file = create_proc_entry(BOFH_FILENAME, 0644, NULL);
	if (unlikely(bofh_proc_file == NULL)) {
		pr_alert("Error: kmod_bofh could not initialize /proc/%s\n",
			BOFH_FILENAME);
		return -ENOMEM;
	}

	bofh_proc_file->read_proc	= &bofh_content;
	bofh_proc_file->mode		= S_IFREG | S_IRUGO;
	bofh_proc_file->uid	        = GLOBAL_ROOT_UID;
	bofh_proc_file->gid	        = GLOBAL_ROOT_GID;
	bofh_proc_file->size		= 0;

	pr_debug("BOFH module inititialized!\n");
	return 0;
}

static void __exit bofh_exit(void)
{
	remove_proc_entry(BOFH_FILENAME, NULL);
	pr_debug("BOFH module uninitialized!\n");
}

module_init(bofh_init);
module_exit(bofh_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felix Bytow");
MODULE_DESCRIPTION("A BOFH excuse module");

