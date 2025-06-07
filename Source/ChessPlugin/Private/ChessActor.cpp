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

bool AChessActor::CheckMove()
{
	return false;
}

void AChessActor::BeginPlay()
{
	Super::BeginPlay();

}

bool AChessActor::Move(FIntPoint From, FIntPoint To)
{
	FVector2D L_Start = FVector2D(FieldSize / 2.0f) * -1.0f;
	float L_GripCenter = SizeGrid / 2.0f;
	FVector2D L_Position = L_Start + FVector2D(L_GripCenter);
	
	FFigure* L_Active = nullptr;
	
	FFigure* L_King = nullptr;

	TArray<FIntPoint> L_Moves;

	for (int32 i = 0; i < Figures.Num(); i++) {

		if (Figures[i].Location == From) {

			if (Figures[i].Team != ActiveTeam)
				return false;
			else
				L_Active = &Figures[i];
		}
	}
	
	if (L_Active == nullptr)
		return false;

	for (int32 i = 0; i < Figures.Num(); i++) {

		if (Figures[i].Team == ActiveTeam && Figures[i].Type == EFigureType::King)
		{
			L_King = &Figures[i];
			break;
		}
	}

	FIntPoint VectorLength = To - L_Active->Location;

	int length = FMath::TruncToInt(FMath::Sqrt(FMath::Pow(VectorLength.X, 2.f) + FMath::Pow(VectorLength.Y, 2.f)));

	if (length == 0)
		return false;

	FIntPoint Direction = FIntPoint::ZeroValue;

	if (VectorLength.X != 0)
		Direction.X = VectorLength.X / FMath::Abs(VectorLength.X);
	
	if (VectorLength.Y != 0)
		Direction.Y = VectorLength.Y / FMath::Abs(VectorLength.Y);

	bool L_IsMove = false;

	switch (L_Active->Type)
	{
	case EFigureType::Pawn:

		if (!L_Active->IsMove) {

			if (L_Active->Team == 0 && Direction.X > 0 && length <= 2) {
				L_IsMove = L_Active->Location.Y == To.Y;
			}else if(L_Active->Team == 1 && Direction.X < 0 && length <= 2) {
				L_IsMove = L_Active->Location.Y == To.Y;
			}
		}
		
		if (L_Active->Team == 0 && Direction.X > 0 && length == 1) {
			L_IsMove = L_Active->Location.X == To.X || L_Active->Location.Y == To.Y || FMath::Abs(L_Active->Location.X - To.X) == FMath::Abs(L_Active->Location.Y - To.Y);
		}
		else if (L_Active->Team == 1 && Direction.X < 0 && length == 1) {
			L_IsMove = L_Active->Location.X == To.X || L_Active->Location.Y == To.Y || FMath::Abs(L_Active->Location.X - To.X) == FMath::Abs(L_Active->Location.Y - To.Y);
		}
		
		break;
	
	case EFigureType::Bishop:

		L_IsMove = FMath::Abs(L_Active->Location.X - To.X) == FMath::Abs(L_Active->Location.Y - To.Y);

		break;

	case EFigureType::Rock:

		L_IsMove = L_Active->Location.X == To.X || L_Active->Location.Y == To.Y;

		break;

	case EFigureType::Queeu:

		L_IsMove = L_Active->Location.X == To.X || L_Active->Location.Y == To.Y || FMath::Abs(L_Active->Location.X - To.X) == FMath::Abs(L_Active->Location.Y - To.Y);

		break;
	
	case EFigureType::King:

		if (length == 1) {
			L_IsMove = L_Active->Location.X == To.X || L_Active->Location.Y == To.Y || FMath::Abs(L_Active->Location.X - To.X) == FMath::Abs(L_Active->Location.Y - To.Y);
		}

		break;

	case EFigureType::Knight:

		int knightMoves[8][2] = {
			   {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
			   {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
		};

		for (const auto& knightMove : knightMoves) {
			int newX = L_Active->Location.X + knightMove[0];
			int newY = L_Active->Location.Y + knightMove[1];
			if (newX == To.X && newY == To.Y) {
				L_IsMove = true;
				break;
			}
		}

		break;
	}

	if (L_IsMove) {

		int32 L_IndexDestroy = INDEX_NONE;

		for (int32 i = 0; i < Figures.Num(); i++)
		{
			if (Figures[i].Location == From)
				continue;

			if (Figures[i].Location == To)
			{
				L_IndexDestroy = i;
			}
		}

		switch (L_Active->Type)
		{

		case EFigureType::Pawn:

			if (L_Active->Location.Y != To.Y) {

				if(L_IndexDestroy == INDEX_NONE)
					return false;
			}
			else {
				if (L_IndexDestroy != INDEX_NONE)
					return false;
			}

			break;

		case EFigureType::Knight:

			for (int32 i = 0; i < Figures.Num(); i++)
			{
				if (Figures[i].Location == From)
					continue;

				if (Figures[i].Location == To)
					L_IndexDestroy = i;
			}

			break;
		case EFigureType::King:

			if (length != 1)
				return false;

			for (int32 i = 0; i < Figures.Num(); i++)
			{
				if (Figures[i].Location == From)
					continue;

				if (Figures[i].Location == To)
					L_IndexDestroy = i;
			}

			break;

		case EFigureType::Bishop:
		case EFigureType::Queeu:
		case EFigureType::Rock:

			for (int32 x = 1; x < FMath::Abs(length + 1); x++)
			{
				FIntPoint CurrentPoint = From + (Direction * x);

				L_Moves.Add(CurrentPoint);

				for (int32 i = 0; i < Figures.Num(); i++)
				{
					if (Figures[i].Location == From)
						continue;

					if (Figures[i].Location == CurrentPoint)
						L_IndexDestroy = i;
				}
			}
			break;
		}
	
		FIntPoint L_OldLocation = L_Active->Location;
	
		if (L_IndexDestroy != INDEX_NONE) {
			
			if (Figures[L_IndexDestroy].Team != L_Active->Team) {

				L_Active->SetLocation(L_Position, SizeGrid, To, OffsetZ);

				if (L_King->IsMoveSafe(L_OldLocation)) {
					L_Active->SetLocation(L_Position, SizeGrid, L_OldLocation, OffsetZ);
					return false;
				}

				Figures[L_IndexDestroy].Figure->DestroyComponent();
				Figures.RemoveAt(L_IndexDestroy);
				bIsAttackKing = IsAttackKing(*L_Active);

				ActiveTeam = ActiveTeam == 0 ? 1 : 0;
				L_Active->IsMove = true;
				return true;
			}
		}
		else {
			L_Active->SetLocation(L_Position, SizeGrid, To, OffsetZ);
			
			if (L_King->IsMoveSafe(L_OldLocation)) {
				L_Active->SetLocation(L_Position, SizeGrid, L_OldLocation, OffsetZ);
				return false;
			}
			
			bIsAttackKing = IsAttackKing(*L_Active);
			ActiveTeam = ActiveTeam == 0 ? 1 : 0;
			L_Active->IsMove = true;
			return true;
		}
	}

	return false;
}

TArray<FIntPoint> AChessActor::GetMove(FIntPoint From)
{
	FVector2D L_Start = FVector2D(FieldSize / 2.0f) * -1.0f;
	float L_GripCenter = SizeGrid / 2.0f;

	TArray<FIntPoint> L_Moves;

	for (int32 i = 0; i < Figures.Num(); i++) {

		if (Figures[i].Location == From) {

			Figures[i].GetMoves(L_Moves, bIsAttackKing);

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

bool AChessActor::IsAttackKing(FFigure Figure)
{
	TArray<FIntPoint> L_Moves;

	Figure.GetMoves(L_Moves, false);

	FIntPoint L_LocationKing;
	for (int32 f = 0; f < Figures.Num(); f++) {

		if (Figure.Team == Figures[f].Team && Figures[f].Type == EFigureType::King) {
			L_LocationKing = Figures[f].Location;
			break;

		}
	}

	for (int32 i = 0; i < L_Moves.Num(); i++) {
		if (L_LocationKing == L_Moves[i])
			return true;
	}

	return false;
}
