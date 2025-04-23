// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset/VersionDataAsset.h"
#include "Modules/ModuleManager.h"
#include "Factories/Factory.h"

class FVersionningPackageModule : public IModuleInterface
{
	UToolMenus* ToolMenus = nullptr;
	UVersionDataAsset* DataAsset = nullptr;
	static TSharedPtr<FSlateStyleSet> StyleSet;
	TWeakPtr<SWindow> WindowPackageSettings;

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void OpenWindowPackageSettings();
	void AddToolbarCommands();
	void ResetVersion();
	void UpdateMajorVersion();
	void OnWindowClosed(const TSharedRef<SWindow>& ClosedWindow);
	UVersionDataAsset* EnsureVersionDataAssetExists();

	TSharedRef<SWidget> GeneratePackageProject();
};
