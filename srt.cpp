#include <Windows.h>

#include <TlHelp32.h>
#include <d2d1_2.h>
#include <d3d11.h>
#include <dcomp.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dcomp")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "winmm")

#include <algorithm>
#include <memory>
#include <vector>

typedef struct {
  IDWriteFactory *dwFac;
  IDWriteTextFormat *dwFmt;
  IDXGISwapChain1 *sc;
  ID2D1DeviceContext *dc;
  ID2D1SolidColorBrush *brush;
  HWND hWnd;
  float fw, fh, w, h, b;
} DrawData;

typedef struct {
  int32_t curHp;
  int32_t maxHp;
  byte *addr;
} Enemy;

typedef struct {
  HANDLE hProcess;
  byte *modAddr;
  bool isFocus;
  uint32_t InventoryManager;
  uint32_t GameRankSystem;
  uint32_t GameStatsManager;
  uint32_t CharacterManager;
  int32_t ptas;
  int32_t gameRank;
  int32_t killCount;
  std::vector<Enemy> enemies;
} GameData;

bool initWindow(DrawData &dd, HINSTANCE hInstance);
bool initHandle(GameData &gd);
bool update(GameData &gd);
void render(DrawData &dd, GameData &gd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine,
                   int nCmdShow) {
  DrawData dd{};
  initWindow(dd, hInstance);
  ShowWindow(dd.hWnd, nCmdShow);

  GameData gd{};
  gd.hProcess = nullptr;

  MSG msg;
  LARGE_INTEGER pcFrq{};
  QueryPerformanceFrequency(&pcFrq);
  auto style = GetWindowLongPtr(dd.hWnd, GWL_EXSTYLE);
  timeBeginPeriod(1);
  do {
    LARGE_INTEGER t0{};
    QueryPerformanceCounter(&t0);
    auto t1 = t0.QuadPart + pcFrq.QuadPart / 30;
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
      DispatchMessage(&msg);
    if (!update(gd))
      initHandle(gd);
    if (!(style & WS_EX_TRANSPARENT) != (GetForegroundWindow() == dd.hWnd)) {
      style ^= WS_EX_TRANSPARENT;
      SetWindowLongPtr(dd.hWnd, GWL_EXSTYLE, style);
    }
    gd.isFocus = !(style & WS_EX_TRANSPARENT);
    render(dd, gd);
    dd.sc->Present(1, 0);
    LARGE_INTEGER dt{};
    QueryPerformanceCounter(&dt);
    auto delay =
        5 * (600 * (t1 - dt.QuadPart) - pcFrq.QuadPart) / (3 * pcFrq.QuadPart);
    if (delay > 0)
      Sleep(static_cast<DWORD>(delay));
    do {
      YieldProcessor();
      QueryPerformanceCounter(&dt);
    } while (dt.QuadPart < t1);
  } while (msg.message != WM_QUIT);
  timeEndPeriod(1);

  return 0;
}

void drawText(DrawData &dd, const D2D1_POINT_2F &p, uint32_t flags,
              const wchar_t *fmt, ...) {
  wchar_t b[64];
  va_list args;
  va_start(args, fmt);
  _vsnwprintf(b, 64, fmt, args);
  va_end(args);
  if (flags & 0b10) {
    wchar_t c[64]{};
    size_t n = wcslen(b);
    for (size_t i = 0; i < n; i++) {
      if (i % 3 == 0)
        c[i + i / 3 - 1] = L',';
      c[i + i / 3] = b[n - 1 - i];
    }
    n = wcslen(c);
    for (int i = 0; i < n; i++)
      b[i] = c[n - 1 - i];
    b[n] = L'\0';
  }
  IDWriteTextLayout *lyt = nullptr;
  dd.dwFmt->SetTextAlignment((flags & 1) ? DWRITE_TEXT_ALIGNMENT_TRAILING
                                         : DWRITE_TEXT_ALIGNMENT_LEADING);
  dd.dwFac->CreateTextLayout(b, static_cast<uint32_t>(wcslen(b)), dd.dwFmt,
                             dd.w, dd.fh, &lyt);
  dd.dc->DrawTextLayout(p, lyt, dd.brush);
  lyt->Release();
}

void render(DrawData &dd, GameData &gd) {
  dd.dc->BeginDraw();
  dd.dc->Clear();
  dd.brush->SetColor(D2D1::ColorF(0x2e3440, 0.5f));
  dd.dc->FillRoundedRectangle(
      {{0.0f, 0.0f, 2.0f * dd.b + dd.w, 2.0f * dd.b + dd.h}, dd.b, dd.b},
      dd.brush);
  dd.brush->SetColor(D2D1::ColorF(0xeceff4));
  drawText(dd, {dd.b, dd.b}, 0b11, L"%d", gd.ptas);
  drawText(dd, {dd.b, dd.b + dd.fh}, 0b00, L"%d", gd.gameRank);
  drawText(dd, {dd.b, dd.b + dd.fh}, 0b01, L"%d", gd.killCount);
  for (int i = 0; i < min(std::size(gd.enemies), 5); i++) {
    float bx = dd.b;
    float by = dd.b + (i + 2.3f) * dd.fh;
    float bw = 6.5f * dd.fw;
    float bh = 0.4f * dd.fh;
    float r = static_cast<float>(gd.enemies[i].curHp) /
              static_cast<float>(gd.enemies[i].maxHp);
    dd.brush->SetColor(D2D1::ColorF(0xa3be8c));
    dd.dc->FillRectangle({bx, by, bx + bw * r, by + bh}, dd.brush);
    dd.brush->SetColor(D2D1::ColorF(0x4c566a));
    dd.dc->DrawRectangle({bx, by, bx + bw, by + bh}, dd.brush);
    dd.brush->SetColor(D2D1::ColorF(0xeceff4));
    drawText(dd, {dd.b, dd.b + static_cast<float>(i + 2) * dd.fh}, 0b01, L"%d",
             gd.enemies[i].curHp);
  }
  if (gd.isFocus) {
    dd.brush->SetColor(D2D1::ColorF(0x4c566a));
    dd.dc->DrawRoundedRectangle(
        {{0.0f, 0.0f, 2.0f * dd.b + dd.w, 2.0f * dd.b + dd.h}, dd.b, dd.b},
        dd.brush, 2.0f);
  }
  dd.dc->EndDraw();
}

template <typename T>
bool readValue(GameData &gd, T &u, std::initializer_list<uint32_t> offsets) {
  byte *p = gd.modAddr;
  size_t c = 0;
  auto i = std::begin(offsets);
  for (; i != std::prev(std::end(offsets)); i++) {
    if (!ReadProcessMemory(gd.hProcess, p + *i, &p, 8, &c) || c != 8)
      return false;
  }
  T v{};
  if (!ReadProcessMemory(gd.hProcess, p + *i, &v, sizeof(T), &c) ||
      c != sizeof(T))
    return false;
  u = v;
  return true;
}

bool readEnemies(GameData &gd) {
  byte *ep = nullptr;
  if (!readValue(gd, ep, {gd.CharacterManager, 0xa8}))
    return false;
  char hdr[16]{};
  size_t c = 0;
  if (!ReadProcessMemory(gd.hProcess, ep + 0x10, hdr, 16, &c) || c != 16)
    return false;
  byte *p0 = *reinterpret_cast<byte **>(hdr);
  size_t n0 = *reinterpret_cast<int32_t *>(hdr + 8);
  int32_t v0 = *reinterpret_cast<int32_t *>(hdr + 12);
  auto a = std::make_unique<char[]>(8 * n0);
  if (!ReadProcessMemory(gd.hProcess, p0 + 0x20, a.get(), 8 * n0, &c) ||
      c != 8 * n0)
    return false;
  std::vector<Enemy> enemies;
  for (size_t i = 0; i < n0; i++) {
    Enemy e{};
    e.addr = *reinterpret_cast<byte **>(a.get() + 8 * i);
    byte *q = nullptr;
    if (e.addr == nullptr ||
        !ReadProcessMemory(gd.hProcess, e.addr + 0x148, &q, 8, &c) || c != 8)
      return false;
    char h[8]{};
    if (!ReadProcessMemory(gd.hProcess, q + 0x10, h, 8, &c) || c != 8)
      return false;
    e.maxHp = *reinterpret_cast<int32_t *>(h);
    e.curHp = *reinterpret_cast<int32_t *>(h + 4);
    if (e.maxHp > 1 && e.curHp != 0)
      enemies.push_back(e);
  }
  std::sort(std::begin(enemies), std::end(enemies),
            [](const Enemy &a, const Enemy &b) {
              int32_t dmgA = a.maxHp - a.curHp;
              int32_t dmgB = b.maxHp - b.curHp;
              return dmgA != dmgB ? dmgA > dmgB
                                  : (a.maxHp != b.maxHp ? a.maxHp > b.maxHp
                                                        : a.addr < b.addr);
            });
  if (!ReadProcessMemory(gd.hProcess, ep + 0x10, hdr, 16, &c) || c != 16)
    return false;
  byte *p1 = *reinterpret_cast<byte **>(hdr);
  size_t n1 = *reinterpret_cast<int32_t *>(hdr + 8);
  int32_t v1 = *reinterpret_cast<int32_t *>(hdr + 12);
  if (v0 != v1 || n0 != n1 || p0 != p1)
    return false;
  gd.enemies = std::move(enemies);
  return true;
}

bool update(GameData &gd) {
  if (gd.hProcess == nullptr ||
      WaitForSingleObject(gd.hProcess, 0) != WAIT_TIMEOUT)
    return false;
  readValue(gd, gd.ptas, {gd.InventoryManager, 0x10});
  readValue(gd, gd.gameRank, {gd.GameRankSystem, 0x10});
  readValue(gd, gd.killCount, {gd.GameStatsManager, 0x20, 0x18, 0x14});
  readEnemies(gd);
  return true;
}

bool sigScan(const char *p, const char *m, const char *r, uint32_t s,
             uint32_t &o, uint32_t k) {
  uint32_t n = static_cast<uint32_t>(strlen(m));
  for (uint32_t i = 0, j = 0; j < s; j++) {
    if (m[i] != '?' && p[i] != r[j]) {
      j -= i;
      i = 0;
    } else
      i++;
    if (i == n) {
      k += j - n + 1;
      o = *reinterpret_cast<const uint32_t *>(r + k) + k + 4;
      return true;
    }
  }
  return false;
}

bool initHandle(GameData &gd) {
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 pe{};
  pe.dwSize = sizeof(PROCESSENTRY32);
  bool found = false;
  Process32First(hSnapshot, &pe);
  do {
    found = !strcmp("re4.exe", pe.szExeFile);
  } while (!found && Process32Next(hSnapshot, &pe));
  CloseHandle(hSnapshot);
  if (!found)
    return false;
  MODULEENTRY32 me{};
  me.dwSize = sizeof(MODULEENTRY32);
  hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pe.th32ProcessID);
  Module32First(hSnapshot, &me);
  found = false;
  do {
    found = !strcmp("re4.exe", me.szModule);
  } while (!found && Module32Next(hSnapshot, &me));
  CloseHandle(hSnapshot);
  if (!found)
    return false;
  gd.hProcess =
      OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION | SYNCHRONIZE,
                  false, pe.th32ProcessID);
  if (gd.hProcess == nullptr)
    return false;
  gd.modAddr = me.modBaseAddr;
  auto mod = std::make_unique<char[]>(me.modBaseSize);
  size_t c = 0;
  if (!ReadProcessMemory(gd.hProcess, me.modBaseAddr, mod.get(), me.modBaseSize,
                         &c) ||
      c != me.modBaseSize)
    return false;
  if (!sigScan("\x4c\x8b\xc0\x48\x8b\x15\x00\x00\x00\x00\x48\x8b\xcf\xe8",
               "xxxxxx????xxxx", mod.get(), me.modBaseSize, gd.InventoryManager,
               6) ||
      !sigScan("\x4c\x8b\x47\x30\x48\x8b\xcd\x48\x8b\x15\x00\x00\x00\x00\xe8",
               "xxxxxxxxxx????x", mod.get(), me.modBaseSize, gd.GameRankSystem,
               10) ||
      !sigScan("\x48\x8b\xd9\x48\x8b\x15\x00\x00\x00\x00\xe8\x00\x00\x00\x00"
               "\x85\xc0",
               "xxxxxx????x????xx", mod.get(), me.modBaseSize,
               gd.GameStatsManager, 6) ||
      !sigScan("\x48\x8b\xf8\x48\x8b\x15\x00\x00\x00\x00\x48\x8b\xcb\xe8",
               "xxxxxx????xxxx", mod.get(), me.modBaseSize, gd.CharacterManager,
               6))
    return false;
  return true;
}

bool initWindow(DrawData &dd, HINSTANCE hInstance) {
  DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dd.dwFac),
                      reinterpret_cast<IUnknown **>(&dd.dwFac));
  dd.dwFac->CreateTextFormat(
      L"Verdana", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
      DWRITE_FONT_STRETCH_NORMAL, 24.0f, L"en-us", &dd.dwFmt);
  for (int i = 0; i < 10; i++) {
    wchar_t s[4]{};
    _snwprintf(s, 4, L"%d", i);
    IDWriteTextLayout *lyt = nullptr;
    dd.dwFac->CreateTextLayout(s, static_cast<uint32_t>(wcslen(s)), dd.dwFmt,
                               96.0f, 96.0f, &lyt);
    DWRITE_TEXT_METRICS mtc{};
    lyt->GetMetrics(&mtc);
    dd.fw = max(dd.fw, mtc.width);
    dd.fh = max(dd.fh, mtc.height);
    lyt->Release();
  }
  dd.w = 12.0f * dd.fw;
  dd.h = 7.0f * dd.fh;
  dd.b = 10.0f;
  WNDCLASSEX wcx{};
  wcx.cbSize = sizeof(WNDCLASSEX);
  wcx.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam,
                       LPARAM lParam) -> LRESULT {
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
  };
  wcx.hInstance = hInstance;
  wcx.lpszClassName = "re4r_srt";
  RegisterClassEx(&wcx);
  DXGI_SWAP_CHAIN_DESC1 swDsc{};
  swDsc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  swDsc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swDsc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
  swDsc.BufferCount = 2;
  swDsc.SampleDesc.Count = 1;
  swDsc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
  swDsc.Width = static_cast<int32_t>(dd.w + 2.0f * dd.b);
  swDsc.Height = static_cast<int32_t>(dd.h + 2.0f * dd.b);
  dd.hWnd =
      CreateWindowEx(WS_EX_NOREDIRECTIONBITMAP | WS_EX_LAYERED | WS_EX_TOPMOST,
                     wcx.lpszClassName, "re4r srt", WS_POPUP | WS_MINIMIZEBOX,
                     CW_USEDEFAULT, CW_USEDEFAULT, swDsc.Width, swDsc.Height,
                     nullptr, nullptr, hInstance, nullptr);
  ID3D11Device *d3Dev = nullptr;
  D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                    D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0,
                    D3D11_SDK_VERSION, &d3Dev, nullptr, nullptr);
  IDXGIDevice *dxDev = nullptr;
  d3Dev->QueryInterface(&dxDev);
  IDXGIFactory2 *dxFac = nullptr;
  CreateDXGIFactory2(0, __uuidof(dxFac), reinterpret_cast<void **>(&dxFac));
  dxFac->CreateSwapChainForComposition(dxDev, &swDsc, nullptr, &dd.sc);
  ID2D1Factory2 *d2Fac = nullptr;
  D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2Fac);
  ID2D1Device1 *d2Dev = nullptr;
  d2Fac->CreateDevice(dxDev, &d2Dev);
  d2Dev->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &dd.dc);
  IDXGISurface2 *dxSfc = nullptr;
  dd.sc->GetBuffer(0, __uuidof(dxSfc), reinterpret_cast<void **>(&dxSfc));
  D2D1_BITMAP_PROPERTIES1 d2Bmp_prps{};
  d2Bmp_prps.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
  d2Bmp_prps.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
  d2Bmp_prps.bitmapOptions =
      D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
  ID2D1Bitmap1 *d2Bmp = nullptr;
  dd.dc->CreateBitmapFromDxgiSurface(dxSfc, d2Bmp_prps, &d2Bmp);
  dd.dc->SetTarget(d2Bmp);
  IDCompositionDevice *dcDev = nullptr;
  DCompositionCreateDevice(dxDev, __uuidof(dcDev),
                           reinterpret_cast<void **>(&dcDev));
  IDCompositionTarget *dcTgt = nullptr;
  dcDev->CreateTargetForHwnd(dd.hWnd, true, &dcTgt);
  IDCompositionVisual *dcVis = nullptr;
  dcDev->CreateVisual(&dcVis);
  dcVis->SetContent(dd.sc);
  dcTgt->SetRoot(dcVis);
  dcDev->Commit();
  dd.dc->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &dd.brush);
  return true;
}
