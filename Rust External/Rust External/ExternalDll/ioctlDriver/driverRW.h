#pragma once
#include <Windows.h>
#include <cstdint>
#include <TlHelp32.h>
#include <string>
#include "tools.h"




#define ioctl_allocate_virtual_memory CTL_CODE(FILE_DEVICE_UNKNOWN, 0x221, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define ioctl_copy_memory CTL_CODE(FILE_DEVICE_UNKNOWN, 0x224, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define ioctl_get_module_base CTL_CODE(FILE_DEVICE_UNKNOWN, 0x222, METHOD_BUFFERED, FILE_SPECIAL_ACCESS) 
#define ioctl_protect_virutal_memory CTL_CODE(FILE_DEVICE_UNKNOWN, 0x223, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _k_get_base_module_request {
	ULONG pid;
	ULONGLONG handle;
	WCHAR name[260];
} k_get_base_module_request, * pk_get_base_module_request;

typedef struct _k_rw_request {
	uint32_t src_pid;
	uint32_t dst_pid;
	uint64_t src_addr;
	uint64_t dst_addr;
	uint32_t size;
} k_rw_request, * pk_rw_request;

typedef struct _k_alloc_mem_request {
	ULONG pid, allocation_type, protect;
	ULONGLONG addr;
	SIZE_T size;
} k_alloc_mem_request, * pk_alloc_mem_request;

typedef struct _k_protect_mem_request {
	ULONG pid, protect;
	ULONGLONG addr;
	SIZE_T size;
} k_protect_mem_request, * pk_protect_mem_request;

class kernelmode_proc_handler {
	HANDLE handle;
	uint32_t pid;
	uint32_t this_process_pid;
public:
	kernelmode_proc_handler();

	~kernelmode_proc_handler();

	bool is_attached();

	bool attach(const char* proc_name);

	uint64_t get_module_base(const std::string& module_name) ;

	void read_memory(uint64_t src_addr, uint64_t dst_addr, size_t size) ;

	void write_memory(uint64_t dst_addr, uint64_t src_addr, size_t size) ;

	uint32_t virtual_protect(uint64_t address, size_t size, uint32_t protect) ;

	uint64_t virtual_alloc(size_t size, uint32_t allocation_type, uint32_t protect, uint64_t address = NULL) ;

	

	template <typename type>
	type readmem(uint64_t src) {
		type ret = {};
		read_memory(src, (uint64_t)&ret, sizeof(type));
		return ret;
	}

	template <typename type>
	void writemem(uint64_t dst_addr, type var) {
		type inp = var;
		write_memory(dst_addr, (uint64_t)&inp, sizeof(type));
	}

};



extern kernelmode_proc_handler kernelHandler;

#define read(src_addr,type)  kernelHandler.readmem<type>(src_addr)
#define write(dst_addr,var,type)  kernelHandler.writemem<type>(dst_addr,var)
