#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/time.h>
#include <linux/uaccess.h>
#define IPADDRESS(addr)              \
    ((unsigned char *)&addr)[3],     \
        ((unsigned char *)&addr)[2], \
        ((unsigned char *)&addr)[1], \
        ((unsigned char *)&addr)[0]

// Define the IP address prefix to accept
#define NITK "10."
#define LOCAL "127."
#define IRIS "210."
#define SERVER "57.151.115.71"

char *param_var[1];

module_param_array(param_var, charp, NULL, S_IRUSR | S_IWUSR);

int done;

static struct file *logfile;
static struct nf_hook_ops *nf_blockipaddr_ops = NULL;
static unsigned int nf_blockipaddr_handler(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
    struct timespec64 ts;
    struct tm tm;
    char timestr[32];
    // Get current time
    ktime_get_real_ts64(&ts);
    time64_to_tm(ts.tv_sec, 0, &tm);
    // Format time string
    snprintf(timestr, sizeof(timestr), "%04ld-%02d-%02d %02d:%02d:%02d",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);

    if (!skb)
    {
        return NF_ACCEPT;
    }
    else
    {
        if (!done)
        {
            printk(KERN_ALERT "\xC2\xA7 \xC2\xB6 I am become death, the destroyer of packets.\n");
            done++;
        }
        struct sk_buff *sb = NULL;
        struct iphdr *iph;
        u32 sip;
        sb = skb;
        iph = ip_hdr(sb);
        sip = ntohl(iph->saddr);

        char str[16];
        sprintf(str, "%u.%u.%u.%u", IPADDRESS(sip));
        if (strncmp(str, NITK, strlen(NITK)) == 0 || strncmp(str, LOCAL, strlen(LOCAL)) == 0 || strncmp(str, IRIS, strlen(IRIS)) == 0 || strncmp(str, SERVER, strlen(SERVER)) == 0 || strncmp(str, param_var[0], strlen(param_var[0])) == 0)
        {
            printk(KERN_ALERT "\xC2\xA7 \xC2\xB6 [%s] ACCEPT: %s\n", timestr, str);
            return NF_ACCEPT;
        }
        else
        {
            // Check if it's a DNS request to port 53
            if (iph->protocol == IPPROTO_UDP || iph->protocol == IPPROTO_TCP)
            {
                struct udphdr *udph;
                struct tcphdr *tcph;
                if (iph->protocol == IPPROTO_UDP)
                {
                    udph = udp_hdr(sb);
                    //                    if (ntohs(udph->dest) == 53)
                    if (ntohs(udph->source) == 53)
                    {
                        printk(KERN_ALERT "\xC2\xA7 \xC2\xB6 [%s] ACCEPT: DNS Request to port 53\n", timestr);
                        return NF_ACCEPT;
                    }
                }
                else if (iph->protocol == IPPROTO_TCP)
                {
                    tcph = tcp_hdr(sb);
                    //                    if (ntohs(tcph->dest) == 53)
                    if (ntohs(tcph->source) == 53)
                    {
                        printk(KERN_ALERT "\xC2\xA7 \xC2\xB6 [%s] ACCEPT: DNS Request to port 53\n", timestr);
                        return NF_ACCEPT;
                    }
                }
            }
            // If not a DNS request to port 53 or not matching the prefix, drop the packet
            printk(KERN_ALERT "\xC2\xA7 \xC2\xB6 [%s] DROP: %s\n", timestr, str);
            return NF_DROP;
        }
    }
}
static int __init nf_minifirewall_init(void)
{
    nf_blockipaddr_ops = (struct nf_hook_ops *)kcalloc(1, sizeof(struct nf_hook_ops), GFP_KERNEL);
    done = 0;
    if (nf_blockipaddr_ops != NULL)
    {
        nf_blockipaddr_ops->hook = (nf_hookfn *)nf_blockipaddr_handler;
        nf_blockipaddr_ops->hooknum = NF_INET_LOCAL_OUT;
        nf_blockipaddr_ops->pf = NFPROTO_IPV4;
        nf_blockipaddr_ops->priority = NF_IP_PRI_FIRST + 1;

        nf_register_net_hook(&init_net, nf_blockipaddr_ops);
    }
    return 0;
}
static void __exit nf_minifirewall_exit(void)
{
    if (logfile)
        filp_close(logfile, NULL);
    if (nf_blockipaddr_ops != NULL)
    {
        nf_unregister_net_hook(&init_net, nf_blockipaddr_ops);
        kfree(nf_blockipaddr_ops);
    }
    printk(KERN_ALERT "\xC2\xA7 \xC2\xB6 I am become dead, the ex-destroyer of packets.\n");
}
module_init(nf_minifirewall_init);
module_exit(nf_minifirewall_exit);
MODULE_LICENSE("GPL");