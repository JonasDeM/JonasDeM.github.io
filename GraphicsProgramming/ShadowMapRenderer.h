#pragma once

class MeshFilter;
class ShadowMapMaterial;
class RenderTarget;

class ShadowMapRenderer
{
public:
	ShadowMapRenderer();
	~ShadowMapRenderer();
	void Begin(const GameContext& gameContext);
	void Draw(const GameContext& gameContextm, MeshFilter* pMeshFilter, XMFLOAT4X4 world);
	void End(const GameContext& gameContext);
	XMFLOAT3 GetLightDirection() { return m_LightDirection; };
	XMFLOAT4X4 GetLightVP() { return m_LightVP; };
	ShadowMapMaterial* GetMaterial() { return m_pShadowMapMat; };
	ID3D11ShaderResourceView* GetShadowMapSRV() ;
	void Initialize(const GameContext& gameContext);
	void SetLight(XMFLOAT3 position, XMFLOAT3 direction);
private:

	bool m_IsInitialized = false;
	XMFLOAT3 m_LightDirection = XMFLOAT3(0, 0, 0);
	XMFLOAT3 m_LightPosition = XMFLOAT3(0, 0, 0);
	XMFLOAT4X4 m_LightVP = XMFLOAT4X4();

	ShadowMapMaterial* m_pShadowMapMat = nullptr;
	RenderTarget* m_pShadowRT = nullptr;

	const float m_Size = 1.0f; //wtf is this

	ShadowMapRenderer(const ShadowMapRenderer& yRef);
	ShadowMapRenderer& operator=(const ShadowMapRenderer yRef);
};

