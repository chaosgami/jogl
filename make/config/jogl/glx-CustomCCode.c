#include <inttypes.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
/* Linux headers don't work properly */
#define __USE_GNU
#include <dlfcn.h>
#undef __USE_GNU

/* HP-UX doesn't define RTLD_DEFAULT. */
#if defined(_HPUX) && !defined(RTLD_DEFAULT)
#define RTLD_DEFAULT NULL
#endif

/* We expect glXGetProcAddressARB to be defined */
extern void (*glXGetProcAddressARB(const GLubyte *procname))();

static const char * clazzNameBuffers = "com/jogamp/common/nio/Buffers";
static const char * clazzNameBuffersStaticCstrName = "copyByteBuffer";
static const char * clazzNameBuffersStaticCstrSignature = "(Ljava/nio/ByteBuffer;)Ljava/nio/ByteBuffer;";
static const char * clazzNameByteBuffer = "java/nio/ByteBuffer";
static jclass clazzBuffers = NULL;
static jmethodID cstrBuffers = NULL;
static jclass clazzByteBuffer = NULL;

static void _initClazzAccess(JNIEnv *env) {
    jclass c;

    if(NULL!=cstrBuffers) return ;

    c = (*env)->FindClass(env, clazzNameBuffers);
    if(NULL==c) {
        fprintf(stderr, "FatalError: Java_jogamp_opengl_x11_glx_GLX: can't find %s\n", clazzNameBuffers);
        (*env)->FatalError(env, clazzNameBuffers);
    }
    clazzBuffers = (jclass)(*env)->NewGlobalRef(env, c);
    if(NULL==clazzBuffers) {
        fprintf(stderr, "FatalError: Java_jogamp_opengl_x11_glx_GLX: can't use %s\n", clazzNameBuffers);
        (*env)->FatalError(env, clazzNameBuffers);
    }
    c = (*env)->FindClass(env, clazzNameByteBuffer);
    if(NULL==c) {
        fprintf(stderr, "FatalError: Java_jogamp_opengl_x11_glx_GLX: can't find %s\n", clazzNameByteBuffer);
        (*env)->FatalError(env, clazzNameByteBuffer);
    }
    clazzByteBuffer = (jclass)(*env)->NewGlobalRef(env, c);
    if(NULL==c) {
        fprintf(stderr, "FatalError: Java_jogamp_opengl_x11_glx_GLX: can't use %s\n", clazzNameByteBuffer);
        (*env)->FatalError(env, clazzNameByteBuffer);
    }

    cstrBuffers = (*env)->GetStaticMethodID(env, clazzBuffers, 
                            clazzNameBuffersStaticCstrName, clazzNameBuffersStaticCstrSignature);
    if(NULL==cstrBuffers) {
        fprintf(stderr, "FatalError: Java_jogamp_opengl_x11_glx_GLX:: can't create %s.%s %s\n",
            clazzNameBuffers,
            clazzNameBuffersStaticCstrName, clazzNameBuffersStaticCstrSignature);
        (*env)->FatalError(env, clazzNameBuffersStaticCstrName);
    }
}

/*   Java->C glue code:
 *   Java package: jogamp.opengl.x11.glx.GLX
 *    Java method: int glXGetFBConfigAttributes(long dpy, long config, IntBuffer attributes, IntBuffer values)
 */
JNIEXPORT jint JNICALL 
Java_jogamp_opengl_x11_glx_GLX_dispatch_1glXGetFBConfigAttributes(JNIEnv *env, jclass _unused, jlong dpy, jlong config, jint attributeCount, jobject attributes, jint attributes_byte_offset, jobject values, jint values_byte_offset, jlong procAddress) {
  typedef int (APIENTRY*_local_PFNGLXGETFBCONFIGATTRIBPROC)(Display *  dpy, GLXFBConfig config, int attribute, int *  value);
  _local_PFNGLXGETFBCONFIGATTRIBPROC ptr_glXGetFBConfigAttrib = (_local_PFNGLXGETFBCONFIGATTRIBPROC) (intptr_t) procAddress;
  assert(ptr_glXGetFBConfigAttrib != NULL);

  int err = 0;
  if ( attributeCount > 0 && NULL != attributes ) {
    int i;
    int * attributes_ptr = (int *) (((char*) (*env)->GetDirectBufferAddress(env, attributes)) + attributes_byte_offset);
    int * values_ptr = (int *) (((char*) (*env)->GetDirectBufferAddress(env, values)) + values_byte_offset);
    for(i=0; 0 == err && i<attributeCount; i++) {
        err = (* ptr_glXGetFBConfigAttrib) ((Display *) (intptr_t) dpy, (GLXFBConfig) (intptr_t) config, attributes_ptr[i], &values_ptr[i]);
    }
    if( 0 != err ) {
        values_ptr[0] = i;
    }
  }
  return (jint)err;
}

/*   Java->C glue code:
 *   Java package: jogamp.opengl.x11.glx.GLX
 *    Java method: XVisualInfo glXGetVisualFromFBConfig(long dpy, long config)
 *     C function: XVisualInfo *  glXGetVisualFromFBConfig(Display *  dpy, GLXFBConfig config);
 */
JNIEXPORT jobject JNICALL 
Java_jogamp_opengl_x11_glx_GLX_dispatch_1glXGetVisualFromFBConfig(JNIEnv *env, jclass _unused, jlong dpy, jlong config, jlong procAddress) {
  typedef XVisualInfo* (APIENTRY*_local_PFNGLXGETVISUALFROMFBCONFIG)(Display *  dpy, GLXFBConfig config);
  _local_PFNGLXGETVISUALFROMFBCONFIG ptr_glXGetVisualFromFBConfig;
  XVisualInfo *  _res;
  jobject jbyteSource;
  jobject jbyteCopy;
  ptr_glXGetVisualFromFBConfig = (_local_PFNGLXGETVISUALFROMFBCONFIG) (intptr_t) procAddress;
  assert(ptr_glXGetVisualFromFBConfig != NULL);
  _res = (* ptr_glXGetVisualFromFBConfig) ((Display *) (intptr_t) dpy, (GLXFBConfig) (intptr_t) config);
  if (_res == NULL) return NULL;

  _initClazzAccess(env);

  jbyteSource = (*env)->NewDirectByteBuffer(env, _res, sizeof(XVisualInfo));
  jbyteCopy   = (*env)->CallStaticObjectMethod(env, clazzBuffers, cstrBuffers, jbyteSource);

  (*env)->DeleteLocalRef(env, jbyteSource);
  XFree(_res);

  return jbyteCopy;
}

/*   Java->C glue code:
 *   Java package: jogamp.opengl.x11.glx.GLX
 *    Java method: com.jogamp.common.nio.PointerBuffer dispatch_glXChooseFBConfig(long dpy, int screen, java.nio.IntBuffer attribList, java.nio.IntBuffer nitems)
 *     C function: GLXFBConfig *  glXChooseFBConfig(Display *  dpy, int screen, const int *  attribList, int *  nitems);
 */
JNIEXPORT jobject JNICALL 
Java_jogamp_opengl_x11_glx_GLX_dispatch_1glXChooseFBConfig(JNIEnv *env, jclass _unused, jlong dpy, jint screen, jobject attribList, jint attribList_byte_offset, jobject nitems, jint nitems_byte_offset, jlong procAddress) {
  typedef GLXFBConfig *  (APIENTRY*_local_PFNGLXCHOOSEFBCONFIGPROC)(Display *  dpy, int screen, const int *  attribList, int *  nitems);
  _local_PFNGLXCHOOSEFBCONFIGPROC ptr_glXChooseFBConfig;
  int * _attribList_ptr = NULL;
  int * _nitems_ptr = NULL;
  GLXFBConfig *  _res;
  int count;
  jobject jbyteSource;
  jobject jbyteCopy;
    if ( NULL != attribList ) {
        _attribList_ptr = (int *) (((char*) (*env)->GetDirectBufferAddress(env, attribList)) + attribList_byte_offset);
    }
    if ( NULL != nitems ) {
        _nitems_ptr = (int *) (((char*) (*env)->GetDirectBufferAddress(env, nitems)) + nitems_byte_offset);
    }
  ptr_glXChooseFBConfig = (_local_PFNGLXCHOOSEFBCONFIGPROC) (intptr_t) procAddress;
  assert(ptr_glXChooseFBConfig != NULL);
  _res = (* ptr_glXChooseFBConfig) ((Display *) (intptr_t) dpy, (int) screen, (int *) _attribList_ptr, (int *) _nitems_ptr);
  count = _nitems_ptr[0];
  if (NULL == _res) return NULL;

  _initClazzAccess(env);

  jbyteSource = (*env)->NewDirectByteBuffer(env, _res, count * sizeof(GLXFBConfig));
  jbyteCopy   = (*env)->CallStaticObjectMethod(env, clazzBuffers, cstrBuffers, jbyteSource);
  (*env)->DeleteLocalRef(env, jbyteSource);
  XFree(_res);

  return jbyteCopy;
}


/*   Java->C glue code:
 *   Java package: jogamp.opengl.x11.glx.GLX
 *    Java method: XVisualInfo dispatch_glXChooseVisual(long dpy, int screen, java.nio.IntBuffer attribList)
 *     C function: XVisualInfo *  glXChooseVisual(Display *  dpy, int screen, int *  attribList);
 */
JNIEXPORT jobject JNICALL 
Java_jogamp_opengl_x11_glx_GLX_dispatch_1glXChooseVisual(JNIEnv *env, jclass _unused, jlong dpy, jint screen, jobject attribList, jint attribList_byte_offset, jlong procAddress) {
  typedef XVisualInfo *  (APIENTRY*_local_PFNGLXCHOOSEVISUALPROC)(Display *  dpy, int screen, int *  attribList);
  _local_PFNGLXCHOOSEVISUALPROC ptr_glXChooseVisual;
  int * _attribList_ptr = NULL;
  XVisualInfo *  _res;
  jobject jbyteSource;
  jobject jbyteCopy;
    if ( NULL != attribList ) {
        _attribList_ptr = (int *) (((char*) (*env)->GetDirectBufferAddress(env, attribList)) + attribList_byte_offset);
    }
  ptr_glXChooseVisual = (_local_PFNGLXCHOOSEVISUALPROC) (intptr_t) procAddress;
  assert(ptr_glXChooseVisual != NULL);
  _res = (* ptr_glXChooseVisual) ((Display *) (intptr_t) dpy, (int) screen, (int *) _attribList_ptr);
  if (NULL == _res) return NULL;

  _initClazzAccess(env);

  jbyteSource = (*env)->NewDirectByteBuffer(env, _res, sizeof(XVisualInfo));
  jbyteCopy   = (*env)->CallStaticObjectMethod(env, clazzBuffers, cstrBuffers, jbyteSource);

  (*env)->DeleteLocalRef(env, jbyteSource);
  XFree(_res);

  return jbyteCopy;
}

