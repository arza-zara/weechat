/*
 * Copyright (C) 2003-2017 Sébastien Helleu <flashcode@flashtux.org>
 *
 * This file is part of WeeChat, the extensible chat client.
 *
 * WeeChat is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * WeeChat is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WeeChat.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WEECHAT_BUFLIST_CONFIG_H
#define WEECHAT_BUFLIST_CONFIG_H 1

#define BUFLIST_CONFIG_NAME "buflist"

extern struct t_config_file *buflist_config_file;

extern struct t_config_option *buflist_config_format_buffer;
extern struct t_config_option *buflist_config_format_buffer_current;
extern struct t_config_option *buflist_config_format_hotlist;
extern struct t_config_option *buflist_config_format_hotlist_level[4];
extern struct t_config_option *buflist_config_format_hotlist_level_none;
extern struct t_config_option *buflist_config_format_hotlist_separator;
extern struct t_config_option *buflist_config_format_lag;

extern char **buflist_config_sort_fields;
extern int buflist_config_sort_fields_count;

extern int buflist_config_init ();
extern int buflist_config_read ();
extern int buflist_config_write ();
extern void buflist_config_free ();

#endif /* WEECHAT_BUFLIST_CONFIG_H */
