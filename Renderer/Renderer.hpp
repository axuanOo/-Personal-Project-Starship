#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Matrix44.hpp"
#include "Game/EngineBuildPreferences.hpp"
#include <math.h>

struct AABB2;
class Image;
class Window;
class SpriteDefinition;
class BitmapFont;

struct RenderConfig
{
	Window* m_window = nullptr;
};

enum class BlendMode
{
	ALPHA,
	ADDITIVE,
	OPAQUE,
	COUNT
};

enum class SamplerMode
{
	POINT_CLAMP,
	BILINEAR_WRAP,
	COUNT
};

enum class RasterizerMode
{
	SOLID_CULL_NONE,
	SOLID_CULL_BACK,
	WIREFRAME_CULL_NONE,
	WIREFRAME_CULL_BACK,
	COUNT
};

enum class DepthMode
{
	DISABLED,
	ENABLED,
	COUNT
};

enum class VertexType 
{
	Vertex_PCU,
	Vertex_PCUTBN,
	COUNT
};


class SpriteSheet
{
public:
	explicit						SpriteSheet(Texture const& texture, IntVec2 const& simpleGridLayout);

	Texture const&					GetTexture() const;
	int								GetNumSprites() const;
	SpriteDefinition const&			GetSpriteDef(int spriteIndex) const;
	void							GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const;
	AABB2							GetSpriteUVs(int spriteIndex) const;
	AABB2							GetSpriteUVsByIntPos(IntVec2 pos) const;

protected:
	Texture const&					m_texture;
	IntVec2							m_dimensions;
	std::vector<SpriteDefinition>	m_spritedefs;
};

class SpriteDefinition 
{
public:
	explicit						SpriteDefinition( SpriteSheet const& spriteSheet, int spriteIndex, Vec2 const& uvAtMins, Vec2 const& uvAtMaxs);
	void							GetUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs) const;
	AABB2							GetUVs() const;
	SpriteSheet const&				GetSpriteSheet() const;
	Texture const&					GetTexture() const;
	float							GetAspect() const;
protected:
	SpriteSheet const&				m_spriteSheet;
	int								m_spriteIndex = -1;
	Vec2							m_uvAtMins = Vec2::ZERO;
	Vec2							m_uvAtMaxs = Vec2::ONE;
};

//	Safe release macro to check whether or not the dxObject is a nullptr for releasing

#define DX_SAFE_RELEASE(dxObject)							\
{															\
	if ((dxObject) != nullptr)								\
	{														\
		(dxObject)->Release();								\
		(dxObject) = nullptr;								\
	}														\
}

struct ID3D11RasterizerState;
struct ID3D11RenderTargetView;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11VertexShader;
struct ID3D11DepthStencilView;
struct ID3D11DepthStencilState;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11BlendState;
struct ID3D11Buffer;
struct ID3D11SamplerState;

class Shader;
class VertexBuffer;
class ConstantBuffer;
class IndexBuffer;

class Renderer 
{
public:

	Renderer();
	Renderer(RenderConfig const& config);

	void				Startup();
	void				BeginFrame();
	void				EndFrame();
	void				Shutdown();

	void				ClearScreen(const Rgba8& clearColor);
	void				BeginCamera(const Camera& camera);
	void				EndCamera(const Camera& camera);
	void				BindTexture(const Texture* texture);
	void				DrawVertexArray(int numVertexes, const Vertex_PCU* vertexes);
	void				DrawVertexArray(int numVertexes, const Vertex_PCU* vertexes, int numIndexes, const unsigned int* indexes);
	void				DrawVertexArray(int numVertexes, const Vertex_PCUTBN* vertexes);
	void				DrawVertexArray(int numVertexes, const Vertex_PCUTBN* vertexes, int numIndexes, const unsigned int* indexes);
	void				DrawVertexBuffer(VertexBuffer* vbo,int vertexCount, int vertexOffset = 0 , VertexType vertType = VertexType::Vertex_PCU);
	void				DrawVertexBufferAndIndexBuffer(VertexBuffer* vbo, IndexBuffer* ibo, int indexCount, int vertexOffset = 0, VertexType vertType = VertexType::Vertex_PCU);


	BitmapFont*			CreateOrGetBitmapFont(char const* pathWithoutExtension);
	BitmapFont*			GetBitMapFontFromFileName(char const* pathWithoutExtension);
	BitmapFont*			CreateFontFromFile(char const* pathWithoutExtension);


	Texture*			CreateOrGetTextureFromFile(char const* imageFilePath);
	Texture*			GetTextureForFileName(char const* imageFilePath);
	Texture*			CreateTextureFromFile(char const* imageFilePath);
	Texture*			CreateTextureFromImage(const Image& image);
	Texture*			CreateTextureFromData(char const* name, IntVec2 dimensions, int bytesPerTexel, const void* texelData);

	RenderConfig const& GetConfig() const;

	//------------------------------------------------------------------------------------------------------
	Shader*				CreateOrGetShader(char const* shaderName, VertexType vertexType = VertexType::Vertex_PCU);
	Shader*				CreateShader(char const* shaderName, char const* sourceShader, VertexType vertexType = VertexType::Vertex_PCU);
	bool				CompileShaderToByteCode(std::vector<unsigned char>& out_ByteCode, char const* name, 
						char const* source, char const* entryPoint, char const* target);
	void				BindShader(Shader* shader,VertexType vertexType = VertexType::Vertex_PCU);

	//------------------------------------------------------------------------------------------------------
	VertexBuffer*		CreateVertexBuffer(const size_t size);
	IndexBuffer*		CreateIndexBuffer(const size_t size);
	void				CopyCPUToGPU(const void* data, size_t size, VertexBuffer*& vbo);
	void				CopyCPUToGPU(const void* data, size_t size, VertexBuffer*& vbo, const void* indexData , size_t sizeIndex, IndexBuffer*& ibo);
	
	//------------------------------------------------------------------------------------------------------
	// Bind buffers here
	void				BindVertexBuffer(VertexBuffer* vbo);
	void				BindVertexBuffer(VertexBuffer* vbo, int vertSize);
	void				BindIndexBuffer(IndexBuffer* ibo);
	void				BindConstantBuffer(int slot, ConstantBuffer* cbo);

	//------------------------------------------------------------------------------------------------------
	ConstantBuffer*		CreateConstantBuffer(const size_t size);
	void				CopyCPUToGPU(const void* data, size_t size, ConstantBuffer* cbo);

	// Blend mode
	//------------------------------------------------------------------------------------------------------
	void				SetBlendMode(BlendMode blendMode);
	void				SetDepthMode(DepthMode depthMode);
	void				SetStatesIfChanged();

	//------------------------------------------------------------------------------------------------------
	void				SetSamplerMode(SamplerMode samplerMode);

	//------------------------------------------------------------------------------------------------------
	void				SetRasterizerState(RasterizerMode rasterizerMode);
	void				CreateRasterizerStateBaseOnMode();

	//------------------------------------------------------------------------------------------------------
	void				CreateDepthStencilTextureandView();
	void				SetModelConstants(const Mat44& modelMatrix = Mat44(), const Rgba8& modelColor = Rgba8::WHITE);
	void				SetLightConstants(Vec3 sunDirection= Vec3(2.0f, 1.0f, -1.0f), float sunIntensity= 0.85f, float ambientIntensity = 0.35f);
protected:

	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	IDXGISwapChain* m_swapchain = nullptr;
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
	ID3D11InputLayout* m_inputLayoutForVertex_PCU = nullptr;
	ID3D11DepthStencilView*	m_depthStencilView = nullptr;
	ID3D11Texture2D* m_depthStencilTexture = nullptr;
	void* m_dxgiDebugodule = nullptr;
	void* m_dxgiDebug = nullptr;

	// Shader cache to contain all the shaders needed
	std::vector<Shader*> m_loadedShaders;
	Shader* m_currentShader = nullptr;
	Shader*	m_defaultShader = nullptr;
	VertexBuffer* m_immediateVBO = nullptr;
	IndexBuffer* m_indexBuffer = nullptr;
	ConstantBuffer* m_cameraCBO = nullptr;
	ConstantBuffer*	m_modelCBO = nullptr;
	ConstantBuffer*	m_lightCBO = nullptr;

	// Blend state member variable
	ID3D11BlendState* m_blendState = nullptr;
	BlendMode m_desiredBlendMode = BlendMode::ALPHA;
	ID3D11BlendState* m_blendStates[(int)(BlendMode::COUNT)] = {};

	// Sampler state variable
	ID3D11SamplerState* m_samplerState = nullptr;
	SamplerMode m_desiredSamplerMode = SamplerMode::POINT_CLAMP;
	ID3D11SamplerState* m_samplerStates[(int)SamplerMode::COUNT] = {};

	// Rasterizer state variable
	RasterizerMode m_rasterizerMode = RasterizerMode::SOLID_CULL_BACK;
	ID3D11RasterizerState* m_desiredRasterizerState = nullptr;
	ID3D11RasterizerState* m_rasterizedStates[(int)RasterizerMode::COUNT] = {};

	// Depth enabled mode
	DepthMode m_depthMode = DepthMode::ENABLED;
	ID3D11DepthStencilState* m_depthStencilState = nullptr;
	ID3D11DepthStencilState* m_depthStencilStates[(int)DepthMode::COUNT] = {};
	// Texture variables
	const Texture* m_defaultTexture = nullptr;

private:

	// Private data members here
	RenderConfig				m_config;
	void*						m_rc = nullptr;
	std::vector<Texture*>		m_loadedTextures;
	std::vector<BitmapFont*>	m_loadedFonts;
};