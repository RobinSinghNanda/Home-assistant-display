#ifndef NVM_HELPER_HPP_
#define NVM_HELPER_HPP_
class NvmHelper
{
    public:
        static void NvmLoad(const char *sNvsName, const char *sName, void *pSettings, unsigned nSettingsLen);
        static void NvmSave(const char *sNvsName, const char *sName, const void *pSettings, unsigned nSettingsLen);
        static void NvmErase(const char *sNvsName);
};

#endif