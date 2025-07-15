#include "ChessEditor.h"
#include "Modules/ModuleManager.h"

void FChessEditorModule::StartupModule()
{
    ChessType_Actions = MakeShared<FChessType_Actions>();
    FAssetToolsModule::GetModule().Get().RegisterAssetTypeActions(ChessType_Actions.ToSharedRef());
}

void FChessEditorModule::ShutdownModule()
{
    if (!FModuleManager::Get().IsModuleLoaded("AssetTools")) return;
    FAssetToolsModule::GetModule().Get().UnregisterAssetTypeActions(ChessType_Actions.ToSharedRef());
}

IMPLEMENT_MODULE(FChessEditorModule, ChessEditorModule)