#include <boot/multiBoot.h>
#include <display/textStream.h>
#include <display/nullStream.h>
#include <mem/init.h>
#include <stdint.h>
#include <lib/Algo.h>

#define mb_out (cv_verboseParsing ? ((kostream&)kout) : ((kostream&)nout))

MultiBootParser::MultiBootParser()
{
    cv_verboseParsing = false;
}

MultiBootParser::MultiBootParser(void * i_bootHeader)
{
    cv_verboseParsing = false;
    parseHeader(i_bootHeader);
}

void MultiBootParser::parseHeader(void * i_bootHeader)
{
    uint32_t * l_bootHeader = (uint32_t *) i_bootHeader;
    NullStream nout;

    mb_out << "Multiboot header found at: " << (uint32_t) i_bootHeader << endl;

    cv_flags = (l_bootHeader)[0];
    mb_out << "\tMultiboot flags = " << cv_flags << endl;

    uint32_t l_tmpFlags = cv_flags;

    if (l_tmpFlags & 0x01)
    {
	mb_out << "\tFound Memory Description." << endl;
	this->parseMemRange(&l_bootHeader[1]);
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	mb_out << "\tFound Boot Device descriptor." << endl;
	this->parseBootDevice(&l_bootHeader[3]);
    }
    l_tmpFlags >>= 1;
    
    if (l_tmpFlags & 0x01)
    {
	mb_out << "\tFound Command line." << endl;

	cv_cmdLine = (char *) l_bootHeader[4];	
	mb_out << "\t\tCommand line = " << cv_cmdLine << endl;
    }
    l_tmpFlags >>= 1;  

    if (l_tmpFlags & 0x01)
    {
	mb_out << "\tFound modules." << endl;
	this->parseModules(&l_bootHeader[5]);
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	mb_out << "\tFound a.out symbol table." << endl;
	this->parseSymbolTable(&l_bootHeader[7], MultiBootParser::AOUT);
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	mb_out << "\tFound ELF symbol table." << endl;
	this->parseSymbolTable(&l_bootHeader[7], MultiBootParser::ELF);
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	mb_out << "\tFound memory map." << endl;
	this->parseMemMap(&l_bootHeader[11]);
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	mb_out << "\tFound drive specifications." << endl;
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	mb_out << "\tFound BIOS config table." << endl;
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	mb_out << "\tBoot loader was: " << (char *) l_bootHeader[16] << endl;
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	mb_out << "\tFound APM table." << endl;
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	mb_out << "\tFound VBE information." << endl;
    }
    l_tmpFlags >>= 1;

    mb_out << "Finished parsing MultiBoot header." << endl;
}

void MultiBootParser::parseMemRange(void * i_addr)
{
    NullStream nout;

    cv_memLow = ((uint32_t *)i_addr)[0];
    cv_memHigh = ((uint32_t *)i_addr)[1];

    mb_out << "\t\tMemory found: Lower=" << cv_memLow << " kb, Upper=";
    mb_out << cv_memHigh << " kb." << endl;
}

void MultiBootParser::parseBootDevice(void * i_addr)
{
    NullStream nout;
    uint8_t * l_device = (uint8_t *) i_addr;
    
    for (int i = 0; i < 4; i++)
	cv_bootDevice[i] = l_device[i];

    mb_out << "\t\tDevice: " << cv_bootDevice[0] << endl;
    mb_out << "\t\tPartition: " << cv_bootDevice[1] << ", ";
    mb_out << cv_bootDevice[2] << ", " << cv_bootDevice[3] << endl;

}

void MultiBootParser::parseModules(void * i_addr)
{
    NullStream nout;

    cv_modCount = ((uint32_t *)i_addr)[0];
    cv_modAddress = (void *)((uint32_t *)i_addr)[1];

    mb_out << "\t\tModules: " << cv_modCount << " at " ;
    mb_out << (uint32_t) cv_modAddress << endl;

    for(int i = 0; i < cv_modCount; i++)
    {
	Module l_module = 
		{ 
		    (void*)((uint32_t **)cv_modAddress)[4*i] , 
		    (void*)((uint32_t **)cv_modAddress)[4*i+1] 
		};
	cv_modules.push(l_module);
    };
}

void MultiBootParser::parseSymbolTable(void * i_addr, imageType i_type)
{
    NullStream nout;

    cv_imageType = i_type;
    cv_symbolAddress = i_addr;

    mb_out << "\t\tSymbol info at " << (uint32_t) cv_symbolAddress << endl;
}

void MultiBootParser::parseMemMap(void * i_addr)
{
    NullStream nout;

    cv_mapLength = ((uint32_t *) i_addr)[0];
    cv_mapAddress = ((void **) i_addr)[1];

    mb_out << "\t\tMemory Map length = " << cv_mapLength << " at ";
    mb_out << (uint32_t) cv_mapAddress << endl;

    struct l_memMapStruct
    {
	uint32_t cv_size;
	uint32_t cv_baseAddrLo;
	uint32_t cv_baseAddrHi;
	uint32_t cv_lengthLo;
	uint32_t cv_lengthHi;
	uint32_t cv_type;
    } __attribute__((__packed__));
    
    l_memMapStruct * l_mapPointer = (l_memMapStruct *)
			(((uint32_t)cv_mapAddress));
    uint32_t l_bufSize = cv_mapLength;
    for (int i = 0; l_bufSize >= sizeof(l_memMapStruct); i++)
    {
	mb_out << "\t\tMap " << i << "(";
	mb_out << l_mapPointer->cv_type << "):" << endl;
	mb_out << "\t\t\tSize = " << l_mapPointer->cv_size << endl;
	mb_out << "\t\t\tBaseL = " << l_mapPointer->cv_baseAddrLo;
	mb_out << "\tBaseH = " << l_mapPointer->cv_baseAddrHi << endl;
	mb_out << "\t\t\tLenL  = " << l_mapPointer->cv_lengthLo;
	mb_out << "\tLenH  = " << l_mapPointer->cv_lengthHi << endl;
	
	// For now we are just using memory above the first meg.	
	if (l_mapPointer->cv_baseAddrLo >= 0x100000)
	{
	    
	    extern volatile uint32_t __KERNEL_END__;
	    uint32_t l_kern = (uint32_t) &__KERNEL_END__;

	    l_kern = 0xC0000000 ^ align<4096>(l_kern);
	    
	    uint32_t l_addr = l_mapPointer->cv_baseAddrLo;
	    uint32_t l_len = l_mapPointer->cv_lengthLo;
	    
	    if (l_mapPointer->cv_baseAddrLo >= l_kern)
	    {
	    }
	    else if ((l_mapPointer->cv_baseAddrLo + l_mapPointer->cv_lengthLo) >
		    l_kern)
	    {
		l_len -= l_kern - l_addr;
		l_addr = l_kern;
	    }
	    else
	    {
		l_len = 0;
	    }
	    
	    while (l_len > 0)
	    {
		bool l_found = false;
		uint32_t l_module;

		for (int i = 0; i < cv_modules.size(); i++)
		{
		    if (pair_overlap<uint32_t>(
				Pair<uint32_t,uint32_t>((uint32_t)cv_modules[i].cv_start, align<4096>((uint32_t)cv_modules[i].cv_end)),
				Pair<uint32_t,uint32_t>(l_addr, l_addr+l_len)))
		    {
			if (!l_found)
			{
			    l_module = i;
			    l_found = true;
			}
			else if (cv_modules[i].cv_start < 
				 cv_modules[l_module].cv_start)
			{
			    l_module = i;
			}
		    }
		}

		if (!l_found)
		{
		    g_freePageList.insert(l_addr >> 12, l_len >> 12);
		    l_len = 0;
		}
		else
		{
		    uint32_t l_start = (uint32_t)cv_modules[l_module].cv_start;
		    uint32_t l_end = (uint32_t)cv_modules[l_module].cv_end;

		    l_end = align<4096>(l_end);

		    if (l_addr < l_start)
		    {
			g_freePageList.insert(l_addr >> 12, (l_start-l_addr) >> 12);
			l_len -= (l_start-l_addr);
			l_addr = l_start;
		    }
		    if ((l_addr + l_len) < l_end)
		    {
			l_len = 0;
		    }
		    else
		    {
			l_len -= l_end - l_addr;
			l_addr = l_end;
		    }
		}

	    }
	}
		
	l_bufSize -= (l_mapPointer->cv_size + (0 == i ? 4 : 4));
	l_mapPointer = (l_memMapStruct *) 
		    (((uint32_t)l_mapPointer) + l_mapPointer->cv_size + 
			(0 == i ? 4 : 4));
	
	if (cv_verboseParsing)
	    for (int z = 0; z < 1000000; z++) asm volatile("nop");
    }

}

MultiBootParser g_multiBoot;
