/*
 * buflist-config.c - buflist configuration options (file buflist.conf)
 *
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

#include <stdlib.h>
#include <string.h>

#include "../weechat-plugin.h"
#include "buflist.h"
#include "buflist-config.h"
#include "buflist-bar-item.h"


struct t_config_file *buflist_config_file = NULL;

/* buflist config, look section */

struct t_config_option *buflist_config_look_add_newline;
struct t_config_option *buflist_config_look_auto_scroll;
struct t_config_option *buflist_config_look_display_conditions;
struct t_config_option *buflist_config_look_enabled;
struct t_config_option *buflist_config_look_mouse_jump_visited_buffer;
struct t_config_option *buflist_config_look_mouse_move_buffer;
struct t_config_option *buflist_config_look_mouse_wheel;
struct t_config_option *buflist_config_look_nick_prefix;
struct t_config_option *buflist_config_look_nick_prefix_empty;
struct t_config_option *buflist_config_look_signals_refresh;
struct t_config_option *buflist_config_look_sort;

/* buflist config, format section */

struct t_config_option *buflist_config_format_buffer;
struct t_config_option *buflist_config_format_buffer_current;
struct t_config_option *buflist_config_format_hotlist;
struct t_config_option *buflist_config_format_hotlist_level[4];
struct t_config_option *buflist_config_format_hotlist_level_none;
struct t_config_option *buflist_config_format_hotlist_separator;
struct t_config_option *buflist_config_format_indent;
struct t_config_option *buflist_config_format_lag;
struct t_config_option *buflist_config_format_name;
struct t_config_option *buflist_config_format_nick_prefix;
struct t_config_option *buflist_config_format_number;

struct t_hook **buflist_config_signals_refresh = NULL;
int buflist_config_num_signals_refresh = 0;
char **buflist_config_sort_fields = NULL;
int buflist_config_sort_fields_count = 0;


/*
 * Frees the signals hooked for refresh.
 */

void
buflist_config_free_signals_refresh ()
{
    int i;

    if (!buflist_config_signals_refresh)
        return;

    for (i = 0; i < buflist_config_num_signals_refresh; i++)
    {
        weechat_unhook (buflist_config_signals_refresh[i]);
    }

    free (buflist_config_signals_refresh);
    buflist_config_signals_refresh = NULL;

    buflist_config_num_signals_refresh = 0;
}

/*
 * Compares two signals to add them in the sorted arraylist.
 *
 * Returns:
 *   -1: signal1 < signal2
 *    0: signal1 == signal2
 *    1: signal1 > signal2
 */

int
buflist_config_compare_signals (void *data, struct t_arraylist *arraylist,
                                void *pointer1, void *pointer2)
{
    /* make C compiler happy */
    (void) data;
    (void) arraylist;

    return strcmp ((const char *)pointer1, (const char *)pointer2);
}

/*
 * Callback for a signal on a buffer.
 */

int
buflist_config_signal_buffer_cb (const void *pointer, void *data,
                                 const char *signal, const char *type_data,
                                 void *signal_data)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) signal;
    (void) type_data;
    (void) signal_data;

    buflist_bar_item_update ();

    return WEECHAT_RC_OK;
}

/*
 * Hooks the signals for refresh.
 */

void
buflist_config_hook_signals_refresh ()
{
    char **all_signals, **signals;
    const char *ptr_signals_refresh;
    struct t_arraylist *signals_list;
    int count, i;

    all_signals = weechat_string_dyn_alloc (256);
    if (!all_signals)
        return;

    ptr_signals_refresh = weechat_config_string (
        buflist_config_look_signals_refresh);

    weechat_string_dyn_concat (all_signals, BUFLIST_CONFIG_SIGNALS_REFRESH);
    if (ptr_signals_refresh && ptr_signals_refresh[0])
    {
        weechat_string_dyn_concat (all_signals, ",");
        weechat_string_dyn_concat (
            all_signals,
            weechat_config_string (buflist_config_look_signals_refresh));
    }
    if (weechat_config_boolean (buflist_config_look_nick_prefix))
    {
        weechat_string_dyn_concat (all_signals, ",");
        weechat_string_dyn_concat (
            all_signals,
            BUFLIST_CONFIG_SIGNALS_REFRESH_NICK_PREFIX);
    }

    signals = weechat_string_split (*all_signals, ",", 0, 0, &count);
    if (signals)
    {
        signals_list = weechat_arraylist_new (
            32, 1, 0,
            &buflist_config_compare_signals,
            NULL, NULL, NULL);
        if (signals_list)
        {
            for (i = 0; i < count; i++)
            {
                weechat_arraylist_add (signals_list, signals[i]);
            }
            buflist_config_signals_refresh = malloc (
                weechat_arraylist_size (signals_list) *
                sizeof (*buflist_config_signals_refresh));
            if (buflist_config_signals_refresh)
            {
                buflist_config_num_signals_refresh = count;
                for (i = 0; i < weechat_arraylist_size (signals_list); i++)
                {
                    buflist_config_signals_refresh[i] = weechat_hook_signal (
                        weechat_arraylist_get (signals_list, i),
                        &buflist_config_signal_buffer_cb, NULL, NULL);
                }
                if (weechat_buflist_plugin->debug >= 1)
                {
                    weechat_printf (NULL,
                                    _("%s: %d signals hooked"),
                                    BUFLIST_PLUGIN_NAME,
                                    weechat_arraylist_size (signals_list));
                }
            }
            weechat_arraylist_free (signals_list);
        }
        weechat_string_free_split (signals);
    }

    weechat_string_dyn_free (all_signals, 1);
}

/*
 * Callback for changes on option "buflist.look.enabled".
 */

void
buflist_config_change_enabled (const void *pointer, void *data,
                               struct t_config_option *option)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) option;

    buflist_config_free_signals_refresh ();

    if (weechat_config_boolean (buflist_config_look_enabled))
    {
        /* buflist enabled */
        buflist_config_hook_signals_refresh ();
        weechat_command (NULL, "/mute /bar show buflist");
        buflist_bar_item_update ();
    }
    else
    {
        /* buflist disabled */
        weechat_command (NULL, "/mute /bar hide buflist");
        weechat_bar_item_update (BUFLIST_BAR_ITEM_NAME);
    }
}

/*
 * Callback for changes on option "buflist.look.sort".
 */

void
buflist_config_change_sort (const void *pointer, void *data,
                            struct t_config_option *option)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) option;

    if (buflist_config_sort_fields)
        weechat_string_free_split (buflist_config_sort_fields);

    buflist_config_sort_fields = weechat_string_split (
        weechat_config_string (buflist_config_look_sort),
        ",", 0, 0, &buflist_config_sort_fields_count);

    buflist_bar_item_update ();
}

/*
 * Callback for changes on option "buflist.look.signals_refresh".
 */

void
buflist_config_change_signals_refresh (const void *pointer, void *data,
                                       struct t_config_option *option)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) option;

    buflist_config_free_signals_refresh ();
    buflist_config_hook_signals_refresh ();
}

/*
 * Callback for changes on option "buflist.look.nick_prefix".
 */

void
buflist_config_change_nick_prefix (const void *pointer, void *data,
                                   struct t_config_option *option)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) option;

    buflist_config_change_signals_refresh (NULL, NULL, NULL);
    buflist_bar_item_update ();
}

/*
 * Callback for changes on format options.
 */

void
buflist_config_change_buflist (const void *pointer, void *data,
                               struct t_config_option *option)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) option;

    buflist_bar_item_update ();
}

/*
 * Initializes buflist configuration file.
 *
 * Returns:
 *   1: OK
 *   0: error
 */

int
buflist_config_init ()
{
    struct t_config_section *ptr_section;

    buflist_config_file = weechat_config_new (BUFLIST_CONFIG_NAME,
                                              NULL, NULL, NULL);
    if (!buflist_config_file)
        return 0;

    /* look */
    ptr_section = weechat_config_new_section (buflist_config_file, "look",
                                              0, 0,
                                              NULL, NULL, NULL,
                                              NULL, NULL, NULL,
                                              NULL, NULL, NULL,
                                              NULL, NULL, NULL,
                                              NULL, NULL, NULL);
    if (!ptr_section)
    {
        weechat_config_free (buflist_config_file);
        return 0;
    }

    buflist_config_look_add_newline = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "add_newline", "boolean",
        N_("add newline between the buffers displayed, so each buffer is "
           "displayed on a separate line (recommended); if disabled, newlines "
           "must be manually added in the formats with \"${\\n}\", "
           "and the mouse actions are not possible any more"),
        NULL, 0, 0, "on", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_look_auto_scroll = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "auto_scroll", "integer",
        N_("automatically scroll the buflist bar to always see the current "
           "buffer (this works only with a bar on the left/right position "
           "with a \"vertical\" filling); this value is the percent number "
           "of lines displayed before the current buffer when scrolling "
           "(-1 = disable scroll); for example 50 means that after a scroll, "
           "the current buffer is at the middle of bar, 0 means on top of "
           "bar, 100 means at bottom of bar"),
        NULL, -1, 100, "50", NULL, 0,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_look_display_conditions = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "display_conditions", "string",
        N_("conditions to display a buffer "
           "(note: content is evaluated, see /help buflist); for example "
           "to hide server buffers if they are merged with core buffer: "
           "\"${buffer.hidden}==0 && ((${type}!=server && "
           "${buffer.full_name}!=core.weechat) || ${buffer.active}==1)\""),
        NULL, 0, 0, "${buffer.hidden}==0", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_look_enabled = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "enabled", "boolean",
        N_("enable buflist"),
        NULL, 0, 0, "on", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_enabled, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_look_mouse_jump_visited_buffer = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "mouse_jump_visited_buffer", "boolean",
        N_("if enabled, clicks with left/right buttons on the line with "
           "current buffer jump to previous/next visited buffer"),
        NULL, 0, 0, "off", NULL, 0,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_look_mouse_move_buffer = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "mouse_move_buffer", "boolean",
        N_("if enabled, mouse gestures (drag & drop) move buffers in list"),
        NULL, 0, 0, "on", NULL, 0,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_look_nick_prefix = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "nick_prefix", "boolean",
        N_("get the nick prefix and its color from nicklist so that "
           "${nick_prefix} can be used in format; this can be slow on buffers "
           "with lot of nicks in nicklist, so this option is disabled "
           "by default"),
        NULL, 0, 0, "off", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_nick_prefix, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_look_nick_prefix_empty = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "nick_prefix_empty", "boolean",
        N_("when the nick prefix is enabled, display a space instead if there "
           "is no nick prefix on the buffer"),
        NULL, 0, 0, "on", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_look_mouse_wheel = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "mouse_wheel", "boolean",
        N_("if enabled, mouse wheel up/down actions jump to previous/next "
           "buffer in list"),
        NULL, 0, 0, "on", NULL, 0,
        NULL, NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_look_signals_refresh = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "signals_refresh", "string",
        N_("comma-separated list of extra signals that are hooked and trigger "
           "the refresh of buffers list; this can be useful if some custom "
           "variables are used in formats and need specific refresh"),
        NULL, 0, 0, "", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_signals_refresh, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_look_sort = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "sort", "string",
        N_("comma-separated list of fields to sort buffers; each field is "
           "a hdata variable of buffer (\"var\"), a hdata variable of "
           "IRC server (\"irc_server.var\") or a hdata variable of "
           "IRC channel (\"irc_channel.var\"); "
           "char \"-\" can be used before field to reverse order, "
           "char \"~\" can be used to do a case insensitive comparison; "
           "example: \"-~short_name\" for case insensitive and reverse "
           "sort on buffer short name"),
        NULL, 0, 0, "number,-active", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_sort, NULL, NULL,
        NULL, NULL, NULL);

    /* format */
    ptr_section = weechat_config_new_section (buflist_config_file, "format",
                                              0, 0,
                                              NULL, NULL, NULL,
                                              NULL, NULL, NULL,
                                              NULL, NULL, NULL,
                                              NULL, NULL, NULL,
                                              NULL, NULL, NULL);
    if (!ptr_section)
    {
        weechat_config_free (buflist_config_file);
        return 0;
    }

    buflist_config_format_buffer = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "buffer", "string",
        N_("format of each line with a buffer "
           "(note: content is evaluated, see /help buflist)"),
        NULL, 0, 0,
        "${format_number}${indent}${format_nick_prefix}${color_hotlist}"
        "${format_name}",
        NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_format_buffer_current = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "buffer_current", "string",
        N_("format for the line with current buffer "
           "(note: content is evaluated, see /help buflist)"),
        NULL, 0, 0, "${color:,blue}${format_buffer}", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_format_hotlist = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "hotlist", "string",
        N_("format for hotlist "
           "(note: content is evaluated, see /help buflist)"),
        NULL, 0, 0,
        " ${color:green}(${hotlist}${color:green})",
        NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_format_hotlist_level[0] = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "hotlist_low", "string",
        N_("format for a buffer with hotlist level \"low\" "
           "(note: content is evaluated, see /help buflist)"),
        NULL, 0, 0, "${color:white}", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_format_hotlist_level[1] = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "hotlist_message", "string",
        N_("format for a buffer with hotlist level \"message\" "
           "(note: content is evaluated, see /help buflist)"),
        NULL, 0, 0, "${color:brown}", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_format_hotlist_level[2] = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "hotlist_private", "string",
        N_("format for a buffer with hotlist level \"private\" "
           "(note: content is evaluated, see /help buflist)"),
        NULL, 0, 0, "${color:green}", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_format_hotlist_level[3] = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "hotlist_highlight", "string",
        N_("format for a buffer with hotlist level \"highlight\" "
           "(note: content is evaluated, see /help buflist)"),
        NULL, 0, 0, "${color:magenta}", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_format_hotlist_level_none = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "hotlist_none", "string",
        N_("format for a buffer not in hotlist "
           "(note: content is evaluated, see /help buflist)"),
        NULL, 0, 0, "${color:default}", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_format_hotlist_separator = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "hotlist_separator", "string",
        N_("separator for counts in hotlist "
           "(note: content is evaluated, see /help buflist)"),
        NULL, 0, 0, "${color:default},", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_format_indent = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "indent", "string",
        N_("string displayed to indent channel and private buffers "
           "(note: content is evaluated, see /help buflist)"),
        NULL, 0, 0, "  ", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_format_lag = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "lag", "string",
        N_("format for lag on an IRC server buffer "
           "(note: content is evaluated, see /help buflist)"),
        NULL, 0, 0,
        " ${color:green}[${color:brown}${lag}${color:green}]",
        NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_format_name = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "name", "string",
        N_("format for buffer name "
           "(note: content is evaluated, see /help buflist)"),
        NULL, 0, 0, "${name}", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_format_nick_prefix = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "nick_prefix", "string",
        N_("format for nick prefix on a channel "
           "(note: content is evaluated, see /help buflist)"),
        NULL, 0, 0, "${color_nick_prefix}${nick_prefix}", NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);
    buflist_config_format_number = weechat_config_new_option (
        buflist_config_file, ptr_section,
        "number", "string",
        N_("format for buffer number, ${number} is the indented number "
           "(note: content is evaluated, see /help buflist)"),
        NULL, 0, 0,
        "${color:green}${number}${if:${number_displayed}?.: }",
        NULL, 0,
        NULL, NULL, NULL,
        &buflist_config_change_buflist, NULL, NULL,
        NULL, NULL, NULL);

    return 1;
}

/*
 * Reads buflist configuration file.
 */

int
buflist_config_read ()
{
    int rc;

    rc = weechat_config_read (buflist_config_file);

    if (rc == WEECHAT_CONFIG_READ_OK)
    {
        buflist_config_change_sort (NULL, NULL, NULL);
        buflist_config_change_signals_refresh (NULL, NULL, NULL);
    }

    return rc;
}

/*
 * Writes buflist configuration file.
 */

int
buflist_config_write ()
{
    return weechat_config_write (buflist_config_file);
}

/*
 * Frees buflist configuration.
 */

void
buflist_config_free ()
{
    weechat_config_free (buflist_config_file);

    if (buflist_config_signals_refresh)
        buflist_config_free_signals_refresh ();

    if (buflist_config_sort_fields)
    {
        weechat_string_free_split (buflist_config_sort_fields);
        buflist_config_sort_fields = NULL;
        buflist_config_sort_fields_count = 0;
    }
}
