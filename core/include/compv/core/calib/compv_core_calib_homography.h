/* Copyright (C) 2016-2017 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: GPLv3. For commercial license please contact us.
* Source code: https://github.com/DoubangoTelecom/compv
* WebSite: http://compv.org
*/
#if !defined(_COMPV_CORE_CALIB_HOMOGRAPHY_H_)
#define _COMPV_CORE_CALIB_HOMOGRAPHY_H_

#include "compv/core/compv_core_config.h"
#include "compv/core/compv_core_common.h"
#include "compv/base/compv_mat.h"

COMPV_NAMESPACE_BEGIN()

template<class T>
class COMPV_CORE_API CompVHomography
{
public:
	static COMPV_ERROR_CODE find(CompVMatPtrPtr H, const CompVMatPtr &src, const CompVMatPtr &dst, COMPV_MODELEST_TYPE model = COMPV_MODELEST_TYPE_RANSAC);
};

COMPV_NAMESPACE_END()

#endif /* _COMPV_CORE_CALIB_HOMOGRAPHY_H_ */