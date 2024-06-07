#pragma once
#include <string>
//------------------------------------------------------------
struct ShaderConfig
{
	std::string m_name;

	std::string m_vertexShaderName = "VertexShader";
	std::string m_pixelShaderName = "PixelShader";

	std::string m_vertexEntryPoint = "VertexMain";
	std::string m_pixelEntryPoint = "PixelMain";

	std::string m_vertexFlag = "vs_5_0";
	std::string m_pixelFlag = "ps_5_0";
};


struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

//------------------------------------------------------------
class Shader
{
	friend class Renderer;
public:

	Shader(const ShaderConfig& config);
	Shader(const Shader& copy) = delete;
	~Shader();

	const std::string& GetName() const;

	ShaderConfig m_config;
	ID3D11VertexShader* m_vertexShader = nullptr;
	ID3D11PixelShader* m_pixelShader = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;

};

