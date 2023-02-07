/**
 * @file serializeddata.h
 * @author Krisna Pranav
 * @brief Serializing Datas
 * @version 0.1
 * @date 2023-02-07
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditorDevelopers
 * 
 */

#pragma once

class SerializedData final {
public:
    SerializedData() noexcept;
    explicit SerializedData(const Identifier &type);

    SerializedData(const SerializedData &) noexcept;
    SerializedData(SerializedData &&) noexcept;
    SerializedData &operator= (const SerializedData &);

    ~SerializedData();

    Iterator begin() const noexcept;
    Iterator end() const noexcept;

private:
    class SharedData;
    ReferenceCountedObjectPtr<SharedData> data;

    friend class SharedData;
}