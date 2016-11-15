/* Copyright (C) 2016-2017 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: GPLv3. For commercial license please contact us.
* Source code: https://github.com/DoubangoTelecom/compv
* WebSite: http://compv.org
*/
#if !defined(_COMPV_DRAWING_CANVAS_H_)
#define _COMPV_DRAWING_CANVAS_H_

#include "compv/base/compv_config.h"
#include "compv/base/compv_common.h"
#include "compv/base/compv_obj.h"
#include "compv/base/compv_mat.h"

#include <string>

COMPV_NAMESPACE_BEGIN()

class CompVCanvasImpl;
typedef CompVPtr<CompVCanvasImpl* > CompVCanvasImplPtr;
typedef CompVCanvasImplPtr* CompVCanvasImplPtrPtr;

class CompVCanvasImpl : public CompVObj
{
protected:
	CompVCanvasImpl();
public:
	virtual ~CompVCanvasImpl();

	virtual COMPV_ERROR_CODE drawText(const void* textPtr, size_t textLengthInBytes, size_t x, size_t y) = 0;

	static COMPV_ERROR_CODE newObj(CompVCanvasImplPtrPtr canvasImpl);

protected:

private:
};

class CompVCanvas;
typedef CompVPtr<CompVCanvas* > CompVCanvasPtr;
typedef CompVCanvasPtr* CompVCanvasPtrPtr;

class COMPV_DRAWING_API CompVCanvas : public CompVObj
{
protected:
	CompVCanvas();
public:
	virtual ~CompVCanvas();

	virtual COMPV_ERROR_CODE test() = 0;

	static COMPV_ERROR_CODE newObj(CompVCanvasPtrPtr canvas);

protected:

private:
	COMPV_VS_DISABLE_WARNINGS_BEGIN(4251 4267)
	CompVCanvasImplPtr m_ptrImpl;
	COMPV_VS_DISABLE_WARNINGS_END()
};

COMPV_NAMESPACE_END()

#endif /* _COMPV_DRAWING_CANVAS_H_ */
