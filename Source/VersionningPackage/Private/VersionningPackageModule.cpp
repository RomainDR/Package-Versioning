#include "VersionningPackageModule.h"

// Unreal Engine includes
#if WITH_EDITOR
	#include "AssetToolsModule.h"
	#include "IAssetTools.h"
#include "ToolMenus.h"
#include "Asset/VersionDataAsset.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "Window/PluginWindow.h"
#include "Window/WindowVersion.h"

static const FName TabName("MyCustomEditorTab");

#define PATH_MENU "LevelEditor.LevelEditorToolBar.Package"
#define LOCTEXT_NAMESPACE "FVersionningPackageModule"

TSharedPtr<FSlateStyleSet> FVersionningPackageModule::StyleSet = nullptr;

//-----------------------------------------------------------------------------
// Module initialization: Register toolbar menu commands/buttons
void FVersionningPackageModule::StartupModule()
{
	ToolMenus = UToolMenus::Get();
	if (!ToolMenus)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get ToolMenus. Disable Tool Editor"));
		return;
	}
	AddToolbarCommands();
}

//-----------------------------------------------------------------------------
// Module shutdown: Clean up and unregister custom editor tabs
void FVersionningPackageModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TabName);
}

//-----------------------------------------------------------------------------
// Opens the main settings window for packaging and versioning
void FVersionningPackageModule::OpenWindowPackageSettings()
{
	// If the window already exists, bring it to front
	if (WindowPackageSettings.IsValid())
	{
		WindowPackageSettings.Pin()->BringToFront();
		return;
	}

	// Load or create the version data asset
	DataAsset = EnsureVersionDataAssetExists();

	// Create custom Slate window for the plugin
	TSharedRef<SWindow> PackageWindow = SNew(SWindow)
		.Title(LOCTEXT("PackageProjectWindowTitle", "Package Project Settings"))
		.ClientSize(FVector2D(700.0f, 450.0f))
		.SupportsMinimize(false)
		.SupportsMaximize(false)
		[
			SNew(SPluginWindow)
			.VersionDataAsset(DataAsset)
		];
	PackageWindow->SetOnWindowClosed(FOnWindowClosed::CreateRaw(this, &FVersionningPackageModule::OnWindowClosed));

	WindowPackageSettings = PackageWindow;
	FSlateApplication::Get().AddWindow(PackageWindow);
}

void FVersionningPackageModule::OnWindowVersionClosed(const TSharedRef<SWindow>& Window)
{
	if (WindowVersionSettings.IsValid() && WindowVersionSettings.Pin() == Window)
	{
		WindowVersionSettings = nullptr;
	}
}

void FVersionningPackageModule::OpenWindowSetVersionSettings()
{
	if (WindowVersionSettings.IsValid())
	{
		WindowVersionSettings.Pin()->BringToFront();
		return;
	}

	DataAsset = EnsureVersionDataAssetExists();

	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(LOCTEXT("PackageProjectWindowTitle", "Set Version"))
		.ClientSize(FVector2D(700.0f, 250.0f))
		.SupportsMinimize(false)
		.SupportsMaximize(false)
		.SizingRule(ESizingRule::FixedSize)
		[
			SNew(SWindowVersion)
			.VersionDataAsset(DataAsset)
		];

	Window->SetOnWindowClosed(FOnWindowClosed::CreateRaw(this, &FVersionningPackageModule::OnWindowVersionClosed));

	// FIXED: Assign to WindowVersionSettings!
	WindowVersionSettings = Window;

	FSlateApplication::Get().AddWindow(Window);

	// This is safe now, but Window->BringToFront() is also OK.
	WindowVersionSettings.Pin()->BringToFront();
}


//-----------------------------------------------------------------------------
// Callback: Cleans up the pointer when the settings window is closed
void FVersionningPackageModule::OnWindowClosed(const TSharedRef<SWindow>& ClosedWindow)
{
	if (WindowPackageSettings.IsValid() && WindowPackageSettings.Pin() == ClosedWindow)
	{
		WindowPackageSettings = nullptr;
	}
}

//-----------------------------------------------------------------------------
// Generates the dropdown menu for packaging (toolbar button)
TSharedRef<SWidget> FVersionningPackageModule::GeneratePackageProject()
{
	if (ToolMenus->IsMenuRegistered(PATH_MENU))
	{
		return ToolMenus->GenerateWidget(PATH_MENU, nullptr);
	}

	// Create the actual menu
	UToolMenu* TechMenu = ToolMenus->RegisterMenu(PATH_MENU);
	{
		FToolMenuSection& packageProject = TechMenu->AddSection("Package", INVTEXT("Package"));

		// Button: Open main settings window
		packageProject.AddMenuEntry(
			"Package",
			INVTEXT("Package Project"),
			INVTEXT("Package project with add versioning"),
			FSlateIcon(FName("EditorStyle"), "MainFrame.PackageProject"),
			FUIAction(FExecuteAction::CreateRaw(this, &FVersionningPackageModule::OpenWindowPackageSettings))
		);
		// Button: Reset version to zero
		packageProject.AddMenuEntry(
			"PackageReset",
			INVTEXT("Reset version"),
			INVTEXT("Reset the version to 0"),
			FSlateIcon(FName("EditorStyle"), "SourceControl.Actions.Revert"),
			FUIAction(FExecuteAction::CreateRaw(this, &FVersionningPackageModule::ResetVersion))
		);
		// Button: Increase major version
		packageProject.AddMenuEntry(
			"PackageIncreaseMajor",
			INVTEXT("Update Major Version"),
			INVTEXT("Update the major version of project"),
			FSlateIcon(FName("EditorStyle"), "Symbols.Check"),
			FUIAction(FExecuteAction::CreateRaw(this, &FVersionningPackageModule::UpdateMajorVersion))
		);

		// Button: Increase major version
		packageProject.AddMenuEntry(
			"SetVersion",
			INVTEXT("Set Version"),
			INVTEXT("Set a version manually"),
			FSlateIcon(FName("EditorStyle"), "LevelEditor.GameSettings"),
			FUIAction(FExecuteAction::CreateRaw(this, &FVersionningPackageModule::OpenWindowSetVersionSettings))
		);
	}

	return ToolMenus->GenerateWidget(TechMenu);
}

//-----------------------------------------------------------------------------
// Adds the plugin's menu button to the editor toolbar
void FVersionningPackageModule::AddToolbarCommands()
{
	// Extend the user toolbar menu
	UToolMenu* ToolbarMenu = ToolMenus->ExtendMenu("LevelEditor.LevelEditorToolBar.User");

	// Add a "Custom" section
	FToolMenuSection& ToolbarSection = ToolbarMenu->FindOrAddSection("Custom");

	// Add the combo button for package options
	FToolMenuEntry PackageMenu = FToolMenuEntry::InitComboButton(
		FName("Package"),
		FToolUIActionChoice(),
		FOnGetContent::CreateRaw(this, &FVersionningPackageModule::GeneratePackageProject),
		INVTEXT("Package"),
		INVTEXT("Package Project"),
		FSlateIcon(FName("EditorStyle"), "MainFrame.PackageProject")
	);

	PackageMenu.StyleNameOverride = "CalloutToolbar";
	ToolbarSection.AddEntry(PackageMenu);
}

//-----------------------------------------------------------------------------
// Resets the version data to zero and shows a dialog
void FVersionningPackageModule::ResetVersion()
{
	DataAsset = EnsureVersionDataAssetExists();
	if (!DataAsset)
	{
		FMessageDialog::Open(EAppMsgCategory::Success, EAppMsgType::Ok,
		                     FText::FromString(
			                     "Data asset is not found. Please open the window to \ngenerate to dataAsset"));
		return;
	}
	DataAsset->Init();
	FMessageDialog::Open(EAppMsgCategory::Success, EAppMsgType::Ok,
	                     FText::FromString("Version has been reinitialized to 0"));
}

//-----------------------------------------------------------------------------
// Increases major version, shows confirmation dialog
void FVersionningPackageModule::UpdateMajorVersion()
{
	DataAsset = EnsureVersionDataAssetExists();
	if (!DataAsset)
	{
		FMessageDialog::Open(EAppMsgCategory::Error, EAppMsgType::Ok,
		                     FText::FromString(
			                     "Data asset is not found. Please open the window to \ngenerate to dataAsset"));
		return;
	}
	DataAsset->IncreaseMajor();
	FMessageDialog::Open(EAppMsgCategory::Success, EAppMsgType::Ok,
	                     FText::FromString("Major version update to " + FString::FromInt(DataAsset->GetMajor())));
}

//-----------------------------------------------------------------------------
// Utility: Loads or creates the version data asset (singleton); saves it if newly created
UVersionDataAsset* FVersionningPackageModule::EnsureVersionDataAssetExists()
{
#if WITH_EDITOR
	const FString AssetPath = TEXT("/Game/Version/VersionDataAsset");
	const FString PackagePath = TEXT("/Game/Version");
	const FString AssetName = TEXT("VersionDataAsset");

	// Check if asset already exists
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<
		FAssetRegistryModule>("AssetRegistry");
	FAssetData ExistingAsset = AssetRegistryModule.Get().GetAssetByObjectPath(
		FSoftObjectPath(AssetPath + TEXT(".") + AssetName));

	if (ExistingAsset.IsValid())
	{
		return Cast<UVersionDataAsset>(ExistingAsset.GetAsset());
	}

	// Create the asset (if not found)
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	UVersionDataAsset* NewAsset = Cast<UVersionDataAsset>(
		AssetTools.CreateAsset(AssetName, PackagePath, UVersionDataAsset::StaticClass(), nullptr));
	if (!NewAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Creation failed."));
		return nullptr;
	}

	NewAsset->Init();

	// Save the new asset to disk
	UPackage* Package = NewAsset->GetOutermost();
	FString PackageFileName = FPackageName::LongPackageNameToFilename(Package->GetName(),
	                                                                  FPackageName::GetAssetPackageExtension());

	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	SaveArgs.Error = nullptr;
	SaveArgs.bWarnOfLongFilename = true;
	SaveArgs.SaveFlags = SAVE_None;

	bool bSuccess = UPackage::SavePackage(Package, nullptr, *PackageFileName, SaveArgs);

	UE_LOG(LogTemp, Log, TEXT("VersionDataAsset created and save returned %s at %s"),
	       bSuccess ? TEXT("SUCCESS") : TEXT("FAIL"), *AssetPath);

	return NewAsset;
#else
	return nullptr;
#endif
}
#endif

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVersionningPackageModule, VersionningPackageEditorMode)
