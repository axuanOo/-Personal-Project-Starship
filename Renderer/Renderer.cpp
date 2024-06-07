#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Shader.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Renderer/DefaultShader.hpp"
#include "Engine/Renderer/IndexBuffer.hpp"
#include "Engine/Renderer/ConstantBuffer.hpp"
#include "Engine/Renderer/Image.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/Window.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Vec4.hpp"
#include "ThirdParty/stb/stb_image.h"
#include <windows.h>			// #include this (massive, platform-specific) header in VERY few places (and .CPPs only)
//#include <gl/GL.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#if defined(_DEBUG)
#define ENGINE_DEBUG_RENDER
#endif

#if defined(OPAQUE)
#undef OPAQUE
#endif

#if defined(ENGINE_DEBUG_RENDER)
#include <dxgidebug.h>
#pragma comment(lib,"dxguid.lib")
#endif

extern Camera*			g_theCamera;

struct ModelConstants 
{
	Mat44 ModelMatrix;
	Vec4 ModelColor;
};

struct CameraConstants
{
	Mat44 ProjectionMatrix;
	Mat44 ViewMatrix;
};

struct LightConstants 
{
	Vec3 SunDirection;
	float SunIntensity;
	float AmbientIntensity;
	
	Vec3 EMPTY_0;
};

static const int k_lightConstantSlot = 1;
static const int k_cameraConstantSlot = 2;
static const int k_modelConstantSlot = 3;


Renderer::Renderer(RenderConfig const& config)
{
	m_config = config;
}

Renderer::Renderer()
{

}

RenderConfig const& Renderer::GetConfig() const
{
	return m_config;
}

void Renderer::Startup()
{
	// Create debug module
#if defined(ENGINE_DEBUG_RENDER)
	m_dxgiDebugodule = (void*)::LoadLibraryA("dxgidebug.dll");
	if (m_dxgiDebugodule == nullptr)
	{
		ERROR_AND_DIE("Could not load dxgidebug.dll.");
	}

	typedef HRESULT(WINAPI* GetDebugModuleCB)(REFIID, void**);
	((GetDebugModuleCB)::GetProcAddress((HMODULE)m_dxgiDebugodule, "DXGIGetDebugInterface"))
		(__uuidof(IDXGIDebug), &m_dxgiDebug);

	if (m_dxgiDebug == nullptr)
	{
		ERROR_AND_DIE("Could not load debug module");
	}
#endif

	unsigned int deviceFlags = 0;
#if defined(ENGINE_DEBUG_RENDER)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Create device and swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferDesc.Width = m_config.m_window->GetClientDimensions().x;
	swapChainDesc.BufferDesc.Height = m_config.m_window->GetClientDimensions().y;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.OutputWindow = (HWND)m_config.m_window->GetHwnd();
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceFlags,
		nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc,
		&m_swapchain, &m_device, nullptr, &m_deviceContext);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create D3D 11 device and swap chain.");
	}

	// Get back buffer texture (Also known as the render target)
	ID3D11Texture2D* backBuffer;
	hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not get swap chain buffer.");
	}

	hr = m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTargetView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could create render target view for swap chain buffer.");
	}

	backBuffer->Release();


	// Create and bind the vertex shader
	BindShader(m_defaultShader);

	// Create vertex buffer
	m_immediateVBO = CreateVertexBuffer(sizeof(Vertex_PCU));

	// Create Index Buffer
	m_indexBuffer = CreateIndexBuffer(sizeof(unsigned int));

	// Create large enough constant buffer to hold the data
	m_cameraCBO = CreateConstantBuffer(sizeof(CameraConstants));

	// Create model constant buffer
	m_modelCBO = CreateConstantBuffer(sizeof(ModelConstants));

	// Create lighting cbo
	m_lightCBO = CreateConstantBuffer(sizeof(LightConstants));

	// -----------------------------------------------------------------------------------------
	// Create blend mode and add it into the array
	// Create the opaque blend mode 
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = blendDesc.RenderTarget[0].SrcBlend;
	blendDesc.RenderTarget[0].DestBlendAlpha = blendDesc.RenderTarget[0].DestBlend;
	blendDesc.RenderTarget[0].BlendOpAlpha = blendDesc.RenderTarget[0].BlendOp;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	
	hr = m_device->CreateBlendState(&blendDesc, &m_blendStates[(int)(BlendMode::OPAQUE)]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Cannot create the opaque blend mode");
	}

	// Create the alpha blend mode 
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	hr = m_device->CreateBlendState(&blendDesc, &m_blendStates[(int)(BlendMode::ALPHA)]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Cannot create the alpha blend mode");
	}

	// Create the additive blend mode 
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	hr = m_device->CreateBlendState(&blendDesc, &m_blendStates[(int)(BlendMode::ADDITIVE)]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Cannot create the additive blend mode");
	}


	// -----------------------------------------------------------------------------------------
	// Create rasterizer state and set it with a call to RSSetState

	CreateRasterizerStateBaseOnMode();

	//------------------------------------------------------------------------------------------
	// Create the depth stencil states and set texture

	CreateDepthStencilTextureandView();

	// -----------------------------------------------------------------------------------------
	// Create the sampler state and bind it 
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerStates[(int)SamplerMode::POINT_CLAMP]);

	if (!SUCCEEDED(hr)) 
	{
		ERROR_AND_DIE("CreateSamplerState for SamplerMode::POINT_CLAMP failed!");
	}

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	hr = m_device->CreateSamplerState(&samplerDesc, &m_samplerStates[(int)SamplerMode::BILINEAR_WRAP]);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("CreateSamplerState for SamplerMode::BILINEAR_WRAP failed!");
	}

	// Set the default sampler state to the point clamp mode
	m_samplerState = m_samplerStates[(int)SamplerMode::POINT_CLAMP];
	m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);

	// -----------------------------------------------------------------------------------------
	// Initialize the default texture
	Image* defaultImage = new Image(IntVec2(2, 2), Rgba8::WHITE);
	m_defaultTexture = CreateTextureFromImage(*defaultImage);
	BindTexture(m_defaultTexture);
	
	
}

void Renderer::BeginFrame()
{
	// Set render target 
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	
}

void Renderer::EndFrame()
{
	// Present
	HRESULT hr;
	hr = m_swapchain->Present(0, 0);
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		ERROR_AND_DIE("Device has been lost, application will now terminate.");
	}
}

void Renderer::Shutdown()
{
	// Report error leaks and release debug module

	// Delete all the shader pointers
	for (int i = 0; i < m_loadedShaders.size(); ++i)
	{
		if (m_loadedShaders[i] != nullptr)
		{
			delete m_loadedShaders[i];
			m_loadedShaders[i] = nullptr;
		}
	}

	for (int i = 0; i < m_loadedTextures.size(); ++i)
	{
		if (m_loadedTextures[i] != nullptr)
		{
			delete m_loadedTextures[i];
			m_loadedTextures[i] = nullptr;
		}
	}

	// Delete all the blend states
	for (int i = 0; i < (int)(BlendMode::COUNT); ++i)
	{
		DX_SAFE_RELEASE(m_blendStates[i]);
	}

	// Delete all the blend states
	for (int i = 0; i < (int)SamplerMode::COUNT; ++i)
	{
		DX_SAFE_RELEASE(m_samplerStates[i]);
	}

	// Delete all the Rasterizer states
	for (int i = 0; i < (int)RasterizerMode::COUNT; ++i)
	{
		DX_SAFE_RELEASE(m_rasterizedStates[i]);
	}

	for (int i = 0; i < (int)DepthMode::COUNT; ++i)
	{
		DX_SAFE_RELEASE(m_depthStencilStates[i]);
	}

	DX_SAFE_RELEASE(m_depthStencilTexture);
	DX_SAFE_RELEASE(m_depthStencilView);
	DX_SAFE_RELEASE(m_inputLayoutForVertex_PCU);
	DX_SAFE_RELEASE(m_renderTargetView);
	DX_SAFE_RELEASE(m_swapchain);
	DX_SAFE_RELEASE(m_deviceContext);
	DX_SAFE_RELEASE(m_device);


	// Delete the immediate vertex buffer
	delete m_immediateVBO;
	delete m_indexBuffer;
	delete m_cameraCBO;
	delete m_modelCBO;
	delete m_lightCBO;

#if defined(ENGINE_DEBUG_RENDER)
	((IDXGIDebug*)m_dxgiDebug)->ReportLiveObjects(DXGI_DEBUG_ALL,
		(DXGI_DEBUG_RLO_FLAGS)(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL)
	);

	((IDXGIDebug*)m_dxgiDebug)->Release();
	m_dxgiDebug = nullptr;

	::FreeLibrary((HMODULE)m_dxgiDebugodule);
	m_dxgiDebugodule = nullptr;
#endif
}

void Renderer::ClearScreen(const Rgba8& clearColor)
{
	// Clear the screen
	float colorAsFloats[4];
	clearColor.GetAsFloats(colorAsFloats);
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, colorAsFloats);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::BeginCamera(const Camera& camera)
{
	// Set viewport
	D3D11_VIEWPORT viewport = { 0 };
	viewport.TopLeftX = (camera.m_cameraBox.m_mins.x) * (float)m_config.m_window->GetClientDimensions().x;
	viewport.TopLeftY = (1.0f - camera.m_cameraBox.m_maxs.y) * (float)m_config.m_window->GetClientDimensions().y;
	viewport.Width = (camera.m_cameraBox.m_maxs.x - camera.m_cameraBox.m_mins.x) * (float)m_config.m_window->GetClientDimensions().x;
	viewport.Height = (camera.m_cameraBox.m_maxs.y - camera.m_cameraBox.m_mins.y) * (float)m_config.m_window->GetClientDimensions().y;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_deviceContext->RSSetViewports(1, &viewport);

	CameraConstants cameraConst;
	cameraConst.ProjectionMatrix = camera.GetProjectionMatrix();
	cameraConst.ViewMatrix = camera.GetViewMatrix();

	CopyCPUToGPU(&cameraConst, sizeof(CameraConstants), m_cameraCBO);
	BindConstantBuffer(k_cameraConstantSlot,m_cameraCBO);
}

void Renderer::EndCamera(const Camera& camera)
{
	UNUSED(camera);
}

void Renderer::BindTexture(const Texture* texture)
{
	if (texture == nullptr)
	{
		m_deviceContext->PSSetShaderResources(0, 1, &m_defaultTexture->m_shaderResourceView);
		return;
	}
	m_deviceContext->PSSetShaderResources(0, 1, &texture->m_shaderResourceView);
}

void Renderer::SetBlendMode(BlendMode blendMode)
{
	m_desiredBlendMode = blendMode;
}

void Renderer::SetDepthMode(DepthMode depthMode)
{
	m_depthMode = depthMode;
}

void Renderer::DrawVertexArray(int numVertexes, const Vertex_PCU* vertexes)
{
	SetStatesIfChanged();
	CopyCPUToGPU(vertexes, sizeof(vertexes[0]) * numVertexes, m_immediateVBO);
	DrawVertexBuffer(m_immediateVBO, numVertexes);
}

void Renderer::DrawVertexArray(int numVertexes, const Vertex_PCU* vertexes, int numIndexes, const unsigned int* indexes)
{
	SetStatesIfChanged();
	CopyCPUToGPU(vertexes, sizeof(vertexes[0]) * numVertexes, m_immediateVBO, indexes, sizeof(indexes[0])*numIndexes, m_indexBuffer);
	DrawVertexBufferAndIndexBuffer(m_immediateVBO,m_indexBuffer,numIndexes);
}

void Renderer::DrawVertexArray(int numVertexes, const Vertex_PCUTBN* vertexes)
{
	SetStatesIfChanged();
	CopyCPUToGPU(vertexes, sizeof(vertexes[0]) * numVertexes, m_immediateVBO);
	DrawVertexBuffer(m_immediateVBO, numVertexes,0,VertexType::Vertex_PCUTBN);
}

void Renderer::DrawVertexArray(int numVertexes, const Vertex_PCUTBN* vertexes, int numIndexes, const unsigned int* indexes)
{
	SetStatesIfChanged();
	CopyCPUToGPU(vertexes, sizeof(vertexes[0]) * numVertexes, m_immediateVBO, indexes, sizeof(indexes[0]) * numIndexes, m_indexBuffer);
	DrawVertexBufferAndIndexBuffer(m_immediateVBO, m_indexBuffer, numIndexes, 0, VertexType::Vertex_PCUTBN);
}

void Renderer::DrawVertexBuffer(VertexBuffer* vbo, int vertexCount, int vertexOffset /*= 0*/, VertexType vertType)
{
	if (vertType == VertexType::Vertex_PCU)
	{
		BindVertexBuffer(vbo);
	}

	if (vertType == VertexType::Vertex_PCUTBN)
	{
		BindVertexBuffer(vbo,sizeof(Vertex_PCUTBN));
	}
	
	m_deviceContext->Draw(vertexCount, vertexOffset);
}

void Renderer::DrawVertexBufferAndIndexBuffer(VertexBuffer* vbo, IndexBuffer* ibo, int indexCount, int vertexOffset /*= 0*/,VertexType vertType)
{
	if (vertType == VertexType::Vertex_PCU)
	{
		BindVertexBuffer(vbo);
	}

	if (vertType == VertexType::Vertex_PCUTBN)
	{
		BindVertexBuffer(vbo, sizeof(Vertex_PCUTBN));
	}

	BindIndexBuffer(ibo);

	m_deviceContext->DrawIndexed(indexCount, vertexOffset, 0);
}


Shader* Renderer::CreateShader(char const* shaderName, char const* sourceShader, VertexType vertexType)
{
	ShaderConfig shaderConfig = ShaderConfig();
	shaderConfig.m_name = shaderName;

	Shader* shader = new Shader(shaderConfig);

	// Compile the vertex shader and pixel shader
	//--------------------------------------------------------------------------------------------
	std::vector<unsigned char> outVertexShaderByteCode;
	CompileShaderToByteCode(outVertexShaderByteCode, shaderConfig.m_vertexShaderName.c_str(), 
		sourceShader, shaderConfig.m_vertexEntryPoint.c_str(), shaderConfig.m_vertexFlag.c_str());

	std::vector<unsigned char> outPixelShaderByteCode;
	CompileShaderToByteCode(outPixelShaderByteCode,shaderConfig.m_pixelShaderName.c_str(),
		sourceShader, shaderConfig.m_pixelEntryPoint.c_str(), shaderConfig.m_pixelFlag.c_str());


	// Create vertex shader and pixel shader
	//---------------------------------------------------------------------------------------------
	HRESULT hr;
	hr = m_device->CreateVertexShader(
		outVertexShaderByteCode.data(),
		outVertexShaderByteCode.size(),
		NULL, &shader->m_vertexShader);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Could not create vertex shader"));
	}


	hr = m_device->CreatePixelShader(
		outPixelShaderByteCode.data(),
		outPixelShaderByteCode.size(),
		NULL, &shader->m_pixelShader);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Could not create pixel shader."));
	}

	// Create local array of input element descriptions that defines the vertex layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc;

	inputElementDesc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	inputElementDesc.push_back({ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM,
		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	inputElementDesc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	if (vertexType == VertexType::Vertex_PCUTBN)
	{
		inputElementDesc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		inputElementDesc.push_back({ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		inputElementDesc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
			0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	}

	hr = m_device->CreateInputLayout(
		inputElementDesc.data(), (UINT)inputElementDesc.size(),
		outVertexShaderByteCode.data(),
		outVertexShaderByteCode.size(),
		&shader->m_inputLayout
	);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create vertex layout");
	}

	m_loadedShaders.push_back(shader);

	return shader;
}

Shader* Renderer::CreateOrGetShader(char const* shaderName, VertexType vertexType)
{
	for (int i = 0; i < (int)m_loadedShaders.size(); i++)
	{
		if (m_loadedShaders[i] != nullptr)
		{
			if (m_loadedShaders[i]->m_config.m_name == shaderName)
			{
				return m_loadedShaders[i];
			}
		}
	}
	std::string shadeFullPath = std::string(shaderName).append(".hlsl");
	std::string fileString;
	FileReadToString(fileString, shadeFullPath);
	Shader* shader = CreateShader(shaderName, fileString.c_str(), vertexType);
	return shader;
}

VertexBuffer* Renderer::CreateVertexBuffer(const size_t size)
{
	HRESULT hr;
	VertexBuffer* vb = new VertexBuffer(size);

	UINT vertexBufferSize = (UINT)(size);
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = vertexBufferSize;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// Create buffer using device
	hr = m_device->CreateBuffer(&bufferDesc, nullptr, &vb->m_buffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create vertex buffer.");
	}
	return vb;
}

IndexBuffer* Renderer::CreateIndexBuffer(const size_t size)
{
	HRESULT hr;
	IndexBuffer* ib = new IndexBuffer(size);

	UINT indexBufferSize = (UINT)(size);
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = indexBufferSize;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// Create buffer using device
	hr = m_device->CreateBuffer(&bufferDesc, nullptr, &ib->m_buffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create index buffer.");
	}
	return ib;
}

ConstantBuffer* Renderer::CreateConstantBuffer(const size_t size)
{
	HRESULT hr;
	ConstantBuffer* cbo = new ConstantBuffer(size);

	UINT constantBufferSize = (UINT)(size);
	D3D11_BUFFER_DESC bufferDesc = { };
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = constantBufferSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// Create buffer using device
	hr = m_device->CreateBuffer(&bufferDesc, nullptr, &cbo->m_buffer);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create light buffer.");
	}

	return cbo;
}

bool Renderer::CompileShaderToByteCode(std::vector<unsigned char>& out_ByteCode, char const* name, char const* source, char const* entryPoint, char const* target)
{

	DWORD shaderFlags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
#if defined(ENGINE_DEBUG_RENDER)
	shaderFlags = D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif


	// Compile vertex shader
	ID3DBlob* shaderBlob = NULL;
	ID3DBlob* errorBlob = NULL;
	HRESULT hr;

	hr = D3DCompile(
		source, strlen(source),
		name, nullptr, nullptr,
		entryPoint, target, shaderFlags, 0,
		&shaderBlob, &errorBlob
	);

	if (SUCCEEDED(hr))
	{
		out_ByteCode.resize(shaderBlob->GetBufferSize());
		memcpy(
			out_ByteCode.data(),
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize()
		);
	}
	else
	{
		if (errorBlob != NULL)
		{
			DebuggerPrintf((char*)errorBlob->GetBufferPointer());
		}
		ERROR_AND_DIE("Could not compile shader.");
	}
	shaderBlob->Release();
	if (errorBlob != NULL)
	{
		errorBlob->Release();
	}

	return true;
}

void Renderer::CopyCPUToGPU(const void* data, size_t size, ConstantBuffer* cbo)
{
	// Copy the vertex buffer from CPU to GPU
	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(cbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap(cbo->m_buffer, 0);
}

void Renderer::CopyCPUToGPU(const void* data, size_t size, VertexBuffer*& vbo)
{
	if (vbo->m_size < size)
	{
		delete vbo;
		vbo = CreateVertexBuffer(size);
	}

	// Copy the vertex buffer from CPU to GPU
	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(vbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap(vbo->m_buffer, 0);
}

void Renderer::CopyCPUToGPU(const void* data, size_t size, VertexBuffer*& vbo, const void* indexData , size_t sizeIndex, IndexBuffer*& ibo)
{
	if (vbo->m_size < size)
	{
		delete vbo;
		vbo = CreateVertexBuffer(size);
	}

	// Copy the vertex buffer from CPU to GPU
	D3D11_MAPPED_SUBRESOURCE resource;
	m_deviceContext->Map(vbo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, data, size);
	m_deviceContext->Unmap(vbo->m_buffer, 0);


	if (ibo->m_size < sizeIndex)
	{
		delete ibo;
		ibo = CreateIndexBuffer(sizeIndex);
	}

	D3D11_MAPPED_SUBRESOURCE iboResource;
	m_deviceContext->Map(ibo->m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &iboResource);
	memcpy(iboResource.pData, indexData, sizeIndex);
	m_deviceContext->Unmap(ibo->m_buffer, 0);
}

void Renderer::BindShader(Shader* shader,VertexType vertexType)
{
	if (shader == nullptr)
	{
		if (m_defaultShader == nullptr)
		{
			m_defaultShader = CreateShader("Default", shaderSource, vertexType);
		}
		m_deviceContext->IASetInputLayout(m_defaultShader->m_inputLayout);
		m_deviceContext->VSSetShader(m_defaultShader->m_vertexShader, nullptr, 0);
		m_deviceContext->PSSetShader(m_defaultShader->m_pixelShader, nullptr, 0);
		m_currentShader = shader;
		return;
	}

	// Set pipeline state
	m_deviceContext->IASetInputLayout(shader->m_inputLayout);
	m_deviceContext->VSSetShader(shader->m_vertexShader, nullptr, 0);
	m_deviceContext->PSSetShader(shader->m_pixelShader, nullptr, 0);
	m_currentShader = shader;
}

void Renderer::BindVertexBuffer(VertexBuffer* vbo)
{
	// Bind the vertex buffer here
	UINT stride = sizeof(Vertex_PCU);
	UINT startOffset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &vbo->m_buffer, &stride, &startOffset);
	m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void Renderer::BindVertexBuffer(VertexBuffer* vbo, int vertSize)
{
	// Bind the vertex buffer here
	UINT stride = (UINT)vertSize;
	UINT startOffset = 0;
	m_deviceContext->IASetVertexBuffers(0, 1, &vbo->m_buffer, &stride, &startOffset);
	m_deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::BindIndexBuffer(IndexBuffer* ibo)
{
	UINT startOffset = 0;
	m_deviceContext->IASetIndexBuffer(ibo->m_buffer, DXGI_FORMAT_R32_UINT, startOffset);
}

void Renderer::BindConstantBuffer(int slot, ConstantBuffer* cbo)
{
	m_deviceContext->VSSetConstantBuffers(slot, 1, &cbo->m_buffer);
	m_deviceContext->PSSetConstantBuffers(slot, 1, &cbo->m_buffer);
}

void Renderer::SetStatesIfChanged()
{
	if (m_blendState == nullptr || m_blendState != m_blendStates[(int)m_desiredBlendMode])
	{
		m_blendState = m_blendStates[(int)m_desiredBlendMode];
		float blendFactors[4] = { 0.0f,0.0f,0.0f,0.0f };
		UINT sampleMask = 0xffffffff;
		m_deviceContext->OMSetBlendState(m_blendState, blendFactors, sampleMask);
	}
	if (m_samplerState == nullptr || m_samplerState != m_samplerStates[(int)m_desiredSamplerMode])
	{
		m_samplerState = m_samplerStates[(int)m_desiredSamplerMode];
		m_deviceContext->PSSetSamplers(0, 1, &m_samplerState);
	}

	if (m_desiredRasterizerState == nullptr || m_desiredRasterizerState != m_rasterizedStates[(int)m_rasterizerMode])
	{
		m_desiredRasterizerState = m_rasterizedStates[(int)m_rasterizerMode];
		m_deviceContext->RSSetState(m_desiredRasterizerState);
	}

	if (m_depthStencilState == nullptr||m_depthStencilState!=m_depthStencilStates[(int)m_depthMode])
	{
		m_depthStencilState = m_depthStencilStates[(int)m_depthMode];
		m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	}

}

void Renderer::SetSamplerMode(SamplerMode samplerMode)
{
	m_desiredSamplerMode = samplerMode;
}

void Renderer::SetRasterizerState(RasterizerMode rasterizerMode)
{
	m_rasterizerMode = rasterizerMode;
}

void Renderer::CreateRasterizerStateBaseOnMode()
{
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.AntialiasedLineEnable = true;

	HRESULT hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizedStates[(int)RasterizerMode::SOLID_CULL_NONE]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create rasterizer state solid cull none.");
	}


	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizedStates[(int)RasterizerMode::SOLID_CULL_BACK]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create rasterizer state solid cull back.");
	}


	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizedStates[(int)RasterizerMode::WIREFRAME_CULL_NONE]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create rasterizer state wireframe cull none.");
	}

	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizedStates[(int)RasterizerMode::WIREFRAME_CULL_BACK]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could not create rasterizer state wireframe cull back.");
	}

	m_desiredRasterizerState = m_rasterizedStates[(int)m_rasterizerMode];

	m_deviceContext->RSSetState(m_desiredRasterizerState);
}

void Renderer::CreateDepthStencilTextureandView()
{
	D3D11_TEXTURE2D_DESC textureDesc ={ };
	textureDesc.Width = m_config.m_window->GetClientDimensions().x;
	textureDesc.Height = m_config.m_window->GetClientDimensions().y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.Format =DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.SampleDesc.Count = 1;

	HRESULT hr = m_device->CreateTexture2D(&textureDesc, nullptr, &m_depthStencilTexture);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could create texture for depth stencil.");
	}

	hr = m_device->CreateDepthStencilView(m_depthStencilTexture, nullptr, &m_depthStencilView);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could create depth stencil view.");
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStates[(int)DepthMode::DISABLED]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could create depth stencil state disable.");
	}

	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStates[(int)DepthMode::ENABLED]);
	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE("Could create depth stencil state enable.");
	}
}

void Renderer::SetModelConstants(const Mat44& modelMatrix /*= Mat44()*/, const Rgba8& modelColor /*= Rgba8::WHITE*/)
{
	ModelConstants modelConstant;
	modelConstant.ModelMatrix = modelMatrix;
	modelConstant.ModelColor = Vec4(NormalizeByte(modelColor.r),NormalizeByte(modelColor.g),NormalizeByte(modelColor.b),NormalizeByte(modelColor.a));

	CopyCPUToGPU(&modelConstant, sizeof(ModelConstants), m_modelCBO);
	BindConstantBuffer(k_modelConstantSlot, m_modelCBO);
}


void Renderer::SetLightConstants(Vec3 sunDirection/*= Vec3(2.0f, 1.0f, -1.0f)*/, float sunIntensity/*= 0.85f*/, float ambientIntensity /*= 0.35f*/)
{
	LightConstants lightConstant;
	lightConstant.SunDirection = sunDirection.GetNormalized();
	lightConstant.SunIntensity = sunIntensity;
	lightConstant.AmbientIntensity = ambientIntensity;

	CopyCPUToGPU(&lightConstant,sizeof(LightConstants),m_lightCBO);
	BindConstantBuffer(k_lightConstantSlot, m_lightCBO);
}

//------------------------------------------------------------------------------------------------
Texture* Renderer::CreateOrGetTextureFromFile(char const* imageFilePath)
{
	// See if we already have this texture previously loaded
	Texture* existingTexture = GetTextureForFileName(imageFilePath);
	if (existingTexture)
	{
		return existingTexture;
	}

	// Never seen this texture before!  Let's load it.
	Texture* newTexture = CreateTextureFromFile(imageFilePath);
	return newTexture;
}

Texture* Renderer::CreateTextureFromData(char const* name, IntVec2 dimensions, int bytesPerTexel, const void* texelData)
{
	// Check if the load was successful
	GUARANTEE_OR_DIE(texelData, Stringf("CreateTextureFromData failed for \"%s\" - texelData was null!", name));
	GUARANTEE_OR_DIE(bytesPerTexel >= 3 && bytesPerTexel <= 4, Stringf("CreateTextureFromData failed for \"%s\" - unsupported BPP=%i (must be 3 or 4)", name, bytesPerTexel));
	GUARANTEE_OR_DIE(dimensions.x > 0 && dimensions.y > 0, Stringf("CreateTextureFromData failed for \"%s\" - illegal texture dimensions (%i x %i)", name, dimensions.x, dimensions.y));

	Texture* newTexture = new Texture();
	newTexture->m_name = name; // NOTE: m_name must be a std::string, otherwise it may point to temporary data!
	newTexture->m_dimensions = dimensions;

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = dimensions.x;
	textureDesc.Height = dimensions.y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA textureData;
	textureData.pSysMem = texelData;
	textureData.SysMemPitch = 4 * dimensions.x;

	HRESULT hr;
	hr = m_device->CreateTexture2D(&textureDesc, &textureData, &newTexture->m_texture);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Create Texture from Image failed for image file \"%s\".", name));
	}

	hr = m_device->CreateShaderResourceView(newTexture->m_texture,NULL
		,&newTexture->m_shaderResourceView);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Create ShaderResourceView for Image failed for image file \"%s\".", name));
	}

	m_loadedTextures.push_back(newTexture);
	return newTexture;
}

Texture* Renderer::CreateTextureFromFile(char const* imageFilePath)
{
	IntVec2 dimensions = IntVec2::ZERO;		// This will be filled in for us to indicate image width & height
	int bytesPerTexel = 0; // This will be filled in for us to indicate how many color components the image had (e.g. 3=RGB=24bit, 4=RGBA=32bit)
	int numComponentsRequested = 0; // don't care; we support 3 (24-bit RGB) or 4 (32-bit RGBA)

	// Load (and decompress) the image RGB(A) bytes from a file on disk into a memory buffer (array of bytes)
	stbi_set_flip_vertically_on_load(1); // We prefer uvTexCoords has origin (0,0) at BOTTOM LEFT
	unsigned char* texelData = stbi_load(imageFilePath, &dimensions.x, &dimensions.y, &bytesPerTexel, numComponentsRequested);

	// Check if the load was successful
	GUARANTEE_OR_DIE(texelData, Stringf("Failed to load image \"%s\"", imageFilePath));

	Image image = Image(imageFilePath);

	Texture* newTexture = CreateTextureFromImage(image);

	// Free the raw image texel data now that we've sent a copy of it down to the GPU to be stored in video memory
	stbi_image_free(texelData);

	return newTexture;
}

Texture* Renderer::CreateTextureFromImage(const Image& image)
{
	Texture* newTexture = new Texture();
	newTexture->m_name = image.GetImageFilePath(); // NOTE: m_name must be a std::string, otherwise it may point to temporary data!
	newTexture->m_dimensions = image.GetDimensions();

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = image.GetDimensions().x;
	textureDesc.Height = image.GetDimensions().y;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA textureData;
	textureData.pSysMem = image.GetRawData();
	textureData.SysMemPitch = 4 * image.GetDimensions().x;

	HRESULT hr;
	hr = m_device->CreateTexture2D(&textureDesc, &textureData, &newTexture->m_texture);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Create Texture from Image failed for image file"));
	}

	hr = m_device->CreateShaderResourceView(newTexture->m_texture, NULL
		, &newTexture->m_shaderResourceView);

	if (!SUCCEEDED(hr))
	{
		ERROR_AND_DIE(Stringf("Create ShaderResourceView for Image failed for image file"));
	}

	m_loadedTextures.push_back(newTexture);
	return newTexture;
}

Texture* Renderer::GetTextureForFileName(char const* imageFilePath)
{
	for (int textureIndex = 0; textureIndex < m_loadedTextures.size() ; textureIndex++) 
	{
		if (m_loadedTextures[textureIndex]->m_name == imageFilePath)
		{
			return m_loadedTextures[textureIndex];
		}
	}

	return nullptr;
}

BitmapFont* Renderer::CreateOrGetBitmapFont(char const* pathWithoutExtension)
{
	BitmapFont* bitmapFont = GetBitMapFontFromFileName(pathWithoutExtension);
	if (bitmapFont)
	{
		return bitmapFont;
	}

	BitmapFont* newbitmapFont = CreateFontFromFile(pathWithoutExtension);
	return newbitmapFont;
}

BitmapFont* Renderer::GetBitMapFontFromFileName(char const* pathWithoutExtension)
{
	for (int bitmapIndex = 0; bitmapIndex < m_loadedFonts.size(); bitmapIndex++)
	{
		if (m_loadedFonts[bitmapIndex]->m_fontFilePathNameWithNoExtension == pathWithoutExtension)
		{
			return m_loadedFonts[bitmapIndex];
		}
	}

	return nullptr;
}

BitmapFont* Renderer::CreateFontFromFile(char const* pathWithoutExtension)
{
	std::string path = std::string(pathWithoutExtension);

	path.append(".png");

	Texture* bitmapFontTexture = CreateOrGetTextureFromFile(path.c_str());

	BitmapFont* bitmapFont = new BitmapFont(pathWithoutExtension,*bitmapFontTexture);

	m_loadedFonts.push_back(bitmapFont);

	return bitmapFont;
}

//------------------------------------------------------------------------------------------------
SpriteSheet::SpriteSheet(Texture const& texture, IntVec2 const& simpleGridLayout)
	:m_texture(texture),
	m_dimensions(simpleGridLayout)
{
	int totalNum = simpleGridLayout.x * simpleGridLayout.y;
	
	float minUValue = (1.0f / 128.0f) / texture.GetDimensions().x;
	float minVValue = (1.0f / 128.0f) / texture.GetDimensions().y;

	m_spritedefs.reserve(totalNum);

	int startIndex = 0;
	float singleStepX = 1.0f / simpleGridLayout.x;
	float singleStepY = 1.0f / simpleGridLayout.y;

	for (int i = 0; i < simpleGridLayout.y; i++)
	{
		for (int j = 0; j < simpleGridLayout.x; j++)
		{
			float minU = singleStepX * static_cast<float>(j);
			float maxU = singleStepX * static_cast<float>(j + 1);
			float minV = 1.0f - singleStepY * static_cast<float>(i + 1);
			float maxV = 1.0f - singleStepY * static_cast<float>(i);

			minU += minUValue;
			maxU -= minUValue;
			minV += minVValue;
			maxV -= minVValue;

			Vec2 leftBottom(minU, minV);
			Vec2 rightTop(maxU, maxV);

			Vec2 uvAtMins = Vec2(minU,minV);
			Vec2 uvAtMaxs =  Vec2(maxU,maxV);
			SpriteDefinition spriteDef = SpriteDefinition(*this, startIndex, uvAtMins, uvAtMaxs);
			startIndex ++;
			m_spritedefs.push_back(spriteDef);
		}
	}
}

Texture const& SpriteSheet::GetTexture() const
{
	return m_texture;
}

int SpriteSheet::GetNumSprites() const
{
	return (int)m_spritedefs.size();
}

SpriteDefinition const& SpriteSheet::GetSpriteDef(int spriteIndex) const
{
	return m_spritedefs[spriteIndex];
}

void SpriteSheet::GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const
{
	return m_spritedefs[spriteIndex].GetUVs(out_uvAtMins,out_uvAtMaxs);
}

AABB2 SpriteSheet::GetSpriteUVs(int spriteIndex) const
{
	return m_spritedefs[spriteIndex].GetUVs();
}

AABB2 SpriteSheet::GetSpriteUVsByIntPos(IntVec2 pos) const
{
	int spriteDefIndex = (pos.y * m_dimensions.x) + pos.x;

	return GetSpriteUVs(spriteDefIndex);
}


//--------------------------------------------------------------------------------------------------
SpriteDefinition::SpriteDefinition(SpriteSheet const& spriteSheet, int spriteIndex, Vec2 const& uvAtMins, Vec2 const& uvAtMaxs)
	: m_spriteSheet(spriteSheet), m_spriteIndex(spriteIndex), m_uvAtMins(uvAtMins), m_uvAtMaxs(uvAtMaxs) {

}

void SpriteDefinition::GetUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs) const
{
	out_uvAtMins = m_uvAtMins;
	out_uvAtMaxs = m_uvAtMaxs;
}

AABB2 SpriteDefinition::GetUVs() const
{
	return AABB2(m_uvAtMins,m_uvAtMaxs);
}

SpriteSheet const& SpriteDefinition::GetSpriteSheet() const
{
	return m_spriteSheet;
}

Texture const& SpriteDefinition::GetTexture() const
{
	return m_spriteSheet.GetTexture();
}

float SpriteDefinition::GetAspect() const
{
	return (m_uvAtMaxs.x - m_uvAtMins.x) / (m_uvAtMaxs.y - m_uvAtMins.y);
}
