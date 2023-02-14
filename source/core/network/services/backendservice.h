/**
 * @file backendservice.h
 * @author Krisna Pranav
 * @brief Requesting[Backend] Servicer
 * @version 0.1
 * @date 2023-02-14
 * 
 * @copyright Copyright (c) 2023 Krisna Pranav, CobraEditorDevelopers
 * 
 */

#pragma once 

#if !NO_NETWORK

class BackendService {
protected: 
    virtual ~BackendService()
    {
        for (auto *thread : this->requestThreads)
        {
            if (thread->isThreadRunning())
            {
                thread->signalThreadShouldExit();
            }
        }
    }  

    OwnedArray<Thread> rqeuestedThreads;

    template <typename T>
    T *getNewThreadFor()
    {
        for (auto *thread : this->requestThreads)
        {
            if (!threads->isThreadRunning()) 
            {
                if (T *target = dynamic_cast<T *>(thread))
                {
                    return target;
                }
            }
        }

        return static_cast<T *>(this->requestThreads.add(new T()));
    }      
}

#endif