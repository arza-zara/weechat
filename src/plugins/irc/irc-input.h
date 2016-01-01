/*
 * Copyright (C) 2003-2016 Sébastien Helleu <flashcode@flashtux.org>
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

#ifndef WEECHAT_IRC_INPUT_H
#define WEECHAT_IRC_INPUT_H 1

struct t_gui_buffer;

extern void irc_input_user_message_display (struct t_gui_buffer *buffer,
                                            int action, const char *text);
extern int irc_input_data_cb (void *data, struct t_gui_buffer *buffer,
                              const char *input_data);
extern int irc_input_send_cb (void *data, const char *signal,
                              const char *type_data, void *signal_data);

#endif /* WEECHAT_IRC_INPUT_H */
