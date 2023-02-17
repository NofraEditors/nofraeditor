/**
 * @file backendrequest.h
 * @author Krisna Pranav
 * @brief Backend Requestion
 * @version 0.1
 * @date 2023-02-17
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, CobraEditorDevelopers
 * 
 */

#pragma once 

#if !NO_NETWORK

#include "jsonserializer.h"

class BackendRequest final {
public:
    BackendRequest(const String &apiEndpoint);

    struct Response final {
        Response();

        bool is2xx() const noexcept;
        bool is200() const noexcept;
        bool is(int code) const noexcept;
    };

    Response get() const;
    Response post(const SerializedData &payload) const;
    Response put(const SerializedData &payload) const;
    Response del() const;
}; 


#endif