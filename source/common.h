/**
 * @file common.h
 * @author Krisna Pranav
 * @brief Entry Point Of The Application
 * @version 0.1
 * @date 2023-02-06
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditorDevelopers
 * 
 */

#pragma once

#pragma warning(disable: 4100)
#pragma warning(disable: 4458)
#pragma warning(disable: 4503)
#pragma warning(disable: 4127)

#if JUCE_LINUX
#   define JUCE_USE_FREETYPE_AMALGAMATED 1
#endif

#include "JuceHeader.h"

#include <climits>
#include <cfloat>
#include <cmath>

#include "../thirdparty/hopscotch-map/include/tsl/hopscotch_map.h"

template <class Key, class T, class HashFn = std::hash<Key>, class EqualKey = std::equal_to<Key>>
using FlatHashMap = tsl::hopscotch_map<Key, T, HashFn, EqualKey>;

#include "../thirdparty/hopscotch-map/include/tsl/hopscotch_set.h"

template <class Value, class HashFn = std::hash<Value>, class EqualKey = std::equal_to<Value>>
using FlatHashSet = tsl::hopscotch_set<Value, HashFn, EqualKey>;

using HashCode = size_t;

/**
 * @brief String Hashing
 * 
 */
struct StringHash {
    inline HashCode operator()(const juce::String &key) const noexcept {
        return static_cast<HashCode>(key.hashCode());
    }
};

/**
 * @brief Hash Identifer
 * 
 */
struct IdentifierHash {
    inline HashCode operator()(const juce::Identifier &key) const noexcept {
        return static_cast<HashCode>(key.toString().hashCode());
    }

    static int generateHash(const Identifier& key, int upperLimit) noexcept {
        return uint32(key.toString().hashCode()) % (uint32)upperLimit;
    }
};

/**
 * @brief UniquePointer
 * 
 * @tparam T 
 */
template <typename T>
using UniquePointer = std::unique_ptr<T>;

template <typename T, typename... Args> inline
UniquePointer<T> make(Args&&... args) {
    return UniquePointer<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
using Function = std::function<T>;

using std::move;

#if _MSC_VER
inline float roundf(float x) {
    return (x >= 0.0f) ? floorf(x + 0.5f) : ceilf(x - 0.5f);
}
#endif

#if JUCE_ANDROID || JUCE_IOS
#   define PLATFORM_MOBILE 1
#else
#   define PLATFORM_DESKTOP 1
#endif

#define forEachChildWithType(parentElement, child, requiredType) \
    for (const auto &(child) : (parentElement)) if ((child).hasType(requiredType))

#define callbackOnMessageThread(cls, function, ...) \
    MessageManager::getInstance()->callFunctionOnMessageThread([](void *ptr) -> void* \
        { \
            const auto *self = static_cast<cls *>(ptr); \
            if (self->function != nullptr) \
            { \
                self->function(__VA_ARGS__); \
            } \
            return nullptr; \
        }, this)

#define findDefaultColour(x) LookAndFeel::getDefaultLookAndFeel().findColour(x)

constexpr uint32 fnv1a32val = 0x811c9dc5;
constexpr uint64 fnv1a32prime = 0x1000193;
inline constexpr uint32 constexprHash(const char *const str, const uint32 value = fnv1a32val) noexcept {
    return (str[0] == '\0') ? value : constexprHash(&str[1], uint32(value ^ uint32(str[0])) * fnv1a32prime);
}

#if PLATFORM_MOBILE
#   define NO_NETWORK 1
#endif

/**
 * @brief Globals
 * 
 */
namespace Globals
{
    static constexpr auto beatsPerBar = 4;
    static constexpr auto ticksPerBeat = 16;

    static constexpr auto minClipLength = 1.f / static_cast<float>(ticksPerBeat);
    static constexpr auto minNoteLength = 1.f / static_cast<float>(ticksPerBeat);

    static constexpr auto velocitySaveResolution = 1024.f;

    static constexpr auto twelveToneKeyboardSize = 128;
    static constexpr auto twelveTonePeriodSize = 12;
    static constexpr auto numPeriodsInKeyboard =
        static_cast<float>(twelveToneKeyboardSize) /
        static_cast<float>(twelveTonePeriodSize);

    static constexpr auto numChannels = 16;

    static constexpr auto maxMsPerBeat = 250.0;

    /**
     * @brief default settings
     * 
     */
    namespace Defaults
    {
        static constexpr auto msPerBeat = 500;
        static constexpr auto tempoBpm = 60000 / msPerBeat;

        static constexpr auto projectLength = static_cast<float>(beatsPerBar * 8);

        static constexpr auto newNoteLength = 0.5f;
        static constexpr auto newNoteVelocity = 0.25f;
        static constexpr auto emptyClipLength = static_cast<float>(beatsPerBar * 2);

        static constexpr auto previewNoteVelocity = 0.35f;
        static constexpr auto previewNoteLength = static_cast<float>(beatsPerBar);

        static constexpr auto timeSignatureNumerator = 4;
        static constexpr auto timeSignatureDenominator = 4;

        static constexpr auto onOffControllerState = false;
        static constexpr auto automationControllerCurve = 0.5f;
    }

    /**
     * @brief UI Declarations
     * 
     */
    namespace UI
    {
        #if PLATFORM_MOBILE
        static constexpr auto headlineHeight = 40;
        static constexpr auto menuPanelRowHeight = 40;
        static constexpr auto sidebarWidth = 44;
        static constexpr auto rollScrollerHeight = 35;
        #elif PLATFORM_DESKTOP
        static constexpr auto headlineHeight = 32;
        static constexpr auto menuPanelRowHeight = 32;
        static constexpr auto sidebarWidth = 38;
        static constexpr auto rollScrollerHeight = 30;
        #endif

        static constexpr auto rollHeaderHeight = 36;
        static constexpr auto rollHeaderShadowSize = 16;
        static constexpr auto headlineIconSize = 16;
        static constexpr auto sidebarRowHeight = 36;

        static constexpr auto levelsMapHeight = 128;
        static constexpr auto projectMapHeight = 76;

        static constexpr auto fadeInShort = 100;
        static constexpr auto fadeOutShort = 125;

        static constexpr auto fadeInLong = 150;
        static constexpr auto fadeOutLong = 175;

        static constexpr auto tooltipDelayMs = 600;

        static constexpr auto iconSizeStep = 8;

        namespace Fonts
        {
            static constexpr auto XS = 14.f;
            static constexpr auto S = 16.f;
            static constexpr auto M = 18.f;
            static constexpr auto L = 21.f;
            static constexpr auto XL = 28.f;
            static constexpr auto XXL = 37.f;
        }

        namespace FileChooser
        {
            static constexpr auto forFileToSave =
                FileBrowserComponent::saveMode |
                FileBrowserComponent::canSelectFiles |
                FileBrowserComponent::warnAboutOverwriting;

            static constexpr auto forFileToOpen =
                FileBrowserComponent::openMode |
                FileBrowserComponent::canSelectFiles;

            static constexpr auto forDirectory =
                FileBrowserComponent::openMode |
                FileBrowserComponent::canSelectDirectories;
        }
    }
}

inline float roundBeat(float beat) {
    return roundf(beat * static_cast<float>(Globals::ticksPerBeat)) /
        static_cast<float>(Globals::ticksPerBeat);
}

#include "Serializable.h"
#include "app.h"
#include "TranslationKeys.h"

#if defined TRANS
#   undef TRANS
#endif

#define TRANS(singular) App::translate(singular)
#define TRANS_PLURAL(plural, number) App::translate(plural, number)