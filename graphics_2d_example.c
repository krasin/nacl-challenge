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
#include "ppapi/c/ppb_graphics_2d.h"
//#include "ppapi/c/ppb_image_data.h"

#ifndef PPAPI_C_PPB_IMAGE_DATA_H_
#define PPAPI_C_PPB_IMAGE_DATA_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"

/**
 * @file
 * This file defines the <code>PPB_ImageData</code> struct for determining how
 * a browser handles image data.
 */


/**
 * @addtogroup Enums
 * @{
 */
/**
 * <code>PP_ImageDataFormat</code> is an enumeration of the different types of
 * image data formats.
 *
 * The third part of each enumeration value describes the memory layout from
 * the lowest address to the highest. For example, BGRA means the B component
 * is stored in the lowest address, no matter what endianness the platform is
 * using.
 *
 * The PREMUL suffix implies pre-multiplied alpha is used. In this mode, the
 * red, green and blue color components of the pixel data supplied to an image
 * data should be pre-multiplied by their alpha value. For example: starting
 * with floating point color components, here is how to convert them to 8-bit
 * premultiplied components for image data:
 *
 * ...components of a pixel, floats ranging from 0 to 1...
 * <code>float red = 1.0f;</code>
 * <code><code>float green = 0.50f;</code>
 * <code>float blue = 0.0f;</code>
 * <code>float alpha = 0.75f;</code>
 * ...components for image data are 8-bit values ranging from 0 to 255...
 * <code>uint8_t image_data_red_premul = (uint8_t)(red * alpha * 255.0f);
 * </code>
 * <code>uint8_t image_data_green_premul = (uint8_t)(green * alpha * 255.0f);
 * </code>
 * <code>uint8_t image_data_blue_premul = (uint8_t)(blue * alpha * 255.0f);
 * </code>
 * <code>uint8_t image_data_alpha_premul = (uint8_t)(alpha * 255.0f);</code>
 *
 * <strong>Note:</strong> The resulting pre-multiplied red, green and blue
 * components should not be greater than the alpha value.
 */
typedef enum {
  PP_IMAGEDATAFORMAT_BGRA_PREMUL,
  PP_IMAGEDATAFORMAT_RGBA_PREMUL
} PP_ImageDataFormat;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_ImageDataFormat, 4);
/**
 * @}
 */

/**
 * @addtogroup Structs
 * @{
 */
/**
 * The <code>PP_ImageDataDesc</code> structure represents a description of
 * image data.
 */
struct PP_ImageDataDesc {
  /**
   * This value represents one of the image data types in the
   * <code>PP_ImageDataFormat</code> enum.
   */
  PP_ImageDataFormat format;
  /** This value represents the size of the bitmap in pixels. */
  struct PP_Size size;
  /**
   * This value represents the row width in bytes. This may be different than
   * width * 4 since there may be padding at the end of the lines.
   */
  int32_t stride;
};
PP_COMPILE_ASSERT_STRUCT_SIZE_IN_BYTES(PP_ImageDataDesc, 16);
/**
 * @}
 */

/**
 * @addtogroup Interfaces
 * @{
 */
/**
 * The <code>PPB_ImageData</code> interface contains pointers to several
 * functions for determining the browser's treatment of image data.
 */
#define PPB_IMAGEDATA_INTERFACE_0_3 "PPB_ImageData;0.3"
#define PPB_IMAGEDATA_INTERFACE_1_0 "PPB_ImageData;1.0"
#define PPB_IMAGEDATA_INTERFACE PPB_IMAGEDATA_INTERFACE_1_0

struct PPB_ImageData {
  /**
   * GetNativeImageDataFormat() returns the browser's preferred format for
   * image data. The browser uses this format internally for painting. Other
   * formats may require internal conversions to paint or may have additional
   * restrictions depending on the function.
   *
   * @return A <code>PP_ImageDataFormat</code> containing the preferred format.
   */
  PP_ImageDataFormat (*GetNativeImageDataFormat)();
  /**
   * IsImageDataFormatSupported() determines if the given image data format is
   * supported by the browser.
   *
   * @param[in] format The image data format.
   *
   * @return A <code>PP_Bool</code> with <code>PP_TRUE</code> if the given
   * image data format is supported by the browser.
   */
  PP_Bool (*IsImageDataFormatSupported)(PP_ImageDataFormat format);
  /**
   * Create() allocates an image data resource with the given format and size.
   *
   * For security reasons, if uninitialized, the bitmap will not contain random
   * memory, but may contain data from a previous image produced by the same
   * module if the bitmap was cached and re-used.
   *
   * @param[in] instance A <code>PP_Instance</code> indentifying one instance
   * of a module.
   * @param[in] format The desired image data format.
   * @param[in] size A pointer to a <code>PP_Size</code> containing the image
   * size.
   * @param[in] init_to_zero A <code>PP_Bool</code> to determine transparency
   * at creation.
   * Set the <code>init_to_zero</code> flag if you want the bitmap initialized
   * to transparent during the creation process. If this flag is not set, the
   * current contents of the bitmap will be undefined, and the module should
   * be sure to set all the pixels.
   *
   * @return A <code>PP_Resource</code> with a nonzero ID on succes or zero on
   * failure. Failure means the instance, image size, or format was invalid.
   */
  PP_Resource (*Create)(PP_Instance instance,
                        PP_ImageDataFormat format,
                        const struct PP_Size* size,
                        PP_Bool init_to_zero);
  /**
   * IsImageData() determiens if a given resource is image data.
   *
   * @param[in] image_data A <code>PP_Resource</code> corresponding to image
   * data.
   *
   * @return A <code>PP_Bool</code> with <code>PP_TRUE</code> if the given
   * resrouce is an image data or <code>PP_FALSE</code> if the resource is
   * invalid or some type other than image data.
   */
  PP_Bool (*IsImageData)(PP_Resource image_data);
  /**
   * Describe() computes the description of the
   * image data.
   *
   * @param[in] image_data A <code>PP_Resource</code> corresponding to image
   * data.
   * @param[in,out] desc A pointer to a <code>PP_ImageDataDesc</code>
   * containing the description.
   *
   * @return A <code>PP_Bool</code> with <code>PP_TRUE</code> on success or
   * <code>PP_FALSE</code> if the resource is not an image data. On
   * <code>PP_FALSE</code>, the <code>desc</code> structure will be filled
   * with 0.
   */
  PP_Bool (*Describe)(PP_Resource image_data, struct PP_ImageDataDesc* desc);
  /**
   * Map() maps an image data into the module address space.
   *
   * @param[in] image_data A <code>PP_Resource</code> corresponding to image
   * data.
   *
   * @return A pointer to the beginning of the data.
   */
  void* (*Map)(PP_Resource image_data);
  /**
   * Unmap is a pointer to a function that unmaps an image data from the module
   * address space.
   *
   * @param[in] image_data A <code>PP_Resource</code> corresponding to image
   * data.
   */
  void (*Unmap)(PP_Resource image_data);
};
/**
 * @}
 */

#endif  /* PPAPI_C_PPB_IMAGE_DATA_H_ */



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

