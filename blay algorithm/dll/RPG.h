#ifndef RPG_H
#define RPG_H

#ifdef _WIN32
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __attribute((visibility("default")))
#endif

extern "C"
{
    DLL_EXPORT void RPG(int rowInput, int columnInput, char* seedInput, char* output, int outputSize);
}

#endif