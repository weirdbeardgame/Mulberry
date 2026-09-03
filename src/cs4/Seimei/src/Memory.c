#include "common.h"
#include "Memory.h"

allocmng AllocMng[2048];
u_int LastAddr;
u_int UseMemSize;
u_int MaxHeapSize;

int _stack_size;
int _end;

inline int MemGetHeapSize()
{   
    int ret;
    memset(&AllocMng, 0, sizeof(AllocMng));
    ret = (unsigned int)&_end + ((unsigned int)&_stack_size + 0x1000);
    ret += 0x14000;
    return 0x2000000 - ret;
    
}

inline allocmng *MemSetLP(void * lp, allocmng *temp_v1, int size)
{
    int i;


    if (lp == 0) {
        return 0;
    }
    temp_v1 = &AllocMng[0];
    for (i = 0; temp_v1 != &AllocMng[2048]; temp_v1++)
    {
        if (!temp_v1->bUse)
        {
            temp_v1->bUse = 1;
            temp_v1->addr = (u_int)lp;
            temp_v1->size = size;
            return temp_v1;
        }
        // temp_v1++;
    }

    return 0;

}










void MemInit(void) 
{
    void* lp;
    allocmng *temp_v1 = AllocMng;


    int sz = MemGetHeapSize() - 0x10;




























    lp = (void *)malloc(sz);















    LastAddr = (int)lp + sz;


    temp_v1 = MemSetLP(lp, temp_v1, sz);
    temp_v1->addrB = temp_v1->addr;

    
    
    
    UseMemSize = 0;
    MaxHeapSize = sz;
}






void * MemAllocReal(unsigned int size /* r11 */, char * lpFuncName /* r5 */, char * lpUseName /* r2 */)
{
    void * lpAddr; // r2
    
    if (size == 0) return NULL;

        
    lpAddr = AllocateMemory(size, 0x10, lpFuncName, lpUseName);









    UseMemSize += size;
    return lpAddr; 
}






void * MemAllocAlignReal(unsigned int size /* r11 */, unsigned int align /* r2 */, char * lpFuncName /* r2 */, char * lpUseName /* r2 */)
{
    void * lpAddr; // r2 

    if (size == 0) return NULL; 

    
    lpAddr = AllocateMemory(size, align, lpFuncName, lpUseName);









    UseMemSize += size;
    return lpAddr; 
}

/*
    Compile unit: C:\cs4\Seimei\src\Memory.c
    Producer: MW MIPS C Compiler
    Language: C
    Code range: 0x0014AF40 -> 0x0014AF6C
*/
void MemFree(void * lpMem /* r2 */) {

    if (lpMem != NULL) {

        
        UseMemSize -= FreeMemory(lpMem);
    }




}


static inline allocmng *getLpBaseMng(u_int size, u_int align) { // or macro?
    allocmng * lpBaseMng;
    unsigned int AlignAddr;
    unsigned int AlignSize;
    unsigned int OldEndAddr;

    lpBaseMng = AllocMng;

    while (1) {
        if (lpBaseMng->bUse != 0 && lpBaseMng->bFree != 0) {
            AlignAddr = ~(align - 1) & (lpBaseMng->addrB + align - 1);
            AlignAddr += size;
            if (AlignAddr <= lpBaseMng->addr + lpBaseMng->size) {
                return lpBaseMng;
            }
        }
        lpBaseMng++;
        if (lpBaseMng == &AllocMng[0x800]) {
            lpBaseMng = NULL;
            break;
        }
    }
    return lpBaseMng;
}

static inline allocmng *findUnusedAllocMng() { // or macro?
    allocmng * lpBaseMng;

    lpBaseMng = AllocMng;

    while (1) {
            if (lpBaseMng->bUse == 0) {
                lpBaseMng->bUse = 1;
                break;
            } else {
                lpBaseMng++;
                if (lpBaseMng == (&AllocMng[0x800])) {
                    lpBaseMng = NULL;
                    break;
                }
            }
        }
    return lpBaseMng;
}

static inline allocmng *findUnusedAllocMngAligned(u_int size, unsigned int align) { // or macro?
    allocmng * lpBaseMng;
    if (align == 0) {
        lpBaseMng = NULL;
    } else {
        lpBaseMng = AllocMng;
        while (1) {
            if (lpBaseMng->bUse == 0) {
                lpBaseMng->bUse = 1;
                lpBaseMng->addr = align;
                lpBaseMng->size = size;
                break;
            } else {
                lpBaseMng++;
                if (lpBaseMng == (&AllocMng[0x800])) {
                    lpBaseMng = NULL;
                    break;
                }
            }
        }
    }
    return lpBaseMng;
}

static inline int getAlignedAddress(int addr, volatile int align) { // ???
    return (addr + align - 1) & ~(align - 1);
}



















































/*
    Compile unit: C:\cs4\Seimei\src\Memory.c
    Producer: MW MIPS C Compiler
    Language: C
    Code range: 0x0014AF20 -> 0x0014AF34
*/
void SetFixMemoryEndIndex(void) {
    FixMemEndIndex = AllocMng->prev->no;







}






























/*
    Compile unit: C:\cs4\Seimei\src\Memory.c
    Producer: MW MIPS C Compiler
    Language: C
    Code range: 0x0014AD00 -> 0x0014AF20
*/
static void * AllocateMemory(u_int size /* r2 */, u_int align /* r2 */, char * lpFuncName /* r2 */, char * lpUseName)
{
    // Range: 0x14AD00 -> 0x14AF20 
    allocmng * lpBaseMng;    // r9  $t1
    unsigned int AlignAddr;  // r10 $t2
    unsigned int AlignSize;  // r8  $t0
    unsigned int OldEndAddr; // r9  $t1
    allocmng* var_t0;
    u_int temp_v0, temp_a3;


    if ((lpBaseMng = getLpBaseMng(size, align)) != NULL) {


        
        lpBaseMng->bFree = 0;

        AlignAddr = getAlignedAddress(lpBaseMng->addrB, align);
        OldEndAddr = lpBaseMng->addr + lpBaseMng->size, temp_a3 = AlignAddr + size;

        if (temp_a3 == OldEndAddr) {

            lpBaseMng->addr = AlignAddr;
            lpBaseMng->size = size;
            lpBaseMng->lpfuncname = lpFuncName;




            return (void *)lpBaseMng->addr;
        }


        var_t0 = findUnusedAllocMng();
        var_t0->addr = temp_a3;
        var_t0->addrB = var_t0->addr;
        var_t0->size = OldEndAddr - temp_a3;
        var_t0->bFree = 1;

        var_t0->next = lpBaseMng->next;
        var_t0->next->prev = var_t0;
        var_t0->prev = lpBaseMng;
        lpBaseMng->next = var_t0;

        lpBaseMng->addr = AlignAddr;
        lpBaseMng->size = size;
        lpBaseMng->lpfuncname = lpFuncName;





        return (void *)lpBaseMng->addr;
    }


    // Line Numbers suggest bad instruction in inline function
    var_t0 = AllocMng;


    AlignAddr = getAlignedAddress(var_t0->addr, align);





    temp_v0 = (AlignAddr - var_t0->addr);
    lpBaseMng = findUnusedAllocMngAligned(size, AlignAddr);

    lpBaseMng->addrB = var_t0->addr;
    lpBaseMng->lpfuncname = lpFuncName;




    
    var_t0->addr = AlignAddr + size;
    var_t0->addrB = var_t0->addr;
    var_t0->size -= size + temp_v0;


    if (var_t0->prev != NULL) {
        var_t0->prev->next = lpBaseMng;
        lpBaseMng->prev = var_t0->prev;
    }
    var_t0->prev = lpBaseMng;
    lpBaseMng->next = var_t0;





    return (void* ) lpBaseMng->addr;
}

static u_int FreeMemory(void * lpMem /* r2 */) 
{ 
    allocmng * lpBaseMng; allocmng * lpAllocMng; 
    allocmng * lpLinkMng; 
    unsigned int FreeAddr;
    unsigned int EndAddr; 
    unsigned int FreeSize;
    lpBaseMng = AllocMng;






    for(lpAllocMng = lpBaseMng; lpAllocMng != &AllocMng[2048]; lpAllocMng++) {
        if (lpAllocMng->bUse == 0) { continue; }
        if (lpAllocMng->bFree != 0) { continue; }
        if (lpAllocMng->addr != (int)lpMem) { continue; }
        
        
        
        FreeSize = lpAllocMng->size;
        lpLinkMng = lpAllocMng->prev;
        while ((!!lpLinkMng && (lpLinkMng->bFree != 0))) {
            EndAddr = lpAllocMng->addr + lpAllocMng->size;
            lpAllocMng->addr = lpLinkMng->addrB;
            lpAllocMng->addrB = lpAllocMng->addr;
            lpAllocMng->size = EndAddr - lpAllocMng->addr;
            lpAllocMng->prev = (allocmng *)lpLinkMng->prev;
            if (lpAllocMng->prev) { lpAllocMng->prev->next = lpAllocMng; }
    
            lpLinkMng->bUse = 0;
            lpLinkMng->bFree = 0;
            lpLinkMng->next = (allocmng *)0x0;
            lpLinkMng->prev = (allocmng *)0x0;
    
            
            
            
            lpLinkMng = lpAllocMng->prev;
        }
        
        lpLinkMng = lpAllocMng->next;
        while (!!lpLinkMng && (lpLinkMng->bFree != 0)) {
            EndAddr = lpLinkMng->addr + lpLinkMng->size;
            lpAllocMng->size = EndAddr - lpAllocMng->addrB;
            lpAllocMng->addr = lpAllocMng->addrB;
            lpAllocMng->next = (allocmng *)lpLinkMng->next;
            if (lpAllocMng->next) { lpAllocMng->next->prev = lpAllocMng; }
    
            lpLinkMng->bUse = 0;
            lpLinkMng->bFree = 0;
            lpLinkMng->next = (allocmng *)0x0;
            lpLinkMng->prev = (allocmng *)0x0; lpLinkMng = lpAllocMng->next;




        
        }


        if (lpLinkMng == lpBaseMng) {
            lpBaseMng->size = lpBaseMng->addr - lpAllocMng->addrB + lpBaseMng->size;
            lpBaseMng->addr = lpAllocMng->addrB;
            lpBaseMng->addrB = lpBaseMng->addr;
            if (lpAllocMng->prev) { lpAllocMng->prev->next = lpBaseMng; }
            lpBaseMng->prev = lpAllocMng->prev;

            lpAllocMng->bUse = 0;
            lpAllocMng->bFree = 0;
            lpAllocMng->next = 0;
            lpAllocMng->prev = 0;



        } else {
        
            
            lpAllocMng->bFree = 1;
        }
        return FreeSize;
    }
        
    return 0;
}