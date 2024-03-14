// Copyright Emircan Birbil


#include "Character/AuraEnemy.h"

#include "Aura/Aura.h"

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
}

AAuraEnemy::AAuraEnemy()
{
	// Make sure visibility is blocked since it is used for cursor targeting.
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

