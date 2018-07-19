//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "FurComponent.h"
#include "..\Base\GeneralStructs.h"
#include "..\Content\ContentManager.h"
#include "..\Graphics\MeshFilter.h"
#include "..\Graphics\FurMaterial.h"
#include "..\Graphics\ModelAnimator.h"


FurComponent::FurComponent(wstring assetFile):
ModelComponent::ModelComponent(assetFile)
{
}


FurComponent::~FurComponent()
{
}

void FurComponent::Initialize(const GameContext& gameContext)
{
	m_pMeshFilter = ContentManager::Load<MeshFilter>(m_AssetFile);
	m_pMeshFilter->BuildIndexBuffer(gameContext);

	if (m_pMeshFilter->m_HasAnimations)
		m_pAnimator = new ModelAnimator(m_pMeshFilter);

	UpdateMaterial(gameContext);
};

inline void FurComponent::UpdateMaterial(const GameContext& gameContext)
{
	if (m_MaterialSet)
	{
		auto mat = gameContext.pMaterialManager->GetMaterial_Fur(m_MaterialId);
		if (mat == nullptr)
		{
			Logger::LogFormat(LogLevel::Warning, L"FurComponent::UpdateMaterial > FurMaterial with ID \"%i\" doesn't exist!", m_MaterialId);
			return;
		}

		m_pFurMaterial = mat;
		m_pFurMaterial->Initialize(gameContext);
		m_pMeshFilter->BuildVertexBuffer(gameContext, m_pFurMaterial);
	}

}

void FurComponent::Update(const GameContext& gameContext)
{
	UpdateMaterial(gameContext);

	if (m_pAnimator)
		m_pAnimator->Update(gameContext);
};

void FurComponent::Draw(const GameContext& gameContext)
{ 
	if (!m_pFurMaterial)
	{
		Logger::LogWarning(L"FurComponent::Draw() > No FurMaterial!");
		return;
	}

	m_pFurMaterial->SetEffectVariables(gameContext, this);

	//Set Inputlayout
	gameContext.pDeviceContext->IASetInputLayout(m_pFurMaterial->GetInputLayout());

	//Set Vertex Buffer
	UINT offset = 0;
	auto vertexBufferData = m_pMeshFilter->GetVertexBufferData(gameContext, m_pFurMaterial);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &vertexBufferData.pVertexBuffer, &vertexBufferData.VertexStride, &offset);

	//Set Index Buffer
	gameContext.pDeviceContext->IASetIndexBuffer(m_pMeshFilter->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set Primitive Topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//DRAW
	auto tech = m_pFurMaterial->GetTechnique();
	D3DX11_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);

	while (m_pFurMaterial->NextLayerAndUpdate())
	{
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			tech->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
			gameContext.pDeviceContext->DrawIndexed(m_pMeshFilter->m_IndexCount, 0, 0);
		}
	}

};

