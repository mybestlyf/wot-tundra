#ifndef WOTCHEATER_H
#define WOTCHEATER_H

#include <vector>

#include <windows.h>

#define WOT_SET_MODE 1
#define WOT_TUNDRA_ENABLE 2
#define WOT_TUNDRA_THRUNKS_ONLY 4
#define WOT_TUNDRA_SNIPER_MODE 8

class WOTCheater
{
public:
    WOTCheater();
    ~WOTCheater();

    bool updateTundraState(std::vector<unsigned char> & tundraState);
    std::vector<unsigned char> getTundraState();

    void setTrunkMode(bool mode);
    void setSniperMode(bool mode);

    bool isTrunkMode()
    {
        if (m_flags & WOT_TUNDRA_THRUNKS_ONLY)
            return true;
        else
            return false;
    }
    bool isSniperMode(){
        if (m_flags & WOT_TUNDRA_SNIPER_MODE)
            return true;
        else
            return false;
    }

    void setState(bool state);
    bool getState()
    { 
        if (m_flags & WOT_TUNDRA_ENABLE)
            return true;
        else
            return false;
    }

    void setTrunksTundra();
    void setTundra();
    void cleanTundra();

    bool queryCheatStatus();

private:
    bool probeToOpenWOTprocess();

    bool isInSniperMode();

    unsigned char *m_baseAddress;
    unsigned char *m_sniperAddress;

    DWORD m_tankprocessId;
    HANDLE m_hTankProcess;

    unsigned long m_flags;
    unsigned long m_newFlags;

    int m_oldCurrentModeIsSniper;
};

#endif // WOTCHEATER_H
