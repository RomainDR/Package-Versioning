#pragma once
#define OPEN_SUCCESS_POPUP(msg) FMessageDialog::Open(EAppMsgCategory::Success, EAppMsgType::Ok,\
FText::FromString(msg))

#define OPEN_ERROR_POPUP(msg) FMessageDialog::Open(EAppMsgCategory::Error, EAppMsgType::Ok,\
FText::FromString(msg))

#define OPEN_INFO_POPUP(msg) FMessageDialog::Open(EAppMsgCategory::Info, EAppMsgType::Ok,\
FText::FromString(msg))

#define OPEN_WARN_POPUP(msg) FMessageDialog::Open(EAppMsgCategory::Warning, EAppMsgType::Ok,\
FText::FromString(msg))
