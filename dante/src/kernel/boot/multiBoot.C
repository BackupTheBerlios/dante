#include <boot/multiBoot.h>
#include <display/textStream.h>
#include <stdint.h>

MultiBootParser::MultiBootParser()
{
}

MultiBootParser::MultiBootParser(void * i_bootHeader)
{
    parseHeader(i_bootHeader);
}

void MultiBootParser::parseHeader(void * i_bootHeader)
{
    uint32_t * l_bootHeader = (uint32_t *) i_bootHeader;
    
    kout << "Multiboot header found at: " << (uint32_t) i_bootHeader << endl;

    cv_flags = (l_bootHeader)[0];
    kout << "\tMultiboot flags = " << cv_flags << endl;

    uint32_t l_tmpFlags = cv_flags;

    if (l_tmpFlags & 0x01)
    {
	kout << "\tFound Memory Description." << endl;
	this->parseMemRange(&l_bootHeader[1]);
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	kout << "\tFound Boot Device descriptor." << endl;
	this->parseBootDevice(&l_bootHeader[3]);
    }
    l_tmpFlags >>= 1;
    
    if (l_tmpFlags & 0x01)
    {
	kout << "\tFound Command line." << endl;

	cv_cmdLine = (char *) l_bootHeader[4];	
	kout << "\t\tCommand line = " << cv_cmdLine << endl;
    }
    l_tmpFlags >>= 1;  

    if (l_tmpFlags & 0x01)
    {
	kout << "\tFound modules." << endl;
	this->parseModules(&l_bootHeader[5]);
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	kout << "\tFound a.out symbol table." << endl;
	this->parseSymbolTable(&l_bootHeader[7], MultiBootParser::AOUT);
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	kout << "\tFound ELF symbol table." << endl;
	this->parseSymbolTable(&l_bootHeader[7], MultiBootParser::ELF);
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	kout << "\tFound memory map." << endl;
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	kout << "\tFound drive specifications." << endl;
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	kout << "\tFound BIOS config table." << endl;
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	kout << "\tBoot loader was: " << (char *) l_bootHeader[16] << endl;
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	kout << "\tFound APM table." << endl;
    }
    l_tmpFlags >>= 1;

    if (l_tmpFlags & 0x01)
    {
	kout << "\tFound VBE information." << endl;
    }
    l_tmpFlags >>= 1;

    kout << "Finished parsing MultiBoot header." << endl;
}

void MultiBootParser::parseMemRange(void * i_addr)
{
    cv_memLow = ((uint32_t *)i_addr)[0];
    cv_memHigh = ((uint32_t *)i_addr)[1];

    kout << "\t\tMemory found: Lower=" << cv_memLow << " kb, Upper=";
    kout << cv_memHigh << " kb." << endl;
}

void MultiBootParser::parseBootDevice(void * i_addr)
{
    uint8_t * l_device = (uint8_t *) i_addr;
    
    for (int i = 0; i < 4; i++)
	cv_bootDevice[i] = l_device[i];

    kout << "\t\tDevice: " << cv_bootDevice[0] << endl;
    kout << "\t\tPartition: " << cv_bootDevice[1] << ", ";
    kout << cv_bootDevice[2] << ", " << cv_bootDevice[3] << endl;

}

void MultiBootParser::parseModules(void * i_addr)
{
    cv_modCount = ((uint32_t *)i_addr)[0];
    cv_modAddress = (void *)((uint32_t *)i_addr)[1];

    kout << "\t\tModules: " << cv_modCount << " at " ;
    kout << (uint32_t) cv_modAddress << endl;
}

void MultiBootParser::parseSymbolTable(void * i_addr, imageType i_type)
{
    cv_imageType = i_type;
    cv_symbolAddress = i_addr;

    kout << "\t\tSymbol info at " << (uint32_t) cv_symbolAddress << endl;
}
MultiBootParser g_multiBoot;
