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
//#include "ppapi/c/ppb_graphics_2d.h"

#ifndef PPAPI_C_PPB_GRAPHICS_2D_H_
#define PPAPI_C_PPB_GRAPHICS_2D_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_stdint.h"

/**
 * @file
 * Defines the <code>PPB_Graphics2D</code> struct representing a 2D graphics
 * context within the browser.
 */


/**
 * @addtogroup Interfaces
 * @{
 */
/**
 * <code>PPB_Graphics2D</code> defines the interface for a 2D graphics context.
 */
#define PPB_GRAPHICS_2D_INTERFACE_0_4 "PPB_Graphics2D;0.4"
#define PPB_GRAPHICS_2D_INTERFACE_1_0 "PPB_Graphics2D;1.0"
#define PPB_GRAPHICS_2D_INTERFACE PPB_GRAPHICS_2D_INTERFACE_1_0

struct PPB_Graphics2D {
  /**
   * Create() creates a 2D graphics context. The returned graphics context will
   * not be bound to the module instance on creation (call BindGraphics() on
   * the module instance to bind the returned graphics context to the module
   * instance).
   *
   * @param[in] instance The module instance.
   * @param[in] size The size of the graphic context.
   * @param[in] is_always_opaque Set the <code>is_always_opaque</code> flag to
   * <code>PP_TRUE</code> if you know that you will be painting only opaque
   * data to this context. This option will disable blending when compositing
   * the module with the web page, which might give higher performance on some
   * computers.
   *
   * If you set <code>is_always_opaque</code>, your alpha channel should always
   * be set to 0xFF or there may be painting artifacts. The alpha values
   * overwrite the destination alpha values without blending when
   * <code>is_always_opaque</code> is true.
   *
   * @return A <code>PP_Resource</code> containing the 2D graphics context if
   * successful or 0 if unsuccessful.
   */
  PP_Resource (*Create)(PP_Instance instance,
                        const struct PP_Size* size,
                        PP_Bool is_always_opaque);
  /**
   * IsGraphics2D() determines if the given resource is a valid
   * <code>Graphics2D</code>.
   *
   * @param[in] resource A <code>Graphics2D</code> context resource.
   *
   * @return PP_TRUE if the given resource is a valid <code>Graphics2D</code>,
   * <code>PP_FALSE</code> if it is an invalid resource or is a resource of
   * another type.
   */
  PP_Bool (*IsGraphics2D)(PP_Resource resource);
  /**
   * Describe() retrieves the configuration for the given graphics context,
   * filling the given values (which must not be <code>NULL</code>).
   *
   * @param[in] resource The 2D Graphics resource.
   * @param[in,out] size The size of the 2D graphics context in the browser.
   * @param[in,out] is_always_opaque Identifies whether only opaque data
   * will be painted.
   *
   * @return Returns <code>PP_TRUE</code> on succes or <code>PP_FALSE</code> if
   * the resource is invalid. The output parameters will be set to 0 on a
   * <code>PP_FALSE</code>.
   */
  PP_Bool (*Describe)(PP_Resource graphics_2d,
                      struct PP_Size* size,
                      PP_Bool* is_always_opqaue);
  /**
   * PaintImageData() enqueues a paint of the given image into the context.
   * This function has no effect until you call Flush() As a result, what
   * counts is the contents of the bitmap when you call Flush(), not when
   * you call this function.
   *
   * The provided image will be placed at <code>top_left</code> from the top
   *  left of the context's internal backing store. Then the pixels contained
   * in <code>src_rect</code> will be copied into the backing store. This
   * means that the rectangle being painted will be at <code>src_rect</code>
   * offset by <code>top_left</code>.
   *
   * The <code>src_rect</code> is specified in the coordinate system of the
   * image being painted, not the context. For the common case of copying the
   * entire image, you may specify an empty <code>src_rect</code>.
   *
   * The painted area of the source bitmap must fall entirely within the
   * context. Attempting to paint outside of the context will result in an
   * error. However, the source bitmap may fall outside the context, as long
   * as the <code>src_rect</code> subset of it falls entirely within the
   * context.
   *
   * There are two methods most modules will use for painting. The first
   * method is to generate a new <code>ImageData</code> and then paint it. In
   * this case, you'll set the location of your painting to
   * <code>top_left</code> and set <code>src_rect</code> to <code>NULL</code>.
   * The second is that you're generating small invalid regions out of a larger
   * bitmap representing your entire instance. In this case, you would set the
   * location of your image to (0,0) and then set <code>src_rect</code> to the
   * pixels you changed.
   *
   * @param[in] resource The 2D Graphics resource.
   * @param[in] image The <code>ImageData</code> to be painted.
   * @param[in] top_left A <code>Point</code> representing the
   * <code>top_left</code> location where the <code>ImageData</code> will be
   * painted.
   * @param[in] src_rect The rectangular area where the <code>ImageData</code>
   * will be painted.
   */
  void (*PaintImageData)(PP_Resource graphics_2d,
                         PP_Resource image_data,
                         const struct PP_Point* top_left,
                         const struct PP_Rect* src_rect);
  /**
   * Scroll() enqueues a scroll of the context's backing store. This
   * function has no effect until you call Flush(). The data within the
   * provided clipping rectangle will be shifted by (dx, dy) pixels.
   *
   * This function will result in some exposed region which will have undefined
   * contents. The module should call PaintImageData() on these exposed regions
   * to give the correct contents.
   *
   * The scroll can be larger than the area of the clipping rectangle, which
   * means the current image will be scrolled out of the rectangle. This
   * scenario is not an error but will result in a no-op.
   *
   * @param[in] graphics_2d The 2D Graphics resource.
   * @param[in] clip The clipping rectangle.
   * @param[in] amount The amount the area in the clipping rectangle will
   * shifted.
   */
  void (*Scroll)(PP_Resource graphics_2d,
                 const struct PP_Rect* clip_rect,
                 const struct PP_Point* amount);
  /**
   * ReplaceContents() provides a slightly more efficient way to paint the
   * entire module's image. Normally, calling PaintImageData() requires that
   * the browser copy the pixels out of the image and into the graphics
   * context's backing store. This function replaces the graphics context's
   * backing store with the given image, avoiding the copy.
   *
   * The new image must be the exact same size as this graphics context. If the
   * new image uses a different image format than the browser's native bitmap
   * format (use <code>PPB_ImageData.GetNativeImageDataFormat()</code> to
   * retrieve the format), then a conversion will be done inside the browser
   * which may slow the performance a little bit.
   *
   * <strong>Note:</strong> The new image will not be painted until you call
   * Flush().
   *
   * After this call, you should take care to release your references to the
   * image. If you paint to the image after ReplaceContents(), there is the
   * possibility of significant painting artifacts because the page might use
   * partially-rendered data when copying out of the backing store.
   *
   * In the case of an animation, you will want to allocate a new image for the
   * next frame. It is best if you wait until the flush callback has executed
   * before allocating this bitmap. This gives the browser the option of
   * caching the previous backing store and handing it back to you (assuming
   * the sizes match). In the optimal case, this means no bitmaps are allocated
   * during the animation, and the backing store and "front buffer" (which the
   * plugin is painting into) are just being swapped back and forth.
   *
   * @param[in] graphics_2d The 2D Graphics resource.
   * @param[in] image The <code>ImageData</code> to be painted.
   */
  void (*ReplaceContents)(PP_Resource graphics_2d, PP_Resource image_data);
  /**
   * Flush() flushes any enqueued paint, scroll, and replace commands to the
   * backing store. This function actually executes the updates, and causes a
   * repaint of the webpage, assuming this graphics context is bound to a module
   * instance.
   *
   * Flush() runs in asynchronous mode. Specify a callback function and the
   * argument for that callback function. The callback function will be
   * executed on the calling thread when the image has been painted to the
   * screen. While you are waiting for a flush callback, additional calls to
   * Flush() will fail.
   *
   * Because the callback is executed (or thread unblocked) only when the
   * instance's image is actually on the screen, this function provides
   * a way to rate limit animations. By waiting until the image is on the
   * screen before painting the next frame, you can ensure you're not
   * flushing 2D graphics faster than the screen can be updated.
   *
   * <strong>Unbound contexts</strong>
   * If the context is not bound to a module instance, you will
   * still get a callback. The callback will execute after Flush() returns
   * to avoid reentrancy. The callback will not wait until anything is
   * painted to the screen because there will be nothing on the screen. The
   * timing of this callback is not guaranteed and may be deprioritized by
   * the browser because it is not affecting the user experience.
   *
   * <strong>Off-screen instances</strong>
   * If the context is bound to an instance that is currently not visible (for
   * example, scrolled out of view) it will behave like the "unbound context"
   * case.
   *
   * <strong>Detaching a context</strong>
   * If you detach a context from a module instance, any pending flush
   * callbacks will be converted into the "unbound context" case.
   *
   * <strong>Released contexts</strong>
   * A callback may or may not get called even if you have released all
   * of your references to the context. This scenario can occur if there are
   * internal references to the context suggesting it has not been internally
   * destroyed (for example, if it is still bound to an instance) or due to
   * other implementation details. As a result, you should be careful to
   * check that flush callbacks are for the context you expect and that
   * you're capable of handling callbacks for unreferenced contexts.
   *
   * <strong>Shutdown</strong>
   * If a module instance is removed when a flush is pending, the
   * callback will not be executed.
   *
   * @param[in] graphics_2d The 2D Graphics resource.
   * @param[in] callback A <code>CompletionCallback</code> to be called when
   * the image has been painted on the screen.
   *
   * @return Returns <code>PP_OK</code> on success or
   * <code>PP_Error_BadResource</code> if the graphics context is invalid,
   * <code>PP_Error_BadArgument</code> if the callback is null and flush is
   * being called from the main thread of the module, or
   * <code>PP_Error_InProgress</code> if a flush is already pending that has
   * not issued its callback yet.  In the failure case, nothing will be updated
   * and no callback will be scheduled.
   */
  /*
   * TODO(darin): We should ensure that the completion callback always runs, so
   * that it is easier for consumers to manage memory referenced by a callback.
   */
  /*
   * TODO(): Add back in the synchronous mode description once we have support
   * for it.
   */
  int32_t (*Flush)(PP_Resource graphics_2d,
                   struct PP_CompletionCallback callback);
};
/**
 * @}
 */

#endif  /* PPAPI_C_PPB_GRAPHICS_2D_H_ */


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

