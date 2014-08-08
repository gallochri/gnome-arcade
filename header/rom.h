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


/* rom.h */
#ifndef ROM_H
#define ROM_H

extern gchar *rom_tilePath;

extern GdkPixbuf *rom_tileNoImage;
extern GdkPixbuf *rom_tileNowShowing;
extern GdkPixbuf *rom_tileLoading;
extern GdkPixbuf *rom_tileFavorite;
extern GdkPixbuf *rom_tileRank;

extern guint rom_count;

enum rom_sortOrder {
    ROM_SORT_AZ,
    ROM_SORT_ZA
};

struct rom_romItem {
	// mame
	gchar *name;                //	1941 
	gchar *description;	        //	1941: The Counter Attack (World 900227)
	
	// view/sort
	gchar *desc;                //  1941: Counter Attack, The

	// user preference
	gboolean pref;
	guint	 rank;

	// pixbuf 
	gboolean  tileLoading;	
	gboolean  tileLoaded;
	GdkPixbuf *tile;
	GFile     *tileFile;

};

extern GList *rom_romList;
extern GHashTable *rom_cloneTable;

#define ROM_EXTENSION_ZIP	"zip"
#define ROM_EXTENSION_7ZIP	"7z"
#define ROM_MAXRANK 5

void rom_init (void);
void rom_free (void);
void rom_load (void);
enum rom_sortOrder rom_getSort (void);
void rom_setSort (enum rom_sortOrder order);

gboolean rom_isClone (const gchar *romName);
gboolean rom_isParent (const gchar *romName);
gboolean rom_filterBios (const gchar *romDes);

struct rom_romItem* rom_newItem (void);
struct rom_romItem* rom_getItem (int numGame);

void rom_setItemName (struct rom_romItem* item, gchar* name);
void rom_setItemDescription (struct rom_romItem* item, gchar* description);
void rom_setItemDesc (struct rom_romItem* item, gchar* desc);
void rom_setItemTile (struct rom_romItem* item, GdkPixbuf* tile);
void rom_loadItemAsync (struct rom_romItem* item);

const gchar* rom_getItemName (struct rom_romItem* item);
const gchar* rom_getItemDescription (struct rom_romItem* item);
const gchar* rom_getItemDesc (struct rom_romItem* item);
const GdkPixbuf* rom_getItemTile (struct rom_romItem* item);
const gboolean rom_getItemTileLoaded (struct rom_romItem* item);
const gboolean rom_getItemTileLoading (struct rom_romItem* item);
const gchar* rom_parentOf (const gchar *romName);
void rom_invalidateUselessTile (void);

gboolean rom_getItemPref (const struct rom_romItem *item);
void rom_setItemPref (struct rom_romItem *item, gboolean value);
guint rom_getItemRank (const struct rom_romItem *item);
void rom_setItemRank (struct rom_romItem *item, guint rank);

#endif
