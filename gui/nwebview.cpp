#include "nwebview.h"
#include "nwebpage.h"
#include "global.h"
#include <QWebFrame>
#include <QMessageBox>
#include <QtWebKit>
#include "./libencrypt/encrypt_global.h"
#include "./libencrypt/encrypt.h"
#include "../dialog/endecryptdialog.h"


extern Global global;


//****************************************************
//* This inherits everything from QWebView.  It is
//* useful to allow us to setup things that are
//* needed to view & edit notes.
//****************************************************

// Override the constructor so we always use a NWebPage
// rather than a QWebPage
NWebView::NWebView(NBrowserWindow *parent) :
    QWebView(parent)
{
    this->parent = parent;
    editorPage = new NWebPage();
    setPage(editorPage);
    isDirty = false;

    contextMenu = new QMenu(this);
    openAction = new QAction(tr("Open"), this);
    contextMenu->addAction(openAction);
    contextMenu->addSeparator();

    cutAction = new QAction(tr("Cut"), this);
    contextMenu->addAction(cutAction);
    connect(cutAction, SIGNAL(triggered()), parent, SLOT(cutButtonPressed()));

    copyAction = new QAction(tr("Copy"), this);
    contextMenu->addAction(copyAction);
    connect(copyAction, SIGNAL(triggered()), parent, SLOT(copyButtonPressed()));

    pasteAction = new QAction(tr("Paste"), this);
    contextMenu->addAction(pasteAction);
    connect(pasteAction, SIGNAL(triggered()), parent, SLOT(pasteButtonPressed()));

    pasteWithoutFormatAction = new QAction(tr("Paste Without Formatting"), this);
    pasteWithoutFormatAction->setText(tr("Paste Without Formatting"));
    contextMenu->addAction(pasteWithoutFormatAction);
    connect(pasteWithoutFormatAction, SIGNAL(triggered()), parent, SLOT(pasteWithoutFormatButtonPressed()));
    contextMenu->addSeparator();

    QMenu *colorMenu = new QMenu(tr("Background Color"));
    QAction *action = setupColorMenuOption(tr("White"));
    colorMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(setBackgroundWhite()));
    action = setupColorMenuOption(tr("Red"));
    colorMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(setBackgroundRed()));
    action = setupColorMenuOption(tr("Blue"));
    colorMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(setBackgroundBlue()));
    action = setupColorMenuOption(tr("Green"));
    colorMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(setBackgroundGreen()));
    action = setupColorMenuOption(tr("Yellow"));
    colorMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(setBackgroundYellow()));
    action = setupColorMenuOption(tr("Black"));
    colorMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(setBackgroundBlack()));
    action = setupColorMenuOption(tr("Grey"));
    colorMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(setBackgroundGrey()));
    action = setupColorMenuOption(tr("Purple"));
    colorMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(setBackgroundPurple()));
    action = setupColorMenuOption(tr("Brown"));
    colorMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(setBackgroundBrown()));
    action = setupColorMenuOption(tr("Orange"));
    colorMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(setBackgroundOrange()));
    action = setupColorMenuOption(tr("Powder Blue"));
    colorMenu->addAction(action);
    connect(action, SIGNAL(triggered()), this, SLOT(setBackgroundPowderBlue()));

    contextMenu->addMenu(colorMenu);
    contextMenu->addSeparator();

    todoAction = new QAction(tr("To-do"), this);
    contextMenu->addAction(todoAction);
    connect(todoAction, SIGNAL(triggered()), parent, SLOT(todoButtonPressed()));

    encryptAction = new QAction(tr("Encrypted Selected Text"), this);
    contextMenu->addAction(encryptAction);
    connect(encryptAction, SIGNAL(triggered()), parent, SLOT(encryptButtonPressed()));

    insertLinkAction = new QAction(tr("Insert Hyperlink"), this);
    contextMenu->addAction(insertLinkAction);
    connect(insertLinkAction, SIGNAL(triggered()),parent, SLOT(insertLinkButtonPressed()));

    insertQuickLinkAction = new QAction(tr("Quick Link"), this);
    contextMenu->addAction(insertQuickLinkAction);
    connect(insertQuickLinkAction, SIGNAL(triggered()),parent, SLOT(insertQuickLinkButtonPressed()));

    insertLatexAction = new QAction(tr("Insert LaTeX Formula"), this);
    contextMenu->addAction(insertLatexAction);
    connect(insertLatexAction, SIGNAL(triggered()),parent, SLOT(insertLatexButtonPressed()));
    contextMenu->addSeparator();

    tableMenu = new QMenu(tr("Table"));
    contextMenu->addMenu(tableMenu);
    insertTableAction = new QAction(tr("Insert Table"), this);
    tableMenu->addAction(insertTableAction);
    connect(insertTableAction, SIGNAL(triggered()), parent, SLOT(insertTableButtonPressed()));
    tableMenu->addSeparator();
    insertTableRowAction = new QAction(tr("Insert Row"), this);
    tableMenu->addAction(insertTableRowAction);
    connect(insertTableRowAction, SIGNAL(triggered()), parent, SLOT(insertTableRowButtonPressed()));
    insertTableColumnAction = new QAction(tr("Insert Column"), this);
    tableMenu->addAction(insertTableColumnAction);
    connect(insertTableColumnAction, SIGNAL(triggered()), parent, SLOT(insertTableColumnButtonPressed()));
    tableMenu->addSeparator();
    deleteTableRowAction = new QAction(tr("Delete Row"), this);
    tableMenu->addAction(deleteTableRowAction);
    connect(deleteTableRowAction, SIGNAL(triggered()), parent, SLOT(deleteTableRowButtonPressed()));
    deleteTableColumnAction = new QAction(tr("Delete Column"), this);
    tableMenu->addAction(deleteTableColumnAction);
    connect(deleteTableColumnAction, SIGNAL(triggered()), parent, SLOT(deleteTableColumnButtonPressed()));
    contextMenu->addSeparator();

    imageMenu = new QMenu(tr("Image"));
    contextMenu->addMenu(imageMenu);
    rotateImageLeftAction = new QAction(tr("Rotate Left"), this);
    imageMenu->addAction(rotateImageLeftAction);
    connect(rotateImageLeftAction, SIGNAL(triggered()), parent, SLOT(rotateImageLeftButtonPressed()));
    rotateImageRightAction = new QAction(tr("Rotate Right"), this);
    imageMenu->addAction(rotateImageRightAction);
    connect(rotateImageRightAction, SIGNAL(triggered()), parent, SLOT(rotateImageRightButtonPressed()));
    contextMenu->addSeparator();

    downloadAttachmentAction = pageAction(QWebPage::DownloadLinkToDisk);
    downloadAttachmentAction->setText(tr("Save Attachment"));
    contextMenu->addAction(downloadAttachmentAction);

    connect(editorPage, SIGNAL(contentsChanged()), this, SLOT(editAlert()));
    editorPage->setContentEditable(true);

    connect(this->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(exposeToJavascript()));
}


QAction* NWebView::setupColorMenuOption(QString color) {
    QAction *backgroundColor = new QAction(color, this);
    color = color.replace(" ", "");
    //connect(backgroundColor, SIGNAL(triggered()), this, SLOT()
    return backgroundColor;
}

void NWebView::editAlert() {
    //if (!isDirty) {
    isDirty = true;
    emit(noteChanged());
    //}
}


void NWebView::exposeToJavascript() {
    page()->mainFrame()->addToJavaScriptWindowObject("editorWindow", this);
}



void NWebView::decryptText(QString id, QString text, QString hint) {
    /* Suppress unused */
    id=id;
    text=text;
    hint=hint;


    EnCrypt crypt;
    QString plainText;
/*
//    // First, try to decrypt with any keys we already have
    for (int i=0; i<global.passwordRemember.size(); i++) {
        QString key = global.passwordRemember[i].second;
        bool retVal = crypt.decrypt(text, plainText, key);
        if (retVal && plainText != "") {
            //slot = new String(Long.toString(l));
            //Global.passwordSafe.put(slot, Global.passwordRemember.get(i));
            //removeEncryption(id, plainText, false, slot);
            return;
        }
    }


    EnDecryptDialog dialog;
    dialog.hint.setText(hint);
    bool goodReturn = true;
    while (goodReturn && (plainText == "" || !dialog.okPressed)) {
        dialog.exec();
        if (!dialog.okPressed) {
            return;
        }
        QString pswd = dialog.password.text().trimmed();
        crypt.decrypt(text, plainText, pswd);
        if (plainText == "") {
            QMessageBox mb;
            mb.warning(this, tr("Incorrect Password"), tr("The password entered is not correct"));
        }
    }
//    Pair<String,String> passwordPair = new Pair<String,String>();
//    passwordPair.setFirst(dialog.getPassword());
//    passwordPair.setSecond(dialog.getHint());
//    Global.passwordSafe.put(slot, passwordPair);
//    removeEncryption(id, plainText, dialog.permanentlyDecrypt(), slot);
//    if (dialog.rememberPassword()) {
//        Pair<String, String> pair = new Pair<String,String>();
//        pair.setFirst(dialog.getPassword());
//        pair.setSecond(dialog.getHint());
//        Global.passwordRemember.add(pair);
//    }
*/
}



void NWebView::contextMenuEvent(QContextMenuEvent *event) {
    if (event != NULL)
        contextMenu->exec(event->globalPos());
}


bool NWebView::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = (QKeyEvent*)event;
        if (ke->key() == Qt::Key_Tab) {
            parent->tabPressed();
            ke->accept();
            return true;
        }
        if (ke->key() == Qt::Key_Backtab) {
            parent->backtabPressed();
            ke->accept();
            return true;
        }
    }
    return QWebView::event(event);
}


void NWebView::keyPressEvent(QKeyEvent *e) {
    // This is done because if we set the content as editable, the scroll keys are
    // ignored by wbkit.
    if (e->key() == Qt::Key_PageUp || e->key() == Qt::Key_PageDown) {
        int bottom = this->page()->mainFrame()->geometry().bottom();
        int top = this->page()->mainFrame()->geometry().top();
        int scrollValue = top-bottom;
        if (e->key() == Qt::Key_PageDown)
            scrollValue = -1*scrollValue;
        page()->mainFrame()->scroll(0,scrollValue);
    }
    QWebView::keyPressEvent(e);
}



void NWebView::downloadAttachment(QNetworkRequest *req) {
    emit(downloadAttachmentRequested(req));
}



void NWebView::setBackgroundWhite() { parent->setBackgroundColor("white"); }
void NWebView::setBackgroundRed() { parent->setBackgroundColor("red"); }
void NWebView::setBackgroundBlue() { parent->setBackgroundColor("blue"); }
void NWebView::setBackgroundGreen() { parent->setBackgroundColor("green"); }
void NWebView::setBackgroundYellow() { parent->setBackgroundColor("yellow"); }
void NWebView::setBackgroundBlack() { parent->setBackgroundColor("black"); }
void NWebView::setBackgroundPurple() { parent->setBackgroundColor("purple"); }
void NWebView::setBackgroundBrown() { parent->setBackgroundColor("brown"); }
void NWebView::setBackgroundGrey() { parent->setBackgroundColor("grey"); }
void NWebView::setBackgroundOrange() { parent->setBackgroundColor("orange"); }
void NWebView::setBackgroundPowderBlue() { parent->setBackgroundColor("powderblue"); }

void NWebView::printNodeName(QString s) {
    QLOG_DEBUG() << s;
}
