#include <boot/multiBoot.h>
#include <display/textStream.h>
#include <display/nullStream.h>
#include <stdint.h>

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
}

void MultiBootParser::parseSymbolTable(void * i_addr, imageType i_type)
{
    NullStream nout;

    cv_imageType = i_type;
    cv_symbolAddress = i_addr;

    mb_out << "\t\tSymbol info at " << (uint32_t) cv_symbolAddress << endl;
}
MultiBootParser g_multiBoot;
