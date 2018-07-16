#pragma once
#include "Material.h"

class ShadowMapMaterial : public Material
{
public:
	ShadowMapMaterial();
	~ShadowMapMaterial();

	void SetLightVP(XMFLOAT4X4 lightVP);
	void SetWorld(XMFLOAT4X4 world);
	void UpdateVariables(const GameContext& gameContext, ModelComponent* pModelComponent) { UpdateEffectVariables(gameContext, pModelComponent); };

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);


private:

	ID3DX11EffectMatrixVariable* m_pLightVPMatrixVariable = nullptr;
	ID3DX11EffectMatrixVariable* m_pWorldMatrixVariable = nullptr;

	XMFLOAT4X4 m_LightVP;
	XMFLOAT4X4 m_WorldMatrix;
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ShadowMapMaterial(const ShadowMapMaterial &obj);
	ShadowMapMaterial& operator=(const ShadowMapMaterial& obj);
};

