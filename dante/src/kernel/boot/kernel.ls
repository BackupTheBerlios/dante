/* kernel.ls */

physical_load_addr = 0x00101000;
virtual_load_addr =  0xC0101000;
ENTRY (KernelEntry)

SECTIONS
{
    . = virtual_load_addr;
    /*. = physical_load_addr;*/
    .text : AT(physical_load_addr)
    {
	text_section = .;
	*(.multiboot)
        *(.text)
        *(.rodata)
	*(.rodata*)
	*(.gnu.linkonce.t.*)
    }
    
    . = ALIGN(0x1000);
    .data : AT(physical_load_addr + (data_section - text_section))
    {
	data_section = .;
        *(.data)
	__CTOR_LIST__ = .;
	LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2) 
	*(.ctors)   
	LONG(0) 
	__CTOR_END__ = .;  

	__DTOR_LIST__ = .; 
	LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2) 
	*(.dtors) 
	LONG(0) 
	__DTOR_END__ = .; 
	
	*(.gnu.linkonce.d.*)
    }
    
    . = ALIGN(0x1000);
    .bss : AT(physical_load_addr + (bss_section - text_section))
    {
	bss_section = .;
        _sbss = .;
        *(COMMON)
        *(.bss)
        _ebss = .;
    }
}
