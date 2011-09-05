/* Copyright (c) 2011 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#define PROT_READ        0x1
#define PROT_WRITE       0x2

#define MAP_PRIVATE      0x02  /* Changes are private.  */
#define MAP_ANONYMOUS    0x20  /* Don't use a file.  */

/* Keys for auxiliary vector (auxv). */
#define AT_NULL         0   /* Terminating item in auxv array */
#define AT_ENTRY        9   /* Entry point of the executable */
#define AT_SYSINFO      32  /* System call entry point */

#define NULL 0
#define PP_OK 0

typedef short int16_t;
typedef unsigned short uint16_t;
typedef unsigned long size_t;
typedef long ssize_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;
typedef unsigned int* uintptr_t;
typedef long int  clock_t;
typedef long int off_t;

struct timeval;
struct timespec;
struct dirent;
struct stat;

//static ssize_t write(int fd, const void *buf, size_t count);
//static void *malloc(size_t size);
static size_t strlen(const char *str);
int str_eq(const char *s1, const char *s2);
static void _exit(int status);

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

#define NACL_IRT_BASIC_v0_1     "nacl-irt-basic-0.1"
struct nacl_irt_basic {
  void (*exit)(int status);
  int (*gettod)(struct timeval *tv);
  int (*clock)(clock_t *ticks);
  int (*nanosleep)(const struct timespec *req, struct timespec *rem);
  int (*sched_yield)(void);
  int (*sysconf)(int name, int *value);
};

#define NACL_IRT_FDIO_v0_1      "nacl-irt-fdio-0.1"
struct nacl_irt_fdio {
  int (*close)(int fd);
  int (*dup)(int fd, int *newfd);
  int (*dup2)(int fd, int newfd);
  int (*read)(int fd, void *buf, size_t count, size_t *nread);
  int (*write)(int fd, const void *buf, size_t count, size_t *nwrote);
  int (*seek)(int fd, off_t offset, int whence, off_t *new_offset);
  int (*fstat)(int fd, struct stat *);
  int (*getdents)(int fd, struct dirent *, size_t count, size_t *nread);
};

#define NACL_IRT_MEMORY_v0_1    "nacl-irt-memory-0.1"
struct nacl_irt_memory {
  int (*sysbrk)(void **newbrk);
  int (*mmap)(void **addr, size_t len, int prot, int flags, int fd, off_t off);
  int (*munmap)(void *addr, size_t len);
};

typedef size_t (*TYPE_nacl_irt_query)(const char *interface_ident,
                                      void *table, size_t tablesize);

/**************************************************** END OF HEADERS ***************************************/

struct InstanceInfo {
  PP_Instance pp_instance;
  struct PP_Size last_size;

  struct InstanceInfo* next;
};

struct bss_killer {
  TYPE_nacl_irt_query __nacl_irt_query;
  struct nacl_irt_basic __libnacl_irt_basic;
  struct nacl_irt_fdio __libnacl_irt_fdio;
//struct nacl_irt_filename __libnacl_irt_filename;
  struct nacl_irt_memory __libnacl_irt_memory;
//struct nacl_irt_dyncode __libnacl_irt_dyncode;
//struct nacl_irt_tls __libnacl_irt_tls;
//struct nacl_irt_blockhook __libnacl_irt_blockhook;

  PPB_GetInterface g_get_browser_interface;

  const struct PPB_Core* g_core_interface;
  const struct PPB_Graphics2D* g_graphics_2d_interface;
  const struct PPB_ImageData* g_image_data_interface;
  const struct PPB_Instance* g_instance_interface;

  /* This demo supports one instance only */
  struct InstanceInfo the_instance;

  struct PPP_Instance ppp_instance;
};

struct bss_killer ih;

/** Returns a refed resource corresponding to the created graphics 2d. */
PP_Resource MakeAndBindGraphics2D(PP_Instance instance,
                                  const struct PP_Size* size) {
  PP_Resource graphics;

  graphics = ih.g_graphics_2d_interface->Create(instance, size, PP_FALSE);
  if (!graphics)
    return 0;

  if (!ih.g_instance_interface->BindGraphics(instance, graphics)) {
    ih.g_core_interface->ReleaseResource(graphics);
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
  image = ih.g_image_data_interface->Create(
      instance->pp_instance, PP_IMAGEDATAFORMAT_BGRA_PREMUL, size, PP_TRUE);
  if (!image)
    return;
  ih.g_image_data_interface->Describe(image, &image_desc);

  /* Fill the image with blue. */
  image_data = (uint32_t*)ih.g_image_data_interface->Map(image);
  if (!image_data) {
    ih.g_core_interface->ReleaseResource(image);
    return;
  }
  num_words = image_desc.stride * size->height / 4;
  for (i = 0; i < num_words; i++)
    image_data[i] = 0xFF0000FF;

  /* Create the graphics 2d and paint the image to it. */
  graphics = MakeAndBindGraphics2D(instance->pp_instance, size);
  if (!graphics) {
    ih.g_core_interface->ReleaseResource(image);
    return;
  }

  ih.g_graphics_2d_interface->ReplaceContents(graphics, image);
  ih.g_graphics_2d_interface->Flush(graphics,
      PP_MakeCompletionCallback(&FlushCompletionCallback, NULL));

  ih.g_core_interface->ReleaseResource(graphics);
  ih.g_core_interface->ReleaseResource(image);
}

PP_Bool Instance_DidCreate(PP_Instance instance,
                           uint32_t argc,
                           const char* argn[],
                           const char* argv[]) {
  struct InstanceInfo* info = &ih.the_instance;
  info->pp_instance = instance;
  info->last_size.width = 0;
  info->last_size.height = 0;

  return PP_TRUE;
}

void Instance_DidDestroy(PP_Instance instance) {
  // Don't destroy anything. It's a demo!
}

void Instance_DidChangeView(PP_Instance pp_instance,
                            const struct PP_Rect* position,
                            const struct PP_Rect* clip) {
  struct InstanceInfo* info = &ih.the_instance;
  if (info->pp_instance != pp_instance) {
    return;
  }

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

/* Global entrypoints --------------------------------------------------------*/

PP_EXPORT int32_t PPP_InitializeModule(PP_Module module,
                                       PPB_GetInterface get_browser_interface) {
  ih.g_get_browser_interface = get_browser_interface;

  ih.g_core_interface = (const struct PPB_Core*)
      get_browser_interface(PPB_CORE_INTERFACE);
  ih.g_instance_interface = (const struct PPB_Instance*)
      get_browser_interface(PPB_INSTANCE_INTERFACE);
  ih.g_image_data_interface = (const struct PPB_ImageData*)
      get_browser_interface(PPB_IMAGEDATA_INTERFACE);
  ih.g_graphics_2d_interface = (const struct PPB_Graphics2D*)
      get_browser_interface(PPB_GRAPHICS_2D_INTERFACE);
  if (!ih.g_core_interface || !ih.g_instance_interface || !ih.g_image_data_interface ||
      !ih.g_graphics_2d_interface)
    return -1;

  return PP_OK;
}

PP_EXPORT void PPP_ShutdownModule() {
}

PP_EXPORT const void* PPP_GetInterface(const char* interface_name) {
  if (str_eq(interface_name, PPP_INSTANCE_INTERFACE)) {
    struct PPP_Instance* ii = &ih.ppp_instance;
    ii->DidCreate = &Instance_DidCreate;
    ii->DidDestroy =  &Instance_DidDestroy;
    ii->DidChangeView = &Instance_DidChangeView;
    ii->DidChangeFocus = &Instance_DidChangeFocus;
    ii->HandleDocumentLoad = &Instance_HandleDocumentLoad;
    return ii;
  }
  return NULL;
}

struct PP_StartFunctions {
  int32_t (*PPP_InitializeModule)(PP_Module module_id,
                                  PPB_GetInterface get_browser_interface);
  void (*PPP_ShutdownModule)();
  const void *(*PPP_GetInterface)(const char *interface_name);
};

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

static int PpapiPluginStart(TYPE_nacl_irt_query query_func) {
  struct PP_StartFunctions funcs = {
    PPP_InitializeModule,
    PPP_ShutdownModule,
    PPP_GetInterface
  };

  struct nacl_irt_ppapihook hooks;
  query_func(NACL_IRT_PPAPIHOOK_v0_1, &hooks, sizeof(hooks));

  return hooks.ppapi_start(&funcs);
}

/*
 * The true entry point for untrusted code is called with the normal C ABI,
 * taking one argument.  This is a pointer to stack space containing these
 * words:
 *      [0]             cleanup function pointer (always NULL in actual startup)
 *      [1]             envc, count of envp[] pointers
 *      [2]             argc, count of argv[] pointers
 *      [3]             argv[0..argc] pointers, argv[argc] being NULL
 *      [3+argc]        envp[0..envc] pointers, envp[envc] being NULL
 *      [3+argc+envc]   auxv[] pairs
 */

enum NaClStartupInfoIndex {
  NACL_STARTUP_FINI,  /* Cleanup function pointer for dynamic linking.  */
  NACL_STARTUP_ENVC,  /* Count of envp[] pointers.  */
  NACL_STARTUP_ARGC,  /* Count of argv[] pointers.  */
  NACL_STARTUP_ARGV   /* argv[0] pointer.  */
};

/*
 * Return the vector of auxiliary data items.
 */
static inline __attribute__((unused))
Elf32_auxv_t *nacl_startup_auxv(const uint32_t info[]) {
  char ** argv = (char**) &info[NACL_STARTUP_ARGV];
  return (Elf32_auxv_t *) &argv[info[NACL_STARTUP_ENVC] + info[NACL_STARTUP_ARGC] + 2];
}

/*
 * Scan the auxv for AT_SYSINFO, which is the pointer to the IRT query function.
 * Stash that for later use.
 */
static void grok_auxv(const Elf32_auxv_t *auxv) {
  const Elf32_auxv_t *av;
  for (av = auxv; av->a_type != AT_NULL; ++av) {
    if (av->a_type == AT_SYSINFO) {
      ih.__nacl_irt_query = (TYPE_nacl_irt_query) av->a_un.a_val;
    }
  }
}

#define DO_QUERY(ident, ih, name)					\
  do_irt_query(ident, &ih.__libnacl_irt_##name,                                  \
               sizeof(ih.__libnacl_irt_##name), NULL) /*&nacl_irt_##name)*/

static void do_irt_query(const char *interface_ident,
                         void *buffer, size_t table_size,
                         const void *fallback) {
  if (NULL == ih.__nacl_irt_query ||
      ih.__nacl_irt_query(interface_ident, buffer, table_size) != table_size) {
    // start panic!
    //    memcpy(buffer, fallback, table_size);
  }
}


/*
 * Initialize all our IRT function tables using the query function.
 * The query function's address is passed via AT_SYSINFO in auxv.
 */
void __libnacl_irt_init(Elf32_auxv_t *auxv) {
  grok_auxv(auxv);

  DO_QUERY(NACL_IRT_BASIC_v0_1, ih, basic);
  DO_QUERY(NACL_IRT_FDIO_v0_1, ih, fdio);
  //  DO_QUERY(NACL_IRT_FILENAME_v0_1, filename);
  //  DO_QUERY(NACL_IRT_MEMORY_v0_1, ih, memory);
  //  DO_QUERY(NACL_IRT_DYNCODE_v0_1, dyncode);
  //  DO_QUERY(NACL_IRT_TLS_v0_1, tls);
  //  DO_QUERY(NACL_IRT_BLOCKHOOK_v0_1, blockhook);
}

/*
 * This is the true entry point for untrusted code.
 * See nacl_startup.h for the layout at the argument pointer.
 */
void _start(uint32_t *info) {
  Elf32_auxv_t *auxv = nacl_startup_auxv(info);

  __libnacl_irt_init(auxv);

  _exit(PpapiPluginStart(ih.__nacl_irt_query));

  /*NOTREACHED*/
  while (1) *(volatile int *) 0;  /* Crash.  */
}

void _exit(int status) {
  ih.__libnacl_irt_basic.exit(status);
  while (1) *(volatile int *) 0;  /* Crash.  */
}

int str_eq(const char *s1, const char *s2) {
  while (*s1 != 0 && *s2 != 0) {
    if (*s1 != *s2) {
      if (*s1 > *s2) {
        return 1;
      } else {
	return -1;
      }
    }
    s1++;
    s2++;
  }
  return (*s1 == 0 && *s2 == 0);
}

//static void *malloc(size_t size) {
//  void *res = 0;
//  int ret = ih.__libnacl_irt_memory.mmap(&res, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
//  return res;
//}

//static ssize_t write(int fd, const void *buf, size_t count) {
//  ssize_t wrote;
//  ih.__libnacl_irt_fdio.write(fd, buf, count, &wrote);
//  return wrote;
//}
