#include "wotcheater.h"

#include "wotheuristic.h"

#include <vector>

WOTCheater::WOTCheater():
m_tankprocessId(0),
m_hTankProcess(0),
m_flags(0),
m_newFlags(0),
m_oldCurrentModeIsSniper(-1),
m_baseAddress(NULL),
m_sniperAddress(NULL)
{

}

WOTCheater::~WOTCheater()
{
    if (m_hTankProcess!=0)
    {
        cleanTundra();
        ::CloseHandle(m_hTankProcess);
    }
}



bool 
WOTCheater::probeToOpenWOTprocess()
{
    HWND tankMainWndHandle = ::FindWindowW(L"App", L"WoT Client");
    if (tankMainWndHandle==NULL)
        return false;

    m_tankprocessId = 0;
    ::GetWindowThreadProcessId(tankMainWndHandle, &m_tankprocessId);
    if (m_tankprocessId==0)
        return false;

    m_hTankProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, m_tankprocessId);

    if (m_hTankProcess==NULL)
        return false;

    wotEuristic(m_hTankProcess, m_baseAddress, m_sniperAddress);

    return true;
}

std::vector<unsigned char>
WOTCheater::getTundraState()
{
    std::vector<unsigned char> tundraState;
    tundraState.resize(8); // size of the state;

    SIZE_T bytesReaded = 0;
    ::ReadProcessMemory(m_hTankProcess, m_baseAddress, &tundraState.front(), tundraState.size(), &bytesReaded);

    return tundraState;
}

bool
WOTCheater::updateTundraState(std::vector<unsigned char> & tundraState)
{
    SIZE_T bytesWritten = 0;
    ::WriteProcessMemory(m_hTankProcess, m_baseAddress, &tundraState.front(), tundraState.size(), &bytesWritten);

    return bytesWritten==tundraState.size();
}

bool
WOTCheater::isInSniperMode()
{
    unsigned char oneByte;
    SIZE_T bytesReaded = 0;
    ::ReadProcessMemory(m_hTankProcess, m_sniperAddress, &oneByte, sizeof(oneByte), &bytesReaded);

    if (bytesReaded==sizeof(oneByte) && oneByte==1)
        return true;
    else
        return false;
}

void 
WOTCheater::setTrunkMode(bool mode)
{
    m_newFlags = m_flags;

    if (mode)
        m_newFlags |= WOT_TUNDRA_THRUNKS_ONLY;
    else
        m_newFlags &= ~WOT_TUNDRA_THRUNKS_ONLY;

    m_newFlags |= WOT_SET_MODE;
}

void
WOTCheater::setTrunksTundra()
{
    std::vector<unsigned char> premiumState;
	premiumState.push_back(1);
	premiumState.push_back(0);
	premiumState.push_back(1);
	premiumState.push_back(0);
	premiumState.push_back(0);
	premiumState.push_back(0);
	premiumState.push_back(0);
	premiumState.push_back(0);
    updateTundraState(premiumState);
}

void
WOTCheater::setTundra()
{
    std::vector<unsigned char> premiumState;
    premiumState.push_back(0);
    premiumState.push_back(0);
    premiumState.push_back(0);
    premiumState.push_back(0);
    premiumState.push_back(0);
    premiumState.push_back(0);
    premiumState.push_back(0);
    premiumState.push_back(0);
    updateTundraState(premiumState);
}

void
WOTCheater::cleanTundra()
{
    std::vector<unsigned char> premiumState;
    premiumState.push_back(1);
    premiumState.push_back(1);
    premiumState.push_back(1);
    premiumState.push_back(1);
    premiumState.push_back(1);
    premiumState.push_back(1);
    premiumState.push_back(1);
    premiumState.push_back(1);

    updateTundraState(premiumState);
}

bool
WOTCheater::queryCheatStatus()
{
    bool status = false;
    if (m_hTankProcess!=NULL)
    {
        DWORD exitCode;
        if (::GetExitCodeProcess(m_hTankProcess, &exitCode) && exitCode==STILL_ACTIVE)
        {
            if (m_newFlags & WOT_SET_MODE)
            {
                m_flags = m_newFlags & ~WOT_SET_MODE;

                if (m_flags & WOT_TUNDRA_ENABLE && !(m_flags & WOT_TUNDRA_SNIPER_MODE))
                {
                    if (m_flags & WOT_TUNDRA_THRUNKS_ONLY)
                    {
                        setTrunksTundra();
                    }
                    else
                    {
                        setTundra();
                    }
                }
                else
                {
                    cleanTundra();
                }
                
                m_newFlags = 0;
            }

            if (m_flags & WOT_TUNDRA_SNIPER_MODE)
            {
                int currentModeIsSniper = isInSniperMode()?1:0;

                if (currentModeIsSniper != m_oldCurrentModeIsSniper)
                {
                    m_oldCurrentModeIsSniper = currentModeIsSniper;

                    if (currentModeIsSniper)
                    {
                        if (m_flags & WOT_TUNDRA_THRUNKS_ONLY)
                        {
                            setTrunksTundra();
                        }
                        else
                        {
                            setTundra();
                        }
                    }
                    else
                    {
                        cleanTundra();
                    }
                }
            }

            status = true;
        }
        else
        {
            ::CloseHandle(m_hTankProcess);
			m_newFlags = m_flags; // probe to restore state
			m_flags = 0;
            m_hTankProcess = NULL;
        }
    }
    else
    {
        status = probeToOpenWOTprocess();
    }

    return status;
}

void
WOTCheater::setState(bool state)
{
    m_newFlags = m_flags;

    if (state)
        m_newFlags |= WOT_TUNDRA_ENABLE;
    else
        m_newFlags &= ~WOT_TUNDRA_ENABLE;

    m_newFlags |= WOT_SET_MODE;
    m_oldCurrentModeIsSniper = -1;

}

void
WOTCheater::setSniperMode(bool mode)
{
    m_newFlags = m_flags;

    if (mode)
        m_newFlags |= WOT_TUNDRA_SNIPER_MODE;
    else
        m_newFlags &= ~WOT_TUNDRA_SNIPER_MODE;

    m_newFlags |= WOT_SET_MODE;
    m_oldCurrentModeIsSniper = -1;
}

