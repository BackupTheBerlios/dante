#ifndef __SYS_TASK_H
#define __SYS_TASK_H

#include <stdint.h>

#include <sys/cpu.h>
#include <mem/pageDir.h>

struct TaskDescriptor
{
    CPUDescriptor * cv_currentCPU;

    // registers..
    //uint32_t	cr3, esp, eflags;
    //uint32_t	eax, ebx, ecx, edx;
    //uint32_t	ebp, esi, edi;

    void * cv_stack;
    void * cv_frameptr;
    PageDir * cv_pageDir;     
    
    uint32_t cv_runtime; 
     
} __attribute__((__packed__));

void initializeTasking();
void beginIdleThread();

struct TSSDescriptor
{
    uint16_t	cv_backlink;
    uint16_t	cv_unused;
    uint32_t	cv_esp0;
    uint32_t	cv_ss0;
    uint32_t	cv_esp1;
    uint32_t	cv_ss1;
    uint32_t	cv_esp2;
    uint32_t	cv_ss2;
    uint32_t	cv_cr3;
    uint32_t	cv_eip;
    uint32_t	cv_eflags;
    uint32_t	cv_eax, cv_ecx, cv_edx, cv_ebx;
    uint32_t	cv_esp, cv_ebp, cv_esi, cv_edi;
    uint32_t	cv_es, cv_cs, cv_ss, cv_ds, cv_fs, cv_gs;
    uint32_t 	cv_ldt;
    uint16_t 	cv_unused2;
    uint16_t	cv_io_bitmap;
} __attribute__((__packed__));

extern TSSDescriptor g_initialTSS;


#endif
