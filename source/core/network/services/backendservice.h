/**
 * @file backendservice.h
 * @author Krisna Pranav
 * @brief Requesting[Backend] Servicer
 * @version 0.1
 * @date 2023-02-14
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, NofraEditorDevelopers
 * 
 */

#pragma once

#if !NO_NETWORK

class BackendService
{
protected:

    /**
     * @brief Destroy the Backend Service object
     * 
     */
    virtual ~BackendService() {
        for (auto *thread : this->requestThreads)
        {
            if (thread->isThreadRunning())
            {
                thread->signalThreadShouldExit();
            }
        }
    }

    /**
     * @brief RequestedThreads
     * 
     */
    OwnedArray<Thread> requestThreads;

    /**
     * @brief Get the New Thread For object
     * 
     * @tparam T 
     * @return T* 
     */
    template<typename T>
    T *getNewThreadFor() {
        for (auto *thread : this->requestThreads)
        {
            if (!thread->isThreadRunning())
            {
                if (T *target = dynamic_cast<T *>(thread))
                {
                    return target;
                }
            }
        }

        return static_cast<T *>(this->requestThreads.add(new T()));
    }

    /**
     * @brief Get the Running Thread For object
     * 
     * @tparam T 
     * @return T* 
     */
    template<typename T>
    T *getRunningThreadFor() {
        for (auto *thread : this->requestThreads)
        {
            if (thread->isThreadRunning())
            {
                if (T *target = dynamic_cast<T *>(thread))
                {
                    return target;
                }
            }
        }

        return nullptr;
    }
};

#endif