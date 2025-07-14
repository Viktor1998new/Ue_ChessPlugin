#pragma once
#include "CoreMinimal.h"
#include "Figure.generated.h"

UENUM(BlueprintType)
enum class EFigureType : uint8 {

	Pawn = 0,
	Rock = 1,
	Knight = 2,
	Bishop = 3,
	Queeu = 4,
	King = 5,
};

USTRUCT(BlueprintType)
struct CHESSPLUGIN_API FFigure {

	GENERATED_BODY()

	UPROPERTY()
		class AChessActor* Chess;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		EFigureType Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		uint8 Team;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Figure;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FIntPoint Location;

	UPROPERTY()
		bool IsMove = false;

	UPROPERTY()
		bool IsDestroy = false;

	FFigure();

	FFigure(AChessActor* Chess, UStaticMeshComponent* Figure, const uint8& Team, FIntPoint Location, EFigureType Type);

	void GetMoves(TArray<FIntPoint>& Moves, bool IsCheckKing) const;

	void SetLocation(FIntPoint NewLocation);

	bool IsMoveSafe(FIntPoint Move) const;
};