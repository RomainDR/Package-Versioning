#pragma once

#include "VersionningPackageModule.h"
#include "Asset/VersionDataAsset.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

/**
 * @class SPluginWindow
 * Main user interface for the versioning and packaging plugin.
 *
 * This widget allows users to select target platform, build configuration,
 * customize command line, and trigger the packaging process.
 */
class SPluginWindow final : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SPluginWindow)
            : _VersionDataAsset(nullptr)
        {
        }
        
        SLATE_ARGUMENT(UVersionDataAsset*, VersionDataAsset)
    SLATE_END_ARGS()

private:   
    const TArray<TSharedPtr<FText>> OptionsPlatform
    {
        MakeShareable(new FText(FText::FromString("Win64")))
    };
    
    const TArray<TSharedPtr<FText>> OptionsDevType
    {
        MakeShareable(new FText(FText::FromString("Shipping"))),
        MakeShareable(new FText(FText::FromString("Development"))),
    };
    
    UVersionDataAsset* VersionDataAsset = nullptr;    
    TSharedPtr<FText> SelectedPlatform;        
    TSharedPtr<FText> SelectedDevType;        
    TSharedPtr<SMultiLineEditableTextBox> cmdInput;   
    TSharedPtr<SWidget> BoxedCmdInput;   
    TSharedPtr<SEditableTextBox> pathWidget;   
    bool isRunning = false;

public:    
    void Construct(const FArguments& InArgs);

private:
    /**
     * Builds and returns the main overlay UI for the window.
     * @return Root overlay widget.
     */
    TSharedRef<SOverlay> LoadUI();

    /**
     * Helper to build a horizontal box containing multiple controls.
     * @return The built horizontal box widget.
     */
    TSharedRef<SHorizontalBox> BuildHorizontalBox();

    /**
     * Updates the command-line text field according to the current selections.
     */
    void UpdateCmdLine();

    /**
     * Called when the user changes the target platform.
     * @param Text The newly selected platform.
     * @param Arg  Selection info.
     */
    void OnSelectionChangedPlatform(TSharedPtr<FText> Text, ESelectInfo::Type Arg);

    /**
     * Called when the user selects a different build configuration.
     * @param Text The newly selected build type.
     * @param Arg  Selection info.
     */
    void OnSelectionChangedDevType(TSharedPtr<FText> Text, ESelectInfo::Type Arg);

    /**
     * Opens a folder selector dialog for output path.
     * @return Reply indicating whether the dialog was accepted.
     */
    FReply OpenSelectorFolder();

    /**
     * Called when the user clicks the "Package" button to start packaging.
     * @return Reply indicating the result of the click event.
     */
    FReply OnDoPackageClicked();
};
