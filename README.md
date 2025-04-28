# 📘 Project Versioning Plugin for Unreal Engine 5+

[Preview](logo.png)

## 🧩 What is it?

This plugin manages **project versioning** in Unreal Engine 5+.  
It compiles the project and automatically updates a **DataAsset** with the new version number.

## ⚙️ Installation

1. Download the plugin as a ZIP file.  
2. Extract the ZIP.  
3. Copy and paste the `Plugins` folder into the **root directory** of your Unreal project.

## ⚙️ Use
- **C++** 
1. Modify your .Build.cs to add the module
```cs
PublicDependencyModuleNames.AddRange(new[]
		{
			//... Your classic modules unreal ..

		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.AddRange(new[]
			{
				“UnrealEd”,
				“ToolMenus”,
				“VersionningPackage”
			});
		}
```
2. You can include and use the DataAsset via
```cpp
#include “Asset/VersionDataAsset.h”

UPROPERTY(EditAnywhere, Category = “Custom|Version”)
TObjectPtr<UVersionDataAsset> Data;
```

- **Blueprint**
1. Create a variable, of type Version Data Asset, and pass it the Data Asset as reference

Translated with DeepL.com (free version)

## 🛠️ Technologies Used

- **C++**  
- **Slate** (Unreal UI Framework)  
- **GitHub**

---

# 📘 Plugin de Versioning de Projet pour Unreal Engine 5+


## 🧩 C’est quoi ?

Ce plugin permet de gérer la **version du projet** dans Unreal Engine 5+.  
Il compile le projet, puis met à jour un **DataAsset** avec le numéro de version.

## ⚙️ Installation

1. Télécharger le plugin au format ZIP.  
2. Extraire le ZIP.  
3. Copier et coller le dossier `Plugins` dans le **répertoire racine** de votre projet Unreal.

## ⚙️ Utilisation
- **C++** 
1. Modifier votre .Build.cs pour ajouter le module
```cs
PublicDependencyModuleNames.AddRange(new[]
		{
			//... Vos modules classiques unreal ..

		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.AddRange(new[]
			{
				"UnrealEd",
				"ToolMenus",
				"VersionningPackage"
			});
		}
```
2. Vous pouvez inclure et utiliser le DataAsset via
```cpp
#include "Asset/VersionDataAsset.h"

UPROPERTY(EditAnywhere, Category = "Custom|Version")
TObjectPtr<UVersionDataAsset> Data;
```

- **Blueprint**
1. Crée une variable, de type Version Data Asset, et lui passer le Data Asset en réference

## 🛠️ Technologies utilisées

- **C++**  
- **Slate** (Framework UI Unreal)  
- **GitHub**
