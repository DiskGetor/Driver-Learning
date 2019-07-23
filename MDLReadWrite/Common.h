#pragma once
#include <ntifs.h>
#include <ntimage.h>


#define DEV_NAME L"\\Device\\MyDevice"
#define SYM_LINK_NAME L"\\??\\MySymLink"

#define IOCTL_BASE 0x800
#define IOCTL_CODE(i) CTL_CODE(FILE_DEVICE_UNKNOWN,IOCTL_BASE+i,METHOD_BUFFERED,FILE_ANY_ACCESS)

#define IOCTL_READ IOCTL_CODE(1)
#define IOCTL_WRITE IOCTL_CODE(2)
#define IOCTL_INIT IOCTL_CODE(3)

typedef struct WRIO
{
	PVOID		VirtualAddress;
	size_t		Length;
}WRIO, *PWRIO;

NTKERNELAPI
UCHAR *
PsGetProcessImageFileName(
	__in PEPROCESS Process
);


PEPROCESS Process;