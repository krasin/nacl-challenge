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
#include "ppapi/c/ppb_image_data.h"
#include "ppapi/c/ppb_instance.h"
#include "ppapi/c/ppp.h"
//#include "ppapi/c/ppp_instance.h"

#define PPP_INSTANCE_INTERFACE_1_0 "PPP_Instance;1.0"
#define PPP_INSTANCE_INTERFACE PPP_INSTANCE_INTERFACE_1_0

struct PPP_Instance {
  /**
   * DidCreate() is a creation handler that is called when a new instance is
   * created. This function is called for each instantiation on the page,
   * corresponding to one \<embed\> tag on the page.
   *
   * Generally you would handle this call by initializing the information
   * your module associates with an instance and creating a mapping from the
   * given <code>PP_Instance</code> handle to this data. The
   * <code>PP_Instance</code> handle will be used in subsequent calls to
   * identify which instance the call pertains to.
   *
   * It's possible for more than one instance to be created in a single module.
   * This means that you may get more than one <code>OnCreate</code> without an
   * <code>OnDestroy</code> in between, and should be prepared to maintain
   * multiple states associated with each instance.
   *
   * If this function reports a failure (by returning <code>PP_FALSE</code>),
   * the instance will be deleted.
   *
   * @param[in] instance A new <code>PP_Instance</code> indentifying one
   * instance of a module. This is an opaque handle.
   *
   * @param[in] argc The number of arguments contained in <code>argn</code>
   * and <code>argv</code>.
   *
   * @param[in] argn An array of argument names.  These argument names are
   * supplied in the \<embed\> tag, for example:
   * <code>\<embed id="nacl_module" dimensions="2"\></code> will produce two
   * argument names: "id" and "dimensions."
   *
   * @param[in] argv An array of argument values.  These are the values of the
   * arguments listed in the \<embed\> tag, for example
   * <code>\<embed id="nacl_module" dimensions="2"\></code> will produce two
   * argument values: "nacl_module" and "2".  The indices of these values match
   * the indices of the corresponding names in <code>argn</code>.
   *
   * @return <code>PP_TRUE</code> on success or <code>PP_FALSE</code> on
   * failure.
   */
  PP_Bool (*DidCreate)(PP_Instance instance,
                       uint32_t argc,
                       const char* argn[],
                       const char* argv[]);
  /**
   * DidDestroy() is an instance destruction handler. This function is called
   * in many cases (see below) when a module instance is destroyed. It will be
   * called even if DidCreate() returned failure.
   *
   * Generally you will handle this call by deallocating the tracking
   * information and the <code>PP_Instance</code> mapping you created in the
   * DidCreate() call. You can also free resources associated with this
   * instance but this isn't required; all resources associated with the deleted
   * instance will be automatically freed when this function returns.
   *
   * The instance identifier will still be valid during this call, so the module
   * can perform cleanup-related tasks. Once this function returns, the
   * <code>PP_Instance</code> handle will be invalid. This means that you can't
   * do any asynchronous operations like network requests, file writes or
   * messaging from this function since they will be immediately canceled.
   *
   * <strong>Note:</strong> This function will always be skipped on untrusted
   * (Native Client) implementations. This function may be skipped on trusted
   * implementations in certain circumstances when Chrome does "fast shutdown"
   * of a web page. Fast shutdown will happen in some cases when all module
   * instances are being deleted, and no cleanup functions will be called.
   * The module will just be unloaded and the process terminated.
   *
   * @param[in] instance A <code>PP_Instance</code> indentifying one instance
   * of a module.
   */
  void (*DidDestroy)(PP_Instance instance);
  /**
   * DidChangeView() is called when the position, the size, of the clip
   * rectangle of the element in the browser that corresponds to this
   * instance has changed.
   *
   * A typical implementation will check the size of the <code>position</code>
   * argument and reallocate the graphics context when a different size is
   * received. Note that this function will be called for scroll events where
   * the size doesn't change, so you should always check that the size is
   * actually different before doing any reallocations.
   *
   * @param[in] instance A <code>PP_Instance</code> indentifying the instance
   * that has changed.
   *
   * @param[in] position The location on the page of the instance. This is
   * relative to the top left corner of the viewport, which changes as the
   * page is scrolled. Generally the size of this value will be used to create
   * a graphics device, and the position is ignored (most things are relative
   * to the instance so the absolute position isn't useful in most cases).
   *
   * @param[in] clip The visible region of the instance. This is relative to
   * the top left of the module's coordinate system (not the page).  If the
   * module is invisible, <code>clip</code> will be (0, 0, 0, 0).
   *
   * It's recommended to check for invisible instances and to stop
   * generating graphics updates in this case to save system resources. It's
   * not usually worthwhile, however, to generate partial updates according to
   * the clip when the instance is partially visible. Instead, update the entire
   * region. The time saved doing partial paints is usually not significant and
   * it can create artifacts when scrolling (this notification is sent
   * asynchronously from scolling so there can be flashes of old content in the
   * exposed regions).
   */
  void (*DidChangeView)(PP_Instance instance,
                        const struct PP_Rect* position,
                        const struct PP_Rect* clip);
  /**
   * DidChangeFocus() is called when an instance has gained or lost focus.
   * Having focus means that keyboard events will be sent to the instance.
   * An instance's default condition is that it will not have focus.
   *
   * <strong>Note:</strong>Clicks on instances will give focus only if you
   * handle the click event. Return <code>true</code> from
   * <code>HandleInputEvent</code> in <code>PPP_InputEvent</code> (or use
   * unfiltered events) to signal that the click event was handled. Otherwise,
   * the browser will bubble the event and give focus to the element on the page
   * that actually did end up consuming it. If you're not getting focus, check
   * to make sure you're returning true from the mouse click in
   * <code>HandleInputEvent</code>.
   *
   * @param[in] instance A <code>PP_Instance</code> indentifying the instance
   * receiving the input event.
   *
   * @param[in] has_focus Indicates the new focused state of the instance.
   */
  void (*DidChangeFocus)(PP_Instance instance, PP_Bool has_focus);
  /**
   * HandleDocumentLoad() is called after initialize for a full-frame
   * module that was instantiated based on the MIME type of a DOMWindow
   * navigation. This situation only applies to modules that are pre-registered
   * to handle certain MIME types. If you haven't specifically registered to
   * handle a MIME type or aren't positive this applies to you, your
   * implementation of this function can just return <code>PP_FALSE</code>.
   *
   * The given <code>url_loader</code> corresponds to a
   * <code>PPB_URLLoader</code> instance that is already opened. Its response
   * headers may be queried using <code>PPB_URLLoader::GetResponseInfo</code>.
   * The reference count for the URL loader is not incremented automatically on
   * behalf of the module. You need to increment the reference count yourself
   * if you are going to keep a reference to it.
   *
   * This method returns <code>PP_FALSE</code> if the module cannot handle the
   * data. In response to this method, the module should call
   * ReadResponseBody() to read the incoming data.
   *
   * @param[in] instance A <code>PP_Instance</code> indentifying the instance
   * that should do the load.
   *
   * @param[in] url_loader An open <code>PPB_URLLoader</code> instance.
   *
   * @return <code>PP_TRUE</code> if the data was handled,
   * <code>PP_FALSE</code> otherwise.
   */
  PP_Bool (*HandleDocumentLoad)(PP_Instance instance, PP_Resource url_loader);
};
/**
 * @}
 */


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

