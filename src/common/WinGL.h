/*
 * WinGL.h
 *
 *  Created on: May 9, 2011
 *      Author: bmonkey
 */

#ifndef WINGL_H_
#define WINGL_H_
  inline void initWinGL()  {
// Load OpenGL 3.0 functions
glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)glfGetProcAddress("glBindBufferBase");
glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)glfGetProcAddress("glBindFragDataLocation");
glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glfGetProcAddress("glGenerateMipmap");
glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glfGetProcAddress("glDeleteVertexArrays");
//    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glfGetProcAddress("glGenVertexArrays");
glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfGetProcAddress("glBindVertexArray");
glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)glfGetProcAddress("glGenFramebuffers");
glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)glfGetProcAddress("glBindFramebuffer");
glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC)glfGetProcAddress("glFramebufferTextureLayer");
glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)glfGetProcAddress("glFramebufferTexture2D");
glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)glfGetProcAddress("glCheckFramebufferStatus");
glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)glfGetProcAddress("glDeleteFramebuffers");
glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)glfGetProcAddress("glMapBufferRange");
glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)glfGetProcAddress("glFlushMappedBufferRange");
glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)glfGetProcAddress("glGenRenderbuffers");
glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)glfGetProcAddress("glBindRenderbuffer");
glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)glfGetProcAddress("glRenderbufferStorage");
glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)glfGetProcAddress("glFramebufferRenderbuffer");
glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)glfGetProcAddress("glBlitFramebuffer");
glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)glfGetProcAddress("glDeleteRenderbuffers");
glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)glfGetProcAddress("glRenderbufferStorageMultisample");
glColorMaski = (PFNGLCOLORMASKIPROC)glfGetProcAddress("glColorMaski");
glGetBooleani_v = (PFNGLGETBOOLEANI_VPROC)glfGetProcAddress("glGetBooleani_v");
glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC)glfGetProcAddress("glGetIntegeri_v");
glEnablei = (PFNGLENABLEIPROC)glfGetProcAddress("glEnablei");
glDisablei = (PFNGLDISABLEIPROC)glfGetProcAddress("glDisablei");

// Load OpenGL 3.1 functions
glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)glfGetProcAddress("glBindBufferBase");
glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)glfGetProcAddress("glBindBufferRange");
glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)glfGetProcAddress("glDrawArraysInstanced");
glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)glfGetProcAddress("glDrawElementsInstanced");
glTexBuffer = (PFNGLTEXBUFFERPROC)glfGetProcAddress("glTexBuffer");
glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)glfGetProcAddress("glPrimitiveRestartIndex");
glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)glfGetProcAddress("glGetUniformIndices");
glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)glfGetProcAddress("glGetActiveUniformsiv");
glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)glfGetProcAddress("glGetActiveUniformName");
glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)glfGetProcAddress("glGetUniformBlockIndex");
glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)glfGetProcAddress("glGetActiveUniformBlockiv");
glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC)glfGetProcAddress("glGetActiveUniformBlockName");
glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)glfGetProcAddress("glUniformBlockBinding");

// Load OpenGL 3.2 functions
glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)glfGetProcAddress("glDrawElementsBaseVertex");
glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)glfGetProcAddress("glDrawElementsInstancedBaseVertex");
glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)glfGetProcAddress("glDrawRangeElementsBaseVertex");
glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)glfGetProcAddress("glMultiDrawElementsBaseVertex");
glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC)glfGetProcAddress("glTexImage2DMultisample");
glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC)glfGetProcAddress("glTexImage3DMultisample");

// Load OpenGL 3.3 functions
glGenSamplers = (PFNGLGENSAMPLERSPROC)glfGetProcAddress("glGenSamplers");
glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)glfGetProcAddress("glDeleteSamplers");
glIsSampler = (PFNGLISSAMPLERPROC)glfGetProcAddress("glIsSampler");
glBindSampler = (PFNGLBINDSAMPLERPROC)glfGetProcAddress("glBindSampler");
glSamplerParameteri = (PFNGLSAMPLERPARAMETERIPROC)glfGetProcAddress("glSamplerParameteri");
glSamplerParameteriv = (PFNGLSAMPLERPARAMETERIVPROC)glfGetProcAddress("glSamplerParameteriv");
glSamplerParameterf = (PFNGLSAMPLERPARAMETERFPROC)glfGetProcAddress("glSamplerParameterf");
glSamplerParameterfv = (PFNGLSAMPLERPARAMETERFVPROC)glfGetProcAddress("glSamplerParameterfv");
glSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIVPROC)glfGetProcAddress("glSamplerParameterIiv");
glSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIVPROC)glfGetProcAddress("glSamplerParameterIuiv");
glGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIVPROC)glfGetProcAddress("glGetSamplerParameteriv");
glGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIVPROC)glfGetProcAddress("glGetSamplerParameterIiv");
glGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFVPROC)glfGetProcAddress("glGetSamplerParameterfv");
glGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIVPROC)glfGetProcAddress("glGetSamplerParameterIuiv");
glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORARBPROC)glfGetProcAddress("glVertexAttribDivisor");

// Load OpenGL 4.0 functions
glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC)glfGetProcAddress("glBindTransformFeedback");
glDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKSPROC)glfGetProcAddress("glDeleteTransformFeedbacks");
glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC)glfGetProcAddress("glGenTransformFeedbacks");
glIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACKPROC)glfGetProcAddress("glIsTransformFeedback");
glPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACKPROC)glfGetProcAddress("glPauseTransformFeedback");
glResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACKPROC)glfGetProcAddress("glResumeTransformFeedback");
glDrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACKPROC)glfGetProcAddress("glDrawTransformFeedback");
glPatchParameteri = (PFNGLPATCHPARAMETERIPROC)glfGetProcAddress("glPatchParameteri");
glPatchParameterfv = (PFNGLPATCHPARAMETERFVPROC)glfGetProcAddress("glPatchParameterfv");
glGetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAMEPROC)glfGetProcAddress("glGetActiveSubroutineName");
glGetActiveSubroutineUniformName = (PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC)glfGetProcAddress("glGetActiveSubroutineUniformName");
glGetActiveSubroutineUniformiv = (PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC)glfGetProcAddress("glGetActiveSubroutineUniformiv");
glGetProgramStageiv = (PFNGLGETPROGRAMSTAGEIVPROC)glfGetProcAddress("glGetProgramStageiv");
glGetSubroutineIndex = (PFNGLGETSUBROUTINEINDEXPROC)glfGetProcAddress("glGetSubroutineIndex");
glGetSubroutineUniformLocation = (PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC)glfGetProcAddress("glGetSubroutineUniformLocation");
glGetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIVPROC)glfGetProcAddress("glGetUniformSubroutineuiv");
glUniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIVPROC)glfGetProcAddress("glUniformSubroutinesuiv");
glDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECTPROC)glfGetProcAddress("glDrawArraysIndirect");
glDrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECTPROC)glfGetProcAddress("glDrawElementsIndirect");
glBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC)glfGetProcAddress("glBlendEquationSeparatei");
glBlendEquationi = (PFNGLBLENDEQUATIONIPROC)glfGetProcAddress("glBlendEquationi");
glBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC)glfGetProcAddress("glBlendFuncSeparatei");
glBlendFunci = (PFNGLBLENDFUNCIPROC)glfGetProcAddress("glBlendFunci");
glMinSampleShading = (PFNGLMINSAMPLESHADINGPROC)glfGetProcAddress("glMinSampleShading");
glSampleMaski = (PFNGLSAMPLEMASKIPROC)glfGetProcAddress("glSampleMaski");

glUniform1d = (PFNGLUNIFORM1DPROC)glfGetProcAddress("glUniform1d");
glUniform2d = (PFNGLUNIFORM2DPROC)glfGetProcAddress("glUniform2d");
glUniform3d = (PFNGLUNIFORM3DPROC)glfGetProcAddress("glUniform3d");
glUniform4d = (PFNGLUNIFORM4DPROC)glfGetProcAddress("glUniform4d");
glUniform1dv = (PFNGLUNIFORM1DVPROC)glfGetProcAddress("glUniform1dv");
glUniform2dv = (PFNGLUNIFORM2DVPROC)glfGetProcAddress("glUniform2dv");
glUniform3dv = (PFNGLUNIFORM3DVPROC)glfGetProcAddress("glUniform3dv");
glUniform4dv = (PFNGLUNIFORM4DVPROC)glfGetProcAddress("glUniform4dv");
glUniformMatrix2dv = (PFNGLUNIFORMMATRIX2DVPROC)glfGetProcAddress("glUniformMatrix2dv");
glUniformMatrix3dv = (PFNGLUNIFORMMATRIX3DVPROC)glfGetProcAddress("glUniformMatrix3dv");
glUniformMatrix4dv = (PFNGLUNIFORMMATRIX4DVPROC)glfGetProcAddress("glUniformMatrix4dv");
glUniformMatrix2x3dv = (PFNGLUNIFORMMATRIX2X3DVPROC)glfGetProcAddress("glUniformMatrix2x3dv");
glUniformMatrix2x4dv = (PFNGLUNIFORMMATRIX2X4DVPROC)glfGetProcAddress("glUniformMatrix2x4dv");
glUniformMatrix3x2dv = (PFNGLUNIFORMMATRIX3X2DVPROC)glfGetProcAddress("glUniformMatrix3x2dv");
glUniformMatrix3x4dv = (PFNGLUNIFORMMATRIX3X4DVPROC)glfGetProcAddress("glUniformMatrix3x4dv");
glUniformMatrix4x2dv = (PFNGLUNIFORMMATRIX4X2DVPROC)glfGetProcAddress("glUniformMatrix4x2dv");
glUniformMatrix4x3dv = (PFNGLUNIFORMMATRIX4X3DVPROC)glfGetProcAddress("glUniformMatrix4x3dv");

// Load OpenGL 4.1 functions
glVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTERPROC)glfGetProcAddress("glVertexAttribLPointer");

glProgramUniform1i = (PFNGLPROGRAMUNIFORM1IPROC)glfGetProcAddress("glProgramUniform1i");
glProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IVPROC)glfGetProcAddress("glProgramUniform1iv");
glProgramUniform2i = (PFNGLPROGRAMUNIFORM2IPROC)glfGetProcAddress("glProgramUniform2i");
glProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IVPROC)glfGetProcAddress("glProgramUniform2iv");
glProgramUniform3i = (PFNGLPROGRAMUNIFORM3IPROC)glfGetProcAddress("glProgramUniform3i");
glProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IVPROC)glfGetProcAddress("glProgramUniform3iv");
glProgramUniform4i = (PFNGLPROGRAMUNIFORM4IPROC)glfGetProcAddress("glProgramUniform4i");
glProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IVPROC)glfGetProcAddress("glProgramUniform4iv");

glProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UIPROC)glfGetProcAddress("glProgramUniform1ui");
glProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIVPROC)glfGetProcAddress("glProgramUniform1uiv");
glProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UIPROC)glfGetProcAddress("glProgramUniform2ui");
glProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIVPROC)glfGetProcAddress("glProgramUniform2uiv");
glProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UIPROC)glfGetProcAddress("glProgramUniform3ui");
glProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIVPROC)glfGetProcAddress("glProgramUniform3uiv");
glProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UIPROC)glfGetProcAddress("glProgramUniform4ui");
glProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIVPROC)glfGetProcAddress("glProgramUniform4uiv");

glProgramUniform1f = (PFNGLPROGRAMUNIFORM1FPROC)glfGetProcAddress("glProgramUniform1f");
glProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FVPROC)glfGetProcAddress("glProgramUniform1fv");
glProgramUniform2f = (PFNGLPROGRAMUNIFORM2FPROC)glfGetProcAddress("glProgramUniform2f");
glProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FVPROC)glfGetProcAddress("glProgramUniform2fv");
glProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC)glfGetProcAddress("glProgramUniform3f");
glProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FVPROC)glfGetProcAddress("glProgramUniform3fv");
glProgramUniform4f = (PFNGLPROGRAMUNIFORM4FPROC)glfGetProcAddress("glProgramUniform4f");
glProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FVPROC)glfGetProcAddress("glProgramUniform4fv");

glProgramUniform1d = (PFNGLPROGRAMUNIFORM1DPROC)glfGetProcAddress("glProgramUniform1d");
glProgramUniform2d = (PFNGLPROGRAMUNIFORM2DPROC)glfGetProcAddress("glProgramUniform2d");
glProgramUniform3d = (PFNGLPROGRAMUNIFORM3DPROC)glfGetProcAddress("glProgramUniform3d");
glProgramUniform4d = (PFNGLPROGRAMUNIFORM4DPROC)glfGetProcAddress("glProgramUniform4d");
glProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DVPROC)glfGetProcAddress("glProgramUniform1dv");
glProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DVPROC)glfGetProcAddress("glProgramUniform2dv");
glProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DVPROC)glfGetProcAddress("glProgramUniform3dv");
glProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DVPROC)glfGetProcAddress("glProgramUniform4dv");

glProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FVPROC)glfGetProcAddress("glProgramUniformMatrix2fv");
glProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FVPROC)glfGetProcAddress("glProgramUniformMatrix3fv");
glProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FVPROC)glfGetProcAddress("glProgramUniformMatrix4fv");
glProgramUniformMatrix2x3fv = (PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC)glfGetProcAddress("glProgramUniformMatrix2x3fv");
glProgramUniformMatrix2x4fv = (PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC)glfGetProcAddress("glProgramUniformMatrix2x4fv");
glProgramUniformMatrix3x2fv = (PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC)glfGetProcAddress("glProgramUniformMatrix3x2fv");
glProgramUniformMatrix3x4fv = (PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC)glfGetProcAddress("glProgramUniformMatrix3x4fv");
glProgramUniformMatrix4x2fv = (PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC)glfGetProcAddress("glProgramUniformMatrix4x2fv");
glProgramUniformMatrix4x3fv = (PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC)glfGetProcAddress("glProgramUniformMatrix4x3fv");

glProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DVPROC)glfGetProcAddress("glProgramUniformMatrix2dv");
glProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DVPROC)glfGetProcAddress("glProgramUniformMatrix3dv");
glProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DVPROC)glfGetProcAddress("glProgramUniformMatrix4dv");
glProgramUniformMatrix2x3dv = (PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC)glfGetProcAddress("glProgramUniformMatrix2x3dv");
glProgramUniformMatrix2x4dv = (PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC)glfGetProcAddress("glProgramUniformMatrix2x4dv");
glProgramUniformMatrix3x2dv = (PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC)glfGetProcAddress("glProgramUniformMatrix3x2dv");
glProgramUniformMatrix3x4dv = (PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC)glfGetProcAddress("glProgramUniformMatrix3x4dv");
glProgramUniformMatrix4x2dv = (PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC)glfGetProcAddress("glProgramUniformMatrix4x2dv");
glProgramUniformMatrix4x3dv = (PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC)glfGetProcAddress("glProgramUniformMatrix4x3dv");

glUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC)glfGetProcAddress("glUseProgramStages");
glActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC)glfGetProcAddress("glActiveShaderProgram");
glCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)glfGetProcAddress("glCreateShaderProgramv");
glBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC)glfGetProcAddress("glBindProgramPipeline");
glDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC)glfGetProcAddress("glDeleteProgramPipelines");
glGenProgramPipelines = (PFNGLGENPROGRAMPIPELINESPROC)glfGetProcAddress("glGenProgramPipelines");
glIsProgramPipeline = (PFNGLISPROGRAMPIPELINEPROC)glfGetProcAddress("glIsProgramPipeline");
glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)glfGetProcAddress("glProgramParameteri");
glGetProgramPipelineiv = (PFNGLGETPROGRAMPIPELINEIVPROC)glfGetProcAddress("glGetProgramPipelineiv");
glValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINEPROC)glfGetProcAddress("glValidateProgramPipeline");
glGetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOGPROC)glfGetProcAddress("glGetProgramPipelineInfoLog");

glViewportArrayv = (PFNGLVIEWPORTARRAYVPROC)glfGetProcAddress("glViewportArrayv");;
glViewportIndexedf = (PFNGLVIEWPORTINDEXEDFPROC)glfGetProcAddress("glViewportIndexedf");
glViewportIndexedfv = (PFNGLVIEWPORTINDEXEDFVPROC)glfGetProcAddress("glViewportIndexedfv");
glScissorArrayv = (PFNGLSCISSORARRAYVPROC)glfGetProcAddress("glScissorArrayv");
glScissorIndexed = (PFNGLSCISSORINDEXEDPROC)glfGetProcAddress("glScissorIndexed");
glScissorIndexedv = (PFNGLSCISSORINDEXEDVPROC)glfGetProcAddress("glScissorIndexedv");
glDepthRangeArrayv = (PFNGLDEPTHRANGEARRAYVPROC)glfGetProcAddress("glDepthRangeArrayv");
glDepthRangeIndexed = (PFNGLDEPTHRANGEINDEXEDPROC)glfGetProcAddress("glDepthRangeIndexed");
glGetFloati_v = (PFNGLGETFLOATI_VPROC)glfGetProcAddress("glGetFloati_v");
glGetDoublei_v = (PFNGLGETDOUBLEI_VPROC)glfGetProcAddress("glGetDoublei_v");

glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC)glfGetProcAddress("glGetProgramBinary");
glProgramBinary = (PFNGLPROGRAMBINARYPROC)glfGetProcAddress("glProgramBinary");

// Load GL_ARB_debug_output
glDebugMessageControlARB = (PFNGLDEBUGMESSAGECONTROLARBPROC) glfGetProcAddress("glDebugMessageControlARB");
glDebugMessageInsertARB = (PFNGLDEBUGMESSAGEINSERTARBPROC) glfGetProcAddress("glDebugMessageInsertARB");
glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC) glfGetProcAddress("glDebugMessageCallbackARB");
glGetDebugMessageLogARB = (PFNGLGETDEBUGMESSAGELOGARBPROC) glfGetProcAddress("glGetDebugMessageLogARB");

// Load GL_EXT_direct_state_access extension
glNamedBufferDataEXT = (PFNGLNAMEDBUFFERDATAEXTPROC)glfGetProcAddress("glNamedBufferDataEXT");
glTextureParameteriEXT = (PFNGLTEXTUREPARAMETERIEXTPROC)glfGetProcAddress("glTextureParameteriEXT");
glCompressedTextureImage2DEXT = (PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC)glfGetProcAddress("glCompressedTextureImage2DEXT");
glVertexArrayVertexAttribOffsetEXT = (PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC)glfGetProcAddress("glVertexArrayVertexAttribOffsetEXT");
glEnableVertexArrayAttribEXT = (PFNGLENABLEVERTEXARRAYATTRIBEXTPROC)glfGetProcAddress("glEnableVertexArrayAttribEXT");
glBindMultiTextureEXT = (PFNGLBINDMULTITEXTUREEXTPROC)glfGetProcAddress("glBindMultiTextureEXT");
glProgramUniformMatrix4fvEXT = (PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC)glfGetProcAddress("glProgramUniformMatrix4fvEXT");
glProgramUniform1iEXT = (PFNGLPROGRAMUNIFORM1IEXTPROC)glfGetProcAddress("glProgramUniform1iEXT");
glProgramUniform1dEXT = (PFNGLPROGRAMUNIFORM1DEXTPROC)glfGetProcAddress("glProgramUniform1dEXT");
glProgramUniform2dEXT = (PFNGLPROGRAMUNIFORM2DEXTPROC)glfGetProcAddress("glProgramUniform2dEXT");
glProgramUniform3dEXT = (PFNGLPROGRAMUNIFORM3DEXTPROC)glfGetProcAddress("glProgramUniform3dEXT");
glProgramUniform4dEXT = (PFNGLPROGRAMUNIFORM4DEXTPROC)glfGetProcAddress("glProgramUniform4dEXT");
glProgramUniform1dvEXT = (PFNGLPROGRAMUNIFORM1DVEXTPROC)glfGetProcAddress("glProgramUniform1dvEXT");
glProgramUniform2dvEXT = (PFNGLPROGRAMUNIFORM2DVEXTPROC)glfGetProcAddress("glProgramUniform2dvEXT");
glProgramUniform3dvEXT = (PFNGLPROGRAMUNIFORM3DVEXTPROC)glfGetProcAddress("glProgramUniform3dvEXT");
glProgramUniform4dvEXT = (PFNGLPROGRAMUNIFORM4DVEXTPROC)glfGetProcAddress("glProgramUniform4dvEXT");
glProgramUniformMatrix2dvEXT = (PFNGLPROGRAMUNIFORMMATRIX2DVEXTPROC)glfGetProcAddress("glProgramUniformMatrix2dvEXT");
glProgramUniformMatrix3dvEXT = (PFNGLPROGRAMUNIFORMMATRIX3DVEXTPROC)glfGetProcAddress("glProgramUniformMatrix3dvEXT");
glProgramUniformMatrix4dvEXT = (PFNGLPROGRAMUNIFORMMATRIX4DVEXTPROC)glfGetProcAddress("glProgramUniformMatrix4dvEXT");
glProgramUniformMatrix2x3dvEXT = (PFNGLPROGRAMUNIFORMMATRIX2X3DVEXTPROC)glfGetProcAddress("glProgramUniformMatrix2x3dvEXT");
glProgramUniformMatrix2x4dvEXT = (PFNGLPROGRAMUNIFORMMATRIX2X4DVEXTPROC)glfGetProcAddress("glProgramUniformMatrix2x4dvEXT");
glProgramUniformMatrix3x2dvEXT = (PFNGLPROGRAMUNIFORMMATRIX3X2DVEXTPROC)glfGetProcAddress("glProgramUniformMatrix3x2dvEXT");
glProgramUniformMatrix3x4dvEXT = (PFNGLPROGRAMUNIFORMMATRIX3X4DVEXTPROC)glfGetProcAddress("glProgramUniformMatrix3x4dvEXT");
glProgramUniformMatrix4x2dvEXT = (PFNGLPROGRAMUNIFORMMATRIX4X2DVEXTPROC)glfGetProcAddress("glProgramUniformMatrix4x2dvEXT");
glProgramUniformMatrix4x3dvEXT = (PFNGLPROGRAMUNIFORMMATRIX4X3DVEXTPROC)glfGetProcAddress("glProgramUniformMatrix4x3dvEXT");
glVertexArrayVertexAttribLOffsetEXT = (PFNGLVERTEXARRAYVERTEXATTRIBLOFFSETEXTPROC)glfGetProcAddress("glVertexArrayVertexAttribLOffsetEXT");
glEnableVertexArrayAttribEXT = (PFNGLENABLEVERTEXARRAYATTRIBEXTPROC)glfGetProcAddress("glEnableVertexArrayAttribEXT");

// Load GL_EXT_shader_image_load_store extension
glBindImageTextureEXT = (PFNGLBINDIMAGETEXTUREEXTPROC)glfGetProcAddress("glBindImageTextureEXT");
  }
#endif /* WINGL_H_ */
