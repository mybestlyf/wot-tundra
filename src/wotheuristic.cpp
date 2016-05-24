#include "wotheuristic.h"
#include <Windows.h>

#include <string>
#include <fstream>

#include <psapi.h>

#include <stdint.h>

#include <vector>

typedef DWORD (WINAPI * pGetModuleFileNameEx)(HANDLE, HMODULE, LPTSTR, DWORD);

unsigned char  * getTundraBase(unsigned char *memory, unsigned int length)
{
    // WOTDrawManager object

	//.text:0150E141                   mov     dword ptr[esi + 28h], 7F7FFFFFh
	//.text : 0150E148                 mov     dword ptr[esi + 2Ch], 7F7FFFFFh
	//.text : 0150E14F                 mov     dword ptr[esi + 30h], 7F7FFFFFh
	//.text : 0150E156                 mov     dword ptr[esi + 1Ch], 0
	//.text : 0150E15D                 mov     dword ptr[esi + 20h], 0
	//.text : 0150E164                 mov     dword ptr[esi + 24h], 0
	//.text : 0150E16B                 mov     dword ptr[esi + 34h], 0FF7FFFFFh
	//.text : 0150E172                 mov     dword ptr[esi + 38h], 0FF7FFFFFh
	//.text : 0150E179                 mov     dword ptr[esi + 3Ch], 0FF7FFFFFh
	//.text : 0150E180                 mov     dword ptr[esi + 40h], 0
	//.text : 0150E187                 mov     dword ptr[esi + 44h], 0
	//.text : 0150E18E                 mov     eax, esi
	//.text : 0150E190                 mov     byte_1E74FA8, 0


    // ...

	//.data:01E74FA8 byte_1E74FA8    db 1                    
	//.data:01E74FA8                                         
	//.data:01E74FA9 byte_1E74FA9    db 1                    
	//.data:01E74FA9                                         
	//.data:01E74FAA byte_1E74FAA    db 1                    
	//.data:01E74FAA                                         
	//.data:01E74FAB byte_1E74FAB    db 1                    
	//.data:01E74FAB                                         
	//.data:01E74FAC byte_1E74FAC    db 1                    
	//.data:01E74FAC                                         
	//.data:01E74FAD byte_1E74FAD    db 1                    
	//.data:01E74FAD                                         
	//.data:01E74FAE byte_1E74FAE    db 1                    
	//.data:01E74FAE                                         
	//.data:01E74FAF byte_1E74FAF    db 1                    
	//.data:01E74FAF                                         
	//.data:01E74FB0 byte_1E74FB0    db 1                    
	//.data:01E74FB1 byte_1E74FB1    db 1                    
	//.data:01E74FB1                                         
	//.data:01E74FB2                 db    1
	//.data:01E74FB3                 db    0
	//.data:01E74FB4 dword_1E74FB4   dd 43C80000h            
	//.data:01E74FB4                                         
	//.data:01E74FB8 dword_1E74FB8   dd 3F800000h            


    int step = 2;

    for (unsigned int i=0;i<length - 40;++i)
    {

		// C7 46 ?? FF FF 7F FF C7 46 ?? FF FF 7F FF C7 46 ?? FF FF 7F FF C7 46 ?? 00 00 00 00 C7 46 ?? 00 00 00 00 8B C6 C6 05 A8 <address>


        unsigned char *pMemory = memory + i;
        if (*(pMemory + 0)==0xC7 
			&& *(pMemory + 1)==0x46 
			&& *(pMemory + 3)==0xFF 
			&& *(pMemory + 4)==0xFF 
			&& *(pMemory + 5)==0x7F
			&& *(pMemory + 6) == 0xFF			
			&& *(pMemory + 7) == 0xC7
			&& *(pMemory + 8) == 0x46
			&& *(pMemory + 10) == 0xFF
			&& *(pMemory + 11) == 0xFF
			&& *(pMemory + 12) == 0x7F
			&& *(pMemory + 13) == 0xFF
			&& *(pMemory + 14) == 0xC7
			&& *(pMemory + 15) == 0x46
			&& *(pMemory + 17) == 0xFF
			&& *(pMemory + 18) == 0xFF
			&& *(pMemory + 19) == 0x7F
			&& *(pMemory + 20) == 0xFF
			&& *(pMemory + 21) == 0xC7
			&& *(pMemory + 22) == 0x46
			&& *(pMemory + 24) == 0x00
			&& *(pMemory + 25) == 0x00
			&& *(pMemory + 26) == 0x00
			&& *(pMemory + 27) == 0x00
			&& *(pMemory + 28) == 0xC7
			&& *(pMemory + 29) == 0x46
			&& *(pMemory + 31) == 0x00
			&& *(pMemory + 32) == 0x00
			&& *(pMemory + 33) == 0x00
			&& *(pMemory + 34) == 0x00
			&& *(pMemory + 35) == 0x8b
			&& *(pMemory + 36) == 0xc6
			&& *(pMemory + 37) == 0xc6
			)
        {
            return *(unsigned char  **)(pMemory + 39) + 2;
        }
    }

    return NULL;
}

unsigned char  * getSniperBase(unsigned char *memory, unsigned int length)
{
    //    .text:00D0EDFE                 cmp     [m_isSniperMode], 0
    //    .text:00D0EE05                 movss   xmm1, dword ptr [esi+30h]
    //    .text:00D0EE0A                 movss   xmm2, dword ptr [esi+34h]
    //    .text:00D0EE0F                 subss   xmm2, dword ptr [eax+2D8h]
    //    .text:00D0EE17                 subss   xmm1, dword ptr [eax+2D4h]
    //    .text:00D0EE1F                 movss   xmm0, dword ptr [esi+38h]
    //    .text:00D0EE24                 subss   xmm0, dword ptr [eax+2DCh]

    for (unsigned int i=0;i<length - 10;++i)
    {
        unsigned char *pMemory = memory + i;
        if (*(pMemory)==0xF3
            && *(pMemory + 1)==0x0F
            && *(pMemory + 4)==0x30
            && *(pMemory + 5)==0xF3
            && *(pMemory + 6)==0x0F
            && *(pMemory + 9)==0x34
            && *(pMemory + 26)==0xF3
            && *(pMemory + 27)==0x0F
            && *(pMemory + 30)==0x38
            && *(pMemory + 39)==0xF3
            && *(pMemory + 40)==0x0F
            && *(pMemory + 41)==0x59
            && *(pMemory + 42)==0xD2
            && *(pMemory + 43)==0xF3
            && *(pMemory + 44)==0x0F
            && *(pMemory + 45)==0x59
            && *(pMemory + 46)==0xC9)
        {
            return *(unsigned char  **)(memory + i -  5);
        }
    }

    return NULL;
}

bool wotEuristic(HANDLE hProcess, unsigned char * & tundraBase, unsigned char * & sniperBase)
{
    WCHAR filename[MAX_PATH] = {0};

    HMODULE hPsApi = ::LoadLibraryA( "PSAPI.DLL" ) ;

    pGetModuleFileNameEx GetModuleFileNameExFn = (pGetModuleFileNameEx)GetProcAddress(hPsApi, "GetModuleFileNameExW");

    if (GetModuleFileNameExFn && GetModuleFileNameExFn(hProcess, NULL, filename, MAX_PATH) == 0)
    {
          return false;
    }

    HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return false;

    DWORD fileSize = 0;
    fileSize = GetFileSize(hFile, NULL);

    std::vector<unsigned char> binaryContent;
    binaryContent.resize(fileSize);

    DWORD nor;
    ::ReadFile(hFile, &binaryContent.front(), fileSize, &nor, NULL);
    ULONG err = ::GetLastError();
    ::CloseHandle(hFile);

	tundraBase = (unsigned char *)getTundraBase(&binaryContent.front(), binaryContent.size()); // 0x01E74FAA
    sniperBase = getSniperBase(&binaryContent.front(), binaryContent.size());

    return true;
}