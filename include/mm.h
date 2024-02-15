#ifndef __MM_H__
#define __MM_H__

#define KERNEL_VADDR_BASE               0xffff000000000000

/* Paging is configured by TCR. The following basic configuration is used in lab5 */
#define TCR_T0SZ_48bit                  ((64 - 48) << 0)
#define TCR_T1SZ_48bit                  ((64 - 48) << 16)
#define TCR_CONFIG_REGION_48bit         (TCR_T0SZ_48bit | TCR_T1SZ_48bit)

#define TCR_TG0_4KB                     (0b00 << 14)
#define TCR_TG1_4KB                     (0b10 << 30)
#define TCR_CONFIG_4KB                  (TCR_TG0_4KB | TCR_TG1_4KB)

#define TCR_IRGN1_NC                    (0b00 << 24) // No cacheable
#define TCR_ORGN1_NC                    (0b00 << 26) // No cacheable
#define TCR_CONFIG_CACHE_POLICY_NC      (TCR_IRGN1_NC | TCR_ORGN1_NC) // Non-cacheable

#define TCR_CONFIG_DEFAULT              (TCR_CONFIG_REGION_48bit | TCR_CONFIG_4KB | TCR_CONFIG_CACHE_POLICY_NC)

/* MAIR store the memory configuration information */
#define MAIR_DEVICE_nGnRnE              0b00000000
#define MAIR_NORMAL_NOCACHE             0b01000100
#define MAIR_IDX_DEVICE_nGnRnE          0
#define MAIR_IDX_NORMAL_NOCACHE         1

#define MAIR_CONFIG                     ((MAIR_DEVICE_nGnRnE << (MAIR_IDX_DEVICE_nGnRnE * 8)) | (MAIR_NORMAL_NOCACHE << (MAIR_IDX_NORMAL_NOCACHE * 8)))

/* Page table entry configuration */
#define PD_TABLE                        0b11
#define PD_BLOCK                        0b01
#define PD_PAGE                         0b11
#define PD_INVALID                      0b00

#define PD_ACCESS                       (1 << 10)

#define BOOT_PGD_ATTR                   (PD_TABLE)
#define DEVICE_BLOCK_ATTR               (PD_ACCESS | (MAIR_IDX_DEVICE_nGnRnE << 2) | PD_BLOCK)
#define NORMAL_BLOCK_ATTR               (PD_ACCESS | (MAIR_IDX_NORMAL_NOCACHE << 2) | PD_BLOCK)
#define DEVICE_PAGE_ATTR                (PD_ACCESS | (MAIR_IDX_DEVICE_nGnRnE << 2) | PD_PAGE)
#define NORMAL_PAGE_ATTR                (PD_ACCESS | (MAIR_IDX_NORMAL_NOCACHE << 2) | PD_PAGE)


#define PAGE_SIZE                       4096

#define MMU_PGD_ADDR                    0x0000
#define MMU_PUD_ADDR                    0x1000
#define MMU_PMD_ADDR                    0x2000
#define MMU_PTE_ADDR                    0x3000

#define PERIPH_MMIO_BASE                0x3F000000

#endif