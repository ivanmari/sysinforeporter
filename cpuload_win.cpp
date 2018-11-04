#include "cpuload_win.h"
#include "QtDebug"

CpuLoad_Win::CpuLoad_Win():m_hKernel(nullptr), m_pfnGetSystemTimes(nullptr)
{
    GetSystemTimesAddress();
}

int
CpuLoad_Win::get_load() const
{
    FILETIME               ft_sys_idle;
    FILETIME               ft_sys_kernel;
    FILETIME               ft_sys_user;

    ULARGE_INTEGER         ul_sys_idle;
    ULARGE_INTEGER         ul_sys_kernel;
    ULARGE_INTEGER         ul_sys_user;

    static ULARGE_INTEGER    ul_sys_idle_old;
    static ULARGE_INTEGER  ul_sys_kernel_old;
    static ULARGE_INTEGER  ul_sys_user_old;

    CHAR  usage = 0;

    // we cannot directly use GetSystemTimes on C language
    // add this line :: pfnGetSystemTimes
    m_pfnGetSystemTimes(&ft_sys_idle,   // System idle time
        &ft_sys_kernel,  // system kernel time
        &ft_sys_user);   // System user time

    CopyMemory(&ul_sys_idle  , &ft_sys_idle  , sizeof(FILETIME)); // Could been optimized away...
    CopyMemory(&ul_sys_kernel, &ft_sys_kernel, sizeof(FILETIME)); // Could been optimized away...
    CopyMemory(&ul_sys_user  , &ft_sys_user  , sizeof(FILETIME)); // Could been optimized away...

    usage  =
        (
        (
        (
        (
        (ul_sys_kernel.QuadPart - ul_sys_kernel_old.QuadPart)+
        (ul_sys_user.QuadPart   - ul_sys_user_old.QuadPart)
        )
        -
        (ul_sys_idle.QuadPart-ul_sys_idle_old.QuadPart)
        )
        *
        (100)
        )
        /
        (
        (ul_sys_kernel.QuadPart - ul_sys_kernel_old.QuadPart)+
        (ul_sys_user.QuadPart   - ul_sys_user_old.QuadPart)
        )
        );

    ul_sys_idle_old.QuadPart   = ul_sys_idle.QuadPart;
    ul_sys_user_old.QuadPart   = ul_sys_user.QuadPart;
    ul_sys_kernel_old.QuadPart = ul_sys_kernel.QuadPart;

    return usage;
}


void
CpuLoad_Win::GetSystemTimesAddress()
{
    if( m_hKernel == NULL )
    {
        m_hKernel = LoadLibrary( L"Kernel32.dll" );
        if( m_hKernel != NULL )
        {
            m_pfnGetSystemTimes = (pfnGetSystemTimes)GetProcAddress( m_hKernel, "GetSystemTimes" );
            if( m_pfnGetSystemTimes == NULL )
            {
                FreeLibrary( m_hKernel ); m_hKernel = NULL;
            }
        }
    }
}

CpuLoad_Win::~CpuLoad_Win()
{
    FreeLibrary( m_hKernel );
    m_hKernel = NULL;
    m_pfnGetSystemTimes = NULL;

}
