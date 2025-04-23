// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Runtime/Core/Tests/Misc/PathTests.h"
#include "UObject/SavePackage.h"
#include "VersionDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class VERSIONNINGPACKAGE_API UVersionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Version", meta = (AllowPrivateAccess))
	int Major = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Version", meta = (AllowPrivateAccess))
	int Minor = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Version", meta = (AllowPrivateAccess))
	int Patch = 0;
	UPROPERTY(VisibleAnywhere, Category = "Version")
	FString Path = "";

public:
	FORCEINLINE int GetMajor() const { return Major; }
	FORCEINLINE int GetMinor() const { return Minor; }
	FORCEINLINE int GetPatch() const { return Patch; }

	FORCEINLINE FString GetPath()const
	{
		return Path;
	}

	void Init()
	{
		Major = Minor = Patch = 0;
		Save();
	}

	void IncreaseMajor()
	{
		Major++;
		Minor = Patch = 0;
		Save();
	}

	void IncreaseMinor()
	{
		Minor++;
		Patch = 0;
		Save();
	}

	void IncreasePatch()
	{
		Patch++;
		Save();
	}

	void SetVersion(int major, int minor, int patch)
	{
		Major = major;
		Minor = minor;
		Patch = patch;
		Save();
	}
	void UpdatePath(const FString& path)
	{
		Path = path;
		Save();
	}

private:
	void Save() const
	{
		UPackage* Package = GetOutermost();
		FString PackageFileName = FPackageName::LongPackageNameToFilename(
			Package->GetName(), FPackageName::GetAssetPackageExtension());

		FSavePackageArgs SaveArgs;
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		SaveArgs.bWarnOfLongFilename = true;
		SaveArgs.SaveFlags = SAVE_None;

		bool bSuccess = UPackage::SavePackage(Package, nullptr, *PackageFileName, SaveArgs);
		UE_LOG(LogTemp, Log, TEXT("Package saved: %s"), bSuccess ? TEXT("OK") : TEXT("FAILED"));
	}
};
