/* kernel.ls */

physical_load_addr = 0x00101000;
ENTRY (KernelEntry)

SECTIONS
{
    . = physical_load_addr;

    .text :
    {
        *(.text)
        *(.rodata)
	*(.rodata*)
	*(.gnu.linkonce.t.*)
    }

    .data ALIGN (0x1000) :
    {
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

    .bss :
    {
        _sbss = .;
        *(COMMON)
        *(.bss)
        _ebss = .;
    }
}
