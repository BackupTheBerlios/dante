#include <mem/vmm.h>
#include <mem/pageDir.h>

VMM::VMM()
{
    
    cv_freePageLocation = g_kernelPageDirectory.findFreeAddress(true);

    g_kernelPageDirectory.mapPage(cv_freePageLocation, ~0);
    g_kernelPageDirectory.writePageFlags(cv_freePageLocation, ~0);

};

