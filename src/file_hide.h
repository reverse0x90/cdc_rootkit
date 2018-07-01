#ifndef FILE_HIDE
#define FILE_HIDE

// linux_dirent struct def found here: https://elixir.bootlin.com/linux/v4.13/source/fs/readdir.c#L151
struct linux_dirent {
    unsigned long   d_ino;
    unsigned long   d_off;
    unsigned short  d_reclen; // length of entry
    char            d_name[1]; // name of entry
};


int init_file_hide(void);
void cleanup_file_hide(void);

#endif