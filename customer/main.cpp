#include <windows.h>
#include <d3d11.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Customer.h"
#include "FileManager.h"
#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_dx11.h"
#include "../imgui/backends/imgui_impl_win32.h"

static ID3D11Device *g_pd3dDevice = NULL;
static ID3D11DeviceContext *g_pd3dDeviceContext = NULL;
static IDXGISwapChain *g_pSwapChain = NULL;
static ID3D11RenderTargetView *g_mainRenderTargetView = NULL;

static bool CreateRenderTarget()
{
    ID3D11Texture2D *pBackBuffer = NULL;
    if (g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)) != S_OK)
    {
        return false;
    }
    bool result = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView) == S_OK;
    pBackBuffer->Release();
    return result;
}

static void CleanupRenderTarget()
{
    if (g_mainRenderTargetView)
    {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = NULL;
    }
}

static bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0};

    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags,
                                      featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &sd,
                                      &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
    {
        return false;
    }

    return CreateRenderTarget();
}

static void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain)
    {
        g_pSwapChain->Release();
        g_pSwapChain = NULL;
    }
    if (g_pd3dDeviceContext)
    {
        g_pd3dDeviceContext->Release();
        g_pd3dDeviceContext = NULL;
    }
    if (g_pd3dDevice)
    {
        g_pd3dDevice->Release();
        g_pd3dDevice = NULL;
    }
}

static void ResizeSwapChain(HWND hWnd, int width, int height)
{
    if (!g_pd3dDevice)
    {
        return;
    }
    CleanupRenderTarget();
    if (g_pSwapChain)
    {
        g_pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
        CreateRenderTarget();
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            ResizeSwapChain(hWnd, LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

int RunCustomerGui(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEXW wc = {sizeof(WNDCLASSEXW), CS_CLASSDC, WndProc, 0, 0,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      L"CustomerAppClass", NULL};
    RegisterClassExW(&wc);
    HWND hwnd = CreateWindowW(wc.lpszClassName, L"Customer App",
                              WS_OVERLAPPEDWINDOW, 100, 100, 800, 640,
                              NULL, NULL, wc.hInstance, NULL);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ImGui::CreateContext();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    auto fileManager = std::make_unique<FileManager>("../orders.txt");
    fileManager->loadOrders();
    auto customer = std::make_shared<Customer>("Customer");
    char newProduct[128] = "";

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::Begin("Customer App", NULL,
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("Restaurant Ordering System");
        ImGui::Separator();
        ImGui::Text("Enter a new customer order below:");
        ImGui::InputText("Order Name", newProduct, sizeof(newProduct));

        if (ImGui::Button("Place Order", ImVec2(180, 0)))
        {
            if (strlen(newProduct) > 0)
            {
                customer->placeOrder(*fileManager, std::string(newProduct));
                fileManager->loadOrders();
                newProduct[0] = '\0';
            }
        }

        ImGui::Separator();
        if (ImGui::Button("Reload Orders", ImVec2(180, 0)))
        {
            fileManager->loadOrders();
        }

        ImGui::Separator();
        ImGui::Text("Current Orders:");

        for (const Order &order : fileManager->getOrders())
        {
            ImGui::Text("%d | %s | %s", order.getId(), order.getProduct().c_str(), order.getStatus().c_str());
        }

        ImGui::End();

        ImGui::Render();
        const float clearColor[4] = {0.08f, 0.10f, 0.14f, 1.00f};
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clearColor);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_pSwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    DestroyWindow(hwnd);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

int RunCustomerCli(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage:\n";
        std::cout << "  customer.exe view\n";
        std::cout << "  customer.exe place <PRODUCT>\n";
        return 1;
    }

    std::string command = argv[1];
    FileManager fileManager("../orders.txt");
    Customer customer("Customer");

    if (command == "view")
    {
        if (argc != 2)
        {
            std::cout << "Error: 'view' takes no extra arguments.\n";
            return 1;
        }

        fileManager.loadOrders();
        const std::vector<Order>& orders = fileManager.getOrders();
        for (std::size_t i = 0; i < orders.size(); ++i)
        {
            std::cout << orders[i] << "\n";
        }
        return 0;
    }

    if (command == "place")
    {
        if (argc != 3)
        {
            std::cout << "Error: 'place' requires <PRODUCT>.\n";
            return 1;
        }

        std::string product = argv[2];
        if (product.empty())
        {
            std::cout << "Error: PRODUCT cannot be empty.\n";
            return 1;
        }

        fileManager.loadOrders();
        if (!customer.placeOrder(fileManager, product))
        {
            std::cout << "Error: Failed to place order.\n";
            return 1;
        }

        std::cout << "Order placed successfully.\n";
        return 0;
    }

    std::cout << "Error: Unknown command.\n";
    return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    return RunCustomerGui(hInstance, nCmdShow);
}

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        return RunCustomerCli(argc, argv);
    }

    return RunCustomerGui(GetModuleHandle(NULL), SW_SHOWDEFAULT);
}
