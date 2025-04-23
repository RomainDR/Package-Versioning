#include "../Public/Window/PluginWindow.h"

#include "DesktopPlatformModule.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Layout/SScaleBox.h"

TSharedRef<SHorizontalBox> SPluginWindow::BuildHorizontalBox()
{
	TSharedRef<SHorizontalBox> HorizontalBox = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot().FillWidth(1)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(SScaleBox)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Platform"))
					.Justification(ETextJustify::Type::Center)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
				]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SComboBox<TSharedPtr<FText>>)
				.OptionsSource(&OptionsPlatform)
				.InitiallySelectedItem(SelectedPlatform)
				.OnSelectionChanged(this, &SPluginWindow::OnSelectionChangedPlatform)
				.OnGenerateWidget_Lambda([](TSharedPtr<FText> InItem)
				{
					return SNew(STextBlock).Text(*InItem);
				})
				[
					SNew(STextBlock).Text_Lambda([this]() -> FText
					{
						return SelectedPlatform.IsValid()
							       ? *SelectedPlatform
							       : FText::FromString("Choose an option...");
					})
				]
			]
		];

	HorizontalBox->AddSlot()
	             .AutoWidth()
	[
		SNew(SSpacer).Size(FVector2D(10, 1))
	];

	// Slot: DevType
	HorizontalBox->AddSlot()
	             .FillWidth(1)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SNew(SScaleBox)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Package Type"))
				.Justification(ETextJustify::Type::Center)
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
			]
		]
		+ SVerticalBox::Slot()
		[
			SNew(SComboBox<TSharedPtr<FText>>)
			.OptionsSource(&OptionsDevType)
			.InitiallySelectedItem(SelectedDevType)
			.OnSelectionChanged(this, &SPluginWindow::OnSelectionChangedDevType)
			.OnGenerateWidget_Lambda([](TSharedPtr<FText> InItem)
			{
				return SNew(STextBlock).Text(*InItem);
			})
			[
				SNew(STextBlock).Text_Lambda([this]() -> FText
				{
					return SelectedDevType.IsValid()
						       ? *SelectedDevType
						       : FText::FromString("Choose an option...");
				})
			]
		]
	];

	return HorizontalBox;
}

TSharedRef<SOverlay> SPluginWindow::LoadUI()
{
	return SNew(SOverlay)
		+ SOverlay::Slot().Padding(10, 5)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().Padding(30, 5)
			.FillHeight(1)
			[
				SNew(SScaleBox)
				[
					SNew(STextBlock)
					.Text(FText::FromString("Custom Package System"))
					.Justification(ETextJustify::Type::Center)
					.TransformPolicy(ETextTransformPolicy::ToUpper)
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 22))
				]
				.Stretch(EStretch::ScaleToFit)
			]
			+ SVerticalBox::Slot().Padding(20, 5)
			.FillHeight(1)
			[
				BuildHorizontalBox()
			]
			+ SVerticalBox::Slot().Padding(20, 5)
			.FillHeight(0.75)
			.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
			[

				SNew(STextBlock)
				.Text(FText::FromString("Command Line:"))
				.Font(FCoreStyle::GetDefaultFontStyle("Italic", 16))
				.Justification(ETextJustify::Center)
			]
			+ SVerticalBox::Slot().Padding(20, 5)
			.FillHeight(0.75)
			[
				BoxedCmdInput.ToSharedRef()
			]
			+ SVerticalBox::Slot().Padding(20, 5)
			.FillHeight(.5)
			.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)  
			[

				SNew(STextBlock)
				.Text(FText::FromString("Export path:"))
				.Font(FCoreStyle::GetDefaultFontStyle("Italic", 16))
				.Justification(ETextJustify::Center)
			]
			+ SVerticalBox::Slot().Padding(20,5)
			.FillHeight(.5)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(2)
				[
					pathWidget.ToSharedRef()
				]
				+ SHorizontalBox::Slot()
				.FillWidth(0.5)
				[
					SNew(SButton)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						[
							SNew(STextBlock)
							.Text(FText::FromString("..."))
							.Justification(ETextJustify::Type::Center)
							.TransformPolicy(ETextTransformPolicy::ToUpper)
							.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
						]
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)

					]
					.OnClicked(this, &SPluginWindow::OpenSelectorFolder)
				]
			]
			+ SVerticalBox::Slot().Padding(20,5)
			.FillHeight(.5)
			[
				SNew(SButton)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot().Padding(0, 0)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Package Project"))
						.Justification(ETextJustify::Type::Center)
						.TransformPolicy(ETextTransformPolicy::ToUpper)
						.Font(FCoreStyle::GetDefaultFontStyle("Bold", 18))
					]
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)

				]
				.OnClicked(this, &SPluginWindow::OnDoPackageClicked)
			]
		];
}


void SPluginWindow::Construct(const FArguments& InArgs)
{
	VersionDataAsset = InArgs._VersionDataAsset;

	SelectedPlatform = OptionsPlatform[0];
	SelectedDevType = OptionsDevType[0];


	cmdInput = SNew(SMultiLineEditableTextBox).IsReadOnly(true);
	pathWidget = SNew(SEditableTextBox).HintText(FText::FromString("Choose path...")).Text(FText::FromString(""));

	BoxedCmdInput =
		SNew(SBox)
		.HeightOverride(400.f)
		[
			cmdInput.ToSharedRef()
		];

	pathWidget->SetText(FText::FromString(VersionDataAsset->GetPath()));
	UpdateCmdLine();

	ChildSlot
	[
		LoadUI()
	];
}

void SPluginWindow::UpdateCmdLine()
{
	FString ProjectPath = FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath());

	int indexLastChar = -1;
	ProjectPath.FindLastChar('/', indexLastChar);
	FString PlatformStr = SelectedPlatform.IsValid() ? SelectedPlatform->ToString() : TEXT("Windows");
	FString DevTypeStr = SelectedDevType.IsValid() ? SelectedDevType->ToString() : TEXT("Development");
	FString PathExport = pathWidget.Get()->GetText().IsEmpty() ? ProjectPath.Mid(0, indexLastChar).Append("/DefaultBuild") : pathWidget.Get()->GetText().ToString();

	FString AdditionalOptions;

	if (DevTypeStr == TEXT("Shipping"))
	{
		AdditionalOptions += TEXT(" -nodebuginfo");
	}

	FString fullCmd = FString::Printf(
		TEXT(
			"BuildCookRun -project=\"%s\" -noP4 -clientconfig=%s -serverconfig=%s -platform=%s -cook -allmaps -build -stage -package %s -archive -archivedirectory=\"%s\""),
		*ProjectPath,
		*DevTypeStr,
		*DevTypeStr,
		*PlatformStr,
		*AdditionalOptions,
		*PathExport
	);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *fullCmd);

	cmdInput->SetText(FText::FromString(fullCmd));
}


void SPluginWindow::OnSelectionChangedPlatform(TSharedPtr<FText> Text, ESelectInfo::Type Arg)
{
	SelectedPlatform = Text;
	UpdateCmdLine();
}

void SPluginWindow::OnSelectionChangedDevType(TSharedPtr<FText> Text, ESelectInfo::Type Arg)
{
	SelectedDevType = Text;
	UpdateCmdLine();
}

FReply SPluginWindow::OpenSelectorFolder()
{
	if (IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get())
	{
		const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);

		FString OutputDirectory;
		const FString Title = TEXT("Selector a folder to export");
		const FString DefaultPath = FPaths::ProjectDir();
		bool bFolderSelected = DesktopPlatform->OpenDirectoryDialog(
			ParentWindowHandle,
			Title,
			DefaultPath,
			OutputDirectory
		);

		if (bFolderSelected)
		{
			pathWidget->SetText(FText::FromString(OutputDirectory));
			UpdateCmdLine();
			VersionDataAsset->UpdatePath(OutputDirectory);
		}
	}
	return FReply::Handled();
}

FReply SPluginWindow::OnDoPackageClicked()
{
	if (isRunning)
	{
		FMessageDialog::Open(EAppMsgCategory::Error, EAppMsgType::Ok,
		                     FText::FromString("A package is already started."));
		return FReply::Handled();
	}
	isRunning = true;
	bool isShipping = SelectedDevType.Get()->EqualTo(FText::FromString("Shipping"));
	bool isDevelopment = SelectedDevType.Get()->EqualTo(FText::FromString("Development"));

	int major = VersionDataAsset->GetMajor();
	int minor = VersionDataAsset->GetMinor();
	int patch = VersionDataAsset->GetPatch();

	if (isShipping)
	{
		VersionDataAsset->IncreaseMinor();
	}
	else if (isDevelopment)
	{
		VersionDataAsset->IncreasePatch();
	}
	UVersionDataAsset* _asset = VersionDataAsset;
	
	const FString UATPath = FPaths::Combine(FPaths::EngineDir(), TEXT("Build"), TEXT("BatchFiles"), TEXT("RunUAT.bat"));

	const FString cmd = cmdInput->GetText().ToString();
	Async(EAsyncExecution::Thread, [UATPath, cmd, _asset, major, minor, patch, this]
	{
		int32 ReturnCode = 0;
		FString Output;
		FString ErrorOutput;

		FPlatformProcess::ExecProcess(*UATPath, *cmd, &ReturnCode, &Output, &ErrorOutput);

		if (ReturnCode == 0)
		{
			isRunning = false;
			FMessageDialog::Open(EAppMsgCategory::Info, EAppMsgType::Ok,
			                     FText::FromString("Packaging complete"));
		}
		else
		{
			isRunning = false;
			_asset->SetVersion(major, minor, patch);
			FString msg = "Packaging failed";
			if (!ErrorOutput.IsEmpty())
			{
				msg.Append(": " + ErrorOutput);
			}

			FMessageDialog::Open(EAppMsgCategory::Error, EAppMsgType::Ok,
			                     FText::FromString(msg));
		}
	});
	return FReply::Handled();
}
