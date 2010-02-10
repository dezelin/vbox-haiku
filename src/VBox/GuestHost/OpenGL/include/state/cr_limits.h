/* Copyright (c) 2001, Stanford University
 * All rights reserved.
 *
 * See the file LICENSE.txt for information on redistributing this software.
 */

#ifndef CR_LIMITS_H
#define CR_LIMITS_H

#include "chromium.h"
#include "cr_version.h"

#include <iprt/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif


/* glGetString strings */
#define CR_RENDERER "Chromium"
#define CR_VENDOR "Humper"


#define CR_MAX_TEXTURE_UNITS		16
#define CR_MAX_GENERAL_COMBINERS	8
#define CR_MAX_TEXTURE_SIZE		8192
#define CR_MAX_3D_TEXTURE_SIZE		512
#define CR_MAX_CUBE_TEXTURE_SIZE	4096
#define CR_MAX_RECTANGLE_TEXTURE_SIZE	4096
#define CR_MAX_TEXTURE_ANISOTROPY	8.0
#define CR_MAX_LIGHTS			8
#define CR_MAX_CLIP_PLANES		8
#define CR_MAX_PROJECTION_STACK_DEPTH	32
#define CR_MAX_MODELVIEW_STACK_DEPTH	32
#define CR_MAX_TEXTURE_STACK_DEPTH	10
#define CR_MAX_COLOR_STACK_DEPTH	2
#define CR_MAX_ATTRIB_STACK_DEPTH	16
#define CR_MAX_CLIENT_ATTRIB_STACK_DEPTH	16
#define CR_MAX_NAME_STACK_DEPTH		64
#define CR_MAX_ELEMENTS_INDICES		16384
#define CR_MAX_ELEMENTS_VERTICES	16384
#define CR_MAX_EVAL_ORDER		8
#define CR_MAX_LIST_NESTING		64
#define CR_MAX_PIXEL_MAP_TABLE		256
#define CR_MAX_VIEWPORT_DIM		16384
#define CR_SUBPIXEL_BITS		8
#define CR_ALIASED_POINT_SIZE_MIN	1.0
#define CR_ALIASED_POINT_SIZE_MAX	64.0
#define CR_SMOOTH_POINT_SIZE_MIN	1.0
#define CR_SMOOTH_POINT_SIZE_MAX	64.0
#define CR_POINT_SIZE_GRANULARITY	0.5
#define CR_ALIASED_LINE_WIDTH_MIN	1.0
#define CR_ALIASED_LINE_WIDTH_MAX	64.0
#define CR_SMOOTH_LINE_WIDTH_MIN	1.0
#define CR_SMOOTH_LINE_WIDTH_MAX	64.0
#define CR_LINE_WIDTH_GRANULARITY	0.5
#define CR_MAX_VERTEX_ATTRIBS           16
#define CR_MAX_TEXTURE_LOD_BIAS         8.0
#ifdef CR_NV_fragment_program
#define CR_MAX_TEXTURE_COORDS           2
#define CR_MAX_TEXTURE_IMAGE_UNITS      2
#define CR_MAX_FRAGMENT_LOCAL_PARAMS    64
#endif
#ifdef CR_NV_vertex_program
#define CR_MAX_PROGRAM_MATRICES           8
#define CR_MAX_PROGRAM_MATRIX_STACK_DEPTH 4
#endif
#ifdef CR_ARB_fragment_program
#define CR_MAX_FRAGMENT_PROGRAM_INSTRUCTIONS       72
#define CR_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMS       24
#define CR_MAX_FRAGMENT_PROGRAM_ENV_PARAMS         256
#define CR_MAX_FRAGMENT_PROGRAM_TEMPS              16
#define CR_MAX_FRAGMENT_PROGRAM_ATTRIBS            10
#define CR_MAX_FRAGMENT_PROGRAM_ADDRESS_REGS        1
#define CR_MAX_FRAGMENT_PROGRAM_ALU_INSTRUCTIONS   48
#define CR_MAX_FRAGMENT_PROGRAM_TEX_INSTRUCTIONS   24
#define CR_MAX_FRAGMENT_PROGRAM_TEX_INDIRECTIONS    4
#endif
#ifdef CR_ARB_vertex_program
#define CR_MAX_VERTEX_PROGRAM_INSTRUCTIONS 128
#define CR_MAX_VERTEX_PROGRAM_LOCAL_PARAMS  96
#define CR_MAX_VERTEX_PROGRAM_ENV_PARAMS   256 /* for GL_NV_vertex_program2 */
#define CR_MAX_VERTEX_PROGRAM_TEMPS         12
#define CR_MAX_VERTEX_PROGRAM_ATTRIBS       16
#define CR_MAX_VERTEX_PROGRAM_ADDRESS_REGS   1
#endif

#if defined(CR_ARB_vertex_program) || defined(CR_ARB_fragment_program)
/* These must be the max of the fragment and vertex program limits */
#define CR_MAX_PROGRAM_LOCAL_PARAMS  (CR_MAX_VERTEX_PROGRAM_LOCAL_PARAMS > CR_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMS ? CR_MAX_VERTEX_PROGRAM_LOCAL_PARAMS : CR_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMS)

#define CR_MAX_PROGRAM_ENV_PARAMS  (CR_MAX_VERTEX_PROGRAM_ENV_PARAMS > CR_MAX_FRAGMENT_PROGRAM_ENV_PARAMS ? CR_MAX_VERTEX_PROGRAM_ENV_PARAMS : CR_MAX_FRAGMENT_PROGRAM_ENV_PARAMS)
#endif


/* Just need these for autogenerated code in state_get.c, etc */
#if defined(CR_ARB_vertex_program) || defined(CR_NV_vertex_program)
#define CR_any_vertex_program 1
#endif

#if defined(CR_ARB_fragment_program) || defined(CR_NV_fragment_program)
#define CR_any_fragment_program 1
#endif

#if defined(CR_any_vertex_program) || defined(CR_any_fragment_program)
#define CR_any_program 1
#endif



/*
 * OpenGL's implementation-dependent values (not part of any attribute group).
 */
typedef struct {
	GLuint maxTextureUnits;
	GLuint maxTextureSize;
	GLuint max3DTextureSize;	/* OpenGL 1.2 */
#ifdef CR_ARB_texture_cube_map
	GLuint maxCubeMapTextureSize;
#endif
	GLuint maxLights;
	GLuint maxClipPlanes;
	GLuint maxProjectionStackDepth;
	GLuint maxModelviewStackDepth;
	GLuint maxTextureStackDepth;
	GLuint maxColorStackDepth;	/* OpenGL 1.2 */
	GLuint maxAttribStackDepth;
	GLuint maxClientAttribStackDepth;
	GLuint maxNameStackDepth;
	GLuint maxElementsIndices;
	GLuint maxElementsVertices;
	GLuint maxEvalOrder;
	GLuint maxListNesting;
	GLuint maxPixelMapTable;
	GLint maxViewportDims[2];
	GLuint subpixelBits;
	GLfloat aliasedPointSizeRange[2];
	GLfloat smoothPointSizeRange[2];
	GLfloat pointSizeGranularity;
	GLfloat aliasedLineWidthRange[2];
	GLfloat smoothLineWidthRange[2];
	GLfloat lineWidthGranularity;
#ifdef CR_EXT_texture_lod_bias
	GLfloat maxTextureLodBias;
#endif
#ifdef CR_EXT_texture_filter_anisotropic
	GLfloat maxTextureAnisotropy;
#endif
#ifdef CR_ARB_texture_compression
	GLuint numCompressedFormats;
	GLenum compressedFormats[10];
#endif
#ifdef CR_NV_register_combiners
	GLuint maxGeneralCombiners;
#endif
#ifdef CR_NV_texture_rectangle
	GLuint maxRectTextureSize;
#endif
#ifdef CR_NV_fragment_program
	GLuint maxTextureCoords;
	GLuint maxTextureImageUnits;
	/*GLuint maxFragmentProgramLocalParams;*/
#endif
#ifdef CR_NV_vertex_program
	GLuint maxProgramMatrixStackDepth;
	GLuint maxProgramMatrices;
#endif
#ifdef CR_ARB_fragment_program
	GLuint maxFragmentProgramInstructions;
	GLuint maxFragmentProgramLocalParams;
	GLuint maxFragmentProgramEnvParams;
	GLuint maxFragmentProgramTemps;
	GLuint maxFragmentProgramAttribs;
	GLuint maxFragmentProgramAddressRegs;
	GLuint maxFragmentProgramAluInstructions;
	GLuint maxFragmentProgramTexInstructions;
	GLuint maxFragmentProgramTexIndirections;
#endif
#ifdef CR_ARB_vertex_program
	GLuint maxVertexProgramInstructions;
	GLuint maxVertexProgramLocalParams;
	GLuint maxVertexProgramEnvParams;
	GLuint maxVertexProgramTemps;
	GLuint maxVertexProgramAttribs;
	GLuint maxVertexProgramAddressRegs;
#endif
	const GLubyte *extensions;

	/* Framebuffer/visual attributes */
	GLuint redBits, greenBits, blueBits, alphaBits;
	GLuint depthBits, stencilBits, indexBits;
	GLuint accumRedBits, accumGreenBits, accumBlueBits, accumAlphaBits;
	GLuint auxBuffers;
	GLboolean rgbaMode;
	GLboolean doubleBuffer;
	GLboolean stereo;
	GLuint sampleBuffers;
	GLuint samples;
	GLuint level;

} CRLimitsState;


/* Booleans to indicate which OpenGL extensions are supported at runtime.
 * XXX might merge this into the above structure someday.
 */
typedef struct {
	GLboolean ARB_depth_texture;
	GLboolean ARB_fragment_program;
	GLboolean ARB_imaging;
	GLboolean ARB_multisample;
	GLboolean ARB_multitexture;
	GLboolean ARB_occlusion_query;
	GLboolean ARB_point_parameters;
	GLboolean ARB_point_sprite;
	GLboolean ARB_shadow;
	GLboolean ARB_shadow_ambient;
	GLboolean ARB_texture_border_clamp; /* or SGIS_texture_border_clamp */
	GLboolean ARB_texture_compression;
	GLboolean ARB_texture_cube_map; /* or EXT_texture_cube_map */
	GLboolean ARB_texture_env_add; /* standard in OpenGL 1.3 */
	GLboolean ARB_texture_env_combine; /* standard in OpenGL 1.3 */
	GLboolean ARB_texture_env_crossbar; /* standard in OpenGL 1.4 */
	GLboolean ARB_texture_env_dot3; /* standard in OpenGL 1.3 */
	GLboolean ARB_texture_mirrored_repeat;
	GLboolean ARB_texture_non_power_of_two; /* standard in 1.5 */
	GLboolean ARB_transpose_matrix;
	GLboolean ARB_vertex_buffer_object;
	GLboolean ARB_vertex_program;
	GLboolean ARB_window_pos;
	GLboolean EXT_blend_color;
	GLboolean EXT_blend_logic_op;
	GLboolean EXT_blend_func_separate;
	GLboolean EXT_blend_minmax;
	GLboolean EXT_blend_subtract;
	GLboolean EXT_clip_volume_hint;
	GLboolean EXT_fog_coord;
	GLboolean EXT_multi_draw_arrays;
	GLboolean EXT_secondary_color;
	GLboolean EXT_separate_specular_color;
	GLboolean EXT_shadow_funcs;
	GLboolean EXT_stencil_wrap;
	GLboolean EXT_texture_edge_clamp; /* or SGIS_texture_edge_clamp */
	GLboolean EXT_texture_filter_anisotropic;
	GLboolean EXT_texture_lod_bias;
	GLboolean EXT_texture3D;
	GLboolean IBM_rasterpos_clip;
	GLboolean NV_fog_distance;
	GLboolean NV_fragment_program;
	GLboolean NV_register_combiners;
	GLboolean NV_register_combiners2;
	GLboolean NV_texgen_reflection;
	GLboolean NV_texture_rectangle;
	GLboolean NV_vertex_program;
	GLboolean NV_vertex_program1_1;
	GLboolean NV_vertex_program2;
	GLboolean SGIS_generate_mipmap;
    GLboolean EXT_texture_from_pixmap;

	/* derived from above */
	GLboolean any_vertex_program;    /* NV or ARB */
	GLboolean any_fragment_program;  /* NV or ARB */
	GLboolean any_program;  /* vertex or fragment */
} CRExtensionState;

extern DECLEXPORT(void) crStateLimitsInit(CRLimitsState *limits);
extern DECLEXPORT(void) crStateLimitsDestroy(CRLimitsState *limits);

extern DECLEXPORT(void) crStateLimitsPrint(const CRLimitsState *limits);

extern DECLEXPORT(GLfloat) crStateComputeVersion(float minVersion);

extern DECLEXPORT(GLubyte *) crStateMergeExtensions(GLuint n, const GLubyte **extensions);

extern DECLEXPORT(void) crStateExtensionsInit( CRLimitsState *limits, CRExtensionState *extensions );


#ifdef __cplusplus
}
#endif

#endif /* CR_STATE_FOG_H */
