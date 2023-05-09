#include <kunit/test.h>
#include <linux/mm.h>
#include <linux/memblock.h>

#define __NUM_NODES 3
static phys_addr_t LOWER_BOUND[__NUM_NODES];
static phys_addr_t UPPER_BOUND[__NUM_NODES];
// #define NUMA_ID       (1)

extern struct memblock_region isolate_region[];
extern void *kmalloc_reserve_fake_numa(size_t size, gfp_t flags, bool *pfmemalloc, int);
extern void *kmalloc_reserve_wrapper(size_t size, gfp_t flags, bool *pfmemalloc, int);

#define check_paddr(paddr, i)   ((phys_addr_t)paddr >= LOWER_BOUND[i] && (phys_addr_t)paddr < UPPER_BOUND[i])

static bool init_test(void)
{
    for (int i = 1; i < __NUM_NODES; i++) {
        if (isolate_region[i].nid != i)
            return false;
        LOWER_BOUND[i] = isolate_region[i].base;
        UPPER_BOUND[i] = isolate_region[i].base + isolate_region[i].size;
    }
    
    return true;
}

struct page *page_list[11][1024];
int idx[11];

static void test_alloc_pages(struct kunit *test)
{
    int i, j, times, fake_nid;
    int max_order = 10;

    if (!init_test()) {
        pr_warn("find none fake numa");
        return;
    }
    // memset(idx, 0, sizeof idx);

    for (fake_nid = 1; fake_nid < __NUM_NODES; fake_nid++) {
        memset(idx, 0, sizeof(idx));
        for (times = 0; times < 100; times ++ ) {
            for (i = 0; i < max_order; i ++ ) {
                struct page *page = alloc_pages_node(fake_nid, GFP_KERNEL | ___GFP_FORCE_NID, i);
                void *paddr = (void *)page_to_phys(page);
                page_list[i][idx[i] ++ ] = page;
                
                if (!check_paddr(paddr, fake_nid))
                    KUNIT_FAIL(test, "[%s] times:%d alloc order:%d"
                                    "paddr:0x%llx lower_bound:0x%llx upper_bound:0x%llx",
                                    __func__, times, i, paddr, LOWER_BOUND[fake_nid], UPPER_BOUND[fake_nid]);
            }
        }

        for (i = 0; i < max_order; i ++ ) {
                for (j = 0; j < idx[i]; j ++ ) {
                        __free_pages(page_list[i][j], i);
                }
        }
    }
}

static void test_slub_alloc(struct kunit *test)
{
    int i, nid;
    bool pfmemalloc;

    if (!init_test()) {
        pr_warn("find none fake numa");
        return;
    }
    for (nid = __NUM_NODES - 1; nid > 0; nid--) {
        for (i = 0; i < 1000; i ++ ) {
            void *vaddr = kmalloc_reserve_fake_numa(1024, GFP_KERNEL, &pfmemalloc, nid);
            void *paddr = (void *)virt_to_phys(vaddr);
            if (!check_paddr(paddr, nid))
                KUNIT_FAIL(test, "[%s] times:%d paddr:0x%llx lower_bound:0x%llx upper_bound:0x%llx",
                                __func__, i, paddr, LOWER_BOUND[nid], UPPER_BOUND[nid]);
            
        }
    }
}

/* Try allocating from fake nid without adding flag (__alloc_page path) */
static void test_alloc_without_force(struct kunit *test)
{
    int i, j, times, fake_nid;
    int max_order = 10;

    for (fake_nid = 1; fake_nid < __NUM_NODES; fake_nid++) {
        memset(idx, 0, sizeof(idx));
        for (times = 0; times < 100; times ++ ) {
            for (i = 0; i < max_order; i ++ ) {
                struct page *page = alloc_pages_node(fake_nid, GFP_KERNEL, i);
                void *paddr = (void *)page_to_phys(page);
                page_list[i][idx[i] ++ ] = page;
                
                if (check_paddr(paddr, 1) || check_paddr(paddr, 2))
                    KUNIT_FAIL(test, "[%s] times:%d alloc order:%d"
                                    "paddr:0x%llx lower_bound:0x%llx upper_bound:0x%llx",
                                    __func__, times, i, paddr, LOWER_BOUND[fake_nid], UPPER_BOUND[fake_nid]);
            }
        }

        for (i = 0; i < max_order; i ++ ) {
                for (j = 0; j < idx[i]; j ++ ) {
                        __free_pages(page_list[i][j], i);
                }
        }
    }
}

/* Try allocating from fake nid without adding flag (slub path) */
static void test_slub_without_force(struct kunit *test)
{
    int i, nid;
    bool pfmemalloc;

    for (nid = __NUM_NODES - 1; nid > 0; nid--) {
        for (i = 0; i < 1000; i ++ ) {
            void *vaddr = kmalloc_reserve_wrapper(1024, GFP_KERNEL, &pfmemalloc, nid);
            void *paddr = (void *)virt_to_phys(vaddr);
            if (check_paddr(paddr, 1) || check_paddr(paddr, 2))
                KUNIT_FAIL(test, "[%s] times:%d paddr:0x%llx lower_bound:0x%llx upper_bound:0x%llx",
                                __func__, i, paddr, LOWER_BOUND[nid], UPPER_BOUND[nid]);
            
        }
    }
}

static struct kunit_case test_suites[] = {
        KUNIT_CASE(test_alloc_pages),
        KUNIT_CASE(test_slub_alloc),
        KUNIT_CASE(test_alloc_without_force),
        KUNIT_CASE(test_slub_without_force),
        {}
};

static struct kunit_suite slub_test_suites = {
        .name = "slub-ut",
        .test_cases = test_suites,
};
// kunit_test_suite(slub_test_suites);
