//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ModelAnimator.h"
#include "../Diagnostics/Logger.h"


ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
m_pMeshFilter(pMeshFilter),
m_Transforms(vector<XMFLOAT4X4>()),
m_IsPlaying(false), 
m_Reversed(false),
m_ClipSet(false),
m_TickCount(0),
m_AnimationSpeed(1.0f)
{
	SetAnimation(0);
}


ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	if (!(clipNumber < GetClipCount()))
	{
		Reset();
		Logger::LogWarning(L"ModelAnimator::SetAnimation> clipnumber is smaller than amount animationClips");
		return;
	}
	else
	{
		SetAnimation(m_pMeshFilter->m_AnimationClips[clipNumber]);
	}
	//If not,
	//	Call Reset
	//	Log a warning with an appropriate message
	//	return
	//else
	//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
	//	Call SetAnimation(AnimationClip clip)
}

void ModelAnimator::SetAnimation(wstring clipName)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	for (AnimationClip a : m_pMeshFilter->m_AnimationClips)
	{
		if (a.Name == clipName)
		{
			SetAnimation(a);
			return;
		}
	}
	Reset();
	Logger::LogWarning(L"ModelAnimator::SetAnimation> an animationCip with that name doesn't exist");
	
	//If found,
	//	Call SetAnimation(Animation Clip) with the found clip
	//Else
	//	Call Reset
	//	Log a warning with an appropriate message
}

void ModelAnimator::SetAnimation(AnimationClip clip)
{
	//Set m_ClipSet to true
	m_ClipSet = true;
	//Set m_CurrentClip
	m_CurrentClip = clip;
	
	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	//If pause is true, set m_IsPlaying to false
	if (pause) m_IsPlaying = false;
	//Set m_TickCount to zero
	m_TickCount = 0;
	//Set m_AnimationSpeed to 1.0f
	m_AnimationSpeed = 1.0f;
	
	//If m_ClipSet is true
	if (m_ClipSet)
	{
		m_Transforms.assign(m_CurrentClip.Keys[0].BoneTransforms.begin()
			, m_CurrentClip.Keys[0].BoneTransforms.end());
	}
	else
	{
		XMFLOAT4X4 identity;
		XMStoreFloat4x4(&identity,XMMatrixIdentity());

		m_Transforms.assign(m_pMeshFilter->m_BoneCount,identity);
	}
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
	//Else
	//	Create an IdentityMatrix 
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
}

void ModelAnimator::Update(const GameContext& gameContext)
{
	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		auto passedTicks = gameContext.pGameTime->GetElapsed() *
			m_CurrentClip.TicksPerSecond * m_AnimationSpeed;

		fmod(passedTicks, m_CurrentClip.Duration);
		//Calculate the passedTicks (see the lab document)
		//auto passedTicks = ...
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)

		//2. 
		//IF m_Reversed is true
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0)
			{
				m_TickCount += m_CurrentClip.Duration;
			}
		}
		else
		{
			m_TickCount += passedTicks;

			if (m_TickCount > m_CurrentClip.Duration)
			{
				m_TickCount -= m_CurrentClip.Duration;
			}
		}
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount

		//3.
		//Find the enclosing keys
		AnimationKey keyA, keyB;
		keyA.Tick = -FLT_MAX;
		keyB.Tick = FLT_MAX;
		for (AnimationKey k : m_CurrentClip.Keys)
		{
			if (k.Tick <= m_TickCount && k.Tick > keyA.Tick)
			{
				keyA = k;
			}
			if (k.Tick > m_TickCount && k.Tick < keyB.Tick)
			{
				keyB = k;
			}
		}
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount

		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		auto blendfactor = (m_TickCount - keyA.Tick) / (keyB.Tick - keyA.Tick);
		//Clear the m_Transforms vector
		m_Transforms.clear();
		//FOR every boneTransform in a key (So for every bone)
		
		for (size_t i = 0; i < keyA.BoneTransforms.size(); i++)
		{
			auto transformA = keyA.BoneTransforms[i];
			auto transformB = keyB.BoneTransforms[i];

			XMVECTOR scaleA;
			XMVECTOR rotA;
			XMVECTOR translA;
			XMMatrixDecompose(&scaleA, &rotA, &translA, XMLoadFloat4x4(&transformA));

			XMVECTOR scaleB;
			XMVECTOR rotB;
			XMVECTOR translB;
			XMMatrixDecompose(&scaleB, &rotB, &translB, XMLoadFloat4x4(&transformB));


			XMMATRIX s = XMMatrixScalingFromVector(XMVectorLerp(scaleA, scaleB, blendfactor));
			XMMATRIX r = XMMatrixRotationQuaternion(XMQuaternionSlerp(rotA, rotB, blendfactor));
			XMMATRIX t = XMMatrixTranslationFromVector(XMVectorLerp(translA, translB, blendfactor));

			XMFLOAT4X4 newTransform;
			XMStoreFloat4x4(&newTransform, XMMatrixMultiply(XMMatrixMultiply(s, r), t));
			m_Transforms.push_back(newTransform);
			//	Retrieve the transform from keyA (transformA)
			//	auto transformA = ...
			// 	Retrieve the transform from keyB (transformB)
			//	auto transformB = ...
			//	Decompose both transforms
			//	Lerp between all the transformations (Position, Scale, Rotation)
			//	Compose a transformation matrix with the lerp-results
			//	Add the resulting matrix to the m_Transforms vector
		}
	}
}
