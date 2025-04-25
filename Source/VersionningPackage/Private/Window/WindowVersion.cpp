#include "Window/WindowVersion.h"
#include "VersionningPackage/Utils/Util.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Framework/Application/SlateApplication.h"

void SWindowVersion::Construct(const FArguments& InArgs)
{
	VersionDataAsset = InArgs._VersionDataAsset;

	majorField = SNew(SEditableTextBox)
		.IsReadOnly(true)
		.Text(FText::FromString(FString::FromInt(VersionDataAsset ? VersionDataAsset->GetMajor() : 0)))
		.HintText(FText::FromString(FString::FromInt(0)));
	minorField = SNew(SEditableTextBox)
		.Text(FText::FromString(FString::FromInt(VersionDataAsset ? VersionDataAsset->GetMinor() : 0)))
		.HintText(FText::FromString(FString::FromInt(0)));
	patchField = SNew(SEditableTextBox)
		.Text(FText::FromString(FString::FromInt(VersionDataAsset ? VersionDataAsset->GetPatch() : 0)))
		.HintText(FText::FromString(FString::FromInt(0)));

	ChildSlot
	[
		LoadUI()
	];
}

FReply SWindowVersion::SetVersion()
{
	auto IsNumeric = [](const TSharedPtr<SEditableTextBox>& _field)
	{
		return _field->GetText().ToString().IsNumeric();
	};
	auto GetIntValue = [](const TSharedPtr<SEditableTextBox>& _field)
	{
		return FCString::Atoi(*_field->GetText().ToString());
	};

	if (!IsNumeric(majorField) || !IsNumeric(minorField) || !IsNumeric(patchField))
	{
		OPEN_WARN_POPUP("Warning ! All fields must be numeric !");
		return FReply::Handled();
	}
	const int32 major = GetIntValue(majorField);
	const int32 minor = GetIntValue(minorField);
	const int32 patch = GetIntValue(patchField);

	VersionDataAsset->SetVersion(major, minor, patch);
	OPEN_SUCCESS_POPUP(
		"Version has been set to " + FString::FromInt(major) + "." + FString::FromInt(minor) + "." + FString::FromInt(
			patch) + ".");
	return FReply::Handled();
}

TSharedRef<SVerticalBox> SWindowVersion::CreateVersionField(const FString& Label,
                                                            const TSharedPtr<SWidget>& FieldWidget)
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SNew(STextBlock)
			.Text(FText::FromString(Label))
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 22))
		]
		+ SVerticalBox::Slot()
		[
			FieldWidget.ToSharedRef()
		];
}

TSharedRef<SOverlay> SWindowVersion::LoadUI()
{
	return SNew(SOverlay)

		+ SOverlay::Slot().Padding(10, 5)
		[
			SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.Padding(30, 5)
			.FillHeight(1)
			[
				SNew(SScaleBox)
				.Stretch(EStretch::ScaleToFit)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Set Version"))
					.Justification(ETextJustify::Type::Center)
					.TransformPolicy(ETextTransformPolicy::ToUpper)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 22))
				]
			]
			+ SVerticalBox::Slot()
			.Padding(20, 5)
			.FillHeight(1)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					CreateVersionField("Major", majorField)
				]
				+ SHorizontalBox::Slot()
				[
					CreateVersionField("Minor", minorField)
				]
				+ SHorizontalBox::Slot()
				[
					CreateVersionField("Patch", patchField)
				]
			]
			+ SVerticalBox::Slot()
			.Padding(20, 5)
			.FillHeight(0.5)
			[
				SNew(SButton)
				.OnClicked(this, &SWindowVersion::SetVersion)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.Padding(4)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Set Version"))
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
					]
				]
			]
		];
}
