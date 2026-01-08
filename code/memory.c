// 在文件顶部添加版本检查
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0))
// 对于6.6+内核，可能需要不同的头文件包含方式
#include <linux/pgtable.h>
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 61))
extern void mmput(struct mm_struct *);

phys_addr_t translate_linear_address(struct mm_struct *mm, uintptr_t va)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0))
    // 针对6.6+内核的页表遍历
    pgd_t *pgd;
    p4d_t *p4d;
    pmd_t *pmd;
    pte_t *pte;
    pud_t *pud;

    phys_addr_t page_addr;
    uintptr_t page_offset;

    pgd = pgd_offset(mm, va);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
    {
        return 0;
    }
    
    p4d = p4d_offset(pgd, va);
    if (p4d_none(*p4d) || p4d_bad(*p4d))
    {
        return 0;
    }
    
    pud = pud_offset(p4d, va);
    if (pud_none(*pud) || pud_bad(*pud))
    {
        return 0;
    }
    
    pmd = pmd_offset(pud, va);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
    {
        return 0;
    }
    
    pte = pte_offset_kernel(pmd, va);
    if (pte_none(*pte))
    {
        return 0;
    }
    
    if (!pte_present(*pte))
    {
        return 0;
    }
    
    // 获取物理地址
    page_addr = (phys_addr_t)(pte_pfn(*pte) << PAGE_SHIFT);
    page_offset = va & (PAGE_SIZE - 1);
    
    return page_addr + page_offset;
#else
    // 原有的5.4.61+代码
    pgd_t *pgd;
    p4d_t *p4d;
    pmd_t *pmd;
    pte_t *pte;
    pud_t *pud;

    phys_addr_t page_addr;
    uintptr_t page_offset;

    pgd = pgd_offset(mm, va);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
    {
        return 0;
    }
    p4d = p4d_offset(pgd, va);
    if (p4d_none(*p4d) || p4d_bad(*p4d))
    {
        return 0;
    }
    pud = pud_offset(p4d, va);
    if (pud_none(*pud) || pud_bad(*pud))
    {
        return 0;
    }
    pmd = pmd_offset(pud, va);
    if (pmd_none(*pmd))
    {
        return 0;
    }
    pte = pte_offset_kernel(pmd, va);
    if (pte_none(*pte))
    {
        return 0;
    }
    if (!pte_present(*pte))
    {
        return 0;
    }
    page_addr = (phys_addr_t)(pte_pfn(*pte) << PAGE_SHIFT);
    page_offset = va & (PAGE_SIZE - 1);

    return page_addr + page_offset;
#endif
}
#else
// 原有的旧版本代码
phys_addr_t translate_linear_address(struct mm_struct *mm, uintptr_t va)
{
    pgd_t *pgd;
    pmd_t *pmd;
    pte_t *pte;
    pud_t *pud;

    phys_addr_t page_addr;
    uintptr_t page_offset;

    pgd = pgd_offset(mm, va);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
    {
        return 0;
    }
    pud = pud_offset(pgd, va);
    if (pud_none(*pud) || pud_bad(*pud))
    {
        return 0;
    }
    pmd = pmd_offset(pud, va);
    if (pmd_none(*pmd))
    {
        return 0;
    }
    pte = pte_offset_kernel(pmd, va);
    if (pte_none(*pte))
    {
        return 0;
    }
    if (!pte_present(*pte))
    {
        return 0;
    }
    page_addr = (phys_addr_t)(pte_pfn(*pte) << PAGE_SHIFT);
    page_offset = va & (PAGE_SIZE - 1);

    return page_addr + page_offset;
}
#endif