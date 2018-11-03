/*
 * plugin-api-info.c - extra info functions for plugin API
 *
 * Copyright (C) 2003-2018 Sébastien Helleu <flashcode@flashtux.org>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../core/weechat.h"
#include "../core/wee-config.h"
#include "../core/wee-hook.h"
#include "../core/wee-infolist.h"
#include "../core/wee-proxy.h"
#include "../core/wee-secure.h"
#include "../core/wee-string.h"
#include "../core/wee-url.h"
#include "../core/wee-util.h"
#include "../core/wee-version.h"
#include "../gui/gui-bar.h"
#include "../gui/gui-bar-item.h"
#include "../gui/gui-bar-window.h"
#include "../gui/gui-buffer.h"
#include "../gui/gui-color.h"
#include "../gui/gui-cursor.h"
#include "../gui/gui-filter.h"
#include "../gui/gui-history.h"
#include "../gui/gui-hotlist.h"
#include "../gui/gui-key.h"
#include "../gui/gui-layout.h"
#include "../gui/gui-line.h"
#include "../gui/gui-nick.h"
#include "../gui/gui-nicklist.h"
#include "../gui/gui-window.h"
#include "plugin.h"


/*
 * Returns WeeChat info "version".
 */

const char *
plugin_api_info_version_cb (const void *pointer, void *data,
                            const char *info_name,
                            const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    return version_get_version ();
}

/*
 * Returns WeeChat info "version_number".
 */

const char *
plugin_api_info_version_number_cb (const void *pointer, void *data,
                                   const char *info_name,
                                   const char *arguments)
{
    static char version_number[32] = { '\0' };

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    if (!version_number[0])
    {
        snprintf (version_number, sizeof (version_number), "%d",
                  util_version_number (version_get_version ()));
    }
    return version_number;
}

/*
 * Returns WeeChat info "version_git".
 */

const char *
plugin_api_info_version_git_cb (const void *pointer, void *data,
                                const char *info_name,
                                const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    return version_get_git ();
}

/*
 * Returns WeeChat info "date".
 */

const char *
plugin_api_info_date_cb (const void *pointer, void *data,
                         const char *info_name,
                         const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    return version_get_compilation_date_time ();
}

/*
 * Returns WeeChat info "pid".
 */

const char *
plugin_api_info_pid_cb (const void *pointer, void *data,
                        const char *info_name,
                        const char *arguments)
{
    static char value[32];

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    snprintf (value, sizeof (value), "%d", (int)getpid ());
    return value;
}

/*
 * Returns WeeChat info "dir_separator".
 */

const char *
plugin_api_info_dir_separator_cb (const void *pointer, void *data,
                                  const char *info_name,
                                  const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    return DIR_SEPARATOR;
}

/*
 * Returns WeeChat info "weechat_dir".
 */

const char *
plugin_api_info_weechat_dir_cb (const void *pointer, void *data,
                                const char *info_name,
                                const char *arguments)
{
    static char weechat_dir_absolute_path[PATH_MAX] = { '\0' };

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    if (!weechat_dir_absolute_path[0])
    {
        if (!realpath (weechat_home, weechat_dir_absolute_path))
            return NULL;
    }
    return (weechat_dir_absolute_path[0]) ?
        weechat_dir_absolute_path : weechat_home;
}

/*
 * Returns WeeChat info "weechat_libdir".
 */

const char *
plugin_api_info_weechat_libdir_cb (const void *pointer, void *data,
                                   const char *info_name,
                                   const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    return WEECHAT_LIBDIR;
}

/*
 * Returns WeeChat info "weechat_sharedir".
 */

const char *
plugin_api_info_weechat_sharedir_cb (const void *pointer, void *data,
                                     const char *info_name,
                                     const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    return WEECHAT_SHAREDIR;
}

/*
 * Returns WeeChat info "weechat_localedir".
 */

const char *
plugin_api_info_weechat_localedir_cb (const void *pointer, void *data,
                                      const char *info_name,
                                      const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    return LOCALEDIR;
}

/*
 * Returns WeeChat info "weechat_site".
 */

const char *
plugin_api_info_weechat_site_cb (const void *pointer, void *data,
                                 const char *info_name,
                                 const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    return WEECHAT_WEBSITE;
}

/*
 * Returns WeeChat info "weechat_site_download".
 */

const char *
plugin_api_info_weechat_site_download_cb (const void *pointer, void *data,
                                          const char *info_name,
                                          const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    return WEECHAT_WEBSITE_DOWNLOAD;
}

/*
 * Returns WeeChat info "weechat_upgrading".
 */

const char *
plugin_api_info_weechat_upgrading_cb (const void *pointer, void *data,
                                      const char *info_name,
                                      const char *arguments)
{
    static char value[32];

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    snprintf (value, sizeof (value), "%d", weechat_upgrading);
    return value;
}

/*
 * Returns WeeChat info "charset_terminal".
 */

const char *
plugin_api_info_charset_terminal_cb (const void *pointer, void *data,
                                     const char *info_name,
                                     const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    return weechat_local_charset;
}

/*
 * Returns WeeChat info "charset_internal".
 */

const char *
plugin_api_info_charset_internal_cb (const void *pointer, void *data,
                                     const char *info_name,
                                     const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    return WEECHAT_INTERNAL_CHARSET;
}

/*
 * Returns WeeChat info "locale".
 */

const char *
plugin_api_info_locale_cb (const void *pointer, void *data,
                           const char *info_name,
                           const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    return setlocale (LC_MESSAGES, NULL);
}

/*
 * Returns WeeChat info "inactivity".
 */

const char *
plugin_api_info_inactivity_cb (const void *pointer, void *data,
                               const char *info_name,
                               const char *arguments)
{
    time_t inactivity;
    static char value[32];

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    if (gui_key_last_activity_time == 0)
        inactivity = 0;
    else
        inactivity = time (NULL) - gui_key_last_activity_time;
    snprintf (value, sizeof (value), "%lld", (long long)inactivity);
    return value;
}

/*
 * Returns WeeChat info "filters_enabled".
 */

const char *
plugin_api_info_filters_enabled_cb (const void *pointer, void *data,
                                    const char *info_name,
                                    const char *arguments)
{
    static char value[32];

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    snprintf (value, sizeof (value), "%d", gui_filters_enabled);
    return value;
}

/*
 * Returns WeeChat info "cursor_mode".
 */

const char *
plugin_api_info_cursor_mode_cb (const void *pointer, void *data,
                                const char *info_name,
                                const char *arguments)
{
    static char value[32];

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    snprintf (value, sizeof (value), "%d", gui_cursor_mode);
    return value;
}

/*
 * Returns WeeChat info "term_width".
 */

const char *
plugin_api_info_term_width_cb (const void *pointer, void *data,
                               const char *info_name,
                               const char *arguments)
{
    static char value[32];

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    snprintf (value, sizeof (value), "%d", gui_window_get_width ());
    return value;
}

/*
 * Returns WeeChat info "term_height".
 */

const char *
plugin_api_info_term_height_cb (const void *pointer, void *data,
                                const char *info_name,
                                const char *arguments)
{
    static char value[32];

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    snprintf (value, sizeof (value), "%d", gui_window_get_height ());
    return value;
}

/*
 * Returns WeeChat info "color_ansi_regex".
 */

const char *
plugin_api_info_color_ansi_regex_cb (const void *pointer, void *data,
                                     const char *info_name,
                                     const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;
    (void) arguments;

    return GUI_COLOR_REGEX_ANSI_DECODE;
}

/*
 * Returns WeeChat info "color_term2rgb".
 */

const char *
plugin_api_info_color_term2rgb_cb (const void *pointer, void *data,
                                   const char *info_name,
                                   const char *arguments)
{
    static char value[32];

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;

    if (!arguments || !arguments[0])
        return NULL;

    snprintf (value, sizeof (value),
              "%d",
              gui_color_convert_term_to_rgb (atoi (arguments)));
    return value;
}

/*
 * Returns WeeChat info "color_rgb2term".
 */

const char *
plugin_api_info_color_rgb2term_cb (const void *pointer, void *data,
                                   const char *info_name,
                                   const char *arguments)
{
    static char value[32];
    int rgb, limit;
    char *pos, *color;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;

    if (!arguments || !arguments[0])
        return NULL;

    limit = 256;
    pos = strchr (arguments, ',');
    if (pos)
    {
        color = string_strndup (arguments, pos - arguments);
        if (!color)
            return NULL;
        rgb = atoi (color);
        limit = atoi (pos + 1);
        free (color);
    }
    else
    {
        rgb = atoi (arguments);
    }
    snprintf (value, sizeof (value),
              "%d",
              gui_color_convert_rgb_to_term (rgb, limit));

    return value;
}

/*
 * Returns nick color code for a nickname.
 */

const char *
plugin_api_info_nick_color_cb (const void *pointer, void *data,
                               const char *info_name,
                               const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;

    return gui_nick_find_color (arguments);
}

/*
 * Returns nick color name for a nickname.
 */

const char *
plugin_api_info_nick_color_name_cb (const void *pointer, void *data,
                                    const char *info_name,
                                    const char *arguments)
{
    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;

    return gui_nick_find_color_name (arguments);
}

/*
 * Returns WeeChat info "uptime".
 */

const char *
plugin_api_info_uptime_cb (const void *pointer, void *data,
                           const char *info_name,
                           const char *arguments)
{
    static char value[32];
    time_t total_seconds;
    int days, hours, minutes, seconds;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;

    if (!arguments || !arguments[0])
    {
        /* return uptime with format: "days:hh:mm:ss" */
        util_get_uptime (NULL, &days, &hours, &minutes, &seconds);
        snprintf (value, sizeof (value), "%d:%02d:%02d:%02d",
                  days, hours, minutes, seconds);
        return value;
    }

    if (strcmp (arguments, "days") == 0)
    {
        /* return the number of days */
        util_get_uptime (NULL, &days, NULL, NULL, NULL);
        snprintf (value, sizeof (value), "%d", days);
        return value;
    }

    if (strcmp (arguments, "seconds") == 0)
    {
        /* return the number of seconds */
        util_get_uptime (&total_seconds, NULL, NULL, NULL, NULL);
        snprintf (value, sizeof (value), "%lld", (long long)total_seconds);
        return value;
    }

    return NULL;
}

/*
 * Returns WeeChat info "totp_generate": generates a Time-based One-Time
 * Password (TOTP).
 *
 * Arguments: "secret,timestamp,digits" (timestamp and digits are optional).
 */

const char *
plugin_api_info_totp_generate_cb (const void *pointer, void *data,
                                  const char *info_name,
                                  const char *arguments)
{
    static char value[32];
    char **argv, *ptr_secret, *error, *totp;
    int argc, digits, length;
    long number;
    time_t totp_time;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;

    argv = NULL;
    totp = NULL;

    if (!arguments || !arguments[0])
        goto error;

    argv = string_split (arguments, ",", 0, 0, &argc);
    if (!argv || (argc < 1))
        goto error;

    ptr_secret = argv[0];
    totp_time = 0;
    digits = 6;

    if (argc > 1)
    {
        error = NULL;
        number = (int)strtol (argv[1], &error, 10);
        if (!error || error[0] || (number < 0))
            goto error;
        totp_time = (time_t)number;
    }
    if (argc > 2)
    {
        error = NULL;
        number = (int)strtol (argv[2], &error, 10);
        if (!error || error[0] || (number < 0))
            goto error;
        digits = number;
    }

    totp = secure_totp_generate (ptr_secret, totp_time, digits);
    if (!totp)
        goto error;

    length = snprintf (value, sizeof (value), "%s", totp);
    if (length != digits)
        goto error;

    string_free_split (argv);
    free (totp);

    return value;

error:
    if (argv)
        string_free_split (argv);
    if (totp)
        free (totp);
    return NULL;
}

/*
 * Returns WeeChat info "totp_validate": validates a Time-based One-Time
 * Password (TOTP).
 *
 * Arguments: "secret,otp,timestamp,window" (timestamp and window are optional).
 */

const char *
plugin_api_info_totp_validate_cb (const void *pointer, void *data,
                                  const char *info_name,
                                  const char *arguments)
{
    static char value[16];
    char **argv, *ptr_secret, *ptr_otp, *error;
    int argc, window, rc;
    long number;
    time_t totp_time;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) info_name;

    argv = NULL;

    if (!arguments || !arguments[0])
        goto error;

    argv = string_split (arguments, ",", 0, 0, &argc);
    if (!argv || (argc < 2))
        goto error;

    ptr_secret = argv[0];
    ptr_otp = argv[1];
    totp_time = 0;
    window = 0;

    if (argc > 2)
    {
        error = NULL;
        number = (int)strtol (argv[2], &error, 10);
        if (!error || error[0] || (number < 0))
            goto error;
        totp_time = (time_t)number;
    }
    if (argc > 3)
    {
        error = NULL;
        number = (int)strtol (argv[3], &error, 10);
        if (!error || error[0] || (number < 0))
            goto error;
        window = number;
    }

    rc = secure_totp_validate (ptr_secret, totp_time, window, ptr_otp);

    snprintf (value, sizeof (value), "%d", rc);

    string_free_split (argv);

    return value;

error:
    if (argv)
        string_free_split (argv);
    return NULL;
}

/*
 * Returns WeeChat infolist "bar".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_bar_cb (const void *pointer, void *data,
                            const char *infolist_name,
                            void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;
    struct t_gui_bar *ptr_bar;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;

    /* invalid bar pointer ? */
    if (obj_pointer && (!gui_bar_valid (obj_pointer)))
        return NULL;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    if (obj_pointer)
    {
        /* build list with only one bar */
        if (!gui_bar_add_to_infolist (ptr_infolist, obj_pointer))
        {
            infolist_free (ptr_infolist);
            return NULL;
        }
        return ptr_infolist;
    }
    else
    {
        /* build list with all bars matching arguments */
        for (ptr_bar = gui_bars; ptr_bar; ptr_bar = ptr_bar->next_bar)
        {
            if (!arguments || !arguments[0]
                || string_match (ptr_bar->name, arguments, 0))
            {
                if (!gui_bar_add_to_infolist (ptr_infolist, ptr_bar))
                {
                    infolist_free (ptr_infolist);
                    return NULL;
                }
            }
        }
        return ptr_infolist;
    }

    return NULL;
}

/*
 * Returns WeeChat infolist "bar_item".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_bar_item_cb (const void *pointer, void *data,
                                 const char *infolist_name,
                                 void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;
    struct t_gui_bar_item *ptr_bar_item;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;

    /* invalid bar item pointer ? */
    if (obj_pointer && (!gui_bar_item_valid (obj_pointer)))
        return NULL;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    if (obj_pointer)
    {
        /* build list with only one bar item */
        if (!gui_bar_item_add_to_infolist (ptr_infolist, obj_pointer))
        {
            infolist_free (ptr_infolist);
            return NULL;
        }
        return ptr_infolist;
    }
    else
    {
        /* build list with all bar items matching arguments */
        for (ptr_bar_item = gui_bar_items; ptr_bar_item;
             ptr_bar_item = ptr_bar_item->next_item)
        {
            if (!arguments || !arguments[0]
                || string_match (ptr_bar_item->name, arguments, 0))
            {
                if (!gui_bar_item_add_to_infolist (ptr_infolist, ptr_bar_item))
                {
                    infolist_free (ptr_infolist);
                    return NULL;
                }
            }
        }
        return ptr_infolist;
    }

    return NULL;
}

/*
 * Returns WeeChat infolist "bar_window".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_bar_window_cb (const void *pointer, void *data,
                                   const char *infolist_name,
                                   void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;
    struct t_gui_bar *ptr_bar;
    struct t_gui_bar_window *ptr_bar_window;
    struct t_gui_window *ptr_window;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;
    (void) arguments;

    /* invalid bar window pointer ? */
    if (obj_pointer && (!gui_bar_window_valid (obj_pointer)))
        return NULL;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    if (obj_pointer)
    {
        /* build list with only one bar window */
        if (!gui_bar_window_add_to_infolist (ptr_infolist, obj_pointer))
        {
            infolist_free (ptr_infolist);
            return NULL;
        }
        return ptr_infolist;
    }
    else
    {
        /* build list with all bar windows (from root and window bars) */
        for (ptr_bar = gui_bars; ptr_bar; ptr_bar = ptr_bar->next_bar)
        {
            if (ptr_bar->bar_window)
            {
                if (!gui_bar_window_add_to_infolist (ptr_infolist, ptr_bar->bar_window))
                {
                    infolist_free (ptr_infolist);
                    return NULL;
                }
            }
        }
        for (ptr_window = gui_windows; ptr_window;
             ptr_window = ptr_window->next_window)
        {
            for (ptr_bar_window = ptr_window->bar_windows;
                 ptr_bar_window;
                 ptr_bar_window = ptr_bar_window->next_bar_window)
            {
                if (!gui_bar_window_add_to_infolist (ptr_infolist, ptr_bar_window))
                {
                    infolist_free (ptr_infolist);
                    return NULL;
                }
            }
        }
        return ptr_infolist;
    }

    return NULL;
}

/*
 * Returns WeeChat infolist "buffer".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_buffer_cb (const void *pointer, void *data,
                               const char *infolist_name,
                               void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;
    struct t_gui_buffer *ptr_buffer;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;

    /* invalid buffer pointer ? */
    if (obj_pointer && (!gui_buffer_valid (obj_pointer)))
        return NULL;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    if (obj_pointer)
    {
        /* build list with only one buffer */
        if (!gui_buffer_add_to_infolist (ptr_infolist, obj_pointer))
        {
            infolist_free (ptr_infolist);
            return NULL;
        }
        return ptr_infolist;
    }
    else
    {
        /* build list with all buffers matching arguments */
        for (ptr_buffer = gui_buffers; ptr_buffer;
             ptr_buffer = ptr_buffer->next_buffer)
        {
            if (!arguments || !arguments[0]
                || string_match (ptr_buffer->full_name, arguments, 0))
            {
                if (!gui_buffer_add_to_infolist (ptr_infolist, ptr_buffer))
                {
                    infolist_free (ptr_infolist);
                    return NULL;
                }
            }
        }
        return ptr_infolist;
    }

    return NULL;
}

/*
 * Returns WeeChat infolist "buffer_lines".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_buffer_lines_cb (const void *pointer, void *data,
                                     const char *infolist_name,
                                     void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;
    struct t_gui_line *ptr_line;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;
    (void) arguments;

    if (!obj_pointer)
        obj_pointer = gui_buffers;
    else
    {
        /* invalid buffer pointer ? */
        if (!gui_buffer_valid (obj_pointer))
            return NULL;
    }

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    for (ptr_line = ((struct t_gui_buffer *)obj_pointer)->own_lines->first_line;
         ptr_line; ptr_line = ptr_line->next_line)
    {
        if (!gui_line_add_to_infolist (ptr_infolist,
                                       ((struct t_gui_buffer *)obj_pointer)->own_lines,
                                       ptr_line))
        {
            infolist_free (ptr_infolist);
            return NULL;
        }
    }
    return ptr_infolist;
}

/*
 * Returns WeeChat infolist "filter".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_filter_cb (const void *pointer, void *data,
                               const char *infolist_name,
                               void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;
    struct t_gui_filter *ptr_filter;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;
    (void) obj_pointer;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    for (ptr_filter = gui_filters; ptr_filter;
         ptr_filter = ptr_filter->next_filter)
    {
        if (!arguments || !arguments[0]
            || string_match (ptr_filter->name, arguments, 0))
        {
            if (!gui_filter_add_to_infolist (ptr_infolist, ptr_filter))
            {
                infolist_free (ptr_infolist);
                return NULL;
            }
        }
    }
    return ptr_infolist;
}

/*
 * Returns WeeChat infolist "history".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_history_cb (const void *pointer, void *data,
                                const char *infolist_name,
                                void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;
    struct t_gui_history *ptr_history;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;
    (void) arguments;

    /* invalid buffer pointer ? */
    if (obj_pointer && (!gui_buffer_valid (obj_pointer)))
        return NULL;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    for (ptr_history = (obj_pointer) ?
             ((struct t_gui_buffer *)obj_pointer)->history : gui_history;
         ptr_history; ptr_history = ptr_history->next_history)
    {
        if (!gui_history_add_to_infolist (ptr_infolist, ptr_history))
        {
            infolist_free (ptr_infolist);
            return NULL;
        }
    }
    return ptr_infolist;
}

/*
 * Returns WeeChat infolist "hook".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_hook_cb (const void *pointer, void *data,
                             const char *infolist_name,
                             void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;

    /* invalid hook pointer ? */
    if (obj_pointer && !hook_valid (obj_pointer))
        return NULL;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    if (!hook_add_to_infolist (ptr_infolist, obj_pointer, arguments))
    {
        infolist_free (ptr_infolist);
        return NULL;
    }
    return ptr_infolist;
}

/*
 * Returns WeeChat infolist "hotlist".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_hotlist_cb (const void *pointer, void *data,
                                const char *infolist_name,
                                void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;
    struct t_gui_hotlist *ptr_hotlist;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;
    (void) obj_pointer;
    (void) arguments;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    for (ptr_hotlist = gui_hotlist; ptr_hotlist;
         ptr_hotlist = ptr_hotlist->next_hotlist)
    {
        if (!gui_hotlist_add_to_infolist (ptr_infolist, ptr_hotlist))
        {
            infolist_free (ptr_infolist);
            return NULL;
        }
    }
    return ptr_infolist;
}

/*
 * Returns WeeChat infolist "key".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_key_cb (const void *pointer, void *data,
                            const char *infolist_name,
                            void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;
    struct t_gui_key *ptr_key;
    int context;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;
    (void) obj_pointer;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    if (arguments && arguments[0])
        context = gui_key_search_context (arguments);
    else
        context = GUI_KEY_CONTEXT_DEFAULT;
    if (context >= 0)
    {
        for (ptr_key = gui_keys[context]; ptr_key;
             ptr_key = ptr_key->next_key)
        {
            if (!gui_key_add_to_infolist (ptr_infolist, ptr_key))
            {
                infolist_free (ptr_infolist);
                return NULL;
            }
        }
    }
    return ptr_infolist;
}

/*
 * Returns WeeChat infolist "layout".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_layout_cb (const void *pointer, void *data,
                               const char *infolist_name,
                               void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;
    struct t_gui_layout *ptr_layout;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;
    (void) obj_pointer;
    (void) arguments;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    for (ptr_layout = gui_layouts; ptr_layout;
         ptr_layout = ptr_layout->next_layout)
    {
        if (!gui_layout_add_to_infolist (ptr_infolist,ptr_layout))
        {
            infolist_free (ptr_infolist);
            return NULL;
        }
    }
    return ptr_infolist;
}

/*
 * Returns WeeChat infolist "nicklist".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_nicklist_cb (const void *pointer, void *data,
                                 const char *infolist_name,
                                 void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;

    /* invalid buffer pointer ? */
    if (!obj_pointer || (!gui_buffer_valid (obj_pointer)))
        return NULL;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    if (!gui_nicklist_add_to_infolist (ptr_infolist, obj_pointer, arguments))
    {
        infolist_free (ptr_infolist);
        return NULL;
    }
    return ptr_infolist;
}

/*
 * Returns WeeChat infolist "option".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_option_cb (const void *pointer, void *data,
                               const char *infolist_name,
                               void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;
    (void) obj_pointer;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    if (!config_file_add_to_infolist (ptr_infolist, arguments))
    {
        infolist_free (ptr_infolist);
        return NULL;
    }
    return ptr_infolist;
}

/*
 * Returns WeeChat infolist "plugin".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_plugin_cb (const void *pointer, void *data,
                               const char *infolist_name,
                               void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;
    struct t_weechat_plugin *ptr_plugin;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;

    /* invalid plugin pointer ? */
    if (obj_pointer && (!plugin_valid (obj_pointer)))
        return NULL;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    if (obj_pointer)
    {
        /* build list with only one plugin */
        if (!plugin_add_to_infolist (ptr_infolist, obj_pointer))
        {
            infolist_free (ptr_infolist);
            return NULL;
        }
        return ptr_infolist;
    }
    else
    {
        /* build list with all plugins matching arguments */
        for (ptr_plugin = weechat_plugins; ptr_plugin;
             ptr_plugin = ptr_plugin->next_plugin)
        {
            if (!arguments || !arguments[0]
                || string_match (ptr_plugin->name, arguments, 0))
            {
                if (!plugin_add_to_infolist (ptr_infolist, ptr_plugin))
                {
                    infolist_free (ptr_infolist);
                    return NULL;
                }
            }
        }
        return ptr_infolist;
    }

    return NULL;
}

/*
 * Returns WeeChat infolist "proxy".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_proxy_cb (const void *pointer, void *data,
                              const char *infolist_name,
                              void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;
    struct t_proxy *ptr_proxy;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;

    /* invalid proxy pointer ? */
    if (obj_pointer && (!proxy_valid (obj_pointer)))
        return NULL;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    if (obj_pointer)
    {
        /* build list with only one proxy */
        if (!proxy_add_to_infolist (ptr_infolist, obj_pointer))
        {
            infolist_free (ptr_infolist);
            return NULL;
        }
        return ptr_infolist;
    }
    else
    {
        /* build list with all proxies matching arguments */
        for (ptr_proxy = weechat_proxies; ptr_proxy;
             ptr_proxy = ptr_proxy->next_proxy)
        {
            if (!arguments || !arguments[0]
                || string_match (ptr_proxy->name, arguments, 0))
            {
                if (!proxy_add_to_infolist (ptr_infolist, ptr_proxy))
                {
                    infolist_free (ptr_infolist);
                    return NULL;
                }
            }
        }
        return ptr_infolist;
    }

    return NULL;
}

/*
 * Returns WeeChat infolist "url_options".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_url_options_cb (const void *pointer, void *data,
                                    const char *infolist_name,
                                    void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;
    int i;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;
    (void) obj_pointer;
    (void) arguments;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    for (i = 0; url_options[i].name; i++)
    {
        if (!weeurl_option_add_to_infolist (ptr_infolist, &url_options[i]))
        {
            infolist_free (ptr_infolist);
            return NULL;
        }
    }
    return ptr_infolist;
}

/*
 * Returns WeeChat infolist "window".
 *
 * Note: result must be freed after use with function weechat_infolist_free().
 */

struct t_infolist *
plugin_api_infolist_window_cb (const void *pointer, void *data,
                               const char *infolist_name,
                               void *obj_pointer, const char *arguments)
{
    struct t_infolist *ptr_infolist;
    struct t_gui_window *ptr_window;
    int number;
    char *error;

    /* make C compiler happy */
    (void) pointer;
    (void) data;
    (void) infolist_name;

    /* invalid window pointer ? */
    if (obj_pointer && (!gui_window_valid (obj_pointer)))
        return NULL;

    ptr_infolist = infolist_new (NULL);
    if (!ptr_infolist)
        return NULL;

    if (obj_pointer)
    {
        /* build list with only one window */
        if (!gui_window_add_to_infolist (ptr_infolist, obj_pointer))
        {
            infolist_free (ptr_infolist);
            return NULL;
        }
        return ptr_infolist;
    }
    else
    {
        if (arguments && arguments[0])
        {
            if ((string_strcasecmp (arguments, "current") == 0))
            {
                if (gui_current_window)
                {
                    if (!gui_window_add_to_infolist (ptr_infolist,
                                                     gui_current_window))
                    {
                        infolist_free (ptr_infolist);
                        return NULL;
                    }
                    return ptr_infolist;
                }
                return NULL;
            }
            /* check if argument is a window number */
            error = NULL;
            number = (int)strtol (arguments, &error, 10);
            if (error && !error[0])
            {
                ptr_window = gui_window_search_by_number (number);
                if (ptr_window)
                {
                    if (!gui_window_add_to_infolist (ptr_infolist,
                                                     ptr_window))
                    {
                        infolist_free (ptr_infolist);
                        return NULL;
                    }
                    return ptr_infolist;
                }
            }
            return NULL;
        }
        else
        {
            /* build list with all windows */
            for (ptr_window = gui_windows; ptr_window;
                 ptr_window = ptr_window->next_window)
            {
                if (!gui_window_add_to_infolist (ptr_infolist,
                                                 ptr_window))
                {
                    infolist_free (ptr_infolist);
                    return NULL;
                }
            }
            return ptr_infolist;
        }
    }

    return NULL;
}

/*
 * Initializes info/infolist plugin API.
 */

void
plugin_api_info_init ()
{
    /* WeeChat core info hooks */
    hook_info (NULL, "version",
               N_("WeeChat version"),
               NULL, &plugin_api_info_version_cb, NULL, NULL);
    hook_info (NULL, "version_number",
               N_("WeeChat version (as number)"),
               NULL, &plugin_api_info_version_number_cb, NULL, NULL);
    hook_info (NULL, "version_git",
               N_("WeeChat git version (output of command \"git describe\" "
                  "for a development version only, empty for a stable "
                  "release)"),
               NULL, &plugin_api_info_version_git_cb, NULL, NULL);
    hook_info (NULL, "date",
               N_("WeeChat compilation date/time"),
               NULL, &plugin_api_info_date_cb, NULL, NULL);
    hook_info (NULL, "pid",
               N_("WeeChat PID (process ID)"),
               NULL, &plugin_api_info_pid_cb, NULL, NULL);
    hook_info (NULL, "dir_separator",
               N_("directory separator"),
               NULL, &plugin_api_info_dir_separator_cb, NULL, NULL);
    hook_info (NULL, "weechat_dir",
               N_("WeeChat directory"),
               NULL, &plugin_api_info_weechat_dir_cb, NULL, NULL);
    hook_info (NULL, "weechat_libdir",
               N_("WeeChat \"lib\" directory"),
               NULL, &plugin_api_info_weechat_libdir_cb, NULL, NULL);
    hook_info (NULL, "weechat_sharedir",
               N_("WeeChat \"share\" directory"),
               NULL, &plugin_api_info_weechat_sharedir_cb, NULL, NULL);
    hook_info (NULL, "weechat_localedir",
               N_("WeeChat \"locale\" directory"),
               NULL, &plugin_api_info_weechat_localedir_cb, NULL, NULL);
    hook_info (NULL, "weechat_site",
               N_("WeeChat site"),
               NULL, &plugin_api_info_weechat_site_cb, NULL, NULL);
    hook_info (NULL, "weechat_site_download",
               N_("WeeChat site, download page"),
               NULL, &plugin_api_info_weechat_site_download_cb, NULL, NULL);
    hook_info (NULL, "weechat_upgrading",
               N_("1 if WeeChat is upgrading (command `/upgrade`)"),
               NULL, &plugin_api_info_weechat_upgrading_cb, NULL, NULL);
    hook_info (NULL, "charset_terminal",
               N_("terminal charset"),
               NULL, &plugin_api_info_charset_terminal_cb, NULL, NULL);
    hook_info (NULL, "charset_internal",
               N_("WeeChat internal charset"),
               NULL, &plugin_api_info_charset_internal_cb, NULL, NULL);
    hook_info (NULL, "locale",
               N_("locale used for translating messages"),
               NULL, &plugin_api_info_locale_cb, NULL, NULL);
    hook_info (NULL, "inactivity",
               N_("keyboard inactivity (seconds)"),
               NULL, &plugin_api_info_inactivity_cb, NULL, NULL);
    hook_info (NULL, "filters_enabled",
               N_("1 if filters are enabled"),
               NULL, &plugin_api_info_filters_enabled_cb, NULL, NULL);
    hook_info (NULL, "cursor_mode",
               N_("1 if cursor mode is enabled"),
               NULL, &plugin_api_info_cursor_mode_cb, NULL, NULL);
    hook_info (NULL, "term_width",
               N_("width of terminal"),
               NULL, &plugin_api_info_term_width_cb, NULL, NULL);
    hook_info (NULL, "term_height",
               N_("height of terminal"),
               NULL, &plugin_api_info_term_height_cb, NULL, NULL);
    hook_info (NULL, "color_ansi_regex",
               N_("POSIX extended regular expression to search ANSI escape "
                  "codes"),
               NULL, &plugin_api_info_color_ansi_regex_cb, NULL, NULL);
    hook_info (NULL, "color_term2rgb",
               N_("terminal color (0-255) converted to RGB color"),
               N_("color (terminal color: 0-255)"),
               &plugin_api_info_color_term2rgb_cb, NULL, NULL);
    hook_info (NULL, "color_rgb2term",
               N_("RGB color converted to terminal color (0-255)"),
               N_("rgb,limit (limit is optional and is set to 256 by default)"),
               &plugin_api_info_color_rgb2term_cb, NULL, NULL);
    hook_info (NULL, "nick_color",
               N_("get nick color code"),
               N_("nickname"),
               &plugin_api_info_nick_color_cb, NULL, NULL);
    hook_info (NULL, "nick_color_name",
               N_("get nick color name"),
               N_("nickname"),
               &plugin_api_info_nick_color_name_cb, NULL, NULL);
    hook_info (NULL, "uptime",
               N_("WeeChat uptime (format: \"days:hh:mm:ss\")"),
               N_("\"days\" (number of days) or \"seconds\" (number of "
                  "seconds) (optional)"),
               &plugin_api_info_uptime_cb, NULL, NULL);
    hook_info (NULL, "totp_generate",
               N_("generate a Time-based One-Time Password (TOTP)"),
               N_("secret (in base32), timestamp (optional, current time by "
                  "default), number of digits (optional, between 4 and 10, "
                  "6 is default and recommended value)"),
               &plugin_api_info_totp_generate_cb, NULL, NULL);
    hook_info (NULL, "totp_validate",
               N_("validate a Time-based One-Time Password (TOTP): 1 if TOTP "
                  "is correct, otherwise 0"),
               N_("secret (in base32), one-time password, "
                  "timestamp (optional), number of OTP after/before to test "
                  "(optional, 0 by default)"),
               &plugin_api_info_totp_validate_cb, NULL, NULL);

    /* WeeChat core infolist hooks */
    hook_infolist (NULL, "bar",
                   N_("list of bars"),
                   N_("bar pointer (optional)"),
                   N_("bar name (wildcard \"*\" is allowed) (optional)"),
                   &plugin_api_infolist_bar_cb, NULL, NULL);
    hook_infolist (NULL, "bar_item",
                   N_("list of bar items"),
                   N_("bar item pointer (optional)"),
                   N_("bar item name (wildcard \"*\" is allowed) (optional)"),
                   &plugin_api_infolist_bar_item_cb, NULL, NULL);
    hook_infolist (NULL, "bar_window",
                   N_("list of bar windows"),
                   N_("bar window pointer (optional)"),
                   NULL,
                   &plugin_api_infolist_bar_window_cb, NULL, NULL);
    hook_infolist (NULL, "buffer",
                   N_("list of buffers"),
                   N_("buffer pointer (optional)"),
                   N_("buffer name (wildcard \"*\" is allowed) (optional)"),
                   &plugin_api_infolist_buffer_cb, NULL, NULL);
    hook_infolist (NULL, "buffer_lines",
                   N_("lines of a buffer"),
                   N_("buffer pointer"),
                   NULL,
                   &plugin_api_infolist_buffer_lines_cb, NULL, NULL);
    hook_infolist (NULL, "filter",
                   N_("list of filters"),
                   NULL,
                   N_("filter name (wildcard \"*\" is allowed) (optional)"),
                   &plugin_api_infolist_filter_cb, NULL, NULL);
    hook_infolist (NULL, "history",
                   N_("history of commands"),
                   N_("buffer pointer (if not set, return global history) (optional)"),
                   NULL,
                   &plugin_api_infolist_history_cb, NULL, NULL);
    hook_infolist (NULL, "hook",
                   N_("list of hooks"),
                   N_("hook pointer (optional)"),
                   N_("type,arguments (type is command/timer/.., arguments to "
                      "get only some hooks (wildcard \"*\" is allowed), "
                      "both are optional)"),
                   &plugin_api_infolist_hook_cb, NULL, NULL);
    hook_infolist (NULL, "hotlist",
                   N_("list of buffers in hotlist"),
                   NULL,
                   NULL,
                   &plugin_api_infolist_hotlist_cb, NULL, NULL);
    hook_infolist (NULL, "key",
                   N_("list of key bindings"),
                   NULL,
                   N_("context (\"default\", \"search\", \"cursor\" or "
                      "\"mouse\") (optional)"),
                   &plugin_api_infolist_key_cb, NULL, NULL);
    hook_infolist (NULL, "layout",
                   N_("list of layouts"),
                   NULL,
                   NULL,
                   &plugin_api_infolist_layout_cb, NULL, NULL);
    hook_infolist (NULL, "nicklist",
                   N_("nicks in nicklist for a buffer"),
                   N_("buffer pointer"),
                   N_("nick_xxx or group_xxx to get only nick/group xxx "
                      "(optional)"),
                   &plugin_api_infolist_nicklist_cb, NULL, NULL);
    hook_infolist (NULL, "option",
                   N_("list of options"),
                   NULL,
                   N_("option name (wildcard \"*\" is allowed) (optional)"),
                   &plugin_api_infolist_option_cb, NULL, NULL);
    hook_infolist (NULL, "plugin",
                   N_("list of plugins"),
                   N_("plugin pointer (optional)"),
                   N_("plugin name (wildcard \"*\" is allowed) (optional)"),
                   &plugin_api_infolist_plugin_cb, NULL, NULL);
    hook_infolist (NULL, "proxy",
                   N_("list of proxies"),
                   N_("proxy pointer (optional)"),
                   N_("proxy name (wildcard \"*\" is allowed) (optional)"),
                   &plugin_api_infolist_proxy_cb, NULL, NULL);
    hook_infolist (NULL, "url_options",
                   N_("options for URL"),
                   NULL,
                   NULL,
                   &plugin_api_infolist_url_options_cb, NULL, NULL);
    hook_infolist (NULL, "window",
                   N_("list of windows"),
                   N_("window pointer (optional)"),
                   N_("\"current\" for current window or a window number (optional)"),
                   &plugin_api_infolist_window_cb, NULL, NULL);
}
