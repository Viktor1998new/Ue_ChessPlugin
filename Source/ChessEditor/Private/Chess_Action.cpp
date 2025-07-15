#include "Chess_Action.h"
#include "ChessActor.h"
#include "ChessToolKit.h"

UClass* FChessType_Actions::GetSupportedClass() const
{
    return UChessData::StaticClass();
}

FText FChessType_Actions::GetName() const
{
    return INVTEXT("Chess");
}

FColor FChessType_Actions::GetTypeColor() const
{
    return FColor::Cyan;
}

uint32 FChessType_Actions::GetCategories()
{
    return EAssetTypeCategories::Misc;
}

void FChessType_Actions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
    MakeShared<FChessToolKit>()->InitEditor(InObjects);
}