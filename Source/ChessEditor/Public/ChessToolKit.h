#pragma once

#include "CoreMinimal.h"
#include "ChessActor.h"
#include "PreviewScene.h"
#include "GameFramework/WorldSettings.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/LineBatchComponent.h"
#include "Components/ChildActorComponent.h"
#include "Toolkits/AssetEditorToolkit.h"

class  FTestPreviewScene : public FPreviewScene
{
public:
	AChessActor* ChessActor;

	FTestPreviewScene(ConstructionValues CVS);

	void AddChess(UChessData* NewAsset) {

		ChessActor = PreviewWorld->SpawnActorDeferred<AChessActor>(AChessActor::StaticClass(), FTransform::Identity);
		ChessActor->ChessData = NewAsset;
		ChessActor->FinishSpawning(FTransform::Identity);
	};
};

FTestPreviewScene::FTestPreviewScene(ConstructionValues CVS) :FPreviewScene(CVS)
{
	// world setting
	GetWorld()->GetWorldSettings()->NotifyBeginPlay();
	GetWorld()->GetWorldSettings()->NotifyMatchStarted();
	GetWorld()->GetWorldSettings()->SetActorHiddenInGame(false);
	GetWorld()->bBegunPlay = true;
	// set light options 
	DirectionalLight->SetRelativeLocation(FVector(-1024.f, 1024.f, 2048.f));
	DirectionalLight->SetRelativeScale3D(FVector(15.f));
	
	SetLightBrightness(4.f);
	DirectionalLight->InvalidateLightingCache();
	DirectionalLight->RecreateRenderState_Concurrent();

	UStaticMeshComponent* SkyComp = NewObject<UStaticMeshComponent>();
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(NULL, TEXT("/Engine/MapTemplates/Sky/SM_SkySphere.SM_SkySphere"), NULL, LOAD_None, NULL);
	SkyComp->SetStaticMesh(StaticMesh);
	UMaterial* SkyMaterial = LoadObject<UMaterial>(NULL, TEXT("/Engine/EditorMaterials/PersonaSky.PersonaSky"), NULL, LOAD_None, NULL);
	SkyComp->SetMaterial(0, SkyMaterial);
	const float SkySphereScale = 1000.f;
	const FTransform SkyTransform(FRotator(0, 0, 0), FVector(0, 0, 0), FVector(SkySphereScale));
	AddComponent(SkyComp, SkyTransform);
}

class FChessToolKit : public FAssetEditorToolkit
{
public:
	void InitEditor(const TArray<UObject*>& InObjects);

	void RegisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;
	void UnregisterTabSpawners(const TSharedRef<class FTabManager>& TabManager) override;

	FName GetToolkitFName() const override { return "ChessDataEditor"; }
	FText GetBaseToolkitName() const override { return INVTEXT("Chess Data Editor"); }
	FString GetWorldCentricTabPrefix() const override { return "Chess Data"; }
	FLinearColor GetWorldCentricTabColorScale() const override { return {}; }
private:
	void OnChangingProperties(const FPropertyChangedEvent& Property);

	FTestPreviewScene* PreviewScene;
	UChessData* ChessData;
};