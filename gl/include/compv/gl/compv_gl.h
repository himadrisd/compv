/* Copyright (C) 2016-2017 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: GPLv3. For commercial license please contact us.
* Source code: https://github.com/DoubangoTelecom/compv
* WebSite: http://compv.org
*/
#if !defined(_COMPV_GL_H_)
#define _COMPV_GL_H_

#include "compv/gl/compv_config.h"
#include "compv/gl/compv_common.h"

COMPV_NAMESPACE_BEGIN()

class COMPV_GL_API CompVGL
{
public:
	static COMPV_ERROR_CODE init();
	static COMPV_ERROR_CODE deInit();
private:
	static bool s_bInitialized;
};

COMPV_NAMESPACE_END()

#endif /* _COMPV_GL_H_ */
