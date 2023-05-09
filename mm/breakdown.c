#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/kvm_para.h>

static int cmd = 0;
static int arg = 0;
#define VCPU_NR (64)
#define EXIT_REASON_MAX (0x405)
module_param(cmd, int, S_IRUGO);
module_param(arg, int, S_IRUGO);
extern atomic64_t tcp_send_count[], tcp_send_cycles[];
// #define atomic64_t uint64_t

// extern atomic64_t vmexit_cnt[][EXIT_REASON_MAX];
// extern atomic64_t vmexit_cycle[][EXIT_REASON_MAX];
// extern atomic64_t mmio_vmgexit_cnt[];
// extern atomic64_t mmio_vmgexit_cycle[];
// extern atomic_t vmexit_first[];

extern uint64_t breakdown_st;


extern bool record_en;


extern uint64_t breakdown_st;
extern uint64_t netrx_count[];
extern uint64_t netrx_cycles[];
extern uint64_t rx_map_cycles[], tx_map_cycles[];
extern uint64_t rx_map_count[], tx_map_count[];
extern uint64_t unmap_count[], unmap_cycles[];
extern uint64_t swiotlb_rx_cycles[], swiotlb_tx_cycles[];
extern uint64_t swiotlb_rx_count[], swiotlb_tx_count[];
extern uint64_t swiotlb_memcpy_count[], swiotlb_memcpy_cycles[];
extern uint64_t swiotlb_find_count[], swiotlb_find_cycles[];
extern uint64_t swiotlb_release_count[], swiotlb_release_cycles[];
// extern atomic64_t tcp_send_count[], tcp_send_cycles[];
#define set(x, y) (*x=y);smp_wmb();
#define read(x) (*x)
static int __init breakdown_init(void)
{

    int i;
    uint64_t st, en, tot;

        // pr_info("%s", __func__);
        switch (cmd)
        {
        case 0:
                pr_info("record_en set to %s\n", 
                                arg == 1?"true":"false");
                breakdown_st = rdtsc_ordered();
                record_en = (arg == 1);

                break;
        case 2:
                record_en = 0;
                for (i = 0; i < VCPU_NR; i++) {
                set(&netrx_count[i], 0);
                set(&netrx_cycles[i], 0);
                /* set(&swiotlb_tx_cycles[i], 0);
                set(&swiotlb_rx_cycles[i], 0);
                set(&swiotlb_tx_count[i], 0);
                set(&swiotlb_rx_count[i], 0); */
                set(&swiotlb_memcpy_cycles[i], 0);
                set(&swiotlb_memcpy_count[i], 0);
                set(&swiotlb_find_cycles[i], 0);
                set(&swiotlb_find_count[i], 0);
                set(&swiotlb_release_cycles[i], 0);
                set(&swiotlb_release_count[i], 0);
                atomic64_set(&tcp_send_cycles[i], 0);
                atomic64_set(&tcp_send_count[i], 0);
                
                }
                pr_info("clear all stat data");
                break;
        case 3:
                if (record_en)
                        en = rdtsc_ordered();
                record_en = 0;
                tot = en - breakdown_st;
                pr_info("Total %lld cycles\n", tot);
                for (i = 0; i < VCPU_NR; i++) {
                    uint64_t cnt, cycles;
                    smp_rmb();
                    cnt = read(&netrx_count[i]);
                    cycles = read(&netrx_cycles[i]);
                    if(cnt) pr_info("cpu %d netrx cnt %lld cycles %lld percent %lld\n", i, cnt, cycles, 100*cycles/tot);
                    cnt = atomic64_read(&tcp_send_count[i]);
                    cycles = atomic64_read(&tcp_send_cycles[i]);
                    if(cnt) pr_info("cpu %d tcp_send cnt %lld cycles %lld percent %lld\n", i, cnt, cycles, 100*cycles/tot);
                    /* cnt = read(&swiotlb_rx_count[i]);
                    cycles = read(&swiotlb_rx_cycles[i]);
                    if(cnt) pr_info("cpu %d swiotlb_rx cnt %lld cycles %lld percent %lld\n", i, cnt, cycles, 100*cycles/tot);
                    cnt = read(&swiotlb_tx_count[i]);
                    cycles = read(&swiotlb_tx_cycles[i]);
                    if(cnt) pr_info("cpu %d swiotlb_tx cnt %lld cycles %lld percent %lld\n", i, cnt, cycles, 100*cycles/tot); */
                    cnt = read(&swiotlb_memcpy_count[i]);
                    cycles = read(&swiotlb_memcpy_cycles[i]);
                    if(cnt) pr_info("cpu %d swiotlb_memcpy cnt %lld cycles %lld percent %lld\n", i, cnt, cycles, 100*cycles/tot);
                    cnt = read(&swiotlb_find_count[i]);
                    cycles = read(&swiotlb_find_cycles[i]);
                    if(cnt) pr_info("cpu %d swiotlb_find cnt %lld cycles %lld percent %lld\n", i, cnt, cycles, 100*cycles/tot);
                    cnt = read(&swiotlb_release_count[i]);
                    cycles = read(&swiotlb_release_cycles[i]);
                    if(cnt) pr_info("cpu %d swiotlb_release cnt %lld cycles %lld percent %lld\n", i, cnt, cycles, 100*cycles/tot);
                }
                break;
         case 4:
                tot = 0;

                for (i = 0; i < arg; i++) {
                        st = rdtsc_ordered();
                        kvm_hypercall0(0);
                        en = rdtsc_ordered();
                        tot += en - st;
                }
                pr_info("%d hypercalls cost %lld cycles\n", arg, tot);
                break;
        case 5:
                tot = 0;

                for (i = 0; i < arg; i++) {
                        st = rdtsc_ordered();
                        native_read_msr(0);
                        en = rdtsc_ordered();
                        tot += en - st;
                }
                pr_info("%d rdmsrs cost %lld cycles\n", arg, tot);
                break;
        default:
                pr_err("unexpected cmd:%d", cmd);
        }
        return 0;
}

static void __exit breakdown_exit(void)
{
        // pr_info("%s", __func__);
}

module_init(breakdown_init)
module_exit(breakdown_exit)
MODULE_LICENSE("GPL");