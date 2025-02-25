/*
 * GStreamer
 * Copyright (C) 2015 Matthew Waters <matthew@centricular.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __GST_VULKAN_WINDOW_H__
#define __GST_VULKAN_WINDOW_H__

#include <gst/gst.h>

#include <gst/vulkan/vulkan.h>

G_BEGIN_DECLS

#define GST_TYPE_VULKAN_WINDOW         (gst_vulkan_window_get_type())
#define GST_VULKAN_WINDOW(o)           (G_TYPE_CHECK_INSTANCE_CAST((o), GST_TYPE_VULKAN_WINDOW, GstVulkanWindow))
#define GST_VULKAN_WINDOW_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), GST_TYPE_VULKAN_WINDOW, GstVulkanWindowClass))
#define GST_IS_VULKAN_WINDOW(o)        (G_TYPE_CHECK_INSTANCE_TYPE((o), GST_TYPE_VULKAN_WINDOW))
#define GST_IS_VULKAN_WINDOW_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE((k), GST_TYPE_VULKAN_WINDOW))
#define GST_VULKAN_WINDOW_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS((o), GST_TYPE_VULKAN_WINDOW, GstVulkanWindowClass))
GST_VULKAN_API
GType gst_vulkan_window_get_type     (void);

#define GST_VULKAN_WINDOW_LOCK(w) g_mutex_lock(&GST_VULKAN_WINDOW(w)->lock)
#define GST_VULKAN_WINDOW_UNLOCK(w) g_mutex_unlock(&GST_VULKAN_WINDOW(w)->lock)
#define GST_VULKAN_WINDOW_GET_LOCK(w) (&GST_VULKAN_WINDOW(w)->lock)

#define GST_VULKAN_WINDOW_ERROR (gst_vulkan_window_error_quark ())
GST_VULKAN_API
GQuark gst_vulkan_window_error_quark (void);

typedef enum
{
  GST_VULKAN_WINDOW_ERROR_FAILED,
  GST_VULKAN_WINDOW_ERROR_OLD_LIBS,
  GST_VULKAN_WINDOW_ERROR_RESOURCE_UNAVAILABLE,
} GstVulkanWindowError;

/**
 * GstVulkanWindow:
 *
 * #GstVulkanWindow is an opaque struct and should only be accessed through the
 * provided api.
 */
struct _GstVulkanWindow {
  /*< private >*/
  GstObject parent;

  GstVulkanDisplay       *display;

  GMutex                  lock;

  gpointer                _reserved[GST_PADDING];
};

/**
 * GstVulkanWindowClass:
 * @parent_class: Parent class
 * @open: open the connection to the display
 * @close: close the connection to the display
 */
struct _GstVulkanWindowClass {
  GstObjectClass parent_class;

  gboolean      (*open)                         (GstVulkanWindow *window,
                                                 GError **error);
  void          (*close)                        (GstVulkanWindow *window);

  VkSurfaceKHR  (*get_surface)                  (GstVulkanWindow *window,
                                                 GError **error);
  gboolean      (*get_presentation_support)     (GstVulkanWindow *window,
                                                 GstVulkanDevice *device,
                                                 guint32 queue_family_idx);
  void          (*set_window_handle)            (GstVulkanWindow *window,
                                                 guintptr handle);
  void          (*get_surface_dimensions)       (GstVulkanWindow *window,
                                                 guint * width,
                                                 guint * height);

  /*< private >*/
  gpointer _reserved[GST_PADDING];
};

GST_VULKAN_API
GstVulkanWindow *  gst_vulkan_window_new                            (GstVulkanDisplay *display);

GST_VULKAN_API
GstVulkanDisplay * gst_vulkan_window_get_display                    (GstVulkanWindow *window);
GST_VULKAN_API
VkSurfaceKHR       gst_vulkan_window_get_surface                    (GstVulkanWindow *window,
                                                                     GError **error);
GST_VULKAN_API
gboolean           gst_vulkan_window_get_presentation_support       (GstVulkanWindow *window,
                                                                     GstVulkanDevice *device,
                                                                     guint32 queue_family_idx);
GST_VULKAN_API
void               gst_vulkan_window_get_surface_dimensions         (GstVulkanWindow *window,
                                                                     guint *width,
                                                                     guint *height);
GST_VULKAN_API
void               gst_vulkan_window_set_window_handle              (GstVulkanWindow *window,
                                                                     guintptr handle);

GST_VULKAN_API
gboolean           gst_vulkan_window_open                           (GstVulkanWindow *window,
                                                                     GError ** error);
GST_VULKAN_API
void               gst_vulkan_window_close                          (GstVulkanWindow *window);

GST_VULKAN_API
void               gst_vulkan_window_resize                         (GstVulkanWindow *window,
                                                                     gint width,
                                                                     gint height);
GST_VULKAN_API
void               gst_vulkan_window_redraw                         (GstVulkanWindow *window);

G_END_DECLS

#endif /* __GST_VULKAN_WINDOW_H__ */
