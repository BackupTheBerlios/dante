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
    }

    .data ALIGN (0x1000) :
    {
        *(.data)
    }

    .bss :
    {
        _sbss = .;
        *(COMMON)
        *(.bss)
        _ebss = .;
    }
}
