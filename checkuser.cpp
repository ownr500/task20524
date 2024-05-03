#pragma comment(lib, "netapi32.lib")
#include <napi.h>
#include <assert.h>
#include <windows.h>
#include <lm.h>
#include <locale>
#include <codecvt>

Napi::Array GetLocalGroups(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring username = converter.from_bytes(info[0].As<Napi::String>().Utf8Value());

    LPLOCALGROUP_USERS_INFO_0 pBuf = NULL;
    DWORD dwLevel = 0;
    DWORD dwFlags = LG_INCLUDE_INDIRECT;
    DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    LPCWSTR server = L"localhost";
    NET_API_STATUS nStatus;

    nStatus = NetUserGetLocalGroups(server,
                                    (LPCWSTR)username.c_str(),
                                    dwLevel,
                                    dwFlags,
                                    (LPBYTE*)&pBuf,
                                    dwPrefMaxLen,
                                    &dwEntriesRead,
                                    &dwTotalEntries);

    Napi::Array result = Napi::Array::New(env);

    if (nStatus == NERR_Success) {
        LPLOCALGROUP_USERS_INFO_0 pTmpBuf;
        DWORD i;

        if ((pTmpBuf = pBuf) != NULL) {
            for (i = 0; i < dwEntriesRead; i++) {
                assert(pTmpBuf != NULL);
                std::wstring groupName = pTmpBuf->lgrui0_name;
                result.Set(i, Napi::String::New(env, converter.to_bytes(groupName)));
                pTmpBuf++;
            }
        }
    }

    if (pBuf != NULL)
        NetApiBufferFree(pBuf);

    return result;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports["getLocalGroups"] = Napi::Function::New(env, GetLocalGroups);
    return exports;
}

NODE_API_MODULE(addon, Init)
