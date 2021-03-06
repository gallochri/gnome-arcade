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

/* mame.h */
#ifndef MAME_H
#define MAME_H

void     mame_gameList  (void);
gboolean mame_playGame  (struct rom_romItem* item, const char* clone);
gboolean mame_isRunning (void);
gchar*   mame_getRomOf  (const gchar* romName);
gchar**  mame_getDeviceRomOf (const gchar* romName);
gboolean mame_needChd (const gchar* romName);

struct inforom_info *mame_getInfoRom (const gchar* romName);
void   mame_freeInfoRom (struct inforom_info *info);

#define MAME_LIST_FULL_FILE   "listfull.txt"
#define MAME_LIST_CLONES_FILE "listclones.txt"

#endif

