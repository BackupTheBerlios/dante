#ifndef __SYS_GDT_H
#define __SYS_GDT_H

#include <stdint.h>

struct GDTDescriptor 
{
    uint16_t cv_limitLow;
    uint16_t cv_baseLow;
    uint8_t  cv_baseMid;
    uint8_t  cv_attribLow;
    unsigned int cv_limitHigh:4;
    unsigned int cv_attribHigh:4;
    uint8_t  cv_baseHigh;
}; __attribute__((__packed__))

#define MakeDescriptor(base,limit,attributes) \
    {\
	(limit & 0xFFFF),\
	(base  & 0xFFFF),\
	((base  & 0xFF0000) >> 16),\
	(attributes & 0xFF),\
	((limit & 0xFF000000) >> 24),\
	((attributes & 0xF00) >> 8),\
	((base & 0xFF000000) >> 24),\
    }

enum DescriptorAttributes
{
    attr_Gran_4k 	= 0x800,
    attr_Big_Addr	= 0x400,
    attr_Present	= 0x080,
    attr_DPL_3		= 0x040,
    attr_DPL_1		= 0x020,
    attr_DPL_2		= attr_DPL_3 | attr_DPL_1,
    attr_Segment	= 0x010,
    attr_Code		= 0x008,
    attr_Code_Read	= 0x002,
    attr_Data_Write	= 0x002,
    attr_Accessed	= 0x001,
    attr_TSS		= 0x009,
    attr_LDT		= 0x002
};

#endif

