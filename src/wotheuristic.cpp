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

    //.text:0100C140                 push    ebp
    //.text:0100C141                 mov     ebp, esp
    //.text:0100C143                 sub     esp, 14h
    //.text:0100C146                 cmp     byte_1FCEE80, 0
    //.text:0100C14D                 push    esi

    // ...

    //  byte_1FCEE80                 db 1                    ; DATA XREF: sub_1009730:loc_1009756r
    //.data:01FCEE80                                         ; sub_100C140+6r
    //.data:01FCEE81 byte_1FCEE81    db 1                    ; DATA XREF: sub_1009730:loc_10097DBr
    //.data:01FCEE81                                         ; sub_100C2C0+6r
    //.data:01FCEE82 byte_1FCEE82    db 1                    ; DATA XREF: sub_FF9AA0+1Dr
    //.data:01FCEE82                                         ; sub_1009730:loc_1009902r ...
    //.data:01FCEE83 byte_1FCEE83    db 1                    ; DATA XREF: sub_FF9AA0+72r
    //.data:01FCEE83                                         ; sub_FFA060+AFr ...
    //.data:01FCEE84 byte_1FCEE84    db 1                    ; DATA XREF: sub_1009710r
    //.data:01FCEE85 byte_1FCEE85    db 1                    ; DATA XREF: sub_FF9AA0+10r
    //.data:01FCEE85                                         ; sub_FFB6A0+4r ...
    //.data:01FCEE86                 db 1
    //.data:01FCEE87 byte_1FCEE87    db 1                    ; DATA XREF: .text:0067F7C6w
    //.data:01FCEE87                                         ; sub_67F7D0+47w ...
    //.data:01FCEE88 dword_1FCEE88   dd 43C80000h            ; DATA XREF: sub_FFA380+1E9r
    //.data:01FCEE88                                         ; sub_FFBBB0+2A8r ...
    //.data:01FCEE8C dword_1FCEE8C   dd 3F800000h            ; DATA XREF: sub_FFCFA0+4r
    //.data:01FCEE8C                                         ; sub_FFCFA0+19w
    //.data:01FCEE90 dword_1FCEE90   dd 0C0000000h           ; DATA XREF: sub_FF8DC0+183r
    //.data:01FCEE90                                         ; sub_FFBBB0+30Er ...
    //.data:01FCEE94 dword_1FCEE94   dd 42480000h            ; DATA XREF: sub_FF8DC0+2DBr
    //.data:01FCEE94                                         ; sub_FFBBB0+371r ...
    //.data:01FCEE98 dword_1FCEE98   dd 43960000h            ; DATA XREF: sub_FF8DC0+301r
    //.data:01FCEE98                                         ; sub_FFBBB0+3E0r ...
    //.data:01FCEE9C dword_1FCEE9C   dd 43020000h            ; DATA XREF: sub_FF8DC0+3B7r
    //.data:01FCEE9C                                         ; sub_FFBBB0+452r ...
    //.data:01FCEEA0 dword_1FCEEA0   dd 3F800000h            ; DATA XREF: sub_FF8DC0+3C7r
    //.data:01FCEEA0                                         ; sub_FFBBB0+4C4r ...
    //.data:01FCEEA4 a333?           db '=¦¦>333?',0         ; DATA XREF: sub_FF8DC0+513o
    //.data:01FCEEA4                                         ; sub_FF8DC0+522r ...
    //

    int step = 2;

    for (unsigned int i=0;i<length - 10;++i)
    {
        unsigned char *pMemory = memory + i;
        if (*(pMemory + 0)==0x55 && *(pMemory + 1)==0x8B && *(pMemory + 2)==0xEC && *(pMemory + 3)==0x83 && *(pMemory + 4)==0xec && *(pMemory + 5)==0x14 && *(pMemory + 6)==0x80 && *(pMemory + 7)==0x3d && *(pMemory + 12)==0x00)
        {
            if (--step!=0)
            {
                 i += 16;
                continue;
            }
            return *(unsigned char  **)(memory + i + 8) - 5;
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

    tundraBase = getTundraBase(&binaryContent.front(), binaryContent.size());
    sniperBase = getSniperBase(&binaryContent.front(), binaryContent.size());

    return true;
}