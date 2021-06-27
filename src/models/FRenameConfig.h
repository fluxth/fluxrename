#ifndef FRENAMECONFIG_H
#define FRENAMECONFIG_H

#import <QString>

#include "../global.h"

namespace RenameConfig {

struct ReplaceConfig {
    bool enabled = false;
    QString find;
    QString replace;

    bool clearExisting = false;
    bool useRegex = false;

    ReplaceConfig() = default;
};

struct TrimConfig {
    bool enabled = false;
    size_t fromStartIndex = 0;
    size_t fromEndIndex = 0;

    bool rangeEnabled = false;
    size_t rangeStartIndex = 0;
    size_t rangeEndIndex = 0;

    TrimConfig() = default;
};

struct AddConfig {
    bool enabled = false;

    bool prefixEnabled = false;
    QString prefix;

    bool insertEnabled = false;
    QString insert;
    size_t insertIndex = 0;

    bool suffixEnabled = false;
    QString suffix;

    AddConfig() = default;
};

struct NumberingConfig {
    bool enabled = false;

    // TODO

    NumberingConfig() = default;
};

enum class StringManipulationTarget : uint8_t {
    None = 0,
    Filename,
    Extension,
    NameAndExtension,
};

struct StringManipulationConfig {
    bool enabled = false;

    StringManipulationTarget upperCase;
    StringManipulationTarget lowerCase;

    StringManipulationConfig() = default;
};

}

struct FRenameConfig {
    bool includeExtensions = false;
    RenameConfig::ReplaceConfig replace;
    RenameConfig::TrimConfig trim;
    RenameConfig::AddConfig add;
    RenameConfig::StringManipulationConfig string;

    FRenameConfig() = default;
};

#endif // FRENAMECONFIG_H
