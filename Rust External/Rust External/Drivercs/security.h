#include <ntdef.h>
#include <ntifs.h>
#include <ntddk.h>
#include <WinDef.h>

#include "definitions.h"



   void clearCache(UNICODE_STRING DriverName, ULONG timeDateStamp);
   NTSTATUS ClearUnloadedDriver(
	   _In_ PUNICODE_STRING	DriverName,
	   _In_ BOOLEAN			AccquireResource
   );

   NTSTATUS FindMmDriverData(VOID);
