#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <thread>
#include <string>
#include <atomic>

#include <bitset> //для побитового вывода
#include <climits>//для побитового вывода


#define _DBG_

#define BITOUT(x) std::cout << std::bitset<sizeof(x) * CHAR_BIT>(x) << "\n"; 

#define LOG(str, ...) fprintf(stdout, str "\n", ##__VA_ARGS__)

template <typename... Args>
void DBG(const char* format, Args... args)
{
#ifdef _DBG_
	printf("[DEBUG]");
	printf(format, args...);
	printf("\n");
#endif 
}

#define frame_rate(target)\
{\
static DWORD fps;\
static DWORD total = 0;\
static DWORD frames = 0;\
static DWORD start = 0;\
frames++;\
total += GetTickCount() - start;\
if (total >= 500)\
{\
	*(short*)target = frames * 1000 / total; frames = 0; total = 0;\
}\
start = GetTickCount();\
}\




uint32_t get_process_pid(const char* process_name);
char* randomStrGen(int length);
void GetDesktopResolution(int& horizontal, int& vertical);