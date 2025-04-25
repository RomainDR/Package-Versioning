#pragma once

#include "Asset/VersionDataAsset.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SEditableTextBox.h"

class SWindowVersion final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SWindowVersion)
			: _VersionDataAsset(nullptr)
		{
		}

		SLATE_ARGUMENT(UVersionDataAsset*, VersionDataAsset)
	SLATE_END_ARGS()

	UVersionDataAsset* VersionDataAsset = nullptr;
	TSharedPtr<SEditableTextBox> majorField;
	TSharedPtr<SEditableTextBox> minorField;
	TSharedPtr<SEditableTextBox> patchField;

	void Construct(const FArguments& InArgs);

private:
	TSharedRef<SOverlay> LoadUI();
	TSharedRef<SVerticalBox> CreateVersionField(const FString& Label, const TSharedPtr<SWidget>& FieldWidget);
	FReply SetVersion();
};
