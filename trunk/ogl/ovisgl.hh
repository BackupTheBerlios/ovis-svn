#ifndef OVIS_OPENGLDRV_OVISGL_HH_INCLUDED
#define OVIS_OPENGLDRV_OVISGL_HH_INCLUDED

/*#ifdef WIN32
#include "./extgl.h"
#else
#include <GL/gl.h>
#endif*/
#ifdef WIN32
#include <windows.h>
#include <GL/gl.h>
#endif
#if !defined(_WIN32) && !defined(__CYGWIN__) && !defined(__MINGW32__)

#define GL_GLEXT_PROTOTYPES
#include <GL/glx.h>

#endif /* _WIN32 */

#ifndef APIENTRY
#define APIENTRY
#endif

/* OpenGL 1.2 */

#ifndef GL_VERSION_1_2
#define GL_ARB_imaging 1
#define GL_VERSION_1_2 1
#define OVIS_IMPLEMENT_GL_VERSION_1_2 1
#define OVIS_IMPLEMENT_GL_ARB_IMAGING 1
#define GL_RESCALE_NORMAL                                       0x803A
#define GL_CLAMP_TO_EDGE                                        0x812F
#define GL_MAX_ELEMENTS_VERTICES                                0x80E8
#define GL_MAX_ELEMENTS_INDICES                                 0x80E9
#define GL_BGR                                                  0x80E0
#define GL_BGRA                                                 0x80E1
#define GL_UNSIGNED_BYTE_3_3_2                                  0x8032
#define GL_UNSIGNED_BYTE_2_3_3_REV                              0x8362
#define GL_UNSIGNED_SHORT_5_6_5                                 0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV                             0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4                               0x8033
#define GL_UNSIGNED_SHORT_4_4_4_4_REV                           0x8365
#define GL_UNSIGNED_SHORT_5_5_5_1                               0x8034
#define GL_UNSIGNED_SHORT_1_5_5_5_REV                           0x8366
#define GL_UNSIGNED_INT_8_8_8_8                                 0x8035
#define GL_UNSIGNED_INT_8_8_8_8_REV                             0x8367
#define GL_UNSIGNED_INT_10_10_10_2                              0x8036
#define GL_UNSIGNED_INT_2_10_10_10_REV                          0x8368
#define GL_LIGHT_MODEL_COLOR_CONTROL                            0x81F8
#define GL_SINGLE_COLOR                                         0x81F9
#define GL_SEPARATE_SPECULAR_COLOR                              0x81FA
#define GL_TEXTURE_MIN_LOD                                      0x813A
#define GL_TEXTURE_MAX_LOD                                      0x813B
#define GL_TEXTURE_BASE_LEVEL                                   0x813C
#define GL_TEXTURE_MAX_LEVEL                                    0x813D
#define GL_SMOOTH_POINT_SIZE_RANGE                              0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY                        0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE                              0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY                        0x0B23
#define GL_ALIASED_POINT_SIZE_RANGE                             0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE                             0x846E
#define GL_PACK_SKIP_IMAGES                                     0x806B
#define GL_PACK_IMAGE_HEIGHT                                    0x806C
#define GL_UNPACK_SKIP_IMAGES                                   0x806D
#define GL_UNPACK_IMAGE_HEIGHT                                  0x806E
#define GL_TEXTURE_3D                                           0x806F
#define GL_PROXY_TEXTURE_3D                                     0x8070
#define GL_TEXTURE_DEPTH                                        0x8071
#define GL_TEXTURE_WRAP_R                                       0x8072
#define GL_MAX_3D_TEXTURE_SIZE                                  0x8073
#define GL_TEXTURE_BINDING_3D                                   0x806A
#define GL_COLOR_TABLE                                          0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE                         0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE                        0x80D2
#define GL_PROXY_COLOR_TABLE                                    0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE                   0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE                  0x80D5
#define GL_COLOR_TABLE_SCALE                                    0x80D6
#define GL_COLOR_TABLE_BIAS                                     0x80D7
#define GL_COLOR_TABLE_FORMAT                                   0x80D8
#define GL_COLOR_TABLE_WIDTH                                    0x80D9
#define GL_COLOR_TABLE_RED_SIZE                                 0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE                               0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE                                0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE                               0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE                           0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE                           0x80DF
#define GL_CONVOLUTION_1D                                       0x8010
#define GL_CONVOLUTION_2D                                       0x8011
#define GL_SEPARABLE_2D                                         0x8012
#define GL_CONVOLUTION_BORDER_MODE                              0x8013
#define GL_CONVOLUTION_FILTER_SCALE                             0x8014
#define GL_CONVOLUTION_FILTER_BIAS                              0x8015
#define GL_REDUCE                                               0x8016
#define GL_CONVOLUTION_FORMAT                                   0x8017
#define GL_CONVOLUTION_WIDTH                                    0x8018
#define GL_CONVOLUTION_HEIGHT                                   0x8019
#define GL_MAX_CONVOLUTION_WIDTH                                0x801A
#define GL_MAX_CONVOLUTION_HEIGHT                               0x801B
#define GL_POST_CONVOLUTION_RED_SCALE                           0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE                         0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE                          0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE                         0x801F
#define GL_POST_CONVOLUTION_RED_BIAS                            0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS                          0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS                           0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS                          0x8023
#define GL_CONSTANT_BORDER                                      0x8151
#define GL_REPLICATE_BORDER                                     0x8153
#define GL_CONVOLUTION_BORDER_COLOR                             0x8154
#define GL_COLOR_MATRIX                                         0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH                             0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH                         0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE                          0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE                        0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE                         0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE                        0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS                           0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS                         0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS                          0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS                         0x80BB
#define GL_HISTOGRAM                                            0x8024
#define GL_PROXY_HISTOGRAM                                      0x8025
#define GL_HISTOGRAM_WIDTH                                      0x8026
#define GL_HISTOGRAM_FORMAT                                     0x8027
#define GL_HISTOGRAM_RED_SIZE                                   0x8028
#define GL_HISTOGRAM_GREEN_SIZE                                 0x8029
#define GL_HISTOGRAM_BLUE_SIZE                                  0x802A
#define GL_HISTOGRAM_ALPHA_SIZE                                 0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE                             0x802C
#define GL_HISTOGRAM_SINK                                       0x802D
#define GL_MINMAX                                               0x802E
#define GL_MINMAX_FORMAT                                        0x802F
#define GL_MINMAX_SINK                                          0x8030
#define GL_TABLE_TOO_LARGE                                      0x8031
#define GL_BLEND_EQUATION                                       0x8009
#define GL_MIN                                                  0x8007
#define GL_MAX                                                  0x8008
#define GL_FUNC_ADD                                             0x8006
#define GL_FUNC_SUBTRACT                                        0x800A
#define GL_FUNC_REVERSE_SUBTRACT                                0x800B
#define GL_BLEND_COLOR                                          0x8005
#define GL_CONSTANT_COLOR                                       0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR                             0x8002
#define GL_CONSTANT_ALPHA                                       0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA                             0x8004

typedef void (APIENTRY * glColorTablePROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table );
typedef void (APIENTRY * glColorSubTablePROC) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data );
typedef void (APIENTRY * glColorTableParameterivPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (APIENTRY * glColorTableParameterfvPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (APIENTRY * glCopyColorSubTablePROC) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width );
typedef void (APIENTRY * glCopyColorTablePROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width );
typedef void (APIENTRY * glGetColorTablePROC) (GLenum target, GLenum format, GLenum type, GLvoid *table );
typedef void (APIENTRY * glGetColorTableParameterfvPROC) (GLenum target, GLenum pname, GLfloat *params );
typedef void (APIENTRY * glGetColorTableParameterivPROC) (GLenum target, GLenum pname, GLint *params );
typedef void (APIENTRY * glBlendEquationPROC) (GLenum mode );
typedef void (APIENTRY * glBlendColorPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
typedef void (APIENTRY * glHistogramPROC) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink );
typedef void (APIENTRY * glResetHistogramPROC) (GLenum target );
typedef void (APIENTRY * glGetHistogramPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values );
typedef void (APIENTRY * glGetHistogramParameterfvPROC) (GLenum target, GLenum pname, GLfloat *params );
typedef void (APIENTRY * glGetHistogramParameterivPROC) (GLenum target, GLenum pname, GLint *params );
typedef void (APIENTRY * glMinmaxPROC) (GLenum target, GLenum internalformat, GLboolean sink );
typedef void (APIENTRY * glResetMinmaxPROC) (GLenum target );
typedef void (APIENTRY * glGetMinmaxPROC) (GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values );
typedef void (APIENTRY * glGetMinmaxParameterfvPROC) (GLenum target, GLenum pname, GLfloat *params );
typedef void (APIENTRY * glGetMinmaxParameterivPROC) (GLenum target, GLenum pname, GLint *params );
typedef void (APIENTRY * glConvolutionFilter1DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image );
typedef void (APIENTRY * glConvolutionFilter2DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image );
typedef void (APIENTRY * glConvolutionParameterfPROC) (GLenum target, GLenum pname, GLfloat params );
typedef void (APIENTRY * glConvolutionParameterfvPROC) (GLenum target, GLenum pname, const GLfloat *params );
typedef void (APIENTRY * glConvolutionParameteriPROC) (GLenum target, GLenum pname, GLint params );
typedef void (APIENTRY * glConvolutionParameterivPROC) (GLenum target, GLenum pname, const GLint *params );
typedef void (APIENTRY * glCopyConvolutionFilter1DPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width );
typedef void (APIENTRY * glCopyConvolutionFilter2DPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (APIENTRY * glGetConvolutionFilterPROC) (GLenum target, GLenum format, GLenum type, GLvoid *image );
typedef void (APIENTRY * glGetConvolutionParameterfvPROC) (GLenum target, GLenum pname, GLfloat *params );
typedef void (APIENTRY * glGetConvolutionParameterivPROC) (GLenum target, GLenum pname, GLint *params );
typedef void (APIENTRY * glSeparableFilter2DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column );
typedef void (APIENTRY * glGetSeparableFilterPROC) (GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span );
typedef void (APIENTRY * glDrawRangeElementsPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices );
typedef void (APIENTRY * glTexImage3DPROC) (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels );
typedef void (APIENTRY * glTexSubImage3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRY * glCopyTexSubImage3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height );

extern glBlendColorPROC glBlendColor;
extern glBlendEquationPROC glBlendEquation;
extern glColorTablePROC glColorTable;
extern glColorTableParameterfvPROC glColorTableParameterfv;
extern glColorTableParameterivPROC glColorTableParameteriv;
extern glCopyColorTablePROC glCopyColorTable;
extern glGetColorTablePROC glGetColorTable;
extern glGetColorTableParameterfvPROC glGetColorTableParameterfv;
extern glGetColorTableParameterivPROC glGetColorTableParameteriv;
extern glColorSubTablePROC glColorSubTable;
extern glCopyColorSubTablePROC glCopyColorSubTable;
extern glConvolutionFilter1DPROC glConvolutionFilter1D;
extern glConvolutionFilter2DPROC glConvolutionFilter2D;
extern glConvolutionParameterfPROC glConvolutionParameterf;
extern glConvolutionParameterfvPROC glConvolutionParameterfv;
extern glConvolutionParameteriPROC glConvolutionParameteri;
extern glConvolutionParameterivPROC glConvolutionParameteriv;
extern glCopyConvolutionFilter1DPROC glCopyConvolutionFilter1D;
extern glCopyConvolutionFilter2DPROC glCopyConvolutionFilter2D;
extern glGetConvolutionFilterPROC glGetConvolutionFilter;
extern glGetConvolutionParameterfvPROC glGetConvolutionParameterfv;
extern glGetConvolutionParameterivPROC glGetConvolutionParameteriv;
extern glGetSeparableFilterPROC glGetSeparableFilter;
extern glSeparableFilter2DPROC glSeparableFilter2D;
extern glGetHistogramPROC glGetHistogram;
extern glGetHistogramParameterfvPROC glGetHistogramParameterfv;
extern glGetHistogramParameterivPROC glGetHistogramParameteriv;
extern glGetMinmaxPROC glGetMinmax;
extern glGetMinmaxParameterfvPROC glGetMinmaxParameterfv;
extern glGetMinmaxParameterivPROC glGetMinmaxParameteriv;
extern glHistogramPROC glHistogram;
extern glMinmaxPROC glMinmax;
extern glResetHistogramPROC glResetHistogram;
extern glResetMinmaxPROC glResetMinmax;
extern glDrawRangeElementsPROC glDrawRangeElements;
extern glTexImage3DPROC glTexImage3D;
extern glTexSubImage3DPROC glTexSubImage3D;
extern glCopyTexSubImage3DPROC glCopyTexSubImage3D;

#endif /* GL_VERSION_1_2 */





#ifndef GL_ARB_texture_compression
#define GL_ARB_texture_compression 1
#define OVIS_IMPLEMENT_GL_ARB_TEXTURE_COMPRESSION 1
#define GL_COMPRESSED_ALPHA_ARB					0x84E9
#define GL_COMPRESSED_LUMINANCE_ARB				0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA_ARB		0x84EB
#define GL_COMPRESSED_INTENSITY_ARB				0x84EC
#define GL_COMPRESSED_RGB_ARB					0x84ED
#define GL_COMPRESSED_RGBA_ARB					0x84EE
#define GL_TEXTURE_COMPRESSION_HINT_ARB			0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB	0x86A0
#define GL_TEXTURE_COMPRESSED_ARB				0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB	0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS_ARB		0x86A3

typedef void (APIENTRY * glCompressedTexImage1DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexImage2DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexImage3DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexSubImage1DARBPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexSubImage2DARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glCompressedTexSubImage3DARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data );
typedef void (APIENTRY * glGetCompressedTexImageARBPROC) (GLenum target, GLint lod, GLvoid *img );

extern glCompressedTexImage3DARBPROC glCompressedTexImage3DARB;
extern glCompressedTexImage2DARBPROC glCompressedTexImage2DARB;
extern glCompressedTexImage1DARBPROC glCompressedTexImage1DARB; 
extern glCompressedTexSubImage3DARBPROC glCompressedTexSubImage3DARB;
extern glCompressedTexSubImage2DARBPROC glCompressedTexSubImage2DARB;
extern glCompressedTexSubImage1DARBPROC glCompressedTexSubImage1DARB;
extern glGetCompressedTexImageARBPROC glGetCompressedTexImageARB;

#endif /* GL_ARB_texture_compression */


#ifndef GL_ARB_texture_mirrored_repeat
#define GL_ARB_texture_mirrored_repeat 1

#define GL_MIRRORED_REPEAT_ARB									0x8370

#endif /* GL_ARB_texture_mirrored_repeat */




#ifndef GL_ARB_multitexture
#define GL_ARB_multitexture 1
#define OVIS_IMPLEMENT_GL_ARB_MULTITEXTURE 1
#define GL_TEXTURE0_ARB                                         0x84C0
#define GL_TEXTURE1_ARB                                         0x84C1
#define GL_TEXTURE2_ARB                                         0x84C2
#define GL_TEXTURE3_ARB                                         0x84C3
#define GL_TEXTURE4_ARB                                         0x84C4
#define GL_TEXTURE5_ARB                                         0x84C5
#define GL_TEXTURE6_ARB                                         0x84C6
#define GL_TEXTURE7_ARB                                         0x84C7
#define GL_TEXTURE8_ARB                                         0x84C8
#define GL_TEXTURE9_ARB                                         0x84C9
#define GL_TEXTURE10_ARB                                        0x84CA
#define GL_TEXTURE11_ARB                                        0x84CB
#define GL_TEXTURE12_ARB                                        0x84CC
#define GL_TEXTURE13_ARB                                        0x84CD
#define GL_TEXTURE14_ARB                                        0x84CE
#define GL_TEXTURE15_ARB                                        0x84CF
#define GL_TEXTURE16_ARB                                        0x84D0
#define GL_TEXTURE17_ARB                                        0x84D1
#define GL_TEXTURE18_ARB                                        0x84D2
#define GL_TEXTURE19_ARB                                        0x84D3
#define GL_TEXTURE20_ARB                                        0x84D4
#define GL_TEXTURE21_ARB                                        0x84D5
#define GL_TEXTURE22_ARB                                        0x84D6
#define GL_TEXTURE23_ARB                                        0x84D7
#define GL_TEXTURE24_ARB                                        0x84D8
#define GL_TEXTURE25_ARB                                        0x84D9
#define GL_TEXTURE26_ARB                                        0x84DA
#define GL_TEXTURE27_ARB                                        0x84DB
#define GL_TEXTURE28_ARB                                        0x84DC
#define GL_TEXTURE29_ARB                                        0x84DD
#define GL_TEXTURE30_ARB                                        0x84DE
#define GL_TEXTURE31_ARB                                        0x84DF
#define GL_ACTIVE_TEXTURE_ARB                                   0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB                            0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB                                0x84E2

typedef void (APIENTRY * glActiveTextureARBPROC) (GLenum texture );
typedef void (APIENTRY * glClientActiveTextureARBPROC) (GLenum texture );
typedef void (APIENTRY * glMultiTexCoord1dARBPROC) (GLenum target, GLdouble s );
typedef void (APIENTRY * glMultiTexCoord1dvARBPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY * glMultiTexCoord1fARBPROC) (GLenum target, GLfloat s );
typedef void (APIENTRY * glMultiTexCoord1fvARBPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY * glMultiTexCoord1iARBPROC) (GLenum target, GLint s );
typedef void (APIENTRY * glMultiTexCoord1ivARBPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY * glMultiTexCoord1sARBPROC) (GLenum target, GLshort s );
typedef void (APIENTRY * glMultiTexCoord1svARBPROC) (GLenum target, const GLshort *v );
typedef void (APIENTRY * glMultiTexCoord2dARBPROC) (GLenum target, GLdouble s, GLdouble t );
typedef void (APIENTRY * glMultiTexCoord2dvARBPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY * glMultiTexCoord2fARBPROC) (GLenum target, GLfloat s, GLfloat t );
typedef void (APIENTRY * glMultiTexCoord2fvARBPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY * glMultiTexCoord2iARBPROC) (GLenum target, GLint s, GLint t );
typedef void (APIENTRY * glMultiTexCoord2ivARBPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY * glMultiTexCoord2sARBPROC) (GLenum target, GLshort s, GLshort t );
typedef void (APIENTRY * glMultiTexCoord2svARBPROC) (GLenum target, const GLshort *v );
typedef void (APIENTRY * glMultiTexCoord3dARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r );
typedef void (APIENTRY * glMultiTexCoord3dvARBPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY * glMultiTexCoord3fARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r );
typedef void (APIENTRY * glMultiTexCoord3fvARBPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY * glMultiTexCoord3iARBPROC) (GLenum target, GLint s, GLint t, GLint r );
typedef void (APIENTRY * glMultiTexCoord3ivARBPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY * glMultiTexCoord3sARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r );
typedef void (APIENTRY * glMultiTexCoord3svARBPROC) (GLenum target, const GLshort *v );
typedef void (APIENTRY * glMultiTexCoord4dARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q );
typedef void (APIENTRY * glMultiTexCoord4dvARBPROC) (GLenum target, const GLdouble *v );
typedef void (APIENTRY * glMultiTexCoord4fARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q );
typedef void (APIENTRY * glMultiTexCoord4fvARBPROC) (GLenum target, const GLfloat *v );
typedef void (APIENTRY * glMultiTexCoord4iARBPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q );
typedef void (APIENTRY * glMultiTexCoord4ivARBPROC) (GLenum target, const GLint *v );
typedef void (APIENTRY * glMultiTexCoord4sARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q );
typedef void (APIENTRY * glMultiTexCoord4svARBPROC) (GLenum target, const GLshort *v );

extern glActiveTextureARBPROC glActiveTextureARB;
extern glClientActiveTextureARBPROC glClientActiveTextureARB;
extern glMultiTexCoord1dARBPROC glMultiTexCoord1dARB;
extern glMultiTexCoord1dvARBPROC glMultiTexCoord1dvARB;
extern glMultiTexCoord1fARBPROC glMultiTexCoord1fARB;
extern glMultiTexCoord1fvARBPROC glMultiTexCoord1fvARB;
extern glMultiTexCoord1iARBPROC glMultiTexCoord1iARB;
extern glMultiTexCoord1ivARBPROC glMultiTexCoord1ivARB;
extern glMultiTexCoord1sARBPROC glMultiTexCoord1sARB;
extern glMultiTexCoord1svARBPROC glMultiTexCoord1svARB;
extern glMultiTexCoord2dARBPROC glMultiTexCoord2dARB;
extern glMultiTexCoord2dvARBPROC glMultiTexCoord2dvARB;
extern glMultiTexCoord2fARBPROC glMultiTexCoord2fARB;
extern glMultiTexCoord2fvARBPROC glMultiTexCoord2fvARB;
extern glMultiTexCoord2iARBPROC glMultiTexCoord2iARB;
extern glMultiTexCoord2ivARBPROC glMultiTexCoord2ivARB;
extern glMultiTexCoord2sARBPROC glMultiTexCoord2sARB;
extern glMultiTexCoord2svARBPROC glMultiTexCoord2svARB;
extern glMultiTexCoord3dARBPROC glMultiTexCoord3dARB;
extern glMultiTexCoord3dvARBPROC glMultiTexCoord3dvARB;
extern glMultiTexCoord3fARBPROC glMultiTexCoord3fARB;
extern glMultiTexCoord3fvARBPROC glMultiTexCoord3fvARB;
extern glMultiTexCoord3iARBPROC glMultiTexCoord3iARB;
extern glMultiTexCoord3ivARBPROC glMultiTexCoord3ivARB;
extern glMultiTexCoord3sARBPROC glMultiTexCoord3sARB;
extern glMultiTexCoord3svARBPROC glMultiTexCoord3svARB;
extern glMultiTexCoord4dARBPROC glMultiTexCoord4dARB;
extern glMultiTexCoord4dvARBPROC glMultiTexCoord4dvARB;
extern glMultiTexCoord4fARBPROC glMultiTexCoord4fARB;
extern glMultiTexCoord4fvARBPROC glMultiTexCoord4fvARB;
extern glMultiTexCoord4iARBPROC glMultiTexCoord4iARB;
extern glMultiTexCoord4ivARBPROC glMultiTexCoord4ivARB;
extern glMultiTexCoord4sARBPROC glMultiTexCoord4sARB;
extern glMultiTexCoord4svARBPROC glMultiTexCoord4svARB;

#endif /* GL_ARB_multitexture */



#ifndef GL_EXT_texture_filter_anisotropic
#define GL_EXT_texture_filter_anisotropic

#define GL_TEXTURE_MAX_ANISOTROPY_EXT          0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT      0x84FF

#endif



#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc 1

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT                         0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT                        0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT                        0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT                        0x83F3

#endif /* GL_EXT_texture_compression_s3tc */



/* GL_ARB_texture_cube_map */

#ifndef GL_ARB_texture_cube_map
#define GL_ARB_texture_cube_map 1

#define GL_NORMAL_MAP_ARB                                       0x8511
#define GL_REFLECTION_MAP_ARB                                   0x8512
#define GL_TEXTURE_CUBE_MAP_ARB                                 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_ARB                         0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB                      0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB                      0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB                      0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB                      0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB                      0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB                      0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB                           0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB                        0x851C

#endif /* GL_ARB_texture_cube_map */



#ifndef GL_ARB_vertex_program
#define GL_ARB_vertex_program 1
#define OVIS_IMPLEMENT_GL_ARB_VERTEX_PROGRAM 1

typedef void (APIENTRY * glVertexAttrib1sARBPROC) (GLuint index, GLshort x);
typedef void (APIENTRY * glVertexAttrib1fARBPROC) (GLuint index, GLfloat x);
typedef void (APIENTRY * glVertexAttrib1dARBPROC) (GLuint index, GLdouble x);
typedef void (APIENTRY * glVertexAttrib2sARBPROC) (GLuint index, GLshort x, GLshort y);
typedef void (APIENTRY * glVertexAttrib2fARBPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (APIENTRY * glVertexAttrib2dARBPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (APIENTRY * glVertexAttrib3sARBPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (APIENTRY * glVertexAttrib3fARBPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY * glVertexAttrib3dARBPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRY * glVertexAttrib4sARBPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (APIENTRY * glVertexAttrib4fARBPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * glVertexAttrib4dARBPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRY * glVertexAttrib4NubARBPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (APIENTRY * glVertexAttrib1svARBPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib1fvARBPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib1dvARBPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib2svARBPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib2fvARBPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib2dvARBPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib3svARBPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib3fvARBPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib3dvARBPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib4bvARBPROC) (GLuint index, const GLbyte *v);
typedef void (APIENTRY * glVertexAttrib4svARBPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib4ivARBPROC) (GLuint index, const GLint *v);
typedef void (APIENTRY * glVertexAttrib4ubvARBPROC) (GLuint index, const GLubyte *v);
typedef void (APIENTRY * glVertexAttrib4usvARBPROC) (GLuint index, const GLushort *v);
typedef void (APIENTRY * glVertexAttrib4uivARBPROC) (GLuint index, const GLuint *v);
typedef void (APIENTRY * glVertexAttrib4fvARBPROC) (GLuint index, const GLfloat *v);
typedef void (APIENTRY * glVertexAttrib4dvARBPROC) (GLuint index, const GLdouble *v);
typedef void (APIENTRY * glVertexAttrib4NbvARBPROC) (GLuint index, const GLbyte *v);
typedef void (APIENTRY * glVertexAttrib4NsvARBPROC) (GLuint index, const GLshort *v);
typedef void (APIENTRY * glVertexAttrib4NivARBPROC) (GLuint index, const GLint *v);
typedef void (APIENTRY * glVertexAttrib4NubvARBPROC) (GLuint index, const GLubyte *v);
typedef void (APIENTRY * glVertexAttrib4NusvARBPROC) (GLuint index, const GLushort *v);
typedef void (APIENTRY * glVertexAttrib4NuivARBPROC) (GLuint index, const GLuint *v);
typedef void (APIENTRY * glVertexAttribPointerARBPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
typedef void (APIENTRY * glEnableVertexAttribArrayARBPROC) (GLuint index);
typedef void (APIENTRY * glDisableVertexAttribArrayARBPROC) (GLuint index);
typedef void (APIENTRY * glProgramStringARBPROC) (GLenum target, GLenum format, GLsizei len, const GLvoid *string); 
typedef void (APIENTRY * glBindProgramARBPROC) (GLenum target, GLuint program);
typedef void (APIENTRY * glDeleteProgramsARBPROC) (GLsizei n, const GLuint *programs);
typedef void (APIENTRY * glGenProgramsARBPROC) (GLsizei n, GLuint *programs);
typedef void (APIENTRY * glProgramEnvParameter4dARBPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRY * glProgramEnvParameter4dvARBPROC) (GLenum target, GLuint index, const GLdouble *params);
typedef void (APIENTRY * glProgramEnvParameter4fARBPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * glProgramEnvParameter4fvARBPROC) (GLenum target, GLuint index, const GLfloat *params);
typedef void (APIENTRY * glProgramLocalParameter4dARBPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (APIENTRY * glProgramLocalParameter4dvARBPROC) (GLenum target, GLuint index, const GLdouble *params);
typedef void (APIENTRY * glProgramLocalParameter4fARBPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRY * glProgramLocalParameter4fvARBPROC) (GLenum target, GLuint index, const GLfloat *params);
typedef void (APIENTRY * glGetProgramEnvParameterdvARBPROC) (GLenum target, GLuint index, GLdouble *params);
typedef void (APIENTRY * glGetProgramEnvParameterfvARBPROC) (GLenum target, GLuint index, GLfloat *params);
typedef void (APIENTRY * glGetProgramLocalParameterdvARBPROC) (GLenum target, GLuint index, GLdouble *params);
typedef void (APIENTRY * glGetProgramLocalParameterfvARBPROC) (GLenum target, GLuint index, GLfloat *params);
typedef void (APIENTRY * glGetProgramivARBPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetProgramStringARBPROC) (GLenum target, GLenum pname, GLvoid *string);
typedef void (APIENTRY * glGetVertexAttribdvARBPROC) (GLuint index, GLenum pname, GLdouble *params);
typedef void (APIENTRY * glGetVertexAttribfvARBPROC) (GLuint index, GLenum pname, GLfloat *params);
typedef void (APIENTRY * glGetVertexAttribivARBPROC) (GLuint index, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetVertexAttribPointervARBPROC) (GLuint index, GLenum pname, GLvoid **pointer);
typedef GLboolean (APIENTRY * glIsProgramARBPROC) (GLuint program);

extern glVertexAttrib1sARBPROC glVertexAttrib1sARB;
extern glVertexAttrib1fARBPROC glVertexAttrib1fARB;
extern glVertexAttrib1dARBPROC glVertexAttrib1dARB;
extern glVertexAttrib2sARBPROC glVertexAttrib2sARB;
extern glVertexAttrib2fARBPROC glVertexAttrib2fARB;
extern glVertexAttrib2dARBPROC glVertexAttrib2dARB;
extern glVertexAttrib3sARBPROC glVertexAttrib3sARB;
extern glVertexAttrib3fARBPROC glVertexAttrib3fARB;
extern glVertexAttrib3dARBPROC glVertexAttrib3dARB;
extern glVertexAttrib4sARBPROC glVertexAttrib4sARB;
extern glVertexAttrib4fARBPROC glVertexAttrib4fARB;
extern glVertexAttrib4dARBPROC glVertexAttrib4dARB;
extern glVertexAttrib4NubARBPROC glVertexAttrib4NubARB;
extern glVertexAttrib1svARBPROC glVertexAttrib1svARB;
extern glVertexAttrib1fvARBPROC glVertexAttrib1fvARB;
extern glVertexAttrib1dvARBPROC glVertexAttrib1dvARB;
extern glVertexAttrib2svARBPROC glVertexAttrib2svARB;
extern glVertexAttrib2fvARBPROC glVertexAttrib2fvARB;
extern glVertexAttrib2dvARBPROC glVertexAttrib2dvARB;
extern glVertexAttrib3svARBPROC glVertexAttrib3svARB;
extern glVertexAttrib3fvARBPROC glVertexAttrib3fvARB;
extern glVertexAttrib3dvARBPROC glVertexAttrib3dvARB;
extern glVertexAttrib4bvARBPROC glVertexAttrib4bvARB;
extern glVertexAttrib4svARBPROC glVertexAttrib4svARB;
extern glVertexAttrib4ivARBPROC glVertexAttrib4ivARB;
extern glVertexAttrib4ubvARBPROC glVertexAttrib4ubvARB;
extern glVertexAttrib4usvARBPROC glVertexAttrib4usvARB;
extern glVertexAttrib4uivARBPROC glVertexAttrib4uivARB;
extern glVertexAttrib4fvARBPROC glVertexAttrib4fvARB;
extern glVertexAttrib4dvARBPROC glVertexAttrib4dvARB;
extern glVertexAttrib4NbvARBPROC glVertexAttrib4NbvARB;
extern glVertexAttrib4NsvARBPROC glVertexAttrib4NsvARB;
extern glVertexAttrib4NivARBPROC glVertexAttrib4NivARB;
extern glVertexAttrib4NubvARBPROC glVertexAttrib4NubvARB;
extern glVertexAttrib4NusvARBPROC glVertexAttrib4NusvARB;
extern glVertexAttrib4NuivARBPROC glVertexAttrib4NuivARB;
extern glVertexAttribPointerARBPROC glVertexAttribPointerARB;
extern glEnableVertexAttribArrayARBPROC glEnableVertexAttribArrayARB;
extern glDisableVertexAttribArrayARBPROC glDisableVertexAttribArrayARB;
extern glProgramStringARBPROC glProgramStringARB;
extern glBindProgramARBPROC glBindProgramARB;
extern glDeleteProgramsARBPROC glDeleteProgramsARB;
extern glGenProgramsARBPROC glGenProgramsARB;
extern glProgramEnvParameter4dARBPROC glProgramEnvParameter4dARB;
extern glProgramEnvParameter4dvARBPROC glProgramEnvParameter4dvARB;
extern glProgramEnvParameter4fARBPROC glProgramEnvParameter4fARB;
extern glProgramEnvParameter4fvARBPROC glProgramEnvParameter4fvARB;
extern glProgramLocalParameter4dARBPROC glProgramLocalParameter4dARB;
extern glProgramLocalParameter4dvARBPROC glProgramLocalParameter4dvARB;
extern glProgramLocalParameter4fARBPROC glProgramLocalParameter4fARB;
extern glProgramLocalParameter4fvARBPROC glProgramLocalParameter4fvARB;
extern glGetProgramEnvParameterdvARBPROC glGetProgramEnvParameterdvARB;
extern glGetProgramEnvParameterfvARBPROC glGetProgramEnvParameterfvARB;
extern glGetProgramLocalParameterdvARBPROC glGetProgramLocalParameterdvARB;
extern glGetProgramLocalParameterfvARBPROC glGetProgramLocalParameterfvARB;
extern glGetProgramivARBPROC glGetProgramivARB;
extern glGetProgramStringARBPROC glGetProgramStringARB;
extern glGetVertexAttribdvARBPROC glGetVertexAttribdvARB;
extern glGetVertexAttribfvARBPROC glGetVertexAttribfvARB;
extern glGetVertexAttribivARBPROC glGetVertexAttribivARB;
extern glGetVertexAttribPointervARBPROC glGetVertexAttribPointervARB;
extern glIsProgramARBPROC glIsProgramARB;

#define GL_VERTEX_PROGRAM_ARB                                   0x8620
#define GL_VERTEX_PROGRAM_POINT_SIZE_ARB                        0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_ARB                          0x8643
#define GL_COLOR_SUM_ARB                                        0x8458
#define GL_PROGRAM_FORMAT_ASCII_ARB                             0x8875
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB                      0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB                         0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB                       0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB                         0x8625
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB                   0x886A
#define GL_CURRENT_VERTEX_ATTRIB_ARB                            0x8626
#define GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB                      0x8645
#define GL_PROGRAM_LENGTH_ARB                                   0x8627
#define GL_PROGRAM_FORMAT_ARB                                   0x8876
#define GL_PROGRAM_BINDING_ARB                                  0x8677
#define GL_PROGRAM_INSTRUCTIONS_ARB                             0x88A0
#define GL_MAX_PROGRAM_INSTRUCTIONS_ARB                         0x88A1
#define GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB                      0x88A2
#define GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB                  0x88A3
#define GL_PROGRAM_TEMPORARIES_ARB                              0x88A4
#define GL_MAX_PROGRAM_TEMPORARIES_ARB                          0x88A5
#define GL_PROGRAM_NATIVE_TEMPORARIES_ARB                       0x88A6
#define GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB                   0x88A7
#define GL_PROGRAM_PARAMETERS_ARB                               0x88A8
#define GL_MAX_PROGRAM_PARAMETERS_ARB                           0x88A9
#define GL_PROGRAM_NATIVE_PARAMETERS_ARB                        0x88AA
#define GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB                    0x88AB
#define GL_PROGRAM_ATTRIBS_ARB                                  0x88AC
#define GL_MAX_PROGRAM_ATTRIBS_ARB                              0x88AD
#define GL_PROGRAM_NATIVE_ATTRIBS_ARB                           0x88AE
#define GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB                       0x88AF
#define GL_PROGRAM_ADDRESS_REGISTERS_ARB                        0x88B0
#define GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB                    0x88B1
#define GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB                 0x88B2
#define GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB             0x88B3
#define GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB                     0x88B4
#define GL_MAX_PROGRAM_ENV_PARAMETERS_ARB                       0x88B5
#define GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB                      0x88B6
#define GL_PROGRAM_STRING_ARB                                   0x8628
#define GL_PROGRAM_ERROR_POSITION_ARB                           0x864B
#define GL_CURRENT_MATRIX_ARB                                   0x8641
#define GL_TRANSPOSE_CURRENT_MATRIX_ARB                         0x88B7
#define GL_CURRENT_MATRIX_STACK_DEPTH_ARB                       0x8640
#define GL_MAX_VERTEX_ATTRIBS_ARB                               0x8869
#define GL_MAX_PROGRAM_MATRICES_ARB                             0x862F
#define GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB                   0x862E
#define GL_PROGRAM_ERROR_STRING_ARB                             0x8874
#define GL_MATRIX0_ARB                                          0x88C0
#define GL_MATRIX1_ARB                                          0x88C1
#define GL_MATRIX2_ARB                                          0x88C2
#define GL_MATRIX3_ARB                                          0x88C3
#define GL_MATRIX4_ARB                                          0x88C4
#define GL_MATRIX5_ARB                                          0x88C5
#define GL_MATRIX6_ARB                                          0x88C6
#define GL_MATRIX7_ARB                                          0x88C7
#define GL_MATRIX8_ARB                                          0x88C8
#define GL_MATRIX9_ARB                                          0x88C9
#define GL_MATRIX10_ARB                                         0x88CA
#define GL_MATRIX11_ARB                                         0x88CB
#define GL_MATRIX12_ARB                                         0x88CC
#define GL_MATRIX13_ARB                                         0x88CD
#define GL_MATRIX14_ARB                                         0x88CE
#define GL_MATRIX15_ARB                                         0x88CF
#define GL_MATRIX16_ARB                                         0x88D0
#define GL_MATRIX17_ARB                                         0x88D1
#define GL_MATRIX18_ARB                                         0x88D2
#define GL_MATRIX19_ARB                                         0x88D3
#define GL_MATRIX20_ARB                                         0x88D4
#define GL_MATRIX21_ARB                                         0x88D5
#define GL_MATRIX22_ARB                                         0x88D6
#define GL_MATRIX23_ARB                                         0x88D7
#define GL_MATRIX24_ARB                                         0x88D8
#define GL_MATRIX25_ARB                                         0x88D9
#define GL_MATRIX26_ARB                                         0x88DA
#define GL_MATRIX27_ARB                                         0x88DB
#define GL_MATRIX28_ARB                                         0x88DC
#define GL_MATRIX29_ARB                                         0x88DD
#define GL_MATRIX30_ARB                                         0x88DE
#define GL_MATRIX31_ARB                                         0x88DF

#endif /* GL_ARB_vertex_program */




/* ARB_vertex_buffer_object */

#ifndef GL_ARB_vertex_buffer_object
#define GL_ARB_vertex_buffer_object 1
#define OVIS_IMPLEMENT_GL_ARB_VERTEX_BUFFER_OBJECT 1

typedef int GLintptrARB;
typedef int GLsizeiptrARB;

#define GL_ARRAY_BUFFER_ARB                                     0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB                             0x8893
#define GL_ARRAY_BUFFER_BINDING_ARB                             0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB                     0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB                      0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB                      0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB                       0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB                       0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB               0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB                   0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB             0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB              0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB                      0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB               0x889F
#define GL_STREAM_DRAW_ARB                                      0x88E0
#define GL_STREAM_READ_ARB                                      0x88E1
#define GL_STREAM_COPY_ARB                                      0x88E2
#define GL_STATIC_DRAW_ARB                                      0x88E4
#define GL_STATIC_READ_ARB                                      0x88E5
#define GL_STATIC_COPY_ARB                                      0x88E6
#define GL_DYNAMIC_DRAW_ARB                                     0x88E8
#define GL_DYNAMIC_READ_ARB                                     0x88E9
#define GL_DYNAMIC_COPY_ARB                                     0x88EA
#define GL_READ_ONLY_ARB                                        0x88B8
#define GL_WRITE_ONLY_ARB                                       0x88B9
#define GL_READ_WRITE_ARB                                       0x88BA
#define GL_BUFFER_SIZE_ARB                                      0x8764
#define GL_BUFFER_USAGE_ARB                                     0x8765
#define GL_BUFFER_ACCESS_ARB                                    0x88BB
#define GL_BUFFER_MAPPED_ARB                                    0x88BC
#define GL_BUFFER_MAP_POINTER_ARB                               0x88BD

typedef void (APIENTRY * glBindBufferARBPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRY * glDeleteBuffersARBPROC) (GLsizei n, const GLuint *buffers);
typedef void (APIENTRY * glGenBuffersARBPROC) (GLsizei n, GLuint *buffers);
typedef GLboolean (APIENTRY * glIsBufferARBPROC) (GLuint buffer);
typedef void (APIENTRY * glBufferDataARBPROC) (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
typedef void (APIENTRY * glBufferSubDataARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
typedef void (APIENTRY * glGetBufferSubDataARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid *data);
typedef void* (APIENTRY * glMapBufferARBPROC) (GLenum target, GLenum access);
typedef GLboolean (APIENTRY * glUnmapBufferARBPROC) (GLenum target);
typedef void (APIENTRY * glGetBufferParameterivARBPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetBufferPointervARBPROC) (GLenum target, GLenum pname, GLvoid **params);

extern glBindBufferARBPROC glBindBufferARB;
extern glDeleteBuffersARBPROC glDeleteBuffersARB;
extern glGenBuffersARBPROC glGenBuffersARB;
extern glIsBufferARBPROC glIsBufferARB;
extern glBufferDataARBPROC glBufferDataARB;
extern glBufferSubDataARBPROC glBufferSubDataARB;
extern glGetBufferSubDataARBPROC glGetBufferSubDataARB;
extern glMapBufferARBPROC glMapBufferARB;
extern glUnmapBufferARBPROC glUnmapBufferARB;
extern glGetBufferParameterivARBPROC glGetBufferParameterivARB;
extern glGetBufferPointervARBPROC glGetBufferPointervARB;

#endif /* GL_ARB_vertex_buffer_object */



#ifndef GL_ARB_occlusion_query
#define GL_ARB_occlusion_query 1
#define OVIS_IMPLEMENT_GL_ARB_OCCLUSION_QUERY 1

#define GL_SAMPLES_PASSED_ARB									0x8914
#define GL_QUERY_COUNTER_BITS_ARB								0x8864
#define GL_CURRENT_QUERY_ARB									0x8865
#define GL_QUERY_RESULT_ARB										0x8866
#define GL_QUERY_RESULT_AVAILABLE_ARB							0x8867

typedef void (APIENTRY * glGenQueriesARBPROC) (GLsizei n, GLuint *ids);
typedef void (APIENTRY * glDeleteQueriesARBPROC) (GLsizei n, const GLuint *ids);
typedef GLboolean (APIENTRY * glIsQueryARBPROC) (GLuint id);
typedef void (APIENTRY * glBeginQueryARBPROC) (GLenum target, GLuint id);
typedef void (APIENTRY * glEndQueryARBPROC) (GLenum target);
typedef void (APIENTRY * glGetQueryivARBPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetQueryObjectivARBPROC) (GLuint id, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetQueryObjectuivARBPROC) (GLuint id, GLenum pname, GLuint *params);

extern glGenQueriesARBPROC glGenQueriesARB;
extern glDeleteQueriesARBPROC glDeleteQueriesARB;
extern glIsQueryARBPROC glIsQueryARB;
extern glBeginQueryARBPROC glBeginQueryARB;
extern glEndQueryARBPROC glEndQueryARB;
extern glGetQueryivARBPROC glGetQueryivARB;
extern glGetQueryObjectivARBPROC glGetQueryObjectivARB;
extern glGetQueryObjectuivARBPROC glGetQueryObjectuivARB;

#endif /* GL_ARB_occlusion_query */




#ifndef GL_NV_texture_rectangle
#define GL_NV_texture_rectangle 1

#define GL_TEXTURE_RECTANGLE_NV                                 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_NV                         0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_NV                           0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_NV                        0x84F8

#endif /* GL_NV_texture_recrangle */



#ifndef GL_EXT_texture_rectangle
#define GL_EXT_texture_rectangle 1

#define GL_TEXTURE_RECTANGLE_EXT                                0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_EXT                        0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_EXT                          0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_EXT                       0x84F8

#endif /* GL_EXT_texture_rectangle */



#ifndef GL_HP_occlusion_test
#define GL_HP_occlusion_test 1

#define GL_OCCLUSION_TEST_HP                                    0x8165
#define GL_OCCLUSION_TEST_RESULT_HP                             0x8166

#endif /* GL_HP_occlusion_test */



#ifndef GL_NV_occlusion_query
#define GL_NV_occlusion_query 1
#define OVIS_IMPLEMENT_GL_NV_OCCLUSION_QUERY 1

#define GL_PIXEL_COUNTER_BITS_NV                                0x8864
#define GL_CURRENT_OCCLUSION_QUERY_ID_NV                        0x8865
#define GL_PIXEL_COUNT_NV                                       0x8866
#define GL_PIXEL_COUNT_AVAILABLE_NV                             0x8867

typedef void (APIENTRY * glGenOcclusionQueriesNVPROC) (GLsizei n, GLuint *ids);
typedef void (APIENTRY * glDeleteOcclusionQueriesNVPROC) (GLsizei n, const GLuint *ids);
typedef GLboolean (APIENTRY * glIsOcclusionQueryNVPROC) (GLuint id);
typedef void (APIENTRY * glBeginOcclusionQueryNVPROC) (GLuint id);
typedef void (APIENTRY * glEndOcclusionQueryNVPROC) (void);
typedef void (APIENTRY * glGetOcclusionQueryivNVPROC) (GLuint id, GLenum pname, GLint *params);
typedef void (APIENTRY * glGetOcclusionQueryuivNVPROC) (GLuint id, GLenum pname, GLuint *params);

extern glGenOcclusionQueriesNVPROC glGenOcclusionQueriesNV;
extern glDeleteOcclusionQueriesNVPROC glDeleteOcclusionQueriesNV;
extern glIsOcclusionQueryNVPROC glIsOcclusionQueryNV;
extern glBeginOcclusionQueryNVPROC glBeginOcclusionQueryNV;
extern glEndOcclusionQueryNVPROC glEndOcclusionQueryNV;
extern glGetOcclusionQueryivNVPROC glGetOcclusionQueryivNV;
extern glGetOcclusionQueryuivNVPROC glGetOcclusionQueryuivNV;

#endif /* GL_NV_occlusion_query */




#ifndef GL_ATI_texture_float
#define GL_ATI_texture_float 1

#define GL_RGBA_FLOAT32_ATI				0x8814
#define GL_RGB_FLOAT32_ATI				0x8815
#define GL_ALPHA_FLOAT32_ATI			0x8816
#define GL_INTENSITY_FLOAT32_ATI		0x8817
#define GL_LUMINANCE_FLOAT32_ATI		0x8818
#define GL_LUMINANCE_ALPHA_FLOAT32_ATI	0x8819
#define GL_RGBA_FLOAT16_ATI				0x881A
#define GL_RGB_FLOAT16_ATI				0x881B
#define GL_ALPHA_FLOAT16_ATI			0x881C
#define GL_INTENSITY_FLOAT16_ATI		0x881D
#define GL_LUMINANCE_FLOAT16_ATI		0x881E
#define GL_LUMINANCE_ALPHA_FLOAT16_ATI	0x881F

#endif /* GL_ATI_texture_float */





#ifndef GL_EXT_texture_filter_anisotropic
#define GL_EXT_texture_filter_anisotropic 1

#define GL_TEXTURE_MAX_ANISOTROPY_EXT		0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT	0x84FF

#endif /* GL_EXT_texture_filter_anisotropic */



#ifndef GL_ARB_depth_texture
#define GL_ARB_depth_texture 1

#define GL_DEPTH_COMPONENT16_ARB	0x81A5
#define GL_DEPTH_COMPONENT24_ARB	0x81A6
#define GL_DEPTH_COMPONENT32_ARB	0x81A7
#define GL_TEXTURE_DEPTH_SIZE_ARB	0x884A
#define GL_DEPTH_TEXTURE_MODE_ARB	0x884B

#endif /* GL_ARB_depth_texture */





#ifndef GL_ARB_shadow
#define GL_ARB_shadow 1

#define GL_TEXTURE_COMPARE_MODE_ARB	0x884C
#define GL_TEXTURE_COMPARE_FUNC_ARB	0x884D
#define GL_COMPARE_R_TO_TEXTURE_ARB	0x884E

#endif /* GL_ARB_shadow */





#ifndef GL_ARB_shadow_ambient
#define GL_ARB_shadow_ambient 1

#define GL_TEXTURE_COMPARE_FAIL_VALUE_ARB  0x80BF

#endif /* GL_ARB_shadow_ambient */




#endif /* __OVIS_OPENGLDRV_IONGL_H */
