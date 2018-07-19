//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ShadowMapMaterial.h"
#include "../Base/GeneralStructs.h"
#include "../Content/ContentManager.h"
#include "../Components/ModelComponent.h"


ShadowMapMaterial::ShadowMapMaterial() : 
	Material(L"./Resources/Effects/ShadowMap.fx", L"")
{

}


ShadowMapMaterial::~ShadowMapMaterial()
{
}

void ShadowMapMaterial::SetLightVP( XMFLOAT4X4 lightVP)
{
	m_LightVP = lightVP;
	m_pLightVPMatrixVariable->SetMatrix(reinterpret_cast<float*>(&m_LightVP));
}

void ShadowMapMaterial::SetWorld( XMFLOAT4X4 world)
{
	m_WorldMatrix = world;
	m_pWorldMatrixVariable->SetMatrix(reinterpret_cast<float*>(&m_WorldMatrix));
}


void ShadowMapMaterial::LoadEffectVariables()
{
	m_pWorldMatrixVariable = m_pEffect->GetVariableBySemantic("WORLD")->AsMatrix();
	m_pLightVPMatrixVariable = m_pEffect->GetVariableByName("gLightViewProj")->AsMatrix();
}

void ShadowMapMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);


}
