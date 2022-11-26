#include "pcbmanager.h"

PCBManager::PCBManager(int maxProcesses)
{
    bitmap = new BitMap(maxProcesses);
    pcbs = new PCB *[maxProcesses];
    printf("pcbs.size() = %d\n", sizeof(pcbs));
    pcbManagerLock = new Lock("PCBManagerLock");

    for (int i = 0; i < maxProcesses; i++)
    {
        pcbs[i] = NULL;
    }
}

PCBManager::~PCBManager()
{

    delete bitmap;

    delete pcbs;
}

PCB *PCBManager::AllocatePCB()
{
    // Aquire pcbManagerLock
    pcbManagerLock->Acquire();

    int pid = bitmap->Find();

    // Release pcbManagerLock
    pcbManagerLock->Release();

    ASSERT(pid != -1);

    pcbs[pid] = new PCB(pid);

    return pcbs[pid];
}

int PCBManager::DeallocatePCB(PCB *pcb)
{
    // printf("held by current thread = %d", pcbManagerLock->isHeldByCurrentThread());
    // Check is pcb is valid -- check pcbs for pcb->pid
    if (pcb->pid >= 0 && pcb->pid < sizeof(pcbs) && pcbManagerLock->isHeldByCurrentThread())
    {
        // Aquire pcbManagerLock
        pcbManagerLock->Acquire();

        bitmap->Clear(pcb->pid);

        // Release pcbManagerLock
        pcbManagerLock->Release();

        delete pcbs[pcb->pid];

        pcbs[pcb->pid] = NULL;

        return 0;
    }
}

PCB *PCBManager::GetPCB(int pid)
{
    return pcbs[pid];
}

int PCBManager::GetLength()
{
    return sizeof(pcbs);
}