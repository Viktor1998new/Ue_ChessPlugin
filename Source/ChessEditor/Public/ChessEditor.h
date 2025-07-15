#include "Modules/ModuleManager.h"
#include "Chess_Action.h"

class FChessEditorModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:

    TSharedPtr<FChessType_Actions> ChessType_Actions;

};