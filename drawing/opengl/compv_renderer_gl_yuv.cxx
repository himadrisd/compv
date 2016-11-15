/* Copyright (C) 2016-2017 Doubango Telecom <https://www.doubango.org>
* File author: Mamadou DIOP (Doubango Telecom, France).
* License: GPLv3. For commercial license please contact us.
* Source code: https://github.com/DoubangoTelecom/compv
* WebSite: http://compv.org
*/
#include "compv/drawing/opengl/compv_renderer_gl_yuv.h"
#if defined(HAVE_OPENGL) || defined(HAVE_OPENGLES)
#include "compv/drawing/compv_drawing.h"
#include "compv/drawing/opengl/compv_utils_gl.h"

COMPV_NAMESPACE_BEGIN()

CompVRendererGLYUV::CompVRendererGLYUV(COMPV_PIXEL_FORMAT eYUVPixelFormat)
	: CompVRendererGL(eYUVPixelFormat)
	, m_bInit(false)
	, m_uTexturesCount(0)
	, m_strPrgVertexData("")
	, m_strPrgFragData("")
{
	memset(&m_uNameTextures, 0, sizeof(m_uNameTextures));
	memset(&m_uWidths, 0, sizeof(m_uWidths));
	memset(&m_uHeights, 0, sizeof(m_uHeights));
	memset(&m_uStrides, 0, sizeof(m_uStrides));
}

CompVRendererGLYUV::~CompVRendererGLYUV()
{
	if (m_bInit) {
		COMPV_CHECK_CODE_ASSERT(deInit());
	}
}

COMPV_ERROR_CODE CompVRendererGLYUV::drawImage(CompVMatPtr mat)
{
	COMPV_CHECK_EXP_RETURN(!mat || mat->isEmpty(), COMPV_ERROR_CODE_E_INVALID_PARAMETER);
	COMPV_CHECK_EXP_RETURN(!CompVUtilsGL::haveCurrentContext(), COMPV_ERROR_CODE_E_GL_NO_CONTEXT);

	// Get pixel format and make sure it's supported
	COMPV_PIXEL_FORMAT pixelFormat = static_cast<COMPV_PIXEL_FORMAT>(mat->subType());
	COMPV_CHECK_EXP_RETURN(CompVRenderer::pixelFormat() != pixelFormat, COMPV_ERROR_CODE_E_INVALID_PARAMETER);

	// Check if format changed
	if (m_uTexturesCount != mat->compCount() || mat->cols(0) != m_uWidths[0] || mat->rows(0) != m_uHeights[0] || mat->stride(0) != m_uStrides[0]) {
		COMPV_DEBUG_INFO("GL renderer format changed");
		COMPV_CHECK_CODE_RETURN(deInit());
	}

	// Init if not already done
	if (!m_bInit) {
		COMPV_CHECK_CODE_RETURN(init(mat));
	}

	for (size_t t = 0; t < m_uTexturesCount; ++t) {
		glActiveTexture(GLenum(GL_TEXTURE0 + t));
		glBindTexture(GL_TEXTURE_2D, m_uNameTextures[t]);
		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			0,
			0,
			static_cast<GLsizei>(mat->stride(static_cast<int32_t>(t))),
			static_cast<GLsizei>(mat->rows(static_cast<int32_t>(t))),
			GL_LUMINANCE,
			GL_UNSIGNED_BYTE,
			mat->ptr(0, 0, static_cast<int32_t>(t)));
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	COMPV_CHECK_CODE_RETURN(COMPV_ERROR_CODE_E_NOT_IMPLEMENTED);

	return COMPV_ERROR_CODE_S_OK;
}

// Private function: do not check imput parameters
COMPV_ERROR_CODE CompVRendererGLYUV::init(CompVMatPtr mat)
{
	COMPV_ERROR_CODE err = COMPV_ERROR_CODE_S_OK;
	COMPV_CHECK_EXP_RETURN(!CompVUtilsGL::haveCurrentContext(), COMPV_ERROR_CODE_E_GL_NO_CONTEXT);
	COMPV_CHECK_EXP_RETURN(m_bInit, COMPV_ERROR_CODE_E_INVALID_STATE);
	COMPV_CHECK_CODE_BAIL(err = CompVRendererGL::init(mat, m_strPrgVertexData, m_strPrgFragData, false, false)); // Base class implementation

	m_uTexturesCount = mat->compCount();
	for (size_t compId = 0; compId < mat->compCount(); ++compId) {
		const int32_t compIdInt32 = static_cast<int32_t>(compId);
		m_uHeights[compId] = mat->rows(compIdInt32);
		m_uWidths[compId] = mat->cols(compIdInt32);
		m_uStrides[compId] = mat->stride(compIdInt32);
		glGenTextures(1, &m_uNameTextures[compIdInt32]);
		glActiveTexture(GLenum(GL_TEXTURE0 + compId));
		glBindTexture(GL_TEXTURE_2D, m_uNameTextures[compId]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, static_cast<GLsizei>(mat->stride(compIdInt32)), static_cast<GLsizei>(mat->rows(compIdInt32)), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

bail:
	if (COMPV_ERROR_CODE_IS_NOK(err)) {
		COMPV_CHECK_CODE_ASSERT(deInit());
	}
	else {
		m_bInit = true;
		return err;
	}
	return err;
}

COMPV_ERROR_CODE CompVRendererGLYUV::deInit()
{
	COMPV_CHECK_EXP_RETURN(!CompVUtilsGL::haveCurrentContext(), COMPV_ERROR_CODE_E_GL_NO_CONTEXT);
	COMPV_CHECK_CODE_RETURN(CompVRendererGL::deInit()); // Base class implementation
	for (size_t t = 0; t < m_uTexturesCount; ++t) {
		if (m_uNameTextures[t]) {
			glDeleteTextures(1, &m_uNameTextures[t]);
			m_uNameTextures[t] = 0;
		}
		m_uWidths[t] = 0;
		m_uHeights[t] = 0;
		m_uStrides[t] = 0;
	}
	m_uTexturesCount = 0;

	m_bInit = false;
	return COMPV_ERROR_CODE_S_OK;
}

COMPV_ERROR_CODE CompVRendererGLYUV::newObj(CompVRendererGLYUVPtrPtr glRenderer, COMPV_PIXEL_FORMAT eYUVPixelFormat)
{
	COMPV_CHECK_CODE_RETURN(CompVDrawing::init());
	COMPV_CHECK_EXP_RETURN(!glRenderer, COMPV_ERROR_CODE_E_INVALID_PARAMETER);
	COMPV_CHECK_EXP_RETURN(
		eYUVPixelFormat != COMPV_PIXEL_FORMAT_GRAYSCALE
		&& eYUVPixelFormat != COMPV_PIXEL_FORMAT_I420
		&& eYUVPixelFormat != COMPV_PIXEL_FORMAT_IYUV,
		COMPV_ERROR_CODE_E_INVALID_PARAMETER);

	CompVRendererGLYUVPtr glRenderer_ = new CompVRendererGLYUV(eYUVPixelFormat);
	COMPV_CHECK_EXP_RETURN(!glRenderer_, COMPV_ERROR_CODE_E_OUT_OF_MEMORY);

	COMPV_CHECK_EXP_RETURN(!(*glRenderer = glRenderer_), COMPV_ERROR_CODE_E_NOT_IMPLEMENTED);
	return COMPV_ERROR_CODE_S_OK;
}

COMPV_NAMESPACE_END()

#endif /* defined(HAVE_OPENGL) || defined(HAVE_OPENGLES) */
