#pragma once
#include "ModelComponent.h"

class MeshFilter;
class ModelAnimator;
class Material;

class FurComponent: public ModelComponent
{
public:
	FurComponent(wstring assetFile);
	~FurComponent();

protected:

	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void Initialize(const GameContext& gameContext) override;

private:

	void UpdateMaterial(const GameContext& gameContext);
	FurMaterial* m_pFurMaterial = nullptr;



private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	FurComponent(const FurComponent& yRef) = delete;
	FurComponent& operator=(const FurComponent& yRef) = delete;

};

