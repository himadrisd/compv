/* Copyright (C) 2016-2017 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: GPLv3. For commercial license please contact us.
* Source code: https://github.com/DoubangoTelecom/compv
* WebSite: http://compv.org
*/
#if !defined(_COMPV_BASE_PRALLEL_MUTEX_H_)
#define _COMPV_BASE_PRALLEL_MUTEX_H_

#include "compv/base/compv_config.h"
#include "compv/base/compv_obj.h"
#include "compv/base/compv_common.h"

COMPV_NAMESPACE_BEGIN()

COMPV_OBJECT_DECLARE_PTRS(Mutex)

class COMPV_BASE_API CompVMutex : public CompVObj
{
protected:
    CompVMutex(bool recursive = true);
public:
    virtual ~CompVMutex();
    virtual COMPV_INLINE const char* getObjectId() {
        return "CompVMutex";
    };

    COMPV_ERROR_CODE lock();
    COMPV_ERROR_CODE unlock();

    COMPV_INLINE const void* handle() {
        return m_pHandle;    // "'pthread_mutex_t*' on Linux and 'HANDLE' on Windows"
    }

    static COMPV_ERROR_CODE newObj(CompVMutexPtrPtr mutex, bool recursive = true);

private:
    void* m_pHandle;
};

COMPV_NAMESPACE_END()

#endif /* _COMPV_BASE_PRALLEL_MUTEX_H_ */

