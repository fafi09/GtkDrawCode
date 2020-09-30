#include <gtk/gtk.h>
//=====================全局区===================
static GtkPrintSettings *settings = NULL;
//=============================================

void
begin_print (GtkPrintOperation *operation,
               GtkPrintContext   *context,
               gpointer           user_data)
{
    g_print ("begin print\n");
    gtk_print_operation_set_n_pages (operation,
                                 1);
}

static void
draw_page (GtkPrintOperation *operation,
           GtkPrintContext   *context,
           gint               page_nr,
           gpointer           user_data)
{

  const gint HEADER_HEIGHT = 800;
  cairo_t *cr;
  PangoLayout *layout;
  gdouble width, text_height;
  gint layout_height;
  PangoFontDescription *desc;

  g_print ("draw_page\n");

  cr = gtk_print_context_get_cairo_context (context);
  width = gtk_print_context_get_width (context);

  cairo_rectangle (cr, 0, 0, width, HEADER_HEIGHT);

  cairo_set_source_rgb (cr, 0.8, 0.8, 0.8);
  cairo_fill (cr);

  layout = gtk_print_context_create_pango_layout (context);

  desc = pango_font_description_from_string ("sans 14");
  pango_layout_set_font_description (layout, desc);
  pango_font_description_free (desc);

  pango_layout_set_text (layout, "some text", -1);
  pango_layout_set_width (layout, width * PANGO_SCALE);
  pango_layout_set_alignment (layout, PANGO_ALIGN_CENTER);

  pango_layout_get_size (layout, NULL, &layout_height);
  text_height = (gdouble)layout_height / PANGO_SCALE;

  cairo_move_to (cr, width / 2,  (HEADER_HEIGHT - text_height) / 2);
  pango_cairo_show_layout (cr, layout);

  g_object_unref (layout);
}

static void
do_print (void)
{
  GtkPrintOperation *print;
  GtkPrintOperationResult res;

  print = gtk_print_operation_new ();

  if (settings != NULL)
    gtk_print_operation_set_print_settings (print, settings);

  g_signal_connect (print, "begin_print", G_CALLBACK (begin_print), NULL);
  g_signal_connect (print, "draw_page", G_CALLBACK (draw_page), NULL);

  res = gtk_print_operation_run (print, GTK_PRINT_OPERATION_ACTION_PRINT,
                                 NULL, NULL);

  if (res == GTK_PRINT_OPERATION_RESULT_APPLY)
    {
      if (settings != NULL)
        g_object_unref (settings);
      settings = g_object_ref (gtk_print_operation_get_print_settings (print));
    }

  g_object_unref (print);
}

static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
  g_print ("Hello World\n");
  do_print();
}

gboolean
draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data)
{
  guint width, height;
  GdkRGBA color;
  GtkStyleContext *context;
  color.red = 0;
  color.green = 0;
  color.blue = 0.255;
  color.alpha = 1;

  guint picWid, picHeight;

  context = gtk_widget_get_style_context (widget);

  width = gtk_widget_get_allocated_width (widget);
  height = gtk_widget_get_allocated_height (widget);

  gtk_render_background (context, cr, 0, 0, width, height);

  //cairo_arc (cr,
  //           width / 2.0, height / 2.0,
  //           MIN (width, height) / 2.0,
  //           0, 2 * G_PI);

  //直接绘制oom发生，原因待查
  //cairo_surface_t *image = cairo_image_surface_create_from_png("Build.bmp");
  // cairo函数读取png文件
  //cairo_set_source_surface(cr, image, 0, 0);

  GError * error =NULL;
  GdkPixbuf * widebright_png = gdk_pixbuf_new_from_file ("Build.bmp",&error);
  picWid = gdk_pixbuf_get_width(widebright_png);
  picHeight = gdk_pixbuf_get_height(widebright_png);
  GdkPixbuf * dst_pixbuf = gdk_pixbuf_scale_simple(widebright_png, picWid, picHeight, GDK_INTERP_BILINEAR);

  gdk_cairo_set_source_pixbuf(cr ,dst_pixbuf,0,0);
  cairo_paint_with_alpha (cr, 1);
	//cairo_paint(cr);	// 绘图

		// 释放资源
	g_object_unref(dst_pixbuf);
	g_object_unref(widebright_png);

	// 绘图与写字共存的测试
	// 如果绘完图片后想继续写字或画线，
	// 必须手动设置画笔颜色cairo_set_source_rgb()
	// 否则，字体或线条会被图片覆盖。
	cairo_set_source_rgb(cr, 0.627, 0, 0);  // 设置字体颜色
	cairo_set_font_size(cr, 40.0);			// 设置字体大小
	cairo_move_to(cr, 20.0, 60.0);			// 写字的起点坐标
	cairo_show_text(cr, "写字");	// 写字
  gtk_style_context_get_color (context,
                               gtk_style_context_get_state (context),
                               &color);
  gdk_cairo_set_source_rgba (cr, &color);

  cairo_fill (cr);

 return FALSE;
}

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *image;
  GtkWidget *button;
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,1);
  button = gtk_button_new_with_label("button");

  GtkWidget *drawing_area = gtk_drawing_area_new ();

  //image = gtk_image_new_from_file ("Build.bmp");

  //gtk_container_add (GTK_CONTAINER (window), image);
  gtk_container_add (GTK_CONTAINER(window), vbox);
  gtk_container_add (GTK_CONTAINER(vbox), drawing_area);
  gtk_container_add (GTK_CONTAINER(vbox), button);
  //gtk_container_add (GTK_CONTAINER(image), drawing_area);

  gtk_widget_set_size_request (drawing_area, 800, 600);
  g_signal_connect (G_OBJECT (drawing_area), "draw",
                    G_CALLBACK (draw_callback), NULL);

  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
