#ifndef CPULOAD_WIN_H
#define CPULOAD_WIN_H

#include "Windows.h"


typedef BOOL ( __stdcall * pfnGetSystemTimes)( LPFILETIME lpIdleTime, LPFILETIME lpKernelTime, LPFILETIME lpUserTime );

class CpuLoad_Win
{
public:
    CpuLoad_Win();

    int get_load() const;

    ~CpuLoad_Win();

private:
   HMODULE m_hKernel;
   pfnGetSystemTimes m_pfnGetSystemTimes;

    void GetSystemTimesAddress();
};

#endif // CPULOAD_WIN_H
