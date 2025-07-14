#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Figure.h"
#include "GameFramework/Actor.h"
#include "ChessActor.generated.h"

UCLASS()
class CHESSPLUGIN_API UChessData : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
		UStaticMesh* Board;

	UPROPERTY(EditAnywhere, Category = "Figures")
		UStaticMesh* Pawn;

	UPROPERTY(EditAnywhere, Category = "Figures")
		UStaticMesh* Rock;

	UPROPERTY(EditAnywhere, Category = "Figures")
		UStaticMesh* Knight;

	UPROPERTY(EditAnywhere, Category = "Figures")
		UStaticMesh* Bishop;

	UPROPERTY(EditAnywhere, Category = "Figures")
		UStaticMesh* Queeu;

	UPROPERTY(EditAnywhere, Category = "Figures")
		UStaticMesh* King;

	UPROPERTY(EditAnywhere, Category = "Figures")
		UMaterialInterface* MaterialWhite;

	UPROPERTY(EditAnywhere, Category = "Figures")
		UMaterialInterface* MaterialBlack;

	UStaticMesh* GetFigureMesh(EFigureType Type) const {

		switch (Type)
		{
		case EFigureType::Pawn:
			return Pawn;
			break;
		case EFigureType::Rock:
			return Rock;
			break;
		case EFigureType::Knight:
			return Knight;
			break;
		case EFigureType::Bishop:
			return Bishop;
			break;
		case EFigureType::Queeu:
			return Queeu;
			break;
		case EFigureType::King:
			return King;
			break;
		}

		return nullptr;

	};
};


UCLASS()
class CHESSPLUGIN_API AChessActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChessActor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UChessData* ChessData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float FieldSize = 600;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float OffsetZ = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float OffsetYaw = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float SizeGrid;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsCheckKing;

	UPROPERTY(BlueprintReadWrite)
		uint8 ActiveTeam;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TArray<FFigure> Figures;

public:

	void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable)
		bool Move(FIntPoint From, FIntPoint To);
		
	UFUNCTION(BlueprintPure)
		TArray<FIntPoint> GetMove(FIntPoint From);
		
	UFUNCTION(BlueprintPure)
		bool IsCeilBusy(FIntPoint Location) const;

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Chess")
		void OnCheck(FIntPoint KingLocaton, FIntPoint EnemyLocation);

	virtual void BeginPlay() override;

private:

	bool CheckMove();

	bool IsCheckKing(FFigure Figure);

};
