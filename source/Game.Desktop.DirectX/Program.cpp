// Program.cpp : Defines the entry point for the application.


#include "pch.h"
#include "GameException.h"
#include "Monster.h"

using namespace std;
using namespace std::string_literals;
using namespace gsl;
using namespace winrt;
using namespace DirectX;
using namespace Library;

void InitializeWindow(HINSTANCE instance, const wstring& className, const wstring& windowTitle, int showCommand);
LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
POINT CenterWindow(const SIZE& windowSize);
void InitializeDirectX();
void Shutdown(const wstring& className);

const SIZE RenderTargetSize = { 1024, 768 };
HWND mWindowHandle;
WNDCLASSEX mWindow;

D3D_FEATURE_LEVEL mFeatureLevel;
com_ptr<ID3D11Device5> mDirect3DDevice;
com_ptr<ID3D11DeviceContext4> mDirect3DDeviceContext;
com_ptr<IDXGISwapChain1> mSwapChain;
com_ptr<ID3D11RenderTargetView> mRenderTargetView;
com_ptr<ID3D11DepthStencilView> mDepthStencilView;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int showCommand)
{
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	ThrowIfFailed(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED), "Error initializing COM.");

	wstring windowClassName = L"RenderingClass"s;
	InitializeWindow(instance, windowClassName, L"DirectX Project"s, showCommand);
	InitializeDirectX();

	MSG message = { 0 };

	Monster m;
	m;

	while (message.message != WM_QUIT)
	{
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{

			const XMVECTORF32 BackgroundColor = { 0.392f, 0.584f, 0.929f, 1.0f };
			mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.get(), BackgroundColor.f);
			mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			// Game-specific code

			ThrowIfFailed(mSwapChain->Present(1, 0), "IDXGISwapChain::Present() failed.");
		}
	}

	Shutdown(windowClassName);
	CoUninitialize();

	return static_cast<int>(message.wParam);
}

void InitializeWindow(HINSTANCE instance, const wstring& className, const wstring& windowTitle, int showCommand)
{
	ZeroMemory(&mWindow, sizeof(mWindow));
	mWindow.cbSize = sizeof(WNDCLASSEX);
	mWindow.style = CS_CLASSDC;
	mWindow.lpfnWndProc = WndProc;
	mWindow.hInstance = instance;
	mWindow.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	mWindow.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	mWindow.hCursor = LoadCursor(nullptr, IDC_ARROW);
	mWindow.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	mWindow.lpszClassName = className.c_str();

	RECT windowRectangle = { 0, 0, RenderTargetSize.cx, RenderTargetSize.cy };
	AdjustWindowRect(&windowRectangle, WS_OVERLAPPEDWINDOW, FALSE);

	RegisterClassEx(&mWindow);
	POINT center = CenterWindow(RenderTargetSize);
	mWindowHandle = CreateWindow(className.c_str(), windowTitle.c_str(), WS_OVERLAPPEDWINDOW, center.x, center.y, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top, nullptr, nullptr, instance, nullptr);

	ShowWindow(mWindowHandle, showCommand);
	UpdateWindow(mWindowHandle);
}

LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(windowHandle, message, wParam, lParam);
}

POINT CenterWindow(const SIZE& windowSize)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	POINT center;
	center.x = (screenWidth - windowSize.cx) / 2;
	center.y = (screenHeight - windowSize.cy) / 2;

	return center;
}

void InitializeDirectX()
{
	uint32_t createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	com_ptr<ID3D11Device> direct3DDevice;
	com_ptr<ID3D11DeviceContext> direct3DDeviceContext;
	ThrowIfFailed(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, narrow_cast<uint32_t>(size(featureLevels)), D3D11_SDK_VERSION, direct3DDevice.put(), &mFeatureLevel, direct3DDeviceContext.put()), "D3D11CreateDevice() failed");
	mDirect3DDevice = direct3DDevice.as<ID3D11Device5>();
	assert(mDirect3DDevice != nullptr);

	mDirect3DDeviceContext = direct3DDeviceContext.as<ID3D11DeviceContext4>();
	assert(mDirect3DDeviceContext != nullptr);

	uint32_t multiSamplingCount = 4;
	uint32_t multiSamplingQualityLevels;
	ThrowIfFailed(mDirect3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, multiSamplingCount, &multiSamplingQualityLevels), "CheckMultisampleQualityLevels() failed.");
	if (multiSamplingQualityLevels == 0)
	{
		throw GameException("Unsupported multi-sampling quality");
	}

#ifndef NDEBUG
	com_ptr<ID3D11Debug> d3dDebug = mDirect3DDevice.as<ID3D11Debug>();
	if (d3dDebug)
	{
		com_ptr<ID3D11InfoQueue> d3dInfoQueue = d3dDebug.as<ID3D11InfoQueue>();
		if (d3dInfoQueue)
		{
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS
			};
			D3D11_INFO_QUEUE_FILTER filter{ 0 };
			filter.DenyList.NumIDs = narrow_cast<uint32_t>(size(hide));
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{ 0 };
	swapChainDesc.Width = RenderTargetSize.cx;
	swapChainDesc.Height = RenderTargetSize.cy;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = multiSamplingCount;
	swapChainDesc.SampleDesc.Quality = multiSamplingQualityLevels - 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	com_ptr<IDXGIDevice4> dxgiDevice = mDirect3DDevice.as<IDXGIDevice4>();
	assert(dxgiDevice != nullptr);

	com_ptr<IDXGIAdapter> dxgiAdapter;
	ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.put()));

	com_ptr<IDXGIFactory2> dxgiFactory;
	ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.put())));

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc{ 0 };
	fullScreenDesc.RefreshRate.Numerator = 60;
	fullScreenDesc.RefreshRate.Denominator = 1;
	fullScreenDesc.Windowed = true;
	ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(mDirect3DDevice.get(), mWindowHandle, &swapChainDesc, &fullScreenDesc, nullptr, mSwapChain.put()), "IDXGIDevice::CreateSwapChainForHwnd() failed.");

	com_ptr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(mSwapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.put())), "IDXGISwapChain1::GetBuffer() failed.");
	ThrowIfFailed(mDirect3DDevice->CreateRenderTargetView(backBuffer.get(), nullptr, mRenderTargetView.put()), "IDXGIDevice::CreateRenderTargetView() failed.");

	D3D11_TEXTURE2D_DESC depthStencilDesc{ 0 };
	depthStencilDesc.Width = RenderTargetSize.cx;
	depthStencilDesc.Height = RenderTargetSize.cy;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.SampleDesc.Count = multiSamplingCount;
	depthStencilDesc.SampleDesc.Quality = multiSamplingQualityLevels - 1;

	com_ptr<ID3D11Texture2D> depthStencilBuffer;
	ThrowIfFailed(mDirect3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencilBuffer.put()), "IDXGIDevice::CreateTexture2D() failed.");
	ThrowIfFailed(mDirect3DDevice->CreateDepthStencilView(depthStencilBuffer.get(), nullptr, mDepthStencilView.put()), "IDXGIDevice::CreateDepthStencilView() failed.");

	D3D11_VIEWPORT viewport = CD3D11_VIEWPORT(0.0f, 0.0f, static_cast<float>(RenderTargetSize.cx), static_cast<float>(RenderTargetSize.cy));
	auto renderTargetViews = mRenderTargetView.get();
	mDirect3DDeviceContext->OMSetRenderTargets(1, &renderTargetViews, mDepthStencilView.get());
	mDirect3DDeviceContext->RSSetViewports(1, &viewport);
}

void Shutdown(const wstring& className)
{
	mDirect3DDeviceContext->ClearState();
	mDirect3DDeviceContext->Flush();

	mDepthStencilView = nullptr;
	mRenderTargetView = nullptr;
	mSwapChain = nullptr;
	mDirect3DDeviceContext = nullptr;
	mDirect3DDevice = nullptr;

	UnregisterClass(className.c_str(), mWindow.hInstance);
}
