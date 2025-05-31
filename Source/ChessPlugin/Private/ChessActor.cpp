//Copyright(c) 2025 Viktor Fritsler

#include "ChessActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

AChessActor::AChessActor()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void AChessActor::BeginPlay()
{
	Super::BeginPlay();

}

void AChessActor::Move(FIntPoint From, FIntPoint To)
{
	FVector2D L_Start = FVector2D(FieldSize / 2.0f) * -1.0f;
	float L_GripCenter = SizeGrid / 2.0f;
	FVector2D L_Position = L_Start + FVector2D(L_GripCenter);

	TArray<FIntPoint> L_Moves;

	for (int32 i = 0; i < Figures.Num(); i++) {

		if (Figures[i].Location == From) {

			Figures[i].GetMoves(L_Moves);

			for (int32 b = 0; b < L_Moves.Num(); b++) {

				if (To == L_Moves[b]) {

					for (int32 c = 0; c < Figures.Num(); c++)
					{
						if (Figures[c].Location == To) {

							if (Figures[c].Team != Figures[i].Team) {

								Figures[c].Figure->DestroyComponent();
								Figures[i].SetLocation(L_Position, SizeGrid, To, OffsetZ);
								Figures.RemoveAt(c);

								return;
							}else {
								return;
							}
						}
					}
					
					Figures[i].SetLocation(L_Position, SizeGrid, To, OffsetZ);
					return;
				}
			}

			break;
		}
	}
}

TArray<FIntPoint> AChessActor::GetMove(FIntPoint From)
{
	FVector2D L_Start = FVector2D(FieldSize / 2.0f) * -1.0f;
	float L_GripCenter = SizeGrid / 2.0f;

	TArray<FIntPoint> L_Moves;

	for (int32 i = 0; i < Figures.Num(); i++) {

		if (Figures[i].Location == From) {

			Figures[i].GetMoves(L_Moves);

			for (int32 b = 0; b < L_Moves.Num(); b++) {

				FIntPoint L_I = L_Moves[b] * SizeGrid;
				L_I += FIntPoint(L_Start.X + L_GripCenter, L_Start.Y + L_GripCenter);
				L_Moves[b] = L_I;
			}
		}
	}

	return L_Moves;
}

bool AChessActor::IsCeilBusy(FIntPoint Location) const
{
	for (int32 i = 0; i < Figures.Num(); i++)
	{
		if (Figures[i].Location == Location) {
			return true;
		}
	}

	return false;
}

void AChessActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FlushPersistentDebugLines(GetWorld());
	Figures.Empty();

	if (!IsValid(ChessData))
		return;

	Root->SetStaticMesh(ChessData->Board);

	FVector2D L_Start = FVector2D(FieldSize / 2.0f) * -1.0f;
	FVector2D L_End = FVector2D(FieldSize / 2.0f);

	SizeGrid = FieldSize / 8.0f;
	float L_GripCenter = SizeGrid / 2.0f;

	for (int32 Team = 0; Team < 2; Team++) {

		FVector2D L_Position = L_Start + FVector2D(L_GripCenter);

		if (Team == 1) {
			L_Position = L_End - FVector2D(L_GripCenter);
		}

		for (int32 i = 0; i < 8; i++)
		{
			EFigureType L_FigureT = EFigureType::Pawn;

			auto L_Figure_Pawn = NewObject<UStaticMeshComponent>(this, *FString::Printf(TEXT("%s_%s_%d"), TEXT("Pawn"), Team == 0 ? TEXT("White") : TEXT("Black"), i));
			AddInstanceComponent(L_Figure_Pawn);
			L_Figure_Pawn->RegisterComponent();
			L_Figure_Pawn->CreationMethod = EComponentCreationMethod::Instance;
			Figures.Add(FFigure(this, L_Figure_Pawn, Team, Team == 0 ? FIntPoint(1, i) : FIntPoint(6, 7 - i), L_FigureT));
			L_Figure_Pawn->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			L_Figure_Pawn->SetStaticMesh(ChessData->GetFigureMesh(L_FigureT));

			if (Team == 0) {
				L_Figure_Pawn->SetRelativeLocation(FVector(L_Position + FVector2D(SizeGrid, SizeGrid * i), OffsetZ));
				L_Figure_Pawn->SetRelativeRotation(FRotator(0.0f, -OffsetYaw, 0.0f));
				L_Figure_Pawn->SetMaterial(0, ChessData->MaterialWhite);
			}
			else {
				L_Figure_Pawn->SetRelativeLocation(FVector(L_Position - FVector2D(SizeGrid, SizeGrid * i), OffsetZ));
				L_Figure_Pawn->SetRelativeRotation(FRotator(0.0f, OffsetYaw, 0.0f));
				L_Figure_Pawn->SetMaterial(0, ChessData->MaterialBlack);
			}

			switch (i)
			{
			case 0:
				L_FigureT = EFigureType::Rock;
				break;
			case 1:
				L_FigureT = EFigureType::Knight;
				break;
			case 2:
				L_FigureT = EFigureType::Bishop;
				break;
			case 3:
				L_FigureT = EFigureType::Queeu;
				break;
			case 4:
				L_FigureT = EFigureType::King;
				break;
			case 5:
				L_FigureT = EFigureType::Bishop;
				break;
			case 6:
				L_FigureT = EFigureType::Knight;
				break;
			case 7:
				L_FigureT = EFigureType::Rock;
				break;
			}

			auto L_Figure = NewObject<UStaticMeshComponent>(this, *FString::Printf(TEXT("%s_%s%s"), Team == 0 ? TEXT("White") : TEXT("Black"), *UEnum::GetDisplayValueAsText(L_FigureT).ToString(), i > 4 ? TEXT("_L") : TEXT("_R")));
			AddInstanceComponent(L_Figure);
			L_Figure->RegisterComponent();
			L_Figure->CreationMethod = EComponentCreationMethod::Instance;
			Figures.Add(FFigure(this, L_Figure, Team, Team == 0 ? FIntPoint(0, i) : FIntPoint(7, 7 - i), L_FigureT));
			L_Figure->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			L_Figure->SetStaticMesh(ChessData->GetFigureMesh(L_FigureT));

			if (Team == 0) {
				L_Figure->SetRelativeLocation(FVector(L_Position + FVector2D(0, SizeGrid * i), OffsetZ));
				L_Figure->SetRelativeRotation(FRotator(0.0f, -OffsetYaw, 0.0f));
				L_Figure->SetMaterial(0, ChessData->MaterialWhite);
			}
			else {
				L_Figure->SetRelativeLocation(FVector(L_Position - FVector2D(0, SizeGrid * i), OffsetZ));
				L_Figure->SetRelativeRotation(FRotator(0.0f, OffsetYaw, 0.0f));
				L_Figure->SetMaterial(0, ChessData->MaterialBlack);
			}

		}
	}

	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(FVector2D(FieldSize / 2.0f), 2.0f), FColor::Red, false, INFINITY);
}