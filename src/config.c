/* gnome-arcade
 * Copyright (c) 2014 Strippato strippato@gmail.com
 *
 * This file is part of gnome-arcade.
 *
 * gnome-arcade is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gnome-arcade is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gnome-arcade.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <string.h>

#include "app.h"
#include "global.h"
#include "config.h"

#define CFG_FILENAME "gnome-arcade.ini"
#define CFG_SECTION  "arcade"

static GHashTable *cfg_default = NULL;
static GHashTable *cfg_config  = NULL;

static GKeyFile *cfg_keyFile = NULL;
static GError 	*err = NULL;	

static void 
cfg_fillDefaultConfig (void)
{
	g_hash_table_insert (cfg_default, "MAME_EXE", "/usr/bin/mame");
	g_hash_table_insert (cfg_default, "MAME_OPTIONS", "-skip_gameinfo -multithreading -video opengl");

	g_hash_table_insert (cfg_default, "USE_DARK_THEME", "1");

	g_hash_table_insert (cfg_default, "ROM_PATH", "~/gnome-arcade/data/rom/");

	g_hash_table_insert (cfg_default, "ROMLIST_FROM_FILE", "0");
	g_hash_table_insert (cfg_default, "ROMLIST_SKIP_BIOS", "1");

	g_hash_table_insert (cfg_default, "TILE_SIZE_W", "210");
	g_hash_table_insert (cfg_default, "TILE_SIZE_H", "210");
	g_hash_table_insert (cfg_default, "TILE_KEEP_ASPECT_RATIO", "1");	
	g_hash_table_insert (cfg_default, "TILE_TITLE_CENTERED", "1");
	g_hash_table_insert (cfg_default, "TILE_PATH", "~/gnome-arcade/data/tile/");
	g_hash_table_insert (cfg_default, "TILE_BORDER_DYNAMIC", "1");
	g_hash_table_insert (cfg_default, "TILE_SHORT_DESCRIPTION", "1");
	g_hash_table_insert (cfg_default, "TILE_SHORT_DESCRIPTION_HIDE_PREFIX", "1");
	g_hash_table_insert (cfg_default, "TILE_SHADOW", "1");

	g_hash_table_insert (cfg_default, "WEB_PROVIDER", "http://www.progettoemma.net/snap/%s/0000.png");
	g_hash_table_insert (cfg_default, "WEB_PATH", "~/gnome-arcade/data/www/");
	g_hash_table_insert (cfg_default, "WEB_DOWNLOAD", "1");
}

/*
static void
cfg_print (gchar *key, gchar *value)
{
	g_print ("Key:%s Value:%s\n", key, value);
}

static void 
cfg_dump (void)
{
	g_print ("Dumping hashtable...\n");
	g_hash_table_foreach (cfg_default, (GHFunc) cfg_print, NULL);
	g_print ("Done!\n");	
}
*/
												

void 
cfg_init (void) 																																	
{
	g_assert (!cfg_keyFile);
	g_assert (!cfg_default);

	cfg_keyFile = NULL;
	err = NULL;
	cfg_default = g_hash_table_new (g_str_hash, g_str_equal);
	cfg_config = g_hash_table_new (g_str_hash, g_str_equal);
	
	cfg_fillDefaultConfig ();
	
	//cfg_dump();
}

void 
cfg_free (void) 
{

	if (cfg_keyFile) g_key_file_free (cfg_keyFile);
	cfg_keyFile = NULL;

	g_hash_table_unref (cfg_default);
	g_hash_table_unref (cfg_config);
	
	cfg_default = NULL;
	cfg_config = NULL;
}

gboolean
cfg_createDefaultConfigFile (void) 
{
	gchar *data;
	GKeyFile* keyFile = NULL;
	GFile *file;
	gsize len;
	gboolean created = TRUE;

	gchar *fileName = g_build_filename (g_get_user_config_dir (), APP_DIRCONFIG, CFG_FILENAME, NULL);
	gchar *pathName = g_build_filename (g_get_user_config_dir (), APP_DIRCONFIG, NULL);	

	g_print ("writing config file (%s)\n", fileName);

	g_assert (fileName);
	g_assert (pathName);	

	keyFile = g_key_file_new ();	

	/* adding default config */
	GHashTableIter iter;
	gpointer key, value;

	g_hash_table_iter_init (&iter, cfg_default);
	while (g_hash_table_iter_next (&iter, &key, &value)) {
		g_key_file_set_string (keyFile, CFG_SECTION, (gchar*) key, (gchar*) value);
	}

	g_key_file_set_comment (keyFile, CFG_SECTION, "WEB_PROVIDER", "WEB_PROVIDER\n" \
																  	"http://www.progettoemma.net/snap/%s/0000.png\n" \
																  	"http://www.progettoemma.net/snap/%s/title.png\n" \
																  	"http://www.progettoemma.net/snap/%s/flyer.png\n" \
																  	"http://www.progettoemma.net/snap/%s/score.png\n" \
																  	"http://www.progettoemma.net/snap/%s/gameover.png\n" \
																  	"\n" \
																  	"http://mrdo.mameworld.info/mame_artwork/%s.png\n" \
																  	"\n" \
																  	"http://www.mamedb.com/snap/%s.png\n" \
																	"http://www.mamedb.com/titles/%s.png\n" \
																	"http://www.mamedb.com/cabinets/%s.png\n"																	
							, &err);
	
	if (err) {
		g_print ("Can't write to output stream: %s\n", err->message);
	    g_error_free (err);
	    err = NULL;
	}

	data = g_key_file_to_data (keyFile, &len, &err);
	if (data) {
		if (!g_mkdir_with_parents (pathName, 0700)) {

		    file = g_file_new_for_path (fileName);
		    
		    GFileOutputStream *outStream = g_file_replace (file, NULL, TRUE, G_FILE_CREATE_PRIVATE, NULL, &err);
		    if (outStream) {
		    	if (g_output_stream_write (G_OUTPUT_STREAM (outStream), data, strlen (data), NULL, &err) == -1) {
					g_print ("Can't write to output stream: %s\n", err->message);
				    g_error_free (err);
				    err = NULL;
				    created = FALSE;
		    	}

		    	g_output_stream_close (G_OUTPUT_STREAM (outStream), NULL, NULL);
		    	g_object_unref (outStream);

		    } else {
				g_print ("Can't create output stream: %s\n", err->message);
			    g_error_free (err);
			    err = NULL;
			    created = FALSE;
		    }
	    	g_object_unref (file);		    
			

		} else {
			g_print ("Can't create config directory\n");
			created = FALSE;
		}
		g_free (data);

	} else {
		g_print ("Can't convert data to string: %s\n", err->message);
	    g_error_free (err);
	    err = NULL;
	    created = FALSE;
	} 

	g_key_file_free (keyFile);
	g_free (fileName);			
	g_free (pathName);				

	return created;
}

gboolean
cfg_load (void) 
{
	gchar *file = g_build_filename (g_get_user_config_dir (), APP_DIRCONFIG, CFG_FILENAME, NULL);
	g_assert (file);
	
	cfg_keyFile = g_key_file_new ();

	g_print ("loading config from %s ", file);

	if (g_key_file_load_from_file (cfg_keyFile, file, G_KEY_FILE_KEEP_COMMENTS, &err)) {
		g_print (SUCCESS_MSG "\n");

		/* loop the default config */
		GHashTableIter iter;
		gpointer key, value;

		g_hash_table_iter_init (&iter, cfg_default);
		while (g_hash_table_iter_next (&iter, &key, &value)) {
			if (g_key_file_has_key (cfg_keyFile, CFG_SECTION, (gchar*) key, NULL)) {
				// found in config
				gchar *cfgValue = g_key_file_get_string (cfg_keyFile, CFG_SECTION, (gchar*) key, NULL);
				g_hash_table_insert (cfg_config, (gchar*) key, cfgValue);
			} else {
				// not found, let's use the default
				g_hash_table_insert (cfg_config, (gchar*) key, (gchar*) value);
				// insert in config file?!? umph... no
				//g_key_file_set_string (cfg_keyFile, CFG_SECTION, (gchar*) key, (gchar*) value);
			}
		}
		g_free (file);                                                  
		return TRUE;
	} else  {
		g_key_file_free (cfg_keyFile);
		g_print ("\n");
		g_print ("Opps, can't read config (%s): %s\n", file, err->message);

		g_free (file);			
		g_error_free (err);
		err = NULL;

		return FALSE;
	}
}

gboolean
cfg_configFileExist (void)
{
	gboolean exist;
	gchar *file = g_build_filename (g_get_user_config_dir (), APP_DIRCONFIG, CFG_FILENAME, NULL);
	if (g_file_test (file, G_FILE_TEST_EXISTS)) {	
		exist = TRUE;
	} else {
		exist = FALSE;
	}
	g_free (file);

	return exist;
}

inline const gchar*
cfg_keyStr (const gchar* key)
{
	g_assert (cfg_config);

	return (gchar*) g_hash_table_lookup (cfg_config, key);
}

inline gint
cfg_keyInt (const gchar* key)
{
	g_assert (cfg_config);
	gpointer out = g_hash_table_lookup (cfg_config, key);

	if (out) {
		return (gint) strtod (out, NULL);	
	} else {
		return 0;
	}
}

inline gdouble
cfg_keyDbl (const gchar* key)
{
	g_assert (cfg_config);

	return strtod (g_hash_table_lookup (cfg_config, key), NULL);
}

inline gboolean
cfg_keyBool (const gchar* key)
{ 
	// 0 -> FALSE
	// else TRUE
	g_assert (cfg_config);

	return strtod ((gchar*) g_hash_table_lookup (cfg_config, key), NULL) == 0? FALSE: TRUE;
}