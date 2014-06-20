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
#include <linux/seq_file.h>
#include <linux/random.h>
#include <linux/version.h>

/* the filename under /proc/ */
#define BOFH_FILENAME "excuse"

/* the BOFH exuses */
#include "bofh_excuses.h"

static struct proc_dir_entry *bofh_proc_file;

/* here the file /proc/{BOFH_FILENAME} is filled with content */
static int bofh_content(struct seq_file *m, void *v)
{
    char const *excuse;
    size_t len;

    /* the index of the chosen excuse... gets a random value */
    unsigned int index;

    get_random_bytes(&index, sizeof(unsigned int));
    index %= max_excuse;

    excuse = excuses[index];
    len = strlen(excuse);

    pr_alert("kmod_bofh: Return BOFH excuse #%d: %s\n", index, excuse);
    seq_printf(m, "%s\n", excuse);

    return 0;
}

static int bofh_open(struct inode * inode, struct file * file)
{
    return single_open(file, &bofh_content, NULL);
}

static int __init bofh_init(void)
{
    static struct file_operations ops = {
        .owner   = THIS_MODULE,
        .open    = &bofh_open,
        .read    = &seq_read,
        .llseek  = &seq_lseek,
        .release = &single_release,
    };

    bofh_proc_file = proc_create(BOFH_FILENAME, 0644, NULL, &ops);

    if (unlikely(bofh_proc_file == NULL)) {
        pr_alert("Error: kmod_bofh could not initialize /proc/%s\n",
            BOFH_FILENAME);
        return -ENOMEM;
    }

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 10, 0)
    bofh_proc_file->uid = 0;
    bofh_proc_file->gid = 0;
    bofh_proc_file->size = 0;
#else
    proc_set_user(bofh_proc_file, GLOBAL_ROOT_UID, GLOBAL_ROOT_GID);
    proc_set_size(bofh_proc_file, 0);
#endif

    pr_debug("BOFH module inititialized!\n");
    return 0;
}

static void __exit bofh_exit(void)
{
    remove_proc_entry(BOFH_FILENAME, bofh_proc_file);
    pr_debug("BOFH module uninitialized!\n");
}

module_init(bofh_init);
module_exit(bofh_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felix Bytow");
MODULE_DESCRIPTION("A BOFH excuse module");

