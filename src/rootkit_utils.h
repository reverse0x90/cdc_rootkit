#ifndef ROOTKIT_UTILS
#define ROOTKIT_UTILS

void enable_write_protect(void);
void disable_write_protect(void);
void** get_syscall_table_address(void);

#endif