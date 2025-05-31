//Copyright (c) 2025 Viktor Fritsler

#include "Figure.h"
#include "ChessActor.h"
#include "Components/StaticMeshComponent.h"

FFigure::FFigure()
	: Chess(nullptr),
	Type(EFigureType::Pawn),
	Team(-1),
	Figure(nullptr),
	Location(FIntPoint::ZeroValue),
	IsMove(false){
}

FFigure::FFigure(AChessActor* Chess, UStaticMeshComponent* Figure, const uint8& Team, FIntPoint Location, EFigureType Type)
	: Chess(Chess),
	Type(Type),
	Team(Team),
	Figure(Figure),
	Location(Location),
	IsMove(false)
{
}

void FFigure::SetLocation(FVector2D Position, float SizeGrid, FIntPoint To, float OffsetZ){

	Figure->SetRelativeLocation(FVector(Position + FVector2D(SizeGrid * To.X, SizeGrid * To.Y), OffsetZ));
	Location = To;
	IsMove = true;
}

void FFigure::GetMoves(TArray<FIntPoint>& Moves) const {

	int32 direction_King[8][2] = {
		{1, 1},{-1, -1}, {1, -1}, {-1, 1}, {1, 0},
		{-1, 0}, {0, 1}, {0, -1}
	};

	int32 direction_Bishop[4][2] = {
		{1, 1},{-1, -1}, {1, -1}, {-1, 1}
	};

	int32 direction_Rock[4][2] = {
		{1, 0},{-1, 0}, {0, 1}, {0, -1}
	};

	int32_t direction_Knight[8][2] = {
		{1, 2}, {2, 1}, {1, -2}, {2, -1},
		{-1, 2}, {-2, 1}, {-1, -2}, {-2, -1}
	};


	switch (Type)
	{
	case EFigureType::Pawn:

		if (Team == 0) {
			if (!Chess->IsCeilBusy(Location + FIntPoint(1, 0)))
				Moves.Add(Location + FIntPoint(1, 0));

			if (Chess->IsCeilBusy(Location + FIntPoint(1, 1)))
				Moves.Add(Location + FIntPoint(1, 1));

			if (Chess->IsCeilBusy(Location + FIntPoint(1, -1)))
				Moves.Add(Location + FIntPoint(1, -1));

			if (!IsMove && !Chess->IsCeilBusy(Location + FIntPoint(2, 0))) {
				Moves.Add(Location + FIntPoint(2, 0));
			}
		}
		else {
			if(!Chess->IsCeilBusy(Location - FIntPoint(1, 0)))
				Moves.Add(Location - FIntPoint(1, 0));

			if (Chess->IsCeilBusy(Location - FIntPoint(1, 1)))
				Moves.Add(Location - FIntPoint(1, 1));
			
			if (Chess->IsCeilBusy(Location - FIntPoint(1, -1)))
				Moves.Add(Location - FIntPoint(1, -1));

			if (!IsMove && !Chess->IsCeilBusy(Location - FIntPoint(2, 0))) {
				Moves.Add(Location - FIntPoint(2, 0));
			}
		}
		break;

	case EFigureType::Rock:

		for (const auto& dir : direction_Rock) {

			for (int32 i = 1; i < 8; i++) {

				auto L_Move = FIntPoint(Location.X + dir[0] * i, Location.Y + dir[1] * i);

				if (L_Move.X >= 8 || L_Move.X < 0 || L_Move.Y >= 8 || L_Move.Y < 0) {
					break;
				}
				Moves.Add(L_Move);

				if (Chess->IsCeilBusy(L_Move))
					break;
			}
		}

		break;

	case EFigureType::Knight:

		for (const auto& dir : direction_Knight) {
			auto L_Move = FIntPoint(Location.X + dir[0], Location.Y + dir[1]);

			if ((L_Move.X >= 0 && L_Move.X <= 7) && (L_Move.Y >= 0 && L_Move.Y <= 7)) {
				Moves.Add(L_Move);
			}
		}

		break;

	case EFigureType::Bishop:

		for (const auto& dir : direction_Bishop) {

			for (int32 i = 1; i < 8; i++) {

				auto L_Move = FIntPoint(Location.X + dir[0] * i, Location.Y + dir[1] * i);

				if (L_Move.X >= 8 || L_Move.X < 0 || L_Move.Y >= 8 || L_Move.Y < 0) {
					break;
				}
				Moves.Add(L_Move);

				if (Chess->IsCeilBusy(L_Move))
					break;
			}
		}

		break;

	case EFigureType::Queeu:

		for (const auto& dir : direction_King) {

			for (int32 i = 1; i < 8; i++) {

				auto L_Move = FIntPoint(Location.X + dir[0] * i, Location.Y + dir[1] * i);

				if (L_Move.X >= 8 || L_Move.X < 0 || L_Move.Y >= 8 || L_Move.Y < 0) {
					break;
				}
				Moves.Add(L_Move);

				if (Chess->IsCeilBusy(L_Move))
					break;
			}
		}

		break;

	case EFigureType::King:

		for (const auto& dir : direction_King) {

			auto L_Move = FIntPoint(Location.X + dir[0], Location.Y + dir[1]);

			if ((L_Move.X >= 0 && L_Move.X <= 7) && (L_Move.Y >= 0 && L_Move.Y <= 7)) {
				Moves.Add(L_Move);
			}
		}

		break;
	}
}