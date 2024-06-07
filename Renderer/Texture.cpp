#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

Texture::Texture()
{

}

Texture::~Texture()
{
	DX_SAFE_RELEASE(m_shaderResourceView);
	DX_SAFE_RELEASE(m_texture);
}
