#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <d3d11.h>
#include <tchar.h>

#include <string>
#include <vector>

#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_dx11.h"
#include "../imgui/backends/imgui_impl_win32.h"

#include "FileManager.h"
#include "KitchenStaff.h"

namespace {
ID3D11Device* g_device = NULL;
ID3D11DeviceContext* g_deviceContext = NULL;
IDXGISwapChain* g_swapChain = NULL;
ID3D11RenderTargetView* g_renderTargetView = NULL;

void createRenderTarget() {
    ID3D11Texture2D* backBuffer = NULL;
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    if (backBuffer) {
        g_device->CreateRenderTargetView(backBuffer, NULL, &g_renderTargetView);
        backBuffer->Release();
    }
}

void cleanupRenderTarget() {
    if (g_renderTargetView) {
        g_renderTargetView->Release();
        g_renderTargetView = NULL;
    }
}

bool createDeviceD3D(HWND hwnd) {
    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = hwnd;
    desc.SampleDesc.Count = 1;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createFlags = 0;
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    D3D_FEATURE_LEVEL createdLevel;

    HRESULT result = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        createFlags,
        featureLevels,
        2,
        D3D11_SDK_VERSION,
        &desc,
        &g_swapChain,
        &g_device,
        &createdLevel,
        &g_deviceContext);

    if (result != S_OK) {
        return false;
    }

    createRenderTarget();
    return true;
}

void cleanupDeviceD3D() {
    cleanupRenderTarget();
    if (g_swapChain) {
        g_swapChain->Release();
        g_swapChain = NULL;
    }
    if (g_deviceContext) {
        g_deviceContext->Release();
        g_deviceContext = NULL;
    }
    if (g_device) {
        g_device->Release();
        g_device = NULL;
    }
}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }

    switch (msg) {
    case WM_SIZE:
        if (g_device != NULL && wParam != SIZE_MINIMIZED) {
            cleanupRenderTarget();
            g_swapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            createRenderTarget();
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = _T("KitchenAppWindow");

    RegisterClassEx(&wc);

    HWND hwnd = CreateWindow(
        wc.lpszClassName,
        _T("Kitchen App"),
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        900,
        600,
        NULL,
        NULL,
        wc.hInstance,
        NULL);

    if (!createDeviceD3D(hwnd)) {
        cleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_device, g_deviceContext);

    FileManager fileManager("../orders.txt");
    KitchenStaff staff("KitchenUser");
    std::vector<Order> orders = fileManager.loadOrders();

    int selectedId = 0;
    char statusBuffer[64] = "Ready";
    std::string message;

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
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
        ImGui::Begin("Kitchen App", NULL,
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
        if (ImGui::Button("Reload Orders")) {
            orders = fileManager.loadOrders();
            message = "Orders reloaded.";
        }

        ImGui::Separator();
        if (ImGui::BeginTable("OrdersTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("ID");
            ImGui::TableSetupColumn("Product");
            ImGui::TableSetupColumn("Status");
            ImGui::TableHeadersRow();

            for (std::size_t i = 0; i < orders.size(); ++i) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%d", orders[i].getId());
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%s", orders[i].getProduct().c_str());
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%s", orders[i].getStatus().c_str());
            }

            ImGui::EndTable();
        }

        ImGui::Separator();
        ImGui::InputInt("Order ID", &selectedId);
        ImGui::InputText("New Status", statusBuffer, sizeof(statusBuffer));

        if (ImGui::Button("Update Status")) {
            if (selectedId <= 0) {
                message = "ID must be greater than 0.";
            } else if (statusBuffer[0] == '\0') {
                message = "Status cannot be empty.";
            } else {
                bool updated = staff.updateOrderStatus(orders, selectedId, std::string(statusBuffer));
                if (updated) {
                    fileManager.saveOrders(orders);
                    message = "Order updated and saved.";
                } else {
                    message = "Order ID not found.";
                }
            }
        }

        if (!message.empty()) {
            ImGui::Separator();
            ImGui::Text("%s", message.c_str());
        }

        ImGui::End();

        ImGui::Render();
        const float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
        g_deviceContext->OMSetRenderTargets(1, &g_renderTargetView, NULL);
        g_deviceContext->ClearRenderTargetView(g_renderTargetView, clearColor);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_swapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    cleanupDeviceD3D();
    DestroyWindow(hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);
    return 0;
}
