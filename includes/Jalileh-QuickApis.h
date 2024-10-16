#pragma once

using pbyte = unsigned char *;
using pchar = char *;
using pvoid = void *;

#include "../includes/pe.h"
#include <Memoryapi.h>


namespace quickMess {

   template < typename T >
   T PullFuncFromModule (pchar FunctionName, pchar Module) {
      return T(pe::pe(LoadLibraryA(FunctionName)).get_export(FunctionName).address);
   }

   template < typename T >
   T PullFuncFromModuleEx (pchar FunctionName, pchar Module, pvoid *save_mod) {
      auto mod = pbyte(LoadLibraryA(Module));
      T func_ret = pe::pe(HMODULE(mod)).get_export(FunctionName);
      *reinterpret_cast< pbyte * >(save_mod) = (save_mod) ? mod : nullptr;
      return func_ret;
   }

   typedef long(__stdcall *_ReadProcessMemory)(
      HANDLE hProcess,
      LPCVOID lpBaseAddress,
      LPVOID lpBuffer,
      SIZE_T nSize,
      SIZE_T *lpNumberOfBytesRead
   );


   typedef BOOL (*_WriteProcessMemory)(
      _In_ HANDLE hProcess,
      _In_ LPVOID lpBaseAddress,
      _In_reads_bytes_(nSize) LPCVOID lpBuffer,
      _In_ SIZE_T nSize,
      _Out_opt_ SIZE_T *lpNumberOfBytesWritten
   );
   
   

  

// yea sorry about this below, I encrypt my strings whenever i load stuff so im used to loading at RT
  inline _WriteProcessMemory CCWriteProcessMemory () {
   static  _WriteProcessMemory wpm = 0;
   if (wpm) {
      return wpm;
   }
   else {
      wpm =  PullFuncFromModule< _WriteProcessMemory >("WriteProcessMemory", "Kernel32.dll");
      return wpm;
   }
}

 inline  _ReadProcessMemory CCReadProcessMemory () {
   
   static  _ReadProcessMemory rpm = 0;
   if (rpm) {
      return rpm;
   }
   else {
      rpm =  PullFuncFromModule< _ReadProcessMemory >("ReadProcessMemory", "Kernel32.dll");
      return rpm;
   }
}
}   // namespace quickMess
 

 
using namespace quickMess;