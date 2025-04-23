#pragma once

#include "VersionningPackageModule.h"
#include "Asset/VersionDataAsset.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"


class SPluginWindow : public SCompoundWidget
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
	TSharedRef<SOverlay> LoadUI();
	TSharedRef<SHorizontalBox> BuildHorizontalBox();
	void UpdateCmdLine();
	void OnSelectionChangedPlatform(TSharedPtr<FText> Text, ESelectInfo::Type Arg);
	void OnSelectionChangedDevType(TSharedPtr<FText> Text, ESelectInfo::Type Arg);

	FReply OpenSelectorFolder();
	FReply OnDoPackageClicked();
};
