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
	TWeakPtr<SWindow> WindowVersionSettings;

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/**
	 * Open custom window of packaging 
	 */
	void OpenWindowPackageSettings();

	void OnWindowVersionClosed(const TSharedRef<SWindow>& Window);
	/**
	 * Open custom window for set version manually
	 */
	void OpenWindowSetVersionSettings();
	/**
	 * Add the custom toolbar
	 */
	void AddToolbarCommands();
	/**
	 * Reset the version of asset
	 */
	void ResetVersion();
	/**
	 * Update major version
	 */
	void UpdateMajorVersion();
	/**
	 * Event when window closed. Delete the variable
	 * @param ClosedWindow Window Reference
	 */
	void OnWindowClosed(const TSharedRef<SWindow>& ClosedWindow);
	/**
	 * Get the Data Asset, or create if not exist
	 * @return Pointer of dataAsset
	 */
	UVersionDataAsset* EnsureVersionDataAssetExists();

	/**
	 * Generate sub menu of category
	 * @return A reference of submenu
	 */
	TSharedRef<SWidget> GeneratePackageProject();
};
