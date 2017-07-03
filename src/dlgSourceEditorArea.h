#ifndef MUDLET_DLGSOURCEEDITORAREA_H
#define MUDLET_DLGSOURCEEDITORAREA_H

/***************************************************************************
 *   Copyright (C) 2008-2009 by Heiko Koehn - KoehnHeiko@googlemail.com    *
 *   Copyright (C) 2014 by Ahmed Charles - acharles@outlook.com            *
 *   Copyright (C) 2017 by Tom Scheper - scheper@gmail.com                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "pre_guard.h"
#include "ui_source_editor_area.h"
#include "post_guard.h"
#include <QSet>

class dlgSourceEditorArea : public QWidget, public Ui::source_editor_area
{
    Q_OBJECT

    Q_DISABLE_COPY(dlgSourceEditorArea)

public:
    dlgSourceEditorArea(QWidget*);
    void toggleCheatSheet();
    bool cheatSheetVisible() { return mIsCheatSheetVisible; }
    void setCheatSheetVisible(bool value);

private:
    bool mIsCheatSheetVisible=false;
    void populateCheatSheet();
    void clearCheatSheet();
    const QHash<QString, QString> COMMANDS{
        {"add_caret_prev_line", "Add cursor to previous line"},
        {"add_caret_next_line", "Add cursor to next line"},
        {"sel_reset", "Deselect all"},
        {"ins_newline", "Insert empty line"},
        {"ins_newline_before", "Insert empty line above"},
        {"ins_newline_after", "Insert empty line below"},
        {"del_word_left", "Delete to start of word"},
        {"del_word_right", "Delete to end of word"},
        {"del_line_left", "Delete to start of line"},
        {"del_line_right", "Delete to end of line"},
        {"duplicate", "Duplicate"},
        {"toggle_comment", "Toggle comment"},
        {"toggle_block_comment", "Toggle block comment"},
        {"indent", "Indent"},
        {"outdent", "Outdent"},
        {"undo", "Undo"},
        {"redo", "Redo"},
        {"soft_undo", "Soft undo"},
        {"soft_redo", "Soft redo"},
        {"find_use_sel", "Find and use selection"},
        {"find_next_match", "Find next match"},
        {"find_prev_match", "Find previous match"},
        {"sel_next_match", "Select next match"},
        {"sel_prev_match", "Select previous match"},
        {"sel_all_matches", "Select all matches"},
        {"select_under_expand", "Select under expand"},
        {"select_all_under", "Select all under"}
      };

    // QHash keys are unordered, so I'm using this QList to determine the
    // order of the commands shown in the cheat sheet

    const QList<QString> CHEATSHEETCOMMANDS{
        "undo",
        "redo",
        "soft_undo",
        "soft_redo",
        "toggle_comment",
        "toggle_block_comment",
        "del_word_left",
        "del_word_right",
        "del_line_left",
        "del_line_right",
        "ins_newline",
        "ins_newline_before",
        "ins_newline_after",
        "indent",
        "outdent",
        "duplicate",
        "find_next_match",
        "find_prev_match",
        "find_use_sel",
        "sel_next_match",
        "sel_prev_match",
        "sel_all_matches",
        "select_under_expand",
        "select_all_under"
    };

        /*"goto_next_char", "goto_prev_char", "goto_next_word", "goto_prev_word",
        "goto_bol", "goto_eol", "goto_next_line", "goto_prev_line", "goto_bof",
        "goto_eof", "goto_page_down", "goto_page_up", "sel_next_char", "sel_prev_char",
        "sel_next_word", "sel_prev_word", "sel_to_bol", "sel_to_eol", "sel_to_next_line",
        "sel_to_prev_Line", "sel_to_bof", "sel_to_eof", "sel_page_down", "sel_page_up",
        "sel_all", "sel_word", "sel_line", "sel_prev_line", "add_caret_prev_line",
        "add_caret_next_line", "sel_reset", "ins_newline", "ins_newline_before",
        "ins_newline_after", "del_left", "del_word_left", "del_line_left", "del_right",
        "del_word_right", "del_line_right", "duplicate", "toggle_comment",
        "toggle_block_comment", "tab", "tab_back", "indent", "outdent", "undo", "redo",
        "soft_undo", "soft_redo", "copy", "cut", "paste", "debug_dump_scopes",
        "debug_rebuild_scopes", "debug_dump_undo_stack", "debug_dump_character_codes",
        "find_use_sel", "find_next_match", "find_prev_match", "sel_next_match",
        "sel_prev_match", "sel_all_matches", "select_under_expand", "select_all_under"*/
private slots:
    void on_btnCheatSheet_clicked();
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // MUDLET_DLGSOURCEEDITORAREA_H
