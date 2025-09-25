#include "ChessToolkit.h"
#include "Widgets/Docking/SDockTab.h"
#include "Modules/ModuleManager.h"
#include "SEditorViewport.h"
#include "EditorViewportTabContent.h"
#include "SAssetEditorViewport.h"

const FName MyToolbarTabId(TEXT("Toolbar"));

void FChessToolKit::InitEditor(const TArray<UObject*>& InObjects)
{
	ChessData = Cast<UChessData>(InObjects[0]);

	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("ChessDataEditorLayout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f)
				->SetHideTabWell(true)
				->AddTab(MyToolbarTabId, ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetSizeCoefficient(0.6f)
				->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.8f)
					->AddTab("ChessViewportTab", ETabState::OpenedTab)
				)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.2f)
					->AddTab("ChessDataDetailsTab", ETabState::OpenedTab)
				)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.4f)
				->AddTab("OutputLog", ETabState::OpenedTab)
			)
		);
	FAssetEditorToolkit::InitAssetEditor(EToolkitMode::Standalone, {}, "ChessDataEditor", Layout, true, true, InObjects);
}

void FChessToolKit::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(INVTEXT("Chess Data Editor"));

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	TSharedRef<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObjects(TArray<UObject*>{ ChessData });

	DetailsView->OnFinishedChangingProperties().AddRaw(this, &FChessToolKit::OnChangingProperties);

	InTabManager->RegisterTabSpawner("ChessDataDetailsTab", FOnSpawnTab::CreateLambda([=](const FSpawnTabArgs&)
	{
		return SNew(SDockTab)
			.Icon((FEditorStyle::GetBrush("PropertyEditor.Properties.TabIcon")))
			[
				DetailsView
			];
	}))
	.SetDisplayName(INVTEXT("Details"))
	
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());
	
	TSharedPtr< FEditorViewportTabContent> ViewportTabContent = MakeShareable(new FEditorViewportTabContent());
	
	PreviewScene = new FTestPreviewScene(FTestPreviewScene::ConstructionValues());
	PreviewScene->AddChess(ChessData);
	
	TSharedPtr<FEditorViewportClient> K_View = MakeShared<FEditorViewportClient>(nullptr, PreviewScene);

	TFunction<TSharedRef<SEditorViewport>(void)> ViewportDelegate = [=]()
	{
		return SNew(SAssetEditorViewport)
			.EditorViewportClient(K_View);
	};

	InTabManager->RegisterTabSpawner("ChessViewportTab", FOnSpawnTab::CreateLambda([=](const FSpawnTabArgs&)
	{
		TSharedRef< SDockTab > DockableTab =
			SNew(SDockTab)
			.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Viewports"));

		const FString LayoutId = FString("BaseAssetViewport");
		ViewportTabContent->Initialize(ViewportDelegate, DockableTab, LayoutId);
		return DockableTab;

	}))
	.SetDisplayName(INVTEXT("Viewport"))
	.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}


void FChessToolKit::OnChangingProperties(const FPropertyChangedEvent& Property) {

	if (Property.GetPropertyName() == "Pawn") {

		for (int i = 0; i < PreviewScene->ChessActor->Figures.Num(); i++)
		{
			if (PreviewScene->ChessActor->Figures[i].Type == EFigureType::Pawn)
				PreviewScene->ChessActor->Figures[i].Figure->SetStaticMesh(ChessData->Pawn);
		}

		return;
	}

	if (Property.GetPropertyName() == "FieldSize") {

	}
}


void FChessToolKit::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner("ChessViewportTab");
	InTabManager->UnregisterTabSpawner("ChessDataDetailsTab");
}
