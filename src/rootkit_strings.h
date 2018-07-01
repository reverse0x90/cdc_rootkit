#ifndef ROOTKIT_STRINGS
#define ROOTKIT_STRINGS


int in_fs_strings(char* input_str);
int in_process_strings(char* input_str);
char** get_rootkit_fs_strings(void);
int get_rootkit_fs_strings_size(void);
char** get_rootkit_process_strings(void);
int get_rootkit_process_strings_size(void);
int add_rootkit_process_string(char* add_str);
int add_rootkit_fs_string(char* add_str);
void int_rootkit_strings(void);
void cleanup_rootkit_strings(void);

#endif