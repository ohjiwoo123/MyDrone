#pragma once

extern "C" __declspec(dllimport) BOOL InitMyDev(int nLevel);
extern "C" __declspec(dllimport) int GetMyDevError(void);
extern "C" __declspec(dllimport) BOOL GetMyDevPosition(int* x, int* y);
extern "C" __declspec(dllimport) void ReleaseMyDev(void);