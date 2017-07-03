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


#include "dlgSourceEditorArea.h"
#include "edbee/edbee.h"
#include "edbee/models/textdocument.h"
#include "edbee/models/texteditorconfig.h"
#include "edbee/models/textgrammar.h"
#include "edbee/texteditorwidget.h"
#include "edbee/views/texteditorscrollarea.h"
#include "edbee/views/textrenderer.h"
#include "edbee/views/texttheme.h"
#include "edbee/models/texteditorkeymap.h"

#include "edbee/texteditorcontroller.h"
#include "edbee/models/texteditorcommandmap.h"

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QWidget>
#include <QToolButton>
#include <QScrollArea>
#include <QSplitter>

#include "FlowLayout.h"

//#include "ui_source_editor_area.h"

dlgSourceEditorArea::dlgSourceEditorArea(QWidget* pF) : QWidget(pF)
{
    // init generated dialog
    setupUi(this);

    // Configuring the editor widget with defaults

    edbee::TextEditorConfig* config = edbeeEditorWidget->config();

    config->beginChanges();

    config->setSmartTab(true); // I'm not fully sure what this does, but it says "Smart" so it must be good
    config->setCaretBlinkRate(200);

    config->setIndentSize(2); // 2 spaces is the Lua default

    config->setThemeName(QLatin1Literal("Mudlet"));
    config->setCaretWidth(1);

    config->endChanges();

    edbeeEditorWidget->textDocument()->setLanguageGrammar(
                edbee::Edbee::instance()->grammarManager()->detectGrammarWithFilename(QLatin1Literal("Buck.lua")));

    // disable shadows as their purpose (notify there is more text) is performed by scrollbars already
    edbeeEditorWidget->textScrollArea()->enableShadowWidget(false);


    // Give the keyboard shortscuts cheat sheet a flow layout

    FlowLayout* fl = new FlowLayout(wCheatSheet);

    //wCheatSheet->setMaximumHeight(400);
    wCheatSheet->setObjectName("cheatsheet");
    wCheatSheet->setStyleSheet("QFrame#command { border: 1px solid #555; background-color: rgba(255,255,255,0.3); border-radius: 1em; } QLabel { height: 30px; }");

    // This is for the floating shortcuts button to stay aligned to the right bottom

    edbeeEditorWidget->installEventFilter(this);

    // Only auto resize the editor, not the cheat sheet
    splitterEditorCheatSheet->setStretchFactor(1, 0);
    splitterEditorCheatSheet->setChildrenCollapsible(false);

    populateCheatSheet();

    scrollCheatSheet->setWidget(wCheatSheet);
    scrollCheatSheet->show();


    //scrollCheatSheet->setVisible(false);
//    wCheatSheet->setVisible(false);

}

void dlgSourceEditorArea::clearCheatSheet() {

    //delete wCheatSheet->findChild<FlowLayout>();

    while ( QFrame* w = wCheatSheet->findChild<QFrame*>(QLatin1Literal("herp"), Qt::FindDirectChildrenOnly))
        delete w;
}

void dlgSourceEditorArea::populateCheatSheet() {

    //QHashIterator<QString, QString> qhi(COMMANDS);

    //FlowLayout* fl = static_cast<FlowLayout>(wCheatSheet->layout());

    QListIterator<QString> commands(CHEATSHEETCOMMANDS);

    while(commands.hasNext()) {

        QString command = commands.next();

        // keyseq is 0 if no shortcut is found for a command
        edbee::TextEditorKey* keyseq = edbeeEditorWidget->keyMap()->get(command);

        if (keyseq) {
            QFrame* container = new QFrame();
            QHBoxLayout* layout = new QHBoxLayout();

            container->setObjectName("command");
            container->setMinimumWidth(300);
            container->setMaximumHeight(40);
            container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

            layout->addWidget(new QLabel(COMMANDS[command]));
            layout->addStretch();
            //layout->addWidget(new QLabel("="));

            QList<QString> keys = keyseq->sequence().toString().split("+");

            QListIterator<QString> qli(keys);

            while (qli.hasNext()) {
                QToolButton* button = new QToolButton(container);
                button->setText(qli.next());
                button->setMinimumHeight(20);

                //button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

                layout->addWidget(button);

                if (qli.hasNext()) { layout->addWidget(new QLabel("+")); }
            }

            //layout->addStretch();

            container->setLayout(layout);

            wCheatSheet->layout()->addWidget(container);
        }
    }
}

void dlgSourceEditorArea::on_btnCheatSheet_clicked() {
    dlgSourceEditorArea::toggleCheatSheet();
}

void dlgSourceEditorArea::toggleCheatSheet() {

    if (cheatSheetVisible()) {
        setCheatSheetVisible(false);
    } else {
        setCheatSheetVisible(true);
    }
}

void dlgSourceEditorArea::setCheatSheetVisible(bool value) {

    scrollCheatSheet->setVisible(value);
    mIsCheatSheetVisible = value;

    btnCheatSheet->setArrowType(mIsCheatSheetVisible ? Qt::ArrowType::DownArrow : Qt::ArrowType::UpArrow);
}

bool dlgSourceEditorArea::eventFilter(QObject *watched, QEvent *event) {
    if (watched==edbeeEditorWidget) {
        if (event->type()==QEvent::Resize) {
            QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
            btnCheatSheet->setGeometry(
                        resizeEvent->size().width()-32,
                        resizeEvent->size().height()-32,
                        btnCheatSheet->width(),
                        btnCheatSheet->height()
                    );
        }
    }

    return false;
}
