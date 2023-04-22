#include <Windows.h>

#include <TlHelp32.h>
#include <d2d1_2.h>
#include <d3d11_2.h>
#include <dcomp.h>
#include <dwrite.h>
#include <dxgi1_3.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dcomp")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "winmm")

#include <algorithm>
#include <cstdio>
#include <vector>

typedef struct {
  ID2D1DeviceContext *d2DC;
  IDXGISwapChain1 *dxSC;
  IDWriteFactory *dwF;
  IDWriteTextFormat *dwTF;
  ID2D1SolidColorBrush *brush;
  float fw, fh, w, h, b;
} DrawData;

typedef struct {
  int curHp;
  int maxHp;
} Enemy;

typedef struct {
  bool captured;
  HANDLE hProcess;
  MODULEENTRY32 me;

  int InventoryManager;
  int GameRankSystem;
  int GameStatsManager;
  int CharacterManager;

  int ptas;
  int gameRank;
  int killCount;
  std::vector<Enemy> enemies;
} GameData;

bool initWindow(HINSTANCE hInstance, HWND &hWnd, DrawData &dd);
bool initHandle(GameData &gd);
void Render(DrawData &dd, GameData &gd);

template <typename T> bool ReadValue(GameData &gd, T &res, int offsets, ...) {
  bool found = true;
  va_list args;
  va_start(args, offsets);
  BYTE *p = gd.me.modBaseAddr;
  for (int i = 0; i < offsets - 1; i++)
    found = found && ReadProcessMemory(gd.hProcess, p + va_arg(args, int), &p,
                                       sizeof(BYTE *), nullptr);
  found = found && ReadProcessMemory(gd.hProcess, p + va_arg(args, int), &res,
                                     sizeof(T), nullptr);
  va_end(args);
  return found;
}

bool Update(GameData &gd) {
  bool found = true;
  found = found && ReadValue(gd, gd.ptas, 2, gd.InventoryManager, 0x10);
  found = found && ReadValue(gd, gd.gameRank, 2, gd.GameRankSystem, 0x10);
  found = found &&
          ReadValue(gd, gd.killCount, 4, gd.GameStatsManager, 0x20, 0x18, 0x14);
  gd.enemies.clear();
  int enemyCount = 0;
  found =
      found && ReadValue(gd, enemyCount, 3, gd.CharacterManager, 0xa8, 0x18);
  for (int i = 0; i < enemyCount; i++) {
    Enemy e{};
    found = found && ReadValue(gd, e.maxHp, 6, gd.CharacterManager, 0xa8, 0x10,
                               0x20 + 0x8 * i, 0x148, 0x10);
    found = found && ReadValue(gd, e.curHp, 6, gd.CharacterManager, 0xa8, 0x10,
                               0x20 + 0x8 * i, 0x148, 0x14);
    if (e.curHp > 0)
      gd.enemies.push_back(std::move(e));
  }
  std::sort(gd.enemies.begin(), gd.enemies.end(), [](Enemy &a, Enemy &b) {
    int dmgA = a.maxHp - a.curHp;
    int dmgB = b.maxHp - b.curHp;
    return dmgA > dmgB || (dmgA == dmgB && a.maxHp > b.maxHp);
  });
  return found;
}

void goTop(HWND hWnd, GameData &gd) {
  HWND hFG = GetForegroundWindow();
  DWORD pid;
  GetWindowThreadProcessId(hFG, &pid);
  if (pid == gd.me.th32ProcessID) {
    SetWindowPos(hWnd, GetWindow(hFG, GW_HWNDPREV), 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE);
  }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine,
                   int nCmdShow) {
  HWND hWnd;
  DrawData dd{};
  initWindow(hInstance, hWnd, dd);
  ShowWindow(hWnd, nCmdShow);

  GameData gd{};
  gd.captured = false;

  MSG msg;
  LARGE_INTEGER freq{};
  QueryPerformanceFrequency(&freq);
  LONGLONG nextAttempt = 0;
  timeBeginPeriod(1);
  do {
    LARGE_INTEGER frameStart{};
    QueryPerformanceCounter(&frameStart);
    auto frameEnd = frameStart.QuadPart + freq.QuadPart / 30;
    if (!gd.captured && frameStart.QuadPart > nextAttempt) {
      initHandle(gd);
      nextAttempt = frameStart.QuadPart + freq.QuadPart;
    }
    if (gd.captured)
      gd.captured = Update(gd);
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
      DispatchMessage(&msg);
    Render(dd, gd);
    dd.dxSC->Present(0, 0);
    goTop(hWnd, gd);
    LARGE_INTEGER frameTime{};
    QueryPerformanceCounter(&frameTime);
    auto delay = 5 * (600 * (frameEnd - frameTime.QuadPart) - freq.QuadPart) /
                 (3 * freq.QuadPart);
    if (delay > 0)
      Sleep(static_cast<DWORD>(delay));
    do {
      YieldProcessor();
      QueryPerformanceCounter(&frameTime);
    } while (frameTime.QuadPart < frameEnd);
  } while (msg.message != WM_QUIT);
  timeEndPeriod(1);

  return 0;
}

void drawText(DrawData &dd, const D2D1_POINT_2F &p, bool isRight, bool wSeps,
              const WCHAR *fmt, ...) {
  WCHAR b[64];
  va_list args;
  va_start(args, fmt);
  _vsnwprintf(b, 64, fmt, args);
  if (wSeps) {
    WCHAR c[64]{};
    int n = wcslen(b);
    for (int i = 0; i < n; i++) {
      if (i % 3 == 0)
        c[i + i / 3 - 1] = L',';
      c[i + i / 3] = b[n - 1 - i];
    }
    n = wcslen(c);
    for (int i = 0; i < n; i++)
      b[i] = c[n - 1 - i];
    b[n] = L'\0';
  }
  IDWriteTextLayout *layout;
  dd.dwTF->SetTextAlignment(isRight ? DWRITE_TEXT_ALIGNMENT_TRAILING
                                    : DWRITE_TEXT_ALIGNMENT_LEADING);
  dd.dwF->CreateTextLayout(b, wcslen(b), dd.dwTF, dd.w, dd.fh, &layout);
  dd.d2DC->DrawTextLayout(p, layout, dd.brush);
  layout->Release();
}

void Render(DrawData &dd, GameData &gd) {
  dd.d2DC->BeginDraw();
  dd.d2DC->Clear();
  dd.brush->SetColor(D2D1::ColorF(0x2e3440, 0.5f));
  dd.d2DC->FillRoundedRectangle(
      {{0.0f, 0.0f, 2.0f * dd.b + dd.w, 2.0f * dd.b + dd.h}, dd.b, dd.b},
      dd.brush);
  dd.brush->SetColor(D2D1::ColorF(0xeceff4));
  drawText(dd, {dd.b, dd.b}, true, true, L"%d", gd.ptas);
  drawText(dd, {dd.b, dd.b + dd.fh}, false, false, L"%d", gd.gameRank);
  drawText(dd, {dd.b, dd.b + dd.fh}, true, false, L"%d", gd.killCount);
  for (int i = 0; i < min(gd.enemies.size(), 5); i++) {
    float bx = dd.b;
    float by = dd.b + (i + 2.3f) * dd.fh;
    float bw = 6.5f * dd.fw;
    float bh = 0.4f * dd.fh;
    float r = static_cast<float>(gd.enemies[i].curHp) /
              static_cast<float>(gd.enemies[i].maxHp);
    dd.brush->SetColor(D2D1::ColorF(0xa3be8c));
    dd.d2DC->FillRectangle({bx, by, bx + bw * r, by + bh}, dd.brush);
    dd.brush->SetColor(D2D1::ColorF(0x4c566a));
    dd.d2DC->DrawRectangle({bx, by, bx + bw, by + bh}, dd.brush);
    dd.brush->SetColor(D2D1::ColorF(0xeceff4));
    drawText(dd, {dd.b, dd.b + (i + 2) * dd.fh}, true, false, L"%d",
             gd.enemies[i].curHp);
  }
  dd.d2DC->EndDraw();
}

int SigScan(const char *p, const char *m, int ps, BYTE *t, int ts) {
  int i = 0;
  int j = 0;
  while (j < ts) {
    if ((m[i] == '?') || (static_cast<BYTE>(p[i]) == t[j])) {
      i++;
      j++;
    } else {
      i = 0;
      j = j - i + 1;
    }
    if (i == ps) {
      return j - ps;
    }
  }
  return -1;
}

bool initHandle(GameData &gd) {
  DWORD pid;
  bool found;
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe{};
  pe.dwSize = sizeof(PROCESSENTRY32);
  found = false;
  Process32First(hSnapshot, &pe);
  do {
    if (strcmp("re4.exe", pe.szExeFile) == 0) {
      pid = pe.th32ProcessID;
      found = true;
      break;
    }
  } while (Process32Next(hSnapshot, &pe));
  CloseHandle(hSnapshot);
  if (!found)
    return false;
  hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
  gd.me.dwSize = sizeof(MODULEENTRY32);
  Module32First(hSnapshot, &gd.me);
  found = false;
  do {
    if (strcmp("re4.exe", gd.me.szModule) == 0) {
      found = true;
      break;
    }
  } while (Module32Next(hSnapshot, &gd.me));
  CloseHandle(hSnapshot);
  if (!found)
    return false;
  gd.hProcess = OpenProcess(PROCESS_VM_READ, false, pid);
  if (gd.hProcess == nullptr)
    return false;
  BYTE *mem = new BYTE[gd.me.modBaseSize];
  found = found && ReadProcessMemory(gd.hProcess, gd.me.modBaseAddr, mem,
                                     gd.me.modBaseSize, nullptr);
  int x = SigScan("\x4c\x8b\xc0\x48\x8b\x15\x00\x00\x00\x00\x48\x8b\xcf\xe8",
                  "xxxxxx????xxxx", 14, mem, gd.me.modBaseSize);
  found = found && (x != -1);
  gd.InventoryManager = x + 10 + *reinterpret_cast<int *>(mem + x + 6);
  x = SigScan("\x4c\x8b\x47\x30\x48\x8b\xcd\x48\x8b\x15\x00\x00\x00\x00\xe8",
              "xxxxxxxxxx????x", 15, mem, gd.me.modBaseSize);
  found = found && (x != -1);
  gd.GameRankSystem = x + 14 + *reinterpret_cast<int *>(mem + x + 10);
  x = SigScan(
      "\x48\x8b\xd9\x48\x8b\x15\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x85\xc0",
      "xxxxxx????x????xx", 17, mem, gd.me.modBaseSize);
  found = found && (x != -1);
  gd.GameStatsManager = x + 10 + *reinterpret_cast<int *>(mem + x + 6);
  x = SigScan("\x48\x8b\xf8\x48\x8b\x15\x00\x00\x00\x00\x48\x8b\xcb\xe8",
              "xxxxxx????xxxx", 14, mem, gd.me.modBaseSize);
  found = found && (x != -1);
  gd.CharacterManager = x + 10 + *reinterpret_cast<int *>(mem + x + 6);
  delete[] mem;
  gd.captured = found;
  return found;
}

LRESULT MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_NCHITTEST: {
    LRESULT x = DefWindowProc(hWnd, uMsg, wParam, lParam);
    return (x == HTCLIENT) ? HTCAPTION : x;
  }
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool initWindow(HINSTANCE hInstance, HWND &hWnd, DrawData &dd) {
  DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dd.dwF),
                      reinterpret_cast<IUnknown **>(&dd.dwF));
  dd.dwF->CreateTextFormat(L"Verdana", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
                           DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
                           24.0f, L"en-us", &dd.dwTF);
  IDWriteTextLayout *layout;
  dd.fw = 0.0f;
  dd.fh = 0.0f;
  for (int i = 0; i < 10; i++) {
    WCHAR s[4];
    _snwprintf(s, 4, L"%d", i);
    dd.dwF->CreateTextLayout(s, wcslen(s), dd.dwTF, 300, 300, &layout);
    DWRITE_TEXT_METRICS mets{};
    layout->GetMetrics(&mets);
    dd.fw = max(dd.fw, mets.width);
    dd.fh = max(dd.fh, mets.height);
    layout->Release();
  }
  dd.w = 12.0f * dd.fw;
  dd.h = 7.0f * dd.fh;
  dd.b = 10.0f;
  WNDCLASSEX wcx{};
  wcx.cbSize = sizeof(WNDCLASSEX);
  wcx.lpfnWndProc = MainWndProc;
  wcx.hInstance = hInstance;
  wcx.lpszClassName = "MainWClass";
  RegisterClassEx(&wcx);
  hWnd = CreateWindowEx(WS_EX_NOREDIRECTIONBITMAP, "MainWClass", "re4r srt",
                        WS_POPUP | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
                        dd.w + 2.0f * dd.b, dd.h + 2.0f * dd.b, nullptr,
                        nullptr, hInstance, nullptr);
  ID3D11Device *d3Device;
  D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                    D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0,
                    D3D11_SDK_VERSION, &d3Device, nullptr, nullptr);
  IDXGIDevice *dxDevice;
  d3Device->QueryInterface(&dxDevice);
  IDXGIFactory2 *dxFactory;
  CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, __uuidof(dxFactory),
                     reinterpret_cast<void **>(&dxFactory));
  DXGI_SWAP_CHAIN_DESC1 desc{};
  desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
  desc.BufferCount = 2;
  desc.SampleDesc.Count = 1;
  desc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
  RECT rect;
  GetClientRect(hWnd, &rect);
  desc.Width = rect.right - rect.left;
  desc.Height = rect.bottom - rect.top;
  dxFactory->CreateSwapChainForComposition(dxDevice, &desc, nullptr, &dd.dxSC);
  ID2D1Factory2 *d2Factory;
  D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2Factory);
  ID2D1Device1 *d2Device;
  d2Factory->CreateDevice(dxDevice, &d2Device);
  d2Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &dd.d2DC);
  IDXGISurface2 *surface;
  dd.dxSC->GetBuffer(0, __uuidof(surface), reinterpret_cast<void **>(&surface));
  D2D1_BITMAP_PROPERTIES1 props{};
  props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
  props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
  props.bitmapOptions =
      D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
  ID2D1Bitmap1 *bitmap;
  dd.d2DC->CreateBitmapFromDxgiSurface(surface, props, &bitmap);
  dd.d2DC->SetTarget(bitmap);
  IDCompositionDevice *dcompDevice;
  DCompositionCreateDevice(dxDevice, __uuidof(dcompDevice),
                           reinterpret_cast<void **>(&dcompDevice));
  IDCompositionTarget *target;
  dcompDevice->CreateTargetForHwnd(hWnd, true, &target);
  IDCompositionVisual *visual;
  dcompDevice->CreateVisual(&visual);
  visual->SetContent(dd.dxSC);
  target->SetRoot(visual);
  dcompDevice->Commit();
  dd.d2DC->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &dd.brush);

  return true;
}
