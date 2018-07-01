#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/kallsyms.h>
#include <linux/dirent.h>
#include <linux/syscalls.h>


void enable_write_protect(void) {
    write_cr0(read_cr0() | 0x10000);
}

void disable_write_protect(void) {
    write_cr0(read_cr0() & (~0x10000));
}

void** get_syscall_table_address(void) {
    return (void**)kallsyms_lookup_name("sys_call_table");
}