/* Copyright (c) 2011 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <stdint.h>
#include <malloc.h>

// #include "ppapi/c/pp_macros.h"

#define PP_INLINE static __inline__

// #include "ppapi/c/pp_completion_callback.h"

typedef void (*PP_CompletionCallback_Func)(void* user_data, int32_t result);

typedef enum {
  PP_COMPLETIONCALLBACK_FLAG_NONE = 0 << 0,
  PP_COMPLETIONCALLBACK_FLAG_OPTIONAL = 1 << 0
} PP_CompletionCallback_Flag;

struct PP_CompletionCallback {
  PP_CompletionCallback_Func func;
  void* user_data;
  int32_t flags;
};

PP_INLINE struct PP_CompletionCallback PP_MakeCompletionCallback(
    PP_CompletionCallback_Func func,
    void* user_data) {
  struct PP_CompletionCallback cc;
  cc.func = func;
  cc.user_data = user_data;
  cc.flags = PP_COMPLETIONCALLBACK_FLAG_NONE;
  return cc;
}

// #include "ppapi/c/pp_errors.h"

enum {
  PP_OK = 0,
  PP_OK_COMPLETIONPENDING = -1,
  PP_ERROR_FAILED = -2,
  PP_ERROR_ABORTED = -3,
  PP_ERROR_BADARGUMENT = -4,
  PP_ERROR_BADRESOURCE = -5,
  PP_ERROR_NOINTERFACE = -6,
  PP_ERROR_NOACCESS = -7,
  PP_ERROR_NOMEMORY = -8,
  PP_ERROR_NOSPACE = -9,
  PP_ERROR_NOQUOTA = -10,
  PP_ERROR_INPROGRESS = -11,
  PP_ERROR_NOTSUPPORTED = -12,
  PP_ERROR_FILENOTFOUND = -20,
  PP_ERROR_FILEEXISTS = -21,
  PP_ERROR_FILETOOBIG = -22,
  PP_ERROR_FILECHANGED = -23,
  PP_ERROR_TIMEDOUT = -30,
  PP_ERROR_USERCANCEL = -40
};

// #include "ppapi/c/pp_instance.h"

typedef int32_t PP_Instance;

// #include "ppapi/c/pp_module.h"

typedef int32_t PP_Module;

// #include "ppapi/c/pp_size.h"
struct PP_Size {
  int32_t width;
  int32_t height;
};

// #include "ppapi/c/pp_bool.h"

typedef enum {
  PP_FALSE = 0,
  PP_TRUE = 1
} PP_Bool;

// #include "ppapi/c/pp_var.h

struct PP_Var {
  int32_t type;
  int32_t padding;
  int64_t value;
};

// #include "ppapi/c/ppb.h"

typedef const void* (*PPB_GetInterface)(const char* interface_name);

// #include "ppapi/c/pp_resource.h"

typedef int32_t PP_Resource;

// #include "ppapi/c/ppb_core.h"

#define PPB_CORE_INTERFACE_1_0 "PPB_Core;1.0"
#define PPB_CORE_INTERFACE PPB_CORE_INTERFACE_1_0

struct PPB_Core {
  void (*AddRefResource)(PP_Resource resource);
  void (*ReleaseResource)(PP_Resource resource);
  double (*GetTime)();
  double (*GetTimeTicks)();
  void (*CallOnMainThread)(int32_t delay_in_milliseconds,
                           struct PP_CompletionCallback callback,
                           int32_t result);
  PP_Bool (*IsMainThread)();
};

// #include "ppapi/c/pp_point.h"

struct PP_Point {
  int32_t x;
  int32_t y;
};

struct PP_FloatPoint {
  float x;
  float y;
};

// #include "ppapi/c/pp_rect.h"

struct PP_Rect {
  struct PP_Point point;
  struct PP_Size size;
};

// #include "ppapi/c/ppb_graphics_2d.h"

#define PPB_GRAPHICS_2D_INTERFACE_0_4 "PPB_Graphics2D;0.4"
#define PPB_GRAPHICS_2D_INTERFACE_1_0 "PPB_Graphics2D;1.0"
#define PPB_GRAPHICS_2D_INTERFACE PPB_GRAPHICS_2D_INTERFACE_1_0

struct PPB_Graphics2D {
  PP_Resource (*Create)(PP_Instance instance,
                        const struct PP_Size* size,
                        PP_Bool is_always_opaque);
  PP_Bool (*IsGraphics2D)(PP_Resource resource);
  PP_Bool (*Describe)(PP_Resource graphics_2d,
                      struct PP_Size* size,
                      PP_Bool* is_always_opqaue);
  void (*PaintImageData)(PP_Resource graphics_2d,
                         PP_Resource image_data,
                         const struct PP_Point* top_left,
                         const struct PP_Rect* src_rect);
  void (*Scroll)(PP_Resource graphics_2d,
                 const struct PP_Rect* clip_rect,
                 const struct PP_Point* amount);
  void (*ReplaceContents)(PP_Resource graphics_2d, PP_Resource image_data);
  int32_t (*Flush)(PP_Resource graphics_2d,
                   struct PP_CompletionCallback callback);
};

// #include "ppapi/c/ppb_image_data.h"

typedef enum {
  PP_IMAGEDATAFORMAT_BGRA_PREMUL,
  PP_IMAGEDATAFORMAT_RGBA_PREMUL
} PP_ImageDataFormat;

struct PP_ImageDataDesc {
  PP_ImageDataFormat format;
  struct PP_Size size;
  int32_t stride;
};

#define PPB_IMAGEDATA_INTERFACE_0_3 "PPB_ImageData;0.3"
#define PPB_IMAGEDATA_INTERFACE_1_0 "PPB_ImageData;1.0"
#define PPB_IMAGEDATA_INTERFACE PPB_IMAGEDATA_INTERFACE_1_0

struct PPB_ImageData {
  PP_ImageDataFormat (*GetNativeImageDataFormat)();
  PP_Bool (*IsImageDataFormatSupported)(PP_ImageDataFormat format);
  PP_Resource (*Create)(PP_Instance instance,
                        PP_ImageDataFormat format,
                        const struct PP_Size* size,
                        PP_Bool init_to_zero);
  PP_Bool (*IsImageData)(PP_Resource image_data);
  PP_Bool (*Describe)(PP_Resource image_data, struct PP_ImageDataDesc* desc);
  void* (*Map)(PP_Resource image_data);
  void (*Unmap)(PP_Resource image_data);
};

// #include "ppapi/c/ppb_instance.h"

#define PPB_INSTANCE_INTERFACE_0_5 "PPB_Instance;0.5"
#define PPB_INSTANCE_INTERFACE_1_0 "PPB_Instance;1.0"
#define PPB_INSTANCE_INTERFACE PPB_INSTANCE_INTERFACE_1_0

struct PPB_Instance {
  PP_Bool (*BindGraphics)(PP_Instance instance, PP_Resource device);
  PP_Bool (*IsFullFrame)(PP_Instance instance);
};


// #include "ppapi/c/ppp.h"

#define PP_EXPORT __attribute__ ((visibility("default")))

PP_EXPORT int32_t PPP_InitializeModule(PP_Module module,
                                       PPB_GetInterface get_browser_interface);
PP_EXPORT void PPP_ShutdownModule();
PP_EXPORT const void* PPP_GetInterface(const char* interface_name);

// #include "ppapi/c/ppp_instance.h"

#define PPP_INSTANCE_INTERFACE_1_0 "PPP_Instance;1.0"
#define PPP_INSTANCE_INTERFACE PPP_INSTANCE_INTERFACE_1_0

struct PPP_Instance {
  PP_Bool (*DidCreate)(PP_Instance instance,
                       uint32_t argc,
                       const char* argn[],
                       const char* argv[]);
  void (*DidDestroy)(PP_Instance instance);
  void (*DidChangeView)(PP_Instance instance,
                        const struct PP_Rect* position,
                        const struct PP_Rect* clip);
  void (*DidChangeFocus)(PP_Instance instance, PP_Bool has_focus);
  PP_Bool (*HandleDocumentLoad)(PP_Instance instance, PP_Resource url_loader);
};

typedef struct PPP_Instance PPP_Instance_1_0;

/**************************************************** END OF HEADERS ***************************************/

PPB_GetInterface g_get_browser_interface = NULL;

const struct PPB_Core* g_core_interface;
const struct PPB_Graphics2D* g_graphics_2d_interface;
const struct PPB_ImageData* g_image_data_interface;
const struct PPB_Instance* g_instance_interface;

/* PPP_Instance implementation -----------------------------------------------*/

struct InstanceInfo {
  PP_Instance pp_instance;
  struct PP_Size last_size;

  struct InstanceInfo* next;
};

/** Linked list of all live instances. */
struct InstanceInfo* all_instances = NULL;

/** Returns a refed resource corresponding to the created graphics 2d. */
PP_Resource MakeAndBindGraphics2D(PP_Instance instance,
                                  const struct PP_Size* size) {
  PP_Resource graphics;

  graphics = g_graphics_2d_interface->Create(instance, size, PP_FALSE);
  if (!graphics)
    return 0;

  if (!g_instance_interface->BindGraphics(instance, graphics)) {
    g_core_interface->ReleaseResource(graphics);
    return 0;
  }
  return graphics;
}

void FlushCompletionCallback(void* user_data, int32_t result) {
  /* Don't need to do anything here. */
}

void Repaint(struct InstanceInfo* instance, const struct PP_Size* size) {
  PP_Resource image, graphics;
  struct PP_ImageDataDesc image_desc;
  uint32_t* image_data;
  int num_words, i;

  /* Create image data to paint into. */
  image = g_image_data_interface->Create(
      instance->pp_instance, PP_IMAGEDATAFORMAT_BGRA_PREMUL, size, PP_TRUE);
  if (!image)
    return;
  g_image_data_interface->Describe(image, &image_desc);

  /* Fill the image with blue. */
  image_data = (uint32_t*)g_image_data_interface->Map(image);
  if (!image_data) {
    g_core_interface->ReleaseResource(image);
    return;
  }
  num_words = image_desc.stride * size->height / 4;
  for (i = 0; i < num_words; i++)
    image_data[i] = 0xFF0000FF;

  /* Create the graphics 2d and paint the image to it. */
  graphics = MakeAndBindGraphics2D(instance->pp_instance, size);
  if (!graphics) {
    g_core_interface->ReleaseResource(image);
    return;
  }

  g_graphics_2d_interface->ReplaceContents(graphics, image);
  g_graphics_2d_interface->Flush(graphics,
      PP_MakeCompletionCallback(&FlushCompletionCallback, NULL));

  g_core_interface->ReleaseResource(graphics);
  g_core_interface->ReleaseResource(image);
}

/** Returns the info for the given instance, or NULL if it's not found. */
struct InstanceInfo* FindInstance(PP_Instance instance) {
  struct InstanceInfo* cur = all_instances;
  while (cur) {
    if (cur->pp_instance == instance)
      return cur;
  }
  return NULL;
}

PP_Bool Instance_DidCreate(PP_Instance instance,
                           uint32_t argc,
                           const char* argn[],
                           const char* argv[]) {
  struct InstanceInfo* info =
      (struct InstanceInfo*)malloc(sizeof(struct InstanceInfo));
  info->pp_instance = instance;
  info->last_size.width = 0;
  info->last_size.height = 0;

  /* Insert into linked list of live instances. */
  info->next = all_instances;
  all_instances = info;
  return PP_TRUE;
}

void Instance_DidDestroy(PP_Instance instance) {
  /* Find the matching item in the linked list, delete it, and patch the
   * links.
   */
  struct InstanceInfo** prev_ptr = &all_instances;
  struct InstanceInfo* cur = all_instances;
  while (cur) {
    if (instance == cur->pp_instance) {
      *prev_ptr = cur->next;
      free(cur);
      return;
    }
    prev_ptr = &cur->next;
  }
}

void Instance_DidChangeView(PP_Instance pp_instance,
                            const struct PP_Rect* position,
                            const struct PP_Rect* clip) {
  struct InstanceInfo* info = FindInstance(pp_instance);
  if (!info)
    return;

  if (info->last_size.width != position->size.width ||
      info->last_size.height != position->size.height) {
    /* Got a resize, repaint the plugin. */
    Repaint(info, &position->size);
    info->last_size.width = position->size.width;
    info->last_size.height = position->size.height;
  }
}

void Instance_DidChangeFocus(PP_Instance pp_instance, PP_Bool has_focus) {
}

PP_Bool Instance_HandleDocumentLoad(PP_Instance pp_instance,
                                    PP_Resource pp_url_loader) {
  return PP_FALSE;
}

static struct PPP_Instance instance_interface = {
  &Instance_DidCreate,
  &Instance_DidDestroy,
  &Instance_DidChangeView,
  &Instance_DidChangeFocus,
  &Instance_HandleDocumentLoad
};


/* Global entrypoints --------------------------------------------------------*/

PP_EXPORT int32_t PPP_InitializeModule(PP_Module module,
                                       PPB_GetInterface get_browser_interface) {
  g_get_browser_interface = get_browser_interface;

  g_core_interface = (const struct PPB_Core*)
      get_browser_interface(PPB_CORE_INTERFACE);
  g_instance_interface = (const struct PPB_Instance*)
      get_browser_interface(PPB_INSTANCE_INTERFACE);
  g_image_data_interface = (const struct PPB_ImageData*)
      get_browser_interface(PPB_IMAGEDATA_INTERFACE);
  g_graphics_2d_interface = (const struct PPB_Graphics2D*)
      get_browser_interface(PPB_GRAPHICS_2D_INTERFACE);
  if (!g_core_interface || !g_instance_interface || !g_image_data_interface ||
      !g_graphics_2d_interface)
    return -1;

  return PP_OK;
}

PP_EXPORT void PPP_ShutdownModule() {
}

PP_EXPORT const void* PPP_GetInterface(const char* interface_name) {
  if (strcmp(interface_name, PPP_INSTANCE_INTERFACE) == 0)
    return &instance_interface;
  return NULL;
}

struct PP_StartFunctions {
  int32_t (*PPP_InitializeModule)(PP_Module module_id,
                                  PPB_GetInterface get_browser_interface);
  void (*PPP_ShutdownModule)();
  const void *(*PPP_GetInterface)(const char *interface_name);
};

static void fatal_error(const char *message) {
  write(2, message, strlen(message));
  _exit(127);
}

/* Define 32-bit specific types */
typedef uint32_t    Elf32_Addr;   /* alignment 4 */
typedef uint16_t    Elf32_Half;   /* alignment 2 */
typedef uint32_t    Elf32_Off;    /* alignment 4 */
typedef int32_t     Elf32_Sword;  /* alignment 4 */
typedef uint32_t    Elf32_Word;   /* alignment 4 */

/*
 * The auxiliary vector is passed on the stack between ELF loaders,
 * dynamic linkers, and program startup code.  The gratuitous union
 * is the historical standard API, though it has no purpose today. */
typedef struct {
  Elf32_Word a_type;            /* Entry type */
  union {
    Elf32_Word a_val;         /* Integer value */
  } a_un;
} Elf32_auxv_t;

/* Keys for auxiliary vector (auxv). */
#define AT_NULL         0   /* Terminating item in auxv array */
#define AT_ENTRY        9   /* Entry point of the executable */
#define AT_SYSINFO      32  /* System call entry point */

struct PP_ThreadFunctions {
  /*
   * This is a cut-down version of pthread_create()/pthread_join().
   * We omit thread creation attributes and the thread's return value.
   *
   * We use uintptr_t as the thread ID type because pthread_t is not
   * part of the stable ABI; a user thread library might choose an
   * arbitrary size for its own pthread_t.   */
  int (*thread_create)(uintptr_t *tid,
                       void (*func)(void *thread_argument),
                       void *thread_argument);
  int (*thread_join)(uintptr_t tid);
};


#define NACL_IRT_PPAPIHOOK_v0_1 "nacl-irt-ppapihook-0.1"
struct nacl_irt_ppapihook {
  int (*ppapi_start)(const struct PP_StartFunctions *);
  void (*ppapi_register_thread_creator)(const struct PP_ThreadFunctions *);
};

/*                                                                                                                                                                                                                                  
 * TODO(mcgrathr): This extremely stupid function should not exist.
 * If the startup calling sequence were sane, this would be done
 * someplace that has the initial pointer locally rather than stealing
 * it from environ.
 * See http://code.google.com/p/nativeclient/issues/detail?id=651 */
static Elf32_auxv_t *find_auxv(void) {
  /*
   * This presumes environ has its startup-time value on the stack.   */
  char **ep = environ;
  while (*ep != NULL)
    ++ep;
  return (void *) (ep + 1);
}

typedef size_t (*TYPE_nacl_irt_query)(const char *interface_ident,
                                      void *table, size_t tablesize);

/*                                                                                                                                                                                                                                  
 * Scan the auxv for AT_SYSINFO, which is the pointer to the IRT query function.
 */
static TYPE_nacl_irt_query grok_auxv(const Elf32_auxv_t *auxv) {
  const Elf32_auxv_t *av;
  for (av = auxv; av->a_type != AT_NULL; ++av) {
    if (av->a_type == AT_SYSINFO)
      return (TYPE_nacl_irt_query) av->a_un.a_val;
  }
  return NULL;
}

/*static int thread_create(uintptr_t *tid,
                         void (*func)(void *thread_argument),
                         void *thread_argument) {
  return pthread_create((pthread_t *) tid, NULL,
			(void *(*)(void *thread_argument)) func,
			thread_argument);
}

static int thread_join(uintptr_t tid) {
  return pthread_join((pthread_t) tid, NULL);
}

const static struct PP_ThreadFunctions thread_funcs = {
  thread_create,
  thread_join
  };*/

static void __nacl_register_thread_creator(const struct nacl_irt_ppapihook *hooks) {
  //  hooks->ppapi_register_thread_creator(&thread_funcs);
}

static int PpapiPluginStart(const struct PP_StartFunctions *funcs) {
  TYPE_nacl_irt_query query_func = grok_auxv(find_auxv());

  if (NULL == query_func)
    fatal_error("PpapiPluginStart: No AT_SYSINFO item found in auxv, "
                "so cannot start PPAPI.  Is the IRT library not present?\n");

  struct nacl_irt_ppapihook hooks;
  if (sizeof(hooks) != query_func(NACL_IRT_PPAPIHOOK_v0_1,
                                  &hooks, sizeof(hooks)))
    fatal_error("PpapiPluginStart: PPAPI hooks not found\n");

  __nacl_register_thread_creator(&hooks);

  return hooks.ppapi_start(funcs);
}

static const struct PP_StartFunctions ppapi_app_start_callbacks = {
  PPP_InitializeModule,
  PPP_ShutdownModule,
  PPP_GetInterface
};

int main(int argc, char* argv[]) {
  return PpapiPluginStart(&ppapi_app_start_callbacks);
}
