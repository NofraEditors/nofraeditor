/**
 * @file document.cpp
 * @author Krisna Pranav
 * @brief Document Source
 * @version 0.1
 * @date 2023-02-17
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditorDevelopers
 * 
 */

#include "common.h"
#include "document.h"
#include "documentowner.h"
#include "documenthelpers.h"
#include "mainlayout.h"

/**
 * @brief Construct a new Document:: Document object
 * 
 * @param documentOwner 
 * @param defaultName 
 * @param defaultExtension 
 */
Document::Document(DocumentOwner &documentOwner,
    const String &defaultName,
    const String &defaultExtension) :
    owner(documentOwner),
    extension(defaultExtension)
{
    if (defaultName.isNotEmpty())
    {
        jassert(!defaultExtension.startsWithChar('.'));
        const auto safeName = File::createLegalFileName(defaultName + "." + defaultExtension);
        this->workingFile = DocumentHelpers::getDocumentSlot(safeName);
        if (this->workingFile.existsAsFile())
        {
            this->workingFile = this->workingFile.getNonexistentSibling(true);
        }
    }

    this->owner.addChangeListener(this);
}

/**
 * @brief Construct a new Document:: Document object
 * 
 * @param documentOwner 
 * @param existingFile 
 */
Document::Document(DocumentOwner &documentOwner, const File &existingFile) :
    owner(documentOwner),
    extension(existingFile.getFileExtension().replace(".", "")),
    workingFile(existingFile)
{
    this->owner.addChangeListener(this);
}

/**
 * @brief Destroy the Document:: Document object
 * 
 */
Document::~Document()
{
    this->owner.removeChangeListener(this);
}

/**
 * @brief Change Listener Callback
 * 
 * @param source 
 */
void Document::changeListenerCallback(ChangeBroadcaster *source)
{
    this->hasChanges = true;
}

/**
 * @brief Object to get a full path
 * 
 * @return String 
 */
String Document::getFullPath() const
{
    return this->workingFile.getFullPathName();
}

/**
 * @brief RevealToUser
 * 
 */
void Document::revealToUser() const
{
    this->workingFile.revealToUser();
}

/**
 * @brief RenameFile
 * 
 * @param newName 
 */
void Document::renameFile(const String &newName)
{
    if (newName == this->workingFile.getFileNameWithoutExtension())
    {
        return;
    }

    const auto safeNewName = File::createLegalFileName(newName).trimCharactersAtEnd(".");

    jassert(!this->extension.startsWithChar('.'));
    File newFile(this->workingFile.getSiblingFile(safeNewName + "." + this->extension));

    if (newFile.existsAsFile())
    {
        newFile = newFile.getNonexistentSibling(true);
    }

    if (this->workingFile.moveFileTo(newFile))
    {
        DBG("Renaming to " + newFile.getFileName());
        this->workingFile = newFile;
    }
}

/**
 * @brief Save
 * 
 */
void Document::save()
{
    if (this->hasChanges &&
        this->workingFile.getFullPathName().isNotEmpty())
    {
        const String fullPath = this->workingFile.getFullPathName();
        const auto firstCharAfterLastSlash = fullPath.lastIndexOfChar(File::getSeparatorChar()) + 1;
        const auto lastDot = fullPath.lastIndexOfChar('.');
        const bool hasEmptyName = (lastDot == firstCharAfterLastSlash);
        if (hasEmptyName)
        {
            return;
        }

        const bool savedOk = this->owner.onDocumentSave(this->workingFile);

        if (savedOk)
        {
            this->hasChanges = false;
            DBG("Document saved: " + this->workingFile.getFullPathName());
            return;
        }

        DBG("Document save failed: " + this->workingFile.getFullPathName());
    }
}

/**
 * @brief Exports
 * 
 * @param exportExtension 
 * @param defaultFilenameWithExtension 
 */
void Document::exportAs(const String &exportExtension,
    const String &defaultFilenameWithExtension)
{
    if (!FileChooser::isPlatformDialogAvailable())
    {
        return;
    }

    this->exportFileChooser = make<FileChooser>(TRANS(I18n::Dialog::documentExport),
        DocumentHelpers::getDocumentSlot(File::createLegalFileName(defaultFilenameWithExtension)),
        exportExtension, true);

    DocumentHelpers::showFileChooser(this->exportFileChooser,
        Globals::UI::FileChooser::forFileToSave,
        [this](URL &url)
    {
        if (url.isLocalFile() && url.getLocalFile().exists())
        {
            url.getLocalFile().deleteFile();
        }

        if (auto outStream = url.createOutputStream())
        {
            outStream->setPosition(0); 
            if (this->owner.onDocumentExport(*outStream.get()))
            {
                App::Layout().showTooltip(TRANS(I18n::Dialog::documentExportDone));
            }
        }
    });
}

/**
 * @brief Load
 * 
 * @param file 
 * @return true 
 * @return false 
 */
bool Document::load(const File &file)
{
    if (!file.existsAsFile())
    {
        jassertfalse; 
        return false;
    }

    this->workingFile = file;
    this->hasChanges = false;

    if (!this->owner.onDocumentLoad(file))
    {
        DBG("Document load failed: " + this->workingFile.getFullPathName());
        return false;
    }

    return true;
}

/**
 * @brief Import
 * 
 * @param filePattern 
 */
void Document::import(const String &filePattern)
{
#if JUCE_ANDROID
    const auto filter = "*/*";
#else
    const auto filter = filePattern;
#endif

    this->importFileChooser = make<FileChooser>(TRANS(I18n::Dialog::documentImport),
        File::getSpecialLocation(File::userDocumentsDirectory), filter, true);

    DocumentHelpers::showFileChooser(this->importFileChooser,
        Globals::UI::FileChooser::forFileToOpen,
        [this](URL &url)
    {

        if (auto inStream = url.createInputStream(URL::InputStreamOptions(URL::ParameterHandling::inAddress)))
        {
            this->owner.onDocumentImport(*inStream.get());
        }
    });
}