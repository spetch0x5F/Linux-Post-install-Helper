/* info-window.c
 *
 * Copyright 2023 Samuel Petch
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gtk/gtk.h>

#include <stdio.h>

#include <stdlib.h>

#include "lpih-window.h"

#include "info-window.h"

///////////////////////////////////////////
///////// INFORMATIONAL WINDOW:  //////////
///////////////////////////////////////////

 gboolean debian_info_open = FALSE;
 gboolean fedora_info_open = FALSE;

gboolean on_info_window_destroy(GtkWidget * widget, gpointer data) {
  // Suppress unused widget warning.
  (void)widget;
  InfoWindowData * info_window_data = (InfoWindowData * ) data;

 if(check_distro_from_window_data(info_window_data) == DEBIAN) {
  debian_info_open = FALSE;
  return TRUE;
  } else if(check_distro_from_window_data(info_window_data) == FEDORA) {
  fedora_info_open = FALSE;
  return TRUE;
  } else { return FALSE; }
}

enum Distro check_distro_from_window_data(gpointer data) {

InfoWindowData * info_window_data = (InfoWindowData *)data;
enum Distro info_distro = OTHER;

  if(info_window_data -> distro_id == DEBIAN) {
  info_distro = DEBIAN;
  } else if(info_window_data -> distro_id == FEDORA) {
  info_distro = FEDORA;
  } else { g_print("Couldn't get distro in function check_distro_from_window_data(gpointer data).  \n"); }

  return info_distro;

}

// The gtk_notebook comprises the info_window. It has 3 tabs, each leading to a page with their own view.
GtkWidget * make_notebook(gpointer data) {
  InfoWindowData * info_window_data = (InfoWindowData * ) data;
  GtkWidget * notebook;
  notebook = gtk_notebook_new();
  if (notebook != NULL) {
    g_print("Notebook initialized. \n");
  } else {
    g_print("Notebook is NULL!");
    return NULL;
  }

  gtk_widget_add_css_class(notebook, info_window_data -> notebook_css_name);

  if (info_window_data -> distro_id == DEBIAN) {
    g_print("Creating debian notebook\n");
    create_notebook_tab(notebook, "deb_tab_view1", "Main", "deb_info_main", "../Resources/deb-info-tab1.txt", "/usr/share/LPIH/text_files/deb-info-tab1.txt");
    create_notebook_tab(notebook, "deb_tab_view2", "Software Management", "deb_info2", "../Resources/deb-info-tab2.txt", "/usr/share/LPIH/text_files/deb-info-tab2.txt");
    create_notebook_tab(notebook, "deb_tab_view3", "Tips", "deb_info3", "../Resources/set_static_ip3.txt", "/usr/share/LPIH/text_files/set_static_ip3.txt");
    g_print("Finished creating Debian notebook.\n");
    return notebook;

  } else if (info_window_data -> distro_id == FEDORA) {
    create_notebook_tab(notebook, "fed_tab_view1", "Main", "fed_info_main", "../Resources/fed-info-tab1.txt", "/usr/share/LPIH/text_files/fed-info-tab1.txt");
    create_notebook_tab(notebook, "fed_tab_view2", "Software Management", "fed_info2", "../Resources/fed-info-tab2.txt", "/usr/share/LPIH/text_files/fed-info-tab2.txt");
    create_notebook_tab(notebook, "fed_tab_view3", "Tips", "fed_info3", "../Resources/set_static_ip3.txt", "/usr/share/LPIH/text_files/set_static_ip3.txt");
    g_print("Finished creating Fedora notebook.\n");
    return notebook;

  } else {
    g_print("ERROR DURING gtk_notebook CREATION: Invalid distro number.  Can't determine which distro's text-files to read.  \n");
    return NULL;
  }
}


gboolean init_info_gui(gpointer data) {
      InfoWindowData * info_window_data = (InfoWindowData *) data;

      GtkWidget * info_window;
      const char * css_class;
      css_class = "deb_info_main";

      info_window = gtk_window_new();
     
      gtk_widget_add_css_class(info_window, css_class);
      gtk_window_set_title(GTK_WINDOW(info_window), info_window_data -> info_window_title);
      gtk_window_set_resizable(GTK_WINDOW(info_window), TRUE);
      gtk_window_set_default_size(GTK_WINDOW(info_window), 700, 700);
      gtk_widget_set_vexpand(info_window, TRUE);
      gtk_widget_set_hexpand(info_window, TRUE);

      GtkWidget * notebook = make_notebook(info_window_data);

      gtk_window_set_child(GTK_WINDOW(info_window), GTK_WIDGET(notebook));
      g_signal_connect(info_window, "destroy", G_CALLBACK(on_info_window_destroy), info_window_data);
      gtk_widget_set_visible(info_window, TRUE);
      
       if (gtk_widget_is_visible(info_window)) {
    g_print("info_window created.  \n");

    return TRUE;

  } else {

    g_print("Failed to create info_window.  \n");

    return FALSE;
  }
} 


void create_notebook_tab(GtkWidget * notebook, gchar * view_css_label, gchar * tab_label, gchar * tab_css_label, gchar * res_path1, gchar * res_path2) {

  GtkWidget * view;
  GtkTextBuffer * buffer;
  GtkWidget * scroll_info_window = gtk_scrolled_window_new();

  gtk_widget_set_size_request(scroll_info_window, 300, 200);
  gtk_widget_set_vexpand(scroll_info_window, TRUE);
  gtk_widget_set_hexpand(scroll_info_window, TRUE);

  view = gtk_text_view_new();
  gtk_widget_set_opacity(view, 0.9);
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);
  gtk_widget_add_css_class(view, view_css_label);
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

  GtkWidget * tab_label_view;
  GtkTextBuffer * tab_buffer;

  tab_label_view = gtk_text_view_new();
  gtk_widget_add_css_class(tab_label_view, tab_css_label);
  tab_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tab_label_view));
  gtk_text_buffer_set_text(tab_buffer, tab_label, -1);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(tab_label_view), FALSE);
  gtk_widget_set_vexpand(tab_label_view, TRUE);
  gtk_widget_set_hexpand(tab_label_view, TRUE);

  gchar * tab_text = NULL;
  gsize length = 0;

  GError * error = g_malloc(sizeof(GError));
  error = g_error_new(G_FILE_ERROR, G_FILE_ERROR_NOENT, "Failed to load the text file.\n");

  if (g_file_get_contents(res_path1, &tab_text, &length, &error)) {
    gtk_text_buffer_set_text(buffer, tab_text, -1);
    g_free(tab_text);
    g_error_free(error);

  } else if (g_file_get_contents(res_path2, &tab_text, &length, &error)) {

    gtk_text_buffer_set_text(buffer, tab_text, -1);
    g_free(tab_text);
    g_error_free(error);

  } else {
    g_print("Failed to load info file: %s\n", error -> message);
    g_error_free(error);
  }

  gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), FALSE);
  gtk_widget_set_can_focus(GTK_WIDGET(view), TRUE);
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view), 13);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view), 13);
  gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll_info_window), view);

  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scroll_info_window, tab_label_view);

}


void make_info_window(GtkWidget * widget, gpointer data) {
  if (widget != NULL) {

       MainWindowData * main_window_data = (MainWindowData * ) data;
    
    if (main_window_data -> distro == DEBIAN) {

      if (debian_info_open != TRUE) {
    
     InfoWindowData * info_window_data = g_malloc(sizeof(InfoWindowData));
    
        debian_info_open = TRUE;

      gchar * info_window_name_debian = "deb_info_window";
      gchar * info_window_title_debian = "LPIH: Debian Info";
      gchar * notebook_css_debian = "deb_notebook";
      enum Distro info_distro_debian = DEBIAN;

      info_window_data -> info_window_name = info_window_name_debian;
      info_window_data -> info_window_title = info_window_title_debian;
      info_window_data -> notebook_css_name = notebook_css_debian;
      info_window_data -> distro_id = info_distro_debian;
     // g_print("info_window_data -> distro_id: %d\n", info_distro_debian );
      info_window_data -> info_open_flag = debian_info_open;
      init_info_gui(info_window_data);
    // g_print("info_window_data -> info_open_flag: %d\n", debian_info_open );
      
    } else { g_print("Debian info window already open.  \n"); }
    
      } else if (main_window_data -> distro == FEDORA) {

       if (fedora_info_open != TRUE) {
          
          fedora_info_open = TRUE;

      InfoWindowData * info_window_data = g_malloc(sizeof(InfoWindowData));
    
      gchar * info_window_name = "fed_info_window";
      gchar * info_window_title = "LPIH: Fedora Info";
      gchar * notebook_css = "fed_notebook";
      enum Distro info_distro_fedora = FEDORA;

      info_window_data -> info_window_name = info_window_name;
      info_window_data -> info_window_title = info_window_title;
      info_window_data -> notebook_css_name = notebook_css;
      info_window_data -> distro_id = info_distro_fedora;
      info_window_data -> info_open_flag = fedora_info_open;
      init_info_gui(info_window_data);
      
    } else {  g_print("Fedora info window already open.  \n"); }
    
       } else {

      g_print("Couldn't get distro of info window to be created.\n");

    }

 } else { g_print("\nWindow is NULL\n\n"); }

}



