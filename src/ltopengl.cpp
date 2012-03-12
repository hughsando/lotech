    #ifdef LTGLES1
#define GLEXT(f) f##OES
#define GL_EXT(f) f##_OES
#else
#define GLEXT(f) f##EXT
#define GL_EXT(f) f##_EXT
#endif

#ifndef GL_DEPTH_COMPONENT16_EXT
#define GL_DEPTH_COMPONENT16_EXT GL_DEPTH_COMPONENT16
#endif

#include "ltopengl.h"

ct_assert(sizeof(GLfloat) == 4);

static char state_str[1024];
static void set_state_str();

//#define LTGLTRACE
#ifdef LTGLTRACE
#define gltrace {set_state_str(); ltLog("%s:%4d %-30s %s", __FILE__, __LINE__, __func__, state_str);}
#else
#define gltrace
#endif

#define LTGLCHECK
#ifdef LTGLCHECK
#define check_for_errors if (glGetError() != GL_NO_ERROR) \
    {ltLog("OpenGL error at %s:%d %s", __FILE__, __LINE__, __func__); ltAbort();}
#else
#define check_for_errors
#endif

// State
static bool texturing;
static bool texture_coord_arrays;
static LTBlendMode blend_mode;
static LTTextureMode texture_mode;
static bool depth_test;
static bool depth_mask;
static bool dither;
static bool alpha_test;
static bool stencil_test;
static bool vertex_arrays;
static bool index_arrays;
static bool color_arrays;
static bool fog;
static LTtexid bound_texture;
static LTframebuf bound_framebuffer;
static LTvertbuf bound_vertbuffer;

void ltInitGLState() {
    glDisable(GL_TEXTURE_2D);
    texturing = false;
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    texture_coord_arrays = false;
    glDisable(GL_BLEND);
    blend_mode = LT_BLEND_MODE_OFF;
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    texture_mode = LT_TEXTURE_MODE_MODULATE;
    glDisable(GL_DEPTH_TEST);
    depth_test = false;
    glDepthMask(GL_TRUE);
    depth_mask = true;
    glDisable(GL_DITHER);
    dither = false;
    glDisable(GL_ALPHA_TEST);
    alpha_test = false;
    glDisable(GL_STENCIL_TEST);
    stencil_test = false;
    glDisableClientState(GL_VERTEX_ARRAY);
    vertex_arrays = false;
    glDisableClientState(GL_INDEX_ARRAY);
    index_arrays = false;
    glDisableClientState(GL_COLOR_ARRAY);
    color_arrays = false;
    glDisable(GL_FOG);
    fog = false;
    glBindTexture(GL_TEXTURE_2D, 0);
    bound_texture = 0;
    GLEXT(glBindFramebuffer)(GL_EXT(GL_FRAMEBUFFER), 0);
    bound_framebuffer = 0;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    bound_vertbuffer = 0;

    check_for_errors
    gltrace
}

void ltEnableTexturing() {
    gltrace
    if (!texturing) {
        glEnable(GL_TEXTURE_2D);
        check_for_errors
        texturing = true;
    }
    gltrace
}

void ltDisableTexturing() {
    gltrace
    if (texturing) {
        glDisable(GL_TEXTURE_2D);
        check_for_errors
        texturing = false;
    }
    gltrace
}

void ltEnableTextureCoordArrays() {
    gltrace
    if (!texture_coord_arrays) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        check_for_errors
        texture_coord_arrays = true;
    }
    gltrace
}

void ltDisableTextureCoordArrays() {
    gltrace
    if (texture_coord_arrays) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        check_for_errors
        texture_coord_arrays = false;
    }
    gltrace
}

void ltTextureMode(LTTextureMode mode) {
    gltrace
    if (mode != texture_mode) {
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
        check_for_errors
        texture_mode = mode;
    }
    gltrace
}

void ltTextureMagFilter(LTTextureFilter filter) {
    gltrace
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    check_for_errors
    gltrace
}

void ltTextureMinFilter(LTTextureFilter filter) {
    gltrace
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter); 
    check_for_errors
    gltrace
}

void ltBindTexture(LTtexid texture_id) {
    gltrace
    if (bound_texture != texture_id) {
        glBindTexture(GL_TEXTURE_2D, texture_id);
        check_for_errors
        bound_texture = texture_id;
    }
    gltrace
}

LTtexid ltGenTexture() {
    gltrace
    LTtexid t;
    glGenTextures(1, &t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
    check_for_errors
    gltrace
    return t;
}

void ltDeleteTexture(LTtexid texture_id) {
    gltrace
    if (bound_texture == texture_id) {
        ltBindTexture(0);
    }
    glDeleteTextures(1, &texture_id);
    check_for_errors
    gltrace
}

void ltTexImage(int width, int height, void *data) {
    gltrace
    #ifdef LTGLES1
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    #else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, data);
    #endif
    check_for_errors
    gltrace
}

void ltBlendMode(LTBlendMode new_mode) {
    gltrace
    LTBlendMode old_mode = blend_mode;
    if (old_mode != new_mode) {
        switch (new_mode) {
            case LT_BLEND_MODE_NORMAL:
                glEnable(GL_BLEND);
                glBlendEquation(GL_FUNC_ADD);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;
            case LT_BLEND_MODE_ADD:
                glEnable(GL_BLEND);
                glBlendEquation(GL_FUNC_ADD);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
            case LT_BLEND_MODE_SUBTRACT:
                glEnable(GL_BLEND);
                glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
                /*
            case LT_BLEND_MODE_DIFF:
                glEnable(GL_BLEND);
                glBlendEquation(GL_FUNC_SUBTRACT);
                glBlendFunc(GL_ONE, GL_ONE);
                break;
                */
            case LT_BLEND_MODE_COLOR:
                glEnable(GL_BLEND);
                glBlendEquation(GL_FUNC_ADD);
                glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
                break;
            case LT_BLEND_MODE_OFF:
                glDisable(GL_BLEND);
                break;
        }
        blend_mode = new_mode;
        check_for_errors
    }
    gltrace
}

void ltEnableDepthTest() {
    gltrace
    if (!depth_test) {
        glEnable(GL_DEPTH_TEST);
        check_for_errors
        depth_test = true;
    }
    gltrace
}

void ltDisableDepthTest() {
    gltrace
    if (depth_test) {
        glDisable(GL_DEPTH_TEST);
        check_for_errors
        depth_test = false;
    }
    gltrace
}

void ltEnableDepthMask() {
    gltrace
    if (!depth_mask) {
        glDepthMask(GL_TRUE);
        check_for_errors
        depth_mask = true;
    }
    gltrace
}

void ltDisableDepthMask() {
    gltrace
    if (depth_mask) {
        glDepthMask(GL_FALSE);
        check_for_errors
        depth_mask = false;
    }
    gltrace
}

void ltDepthFunc(LTDepthFunc f) {
    gltrace
    glDepthFunc(f);
    check_for_errors
    gltrace
}

void ltEnableDither() {
    gltrace
    if (!dither) {
        glEnable(GL_DITHER);
        check_for_errors
        dither = true;
    }
    gltrace
}

void ltDisableDither() {
    gltrace
    if (dither) {
        glDisable(GL_DITHER);
        check_for_errors
        dither = false;
    }
    gltrace
}

void ltEnableAlphaTest() {
    gltrace
    if (!alpha_test) {
        glEnable(GL_ALPHA_TEST);
        check_for_errors
        alpha_test = true;
    }
    gltrace
}

void ltDisableAlphaTest() {
    gltrace
    if (alpha_test) {
        glDisable(GL_ALPHA_TEST);
        check_for_errors
        alpha_test = false;
    }
    gltrace
}

void ltEnableStencilTest() {
    gltrace
    if (!stencil_test) {
        glEnable(GL_STENCIL_TEST);
        check_for_errors
        stencil_test = true;
    }
    gltrace
}

void ltDisableStencilTest() {
    gltrace
    if (stencil_test) {
        glDisable(GL_STENCIL_TEST);
        check_for_errors
        stencil_test = false;
    }
    gltrace
}

void ltEnableVertexArrays() {
    gltrace
    if (!vertex_arrays) {
        glEnableClientState(GL_VERTEX_ARRAY);
        check_for_errors
        vertex_arrays = true;
    }
    gltrace
}

void ltDisableVertexArrays() {
    gltrace
    if (vertex_arrays) {
        glDisableClientState(GL_VERTEX_ARRAY);
        check_for_errors
        vertex_arrays = false;
    }
    gltrace
}

void ltEnableIndexArrays() {
    gltrace
    if (!index_arrays) {
        glEnableClientState(GL_INDEX_ARRAY);
        check_for_errors
        index_arrays = true;
    }
    gltrace
}

void ltDisableIndexArrays() {
    gltrace
    if (index_arrays) {
        glDisableClientState(GL_INDEX_ARRAY);
        check_for_errors
        index_arrays = false;
    }
    gltrace
}

void ltEnableColorArrays() {
    gltrace
    if (!color_arrays) {
        glEnableClientState(GL_COLOR_ARRAY);
        check_for_errors
        color_arrays = true;
    }
    gltrace
}

void ltDisableColorArrays() {
    gltrace
    if (color_arrays) {
        glDisableClientState(GL_COLOR_ARRAY);
        check_for_errors
        color_arrays = false;
    }
    gltrace
}

void ltEnableFog() {
    gltrace
    if (!fog) {
        glEnable(GL_FOG);
        check_for_errors
        fog = true;
    }
    gltrace
}

void ltDisableFog() {
    gltrace
    if (fog) {
        glDisable(GL_FOG);
        check_for_errors
        fog = false;
    }
    gltrace
}

void ltFogColor(LTfloat r, LTfloat g, LTfloat b) {
    gltrace
    GLfloat colv[4];
    colv[0] = r;
    colv[1] = g;
    colv[2] = b;
    colv[3] = 1.0f;
    glFogfv(GL_FOG_COLOR, (const GLfloat*)colv);
    check_for_errors
    gltrace
}

void ltFogStart(LTfloat start) {
    gltrace
    glFogf(GL_FOG_START, start);
    check_for_errors
    gltrace
}

void ltFogEnd(LTfloat end) {
    gltrace
    glFogf(GL_FOG_END, end);
    check_for_errors
    gltrace
}

void ltFogMode(LTFogMode mode) {
    gltrace
    glFogf(GL_FOG_MODE, mode);
    check_for_errors
    gltrace
}

void ltClearColor(LTfloat r, LTfloat g, LTfloat b, LTfloat a) {
    gltrace
    glClearColor(r, g, b, a);
    check_for_errors
    gltrace
}

void ltClear(bool color, bool depthbuf) {
    gltrace
    GLbitfield clear_mask = 0;
    if (color) {
        clear_mask |= GL_COLOR_BUFFER_BIT;
    }
    if (depthbuf) {
        clear_mask |= GL_DEPTH_BUFFER_BIT;
    }
    glClear(clear_mask);
    check_for_errors
    gltrace
}

void ltColor(LTfloat r, LTfloat g, LTfloat b, LTfloat a) {
    gltrace
    glColor4f(r, g, b, a);
    check_for_errors
    gltrace
}

void ltMatrixMode(LTMatrixMode mode) {
    gltrace
    glMatrixMode(mode);
    check_for_errors
    gltrace
}

void ltPushMatrix() {
    gltrace
    glPushMatrix();
    check_for_errors
    gltrace
}

void ltPopMatrix() {
    gltrace
    glPopMatrix();
    check_for_errors
    gltrace
}

void ltMultMatrix(LTfloat *m) {
    gltrace
    glMultMatrixf(m);
    check_for_errors
    gltrace
}

void ltLoadIdentity() {
    gltrace
    glLoadIdentity();
    check_for_errors
    gltrace
}

void ltOrtho(LTfloat left, LTfloat right, LTfloat bottom, LTfloat top, LTfloat near, LTfloat far) {
    gltrace
    #ifdef LTGLES1
    glOrthof(left, right, bottom, top, near, far);
    #else
    glOrtho(left, right, bottom, top, near, far);
    #endif
    check_for_errors
    gltrace
}

void ltFrustum(LTfloat left, LTfloat right, LTfloat bottom, LTfloat top, LTfloat near, LTfloat far) {
    gltrace
    #ifdef LTGLES1
    glFrustumf(left, right, bottom, top, near, far);
    #else
    glFrustum(left, right, bottom, top, near, far);
    #endif
    check_for_errors
    gltrace
}

void ltTranslate(LTfloat x, LTfloat y, LTfloat z) {
    gltrace
    glTranslatef(x, y, z);
    check_for_errors
    gltrace
}

void ltRotate(LTdegrees degrees, LTfloat x, LTfloat y, LTfloat z) {
    gltrace
    glRotatef(degrees, x, y, z);
    check_for_errors
    gltrace
}

void ltScale(LTfloat x, LTfloat y, LTfloat z) {
    gltrace
    glScalef(x, y, z);
    check_for_errors
    gltrace
}

void ltViewport(int x, int y, int width, int height) {
    gltrace
    glViewport(x, y, width, height);
    check_for_errors
    gltrace
}

LTvertbuf ltGenVertBuffer() {
    gltrace
    LTvertbuf vb;
    glGenBuffers(1, &vb);
    check_for_errors
    gltrace
    return vb;
}

void ltBindVertBuffer(LTvertbuf vb) {
    gltrace
    if (bound_vertbuffer != vb) {
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        check_for_errors
        bound_vertbuffer = vb;
    }
    gltrace
}

void ltDeleteVertBuffer(LTvertbuf vb) {
    gltrace
    // Make sure vb is not bound before deleting it.
    // This seems to fix an occasional crash on OSX.
    if (bound_vertbuffer == vb) {
        ltBindVertBuffer(0);
    }
    glDeleteBuffers(1, &vb);
    check_for_errors
    gltrace
}

void ltStaticVertBufferData(int size, const void *data) {
    gltrace
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    check_for_errors
    gltrace
}

void ltVertexPointer(int size, LTVertDataType type, int stride, void *data) {
    gltrace
    glVertexPointer(size, type, stride, data);
    check_for_errors
    gltrace
}

void ltColorPointer(int size, LTVertDataType type, int stride, void *data) {
    gltrace
    glColorPointer(size, type, stride, data);
    check_for_errors
    gltrace
}

void ltTexCoordPointer(int size, LTVertDataType type, int stride, void *data) {
    gltrace
    glTexCoordPointer(size, type, stride, data);
    check_for_errors
    gltrace
}

void ltDrawArrays(LTDrawMode mode, int start, int count) {
    gltrace
    glDrawArrays(mode, start, count);
    check_for_errors
    gltrace
}

void ltDrawElements(LTDrawMode mode, int n, LTvertindex *indices) {
    gltrace
    glDrawElements(mode, n, GL_UNSIGNED_SHORT, indices);
    check_for_errors
    gltrace
}

LTframebuf ltGenFramebuffer() {
    gltrace
    LTframebuf fb;
    GLEXT(glGenFramebuffers)(1, &fb);
    check_for_errors
    gltrace
    return fb;
}

void ltBindFramebuffer(LTframebuf fb) {
    gltrace
    if (bound_framebuffer != fb) {
        GLEXT(glBindFramebuffer)(GL_EXT(GL_FRAMEBUFFER), fb);
        check_for_errors
        bound_framebuffer = fb;
    }
    gltrace
}

void ltDeleteFramebuffer(LTframebuf fb) {
    gltrace
    if (bound_framebuffer == fb) {
        ltBindFramebuffer(0);
    }
    glDeleteFramebuffers(1, &fb);
    check_for_errors
    gltrace
}

void ltFramebufferTexture(LTtexid texture_id) {
    gltrace
    GLEXT(glFramebufferTexture2D)(GL_EXT(GL_FRAMEBUFFER), GL_EXT(GL_COLOR_ATTACHMENT0), GL_TEXTURE_2D, texture_id, 0);
    check_for_errors
    gltrace
}

bool ltFramebufferComplete() {
    GLenum status = GLEXT(glCheckFramebufferStatus)(GL_EXT(GL_FRAMEBUFFER));
    return status == GL_EXT(GL_FRAMEBUFFER_COMPLETE);
}

static void set_state_str() {
    const char *blend_mode_str = "?";
    switch (blend_mode) {
        case LT_BLEND_MODE_NORMAL:
            blend_mode_str = "N";
            break;
        case LT_BLEND_MODE_ADD:
            blend_mode_str = "A";
            break;
        case LT_BLEND_MODE_SUBTRACT:
            blend_mode_str = "S";
            break;
        case LT_BLEND_MODE_COLOR:
            blend_mode_str = "C";
            break;
        case LT_BLEND_MODE_OFF:
            blend_mode_str = "-";
            break;
    }

    const char *tex_mode_str = "?";
    switch (texture_mode) {
        case LT_TEXTURE_MODE_MODULATE:
            tex_mode_str = "M";
            break;
        case LT_TEXTURE_MODE_ADD:
            tex_mode_str = "A";
            break;
        case LT_TEXTURE_MODE_DECAL:
            tex_mode_str = "D";
            break;
        case LT_TEXTURE_MODE_BLEND:
            tex_mode_str = "B";
            break;
        case LT_TEXTURE_MODE_REPLACE:
            tex_mode_str = "R";
            break;
    }

    snprintf(state_str, 1024,
        "FB:%-3d TX:%-3d VB:%-3d BM:%s TM:%s %s %s %s %s %s %s %s %s %s %s %s",
        bound_framebuffer,
        bound_texture,
        bound_vertbuffer,
        blend_mode_str,
        tex_mode_str,
        texturing ? "TX" : "--",
        texture_coord_arrays ? "TA" : "--",
        vertex_arrays ? "VA" : "--",
        index_arrays ? "IA" : "--",
        color_arrays ? "CA" : "--",
        fog ? "FG" : "--",
        depth_test ? "DT" : "--",
        depth_mask ? "DM" : "--",
        dither ? "DH" : "--",
        alpha_test ? "AT" : "--",
        stencil_test ? "ST" : "--");
}