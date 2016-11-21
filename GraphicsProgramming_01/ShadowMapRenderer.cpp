#include "stdafx.h"
#include "ShadowMapRenderer.h"
#include "ShadowMapMaterial.h"
#include "../Scenegraph/SceneManager.h"
#include "../Base/OverlordGame.h"
#include "RenderTarget.h"
#include "MeshFilter.h"

ShadowMapRenderer::ShadowMapRenderer()
{
	m_pShadowMapMat = new ShadowMapMaterial();
}


ShadowMapRenderer::~ShadowMapRenderer()
{
	SafeDelete(m_pShadowRT);
	SafeDelete(m_pShadowMapMat);
}


void ShadowMapRenderer::Initialize(const GameContext & gameContext)
{

	if (m_IsInitialized)
	{
		return;
	}

	//MAKE RENDERTARGET
	m_pShadowRT = new RenderTarget(gameContext.pDevice);

	RENDERTARGET_DESC rtDesc;

	rtDesc.Height = OverlordGame::GetGameSettings().Window.Height;
	rtDesc.Width = OverlordGame::GetGameSettings().Window.Width;
	rtDesc.EnableDepthBuffer = true;
	rtDesc.EnableDepthSRV = true;
	rtDesc.GenerateMipMaps_Color = false;
	// logs errors and warnings on itself
	auto hr = m_pShadowRT->Create(rtDesc);
	Logger::LogHResult(hr, L"ShadowMapRenderer::Initialize> create shadowmap RenderTarget failed");

	//MAKE MATERIAL

	m_pShadowMapMat->Initialize(gameContext);

	m_IsInitialized = true;


}

void ShadowMapRenderer::Begin(const GameContext & gameContext)
{
	if (!m_IsInitialized)
	{
		Logger::LogError(L" ShadowRenderer::Begin > ShadowMapRenderer is not initialized");
	}
	//reset texture register (unbind)
	ID3D11ShaderResourceView *const pSRV[1] = { NULL };
	gameContext.pDeviceContext->PSSetShaderResources(1, 1, pSRV); //startslot?


	//set this rendertarget as current in game
	SceneManager::GetInstance()->GetGame()->SetRenderTarget(m_pShadowRT);
	//gameContext.pDeviceContext->ClearDepthStencilView(m_pShadowRT->GetDepthStencilView(), D3D11_CLEAR_DEPTH,1.0f,0);
	m_pShadowRT->Clear(gameContext, Colors::Pink);

	m_pShadowMapMat->SetLightVP(m_LightVP);
}

void ShadowMapRenderer::Draw(const GameContext & gameContext, MeshFilter *pMeshFilter, XMFLOAT4X4 world)
{
	//Set effect variables
	m_pShadowMapMat->SetWorld(world);
	m_pShadowMapMat->SetLightVP(m_LightVP); //does it needs to be set every frame?


	//Set Inputlayout
	gameContext.pDeviceContext->IASetInputLayout(m_pShadowMapMat->GetInputLayout());

	//Set Vertex Buffer
	UINT offset = 0;
	auto vertexBufferData = pMeshFilter->GetVertexBufferData(gameContext, m_pShadowMapMat);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &vertexBufferData.pVertexBuffer, &vertexBufferData.VertexStride, &offset);

	//Set Index Buffer
	gameContext.pDeviceContext->IASetIndexBuffer(pMeshFilter->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set Primitive Topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//DRAW
	auto tech = m_pShadowMapMat->GetTechnique();
	D3DX11_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		tech->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(pMeshFilter->m_IndexCount, 0, 0);
	}
}

void ShadowMapRenderer::End(const GameContext & gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//now we can use it as a srv
	SceneManager::GetInstance()->GetGame()->SetRenderTarget(nullptr);

}


void ShadowMapRenderer::SetLight(XMFLOAT3 position, XMFLOAT3 direction)
{
	//m_LightPosition = position;
	//m_LightDirection = direction;


	//XMVECTOR posVec = XMLoadFloat3(&position);
	//XMVECTOR dirVec = XMLoadFloat3(&direction);
	//auto up = XMLoadFloat3(&XMFLOAT3(0,1,0));
	////m_LightVP calculate
	//XMStoreFloat4x4(&m_LightVP
	//				,XMMatrixLookAtLH(posVec,XMVectorAdd(posVec,dirVec),up));

	m_LightPosition = position;
	m_LightDirection = direction;

	XMStoreFloat4x4(&m_LightVP, XMMatrixIdentity());
	auto windowSettings = OverlordGame::GetGameSettings().Window;
	XMMATRIX projection, view;

	float viewWidth = (m_Size>0) ? m_Size * windowSettings.AspectRatio : windowSettings.Width;
	float viewHeight = (m_Size>0) ? m_Size : windowSettings.Height;
	projection = XMMatrixOrthographicLH(viewWidth, viewHeight, 0.0f, 800.0f);

	XMVECTOR worldPosition = XMLoadFloat3(&m_LightPosition);
	XMVECTOR lookAt = XMLoadFloat3(&m_LightDirection);
	XMFLOAT3 up = XMFLOAT3(0, 1, 0); 
	XMVECTOR upVec = XMLoadFloat3(&up);

	view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);

	XMStoreFloat4x4(&m_LightVP, view * projection);
}

ID3D11ShaderResourceView * ShadowMapRenderer::GetShadowMapSRV()
{
	return m_pShadowRT->GetDepthShaderResourceView();
}