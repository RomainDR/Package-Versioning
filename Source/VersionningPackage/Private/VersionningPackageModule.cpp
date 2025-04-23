#include "VersionningPackageModule.h"

#include "AssetToolsModule.h"
#include "IAssetTools.h"
#include "ToolMenus.h"
#include "Asset/VersionDataAsset.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Text/STextBlock.h"
#include "Window/PluginWindow.h"

static const FName TabName("MyCustomEditorTab");

#define PATH_MENU "LevelEditor.LevelEditorToolBar.Package"

#define LOCTEXT_NAMESPACE "FVersionningPackageModule"

TSharedPtr<FSlateStyleSet> FVersionningPackageModule::StyleSet = nullptr;

// Startup du module : déclaration de la fenêtre et du menu
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

void FVersionningPackageModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TabName);
}

void FVersionningPackageModule::OpenWindowPackageSettings()
{
	// Si la fenêtre existe et est toujours valide, la mettre au premier plan
	if (WindowPackageSettings.IsValid())
	{
		WindowPackageSettings.Pin()->BringToFront();
		return;
	}

	DataAsset = EnsureVersionDataAssetExists();

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

void FVersionningPackageModule::OnWindowClosed(const TSharedRef<SWindow>& ClosedWindow)
{
	if (WindowPackageSettings.IsValid() && WindowPackageSettings.Pin() == ClosedWindow)
	{
		WindowPackageSettings = nullptr;
	}
}


TSharedRef<SWidget> FVersionningPackageModule::GeneratePackageProject()
{
	if (ToolMenus->IsMenuRegistered(PATH_MENU))
	{
		return ToolMenus->GenerateWidget(PATH_MENU, nullptr);
	}

	UToolMenu* TechMenu = ToolMenus->RegisterMenu(PATH_MENU);
	{
		// Add section ""
		FToolMenuSection& packageProject = TechMenu->AddSection("Package", INVTEXT("Package"));


		packageProject.AddMenuEntry(
			"Package",
			INVTEXT("Package Project"),
			INVTEXT("Package project with add versioning"),
			FSlateIcon(FName("EditorStyle"), "MainFrame.PackageProject"),
			FUIAction(FExecuteAction::CreateRaw(this, &FVersionningPackageModule::OpenWindowPackageSettings))
		);

		packageProject.AddMenuEntry(
			"PackageReset",
			INVTEXT("Reset version"),
			INVTEXT("Reset the version to 0"),
			FSlateIcon(FName("EditorStyle"), "SourceControl.Actions.Revert"),
			FUIAction(FExecuteAction::CreateRaw(this, &FVersionningPackageModule::ResetVersion))
		);
		packageProject.AddMenuEntry(
			"PackageIncreaseMajor",
			INVTEXT("Update Major Version"),
			INVTEXT("Update the major version of projet"),
			FSlateIcon(FName("EditorStyle"), "Symbols.Check"),
			FUIAction(FExecuteAction::CreateRaw(this, &FVersionningPackageModule::UpdateMajorVersion))
		);
	}

	return ToolMenus->GenerateWidget(TechMenu);
}

void FVersionningPackageModule::AddToolbarCommands()
{
	UToolMenu* ToolbarMenu = ToolMenus->ExtendMenu("LevelEditor.LevelEditorToolBar.User");
	FToolMenuSection& ToolbarSection = ToolbarMenu->FindOrAddSection("Custom");

	// Add Tech menu on toolbar
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
	FMessageDialog::Open(EAppMsgCategory::Error, EAppMsgType::Ok,
	                     FText::FromString("Version has been reinitialized to 0"));
}

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

UVersionDataAsset* FVersionningPackageModule::EnsureVersionDataAssetExists()
{
	const FString AssetPath = TEXT("/Game/Version/VersionDataAsset");
	const FString PackagePath = TEXT("/Game/Version");
	const FString AssetName = TEXT("VersionDataAsset");

	// Vérifie l'existence
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<
		FAssetRegistryModule>("AssetRegistry");
	FAssetData ExistingAsset = AssetRegistryModule.Get().GetAssetByObjectPath(
		FSoftObjectPath(AssetPath + TEXT(".") + AssetName));

	if (ExistingAsset.IsValid())
	{
		return Cast<UVersionDataAsset>(ExistingAsset.GetAsset());
	}

	// Crée l'asset
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	UVersionDataAsset* NewAsset = Cast<UVersionDataAsset>(
		AssetTools.CreateAsset(AssetName, PackagePath, UVersionDataAsset::StaticClass(), nullptr));
	if (!NewAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Creation failed."));
		return nullptr;
	}

	NewAsset->Init();

	// Préparation de la sauvegarde
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
}

#undef LOCTEXT_NAMESPACE


IMPLEMENT_MODULE(FVersionningPackageModule, VersionningPackageEditorMode)
