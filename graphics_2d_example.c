/* Copyright (c) 2011 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#include <stdlib.h>
#include <string.h>

#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_size.h"
// #include "ppapi/c/pp_var.h"

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_stdint.h"

/**
 * @file
 * This file defines the API for handling the passing of data types between
 * your module and the page.
 */


/**
 * @addtogroup Enums
 * @{
 */
/**
 * The <code>PP_VarType</code> is an enumeration of the different types that
 * can be contained within a <code>PP_Var</code> structure.
 */
typedef enum {
  /**
   * An undefined value.
   */
  PP_VARTYPE_UNDEFINED,
  /**
   * A NULL value. This is similar to undefined, but JavaScript differentiates
   * the two so it is exposed here as well.
   */
  PP_VARTYPE_NULL,
  /**
   * A boolean value, use the <code>as_bool</code> member of the var.
   */
  PP_VARTYPE_BOOL,
  /**
   * A 32-bit integer value. Use the <code>as_int</code> member of the var.
   */
  PP_VARTYPE_INT32,
  /**
   * A double-precision floating point value. Use the <code>as_double</code>
   * member of the var.
   */
  PP_VARTYPE_DOUBLE,
  /**
   * The Var represents a string. The <code>as_id</code> field is used to
   * identify the string, which may be created and retrieved from the
   * <code>PPB_Var</code> interface.
   */
  PP_VARTYPE_STRING,
  /**
   * Represents a JavaScript object. This vartype is not currently usable
   * from modules, although it is used internally for some tasks.
   */
  PP_VARTYPE_OBJECT,
  /**
   * Arrays and dictionaries are not currently supported but will be added
   * in future revisions. These objects are reference counted so be sure
   * to properly AddRef/Release them as you would with strings to ensure your
   * module will continue to work with future versions of the API.
   */
  PP_VARTYPE_ARRAY,
  PP_VARTYPE_DICTIONARY
} PP_VarType;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_VarType, 4);
/**
 * @}
 */

/**
 * @addtogroup Structs
 * @{
 */
/**
 * The PP_VarValue union stores the data for any one of the types listed
 * in the PP_VarType enum.
 */
union PP_VarValue {
  /**
   * If <code>type</code> is <code>PP_VARTYPE_BOOL</code>,
   * <code>as_bool</code> represents the value of this <code>PP_Var</code> as
   * <code>PP_Bool</code>.
   */
  PP_Bool as_bool;
  /**
   * If <code>type</code> is <code>PP_VARTYPE_INT32</code>,
   * <code>as_int</code> represents the value of this <code>PP_Var</code> as
   * <code>int32_t</code>.
   */
  int32_t as_int;
  /**
   * If <code>type</code> is <code>PP_VARTYPE_DOUBLE</code>,
   * <code>as_double</code> represents the value of this <code>PP_Var</code>
   * as <code>double</code>.
   */
  double as_double;
  /**
   * If <code>type</code> is <code>PP_VARTYPE_STRING</code>,
   * <code>PP_VARTYPE_OBJECT</code>, <code>PP_VARTYPE_ARRAY</code>, or
   * <code>PP_VARTYPE_DICTIONARY</code>,
   * <code>as_id</code> represents the value of this <code>PP_Var</code> as
   * an opaque handle assigned by the browser. This handle is guaranteed
   * never to be 0, so a module can initialize this ID to 0 to indicate a
   * "NULL handle."
   */
  int64_t as_id;
};

/**
 * The <code>PP_VAR</code> struct is a variant data type and can contain any
 * value of one of the types named in the <code>PP_VarType</code> enum. This
 * structure is for passing data between native code which can be strongly
 * typed and the browser (JavaScript) which isn't strongly typed.
 *
 * JavaScript has a "number" type for holding a number, and does not
 * differentiate between floating point and integer numbers. The
 * JavaScript operations will try to optimize operations by using
 * integers when possible, but could end up with doubles. Therefore,
 * you can't assume a numeric <code>PP_Var</code> will be the type you expect.
 * Your code should be capable of handling either int32_t or double for numeric
 * PP_Vars sent from JavaScript.
 */
struct PP_Var {
  PP_VarType type;
  /**
   * The <code>padding</code> ensures <code>value</code> is aligned on an
   * 8-byte boundary relative to the start of the struct. Some compilers
   * align doubles on 8-byte boundaries for 32-bit x86, and some align on
   * 4-byte boundaries.
   */
  int32_t padding;
  /**
   * This <code>value</code> represents the contents of the PP_Var. Only one of
   * the fields of <code>value</code> is valid at a time based upon
   * <code>type</code>.
   */
  union PP_VarValue value;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_Var, 16);
/**
 * @}
 */

/**
 * @addtogroup Functions
 * @{
 */

/**
 * PP_MakeUndefined() is used to wrap an undefined value into a
 * <code>PP_Var</code> struct for passing to the browser.
 *
 * @return A <code>PP_Var</code> structure.
 */
PP_INLINE struct PP_Var PP_MakeUndefined() {
  struct PP_Var result = { PP_VARTYPE_UNDEFINED, 0, {PP_FALSE} };
  return result;
}

/**
 * PP_MakeNull() is used to wrap a null value into a
 * <code>PP_Var</code> struct for passing to the browser.
 *
 * @return A <code>PP_Var</code> structure,
 */
PP_INLINE struct PP_Var PP_MakeNull() {
  struct PP_Var result = { PP_VARTYPE_NULL, 0, {PP_FALSE} };
  return result;
}

/**
 * PP_MakeBool() is used to wrap a boolean value into a
 * <code>PP_Var</code> struct for passing to the browser.
 *
 * @param[in] value A <code>PP_Bool</code> enumeration to
 * wrap.
 *
 * @return A <code>PP_Var</code> structure.
 */
PP_INLINE struct PP_Var PP_MakeBool(PP_Bool value) {
  struct PP_Var result = { PP_VARTYPE_BOOL, 0, {PP_FALSE} };
  result.value.as_bool = value;
  return result;
}

/**
 * PP_MakeInt32() is used to wrap a 32 bit integer value
 * into a <code>PP_Var</code> struct for passing to the browser.
 *
 * @param[in] value An int32 to wrap.
 *
 * @return A <code>PP_Var</code> structure.
 */
PP_INLINE struct PP_Var PP_MakeInt32(int32_t value) {
  struct PP_Var result = { PP_VARTYPE_INT32, 0, {PP_FALSE} };
  result.value.as_int = value;
  return result;
}

/**
 * PP_MakeDouble() is used to wrap a double value into a
 * <code>PP_Var</code> struct for passing to the browser.
 *
 * @param[in] value A double to wrap.
 *
 * @return A <code>PP_Var</code> structure.
 */
PP_INLINE struct PP_Var PP_MakeDouble(double value) {
  struct PP_Var result = { PP_VARTYPE_DOUBLE, 0, {PP_FALSE} };
  result.value.as_double = value;
  return result;
}

// #include "ppapi/c/ppb.h"

typedef const void* (*PPB_GetInterface)(const char* interface_name);

// #include "ppapi/c/pp_resource.h"

typedef int32_t PP_Resource;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_Resource, 4);

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
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_Point, 8);

struct PP_FloatPoint {
  float x;
  float y;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_FloatPoint, 8);

// #include "ppapi/c/pp_rect.h"

struct PP_Rect {
  struct PP_Point point;
  struct PP_Size size;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_Rect, 16);

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
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_ImageDataFormat, 4);

struct PP_ImageDataDesc {
  PP_ImageDataFormat format;
  struct PP_Size size;
  int32_t stride;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_ImageDataDesc, 16);

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
