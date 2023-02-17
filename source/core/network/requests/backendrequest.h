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

    /**
     * @brief Construct a new Backend Request object
     * 
     * @param apiEndpoint 
     */
    BackendRequest(const String &apiEndpoint);

    struct Response final {
        Response();

        /**
         * @brief is 200 above
         * 
         * @return true 
         * @return false 
         */
        bool is2xx() const noexcept;
        bool is200() const noexcept;
        bool is(int code) const noexcept;

        /**
         * @brief has Valid body
         * 
         * @return true 
         * @return false 
         */
        bool hasValidBody() const noexcept;

        // SerializedData wrappers
        bool hasProperty(const Identifier &name) const noexcept;
        var getProperty(const Identifier &name) const noexcept;
        SerializedData getChild(const Identifier &name) const noexcept;

        const Array<String> &getErrors() const noexcept;
        const SerializedData getBody() const noexcept;
        const String getRedirect() const noexcept;

    private:

        SerializedData body;

        Array<String> errors;

        int statusCode = 0;
        StringPairArray headers;

        friend class BackendRequest;
    };

    /**
     * @brief Get Object
     * 
     * @return Response 
     */
    Response get() const;

    /**
     * @brief Post object
     * 
     * @param payload 
     * @return Response 
     */
    Response post(const SerializedData &payload) const;

    /**
     * @brief put object
     * 
     * @param payload 
     * @return Response 
     */
    Response put(const SerializedData &payload) const;

    /**
     * @brief delete object
     * 
     * @return Response 
     */
    Response del() const;

private:

    static constexpr auto connectionTimeoutMs = 0;

    static constexpr auto numConnectAttempts = 3;

    String apiEndpoint;
    JsonSerializer serializer;

    /**
     * @brief doRequest
     * 
     * @param verb 
     * @return Response 
     */
    Response doRequest(const String &verb) const;
    Response doRequest(const SerializedData &payload, const String &verb) const;

    /**
     * @brief Process Response
     * 
     * @param response 
     * @param stream 
     */
    void processResponse(Response &response, InputStream *const stream) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BackendRequest)
};

#endif