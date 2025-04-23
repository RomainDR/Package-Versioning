#define CREATE_TAB() \
FGlobalTabmanager::Get()->RegisterNomadTabSpawner( \
	   MaFenetreTabName, \
	   FOnSpawnTab::CreateRaw(this, &FMaFenetrePluginModule::OnSpawnPluginTab) \
   ) \
   .SetDisplayName(LOCTEXT("MaFenetreTabTitle", "Ma Fenêtre")) \
   .SetMenuType(ETabSpawnerMenuType::Hidden); \
     \
UToolMenus::RegisterStartupCallback( \
	FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMaFenetrePluginModule::RegisterMenus) \
);
