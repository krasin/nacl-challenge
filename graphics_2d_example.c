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
#include "ppapi/c/pp_var.h"
#include "ppapi/c/ppb.h"
#include "ppapi/c/ppb_core.h"

// #include "ppapi/c/pp_rect.h"
#ifndef PPAPI_C_PP_RECT_H_
#define PPAPI_C_PP_RECT_H_

#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"

/**
 * @file
 * This file defines the APIs for creating a 2 dimensional rectangle.
 */


/**
 * @addtogroup Structs
 * @{
 */
/**
 * The <code>PP_Rect</code> struct contains the size and location of a 2D
 * rectangle.
 */
struct PP_Rect {
  /**
   * This value represents the x and y coordinates of the upper-left corner of
   * the rectangle.
   */
  struct PP_Point point;
  /** This value represents the width and height of the rectangle. */
  struct PP_Size size;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_Rect, 16);
/**
 * @}
 */


/**
 * @addtogroup Functions
 * @{
 */

/**
 * PP_MakeRectFromXYWH() creates a <code>PP_Rect</code> given x and y
 * coordinates and width and height dimensions as int32_t values.
 *
 * @param[in] x An int32_t value representing a horizontal coordinate of a
 * point, starting with 0 as the left-most coordinate.
 * @param[in] y An int32_t value representing a vertical coordinate of a point,
 * starting with 0 as the top-most coordinate.
 * @param[in] w An int32_t value representing a width.
 * @param[in] h An int32_t value representing a height.
 *
 * @return A <code>PP_Rect</code> structure.
 */
PP_INLINE struct PP_Rect PP_MakeRectFromXYWH(int32_t x, int32_t y,
                                             int32_t w, int32_t h) {
  struct PP_Rect ret;
  ret.point.x = x;
  ret.point.y = y;
  ret.size.width = w;
  ret.size.height = h;
  return ret;
}
/**
 * @}
 */

#endif  /* PPAPI_C_PP_RECT_H_ */


//#include "ppapi/c/ppb_graphics_2d.h"

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

//#include "ppapi/c/ppb_image_data.h"

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

//#include "ppapi/c/ppb_instance.h"

#define PPB_INSTANCE_INTERFACE_0_5 "PPB_Instance;0.5"
#define PPB_INSTANCE_INTERFACE_1_0 "PPB_Instance;1.0"
#define PPB_INSTANCE_INTERFACE PPB_INSTANCE_INTERFACE_1_0

struct PPB_Instance {
  PP_Bool (*BindGraphics)(PP_Instance instance, PP_Resource device);
  PP_Bool (*IsFullFrame)(PP_Instance instance);
};


//#include "ppapi/c/ppp.h"

#define PP_EXPORT __attribute__ ((visibility("default")))

PP_EXPORT int32_t PPP_InitializeModule(PP_Module module,
                                       PPB_GetInterface get_browser_interface);
PP_EXPORT void PPP_ShutdownModule();
PP_EXPORT const void* PPP_GetInterface(const char* interface_name);

//#include "ppapi/c/ppp_instance.h"

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

