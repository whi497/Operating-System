#include "../../include/mem.h"

unsigned long kmalloc(unsigned long size){
    return dPartitionAlloc(kpMemHandler,size);

}

unsigned long kfree(unsigned long start){
    return dPartitionFree(kpMemHandler,start);
}
