#ifndef _AADBG_SYS_H_
#define _AADBG_SYS_H_

#include <ntddk.h>

#define IOCTL_AADBG_HOOK	CTL_CODE(FILE_DEVICE_UNKNOWN, 0xE01, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_AADBG_UNHOOK	CTL_CODE(FILE_DEVICE_UNKNOWN, 0xE02, METHOD_BUFFERED, FILE_ANY_ACCESS)



#define NT_DEVICE_NAME		L"\\Device\\aadbg_sys"
#define DOS_DEVICE_NAME		L"\\DosDevices\\aadbg_sys"


// type definition

typedef struct _KSERVICE_TABLE_DESCRIPTOR {
	PULONG_PTR ServiceTableBase;// SSDT (System Service Dispatch Table)�Ļ���ַ
	PULONG ServiceCounterTableBase;// ���� checked builds, ���� SSDT ��ÿ�����񱻵��õĴ���
	ULONG NumberOfService;// �������ĸ���, NumberOfService * 4 ����������ַ��Ĵ�С
	PUCHAR ParamTableBase;// SSPT(System Service Parameter Table)�Ļ���ַ
} KSERVICE_TABLE_OBJ, *PKSERVICE_TABLE_OBJ;

typedef struct _SERVICE_DESCRIPTOR_TABLE
{
	KSERVICE_TABLE_DESCRIPTOR ntoskrnl;// ntoskrnl.exe �ķ�����
	KSERVICE_TABLE_DESCRIPTOR win32k;// win32k.sys �ķ�����(GDI32.dll/User32.dll ���ں�֧��)
	KSERVICE_TABLE_DESCRIPTOR Reserved1;
	KSERVICE_TABLE_DESCRIPTOR Reserved2;
}KSERVICE_DESCRIPTOR_TABLE, *PKSERVICE_DESCRIPTOR_TABLE;


// var declarations
extern PKSERVICE_TABLE_DESCRIPTOR	KeServiceDescriptorTable;



extern ULONG	g_ulBuildNum;
extern ULONG	g_ulMajorVer;
extern ULONG	g_ulMinorVer;


// function declarations
NTSTATUS CommDispatch(PDEVICE_OBJECT pDeviceObject, PIRP pIrp);
VOID DriverUnload(PDRIVER_OBJECT pDriverObject);
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath);


#endif