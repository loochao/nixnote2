#ifndef EDITORBUTTONBAR_H
#define EDITORBUTTONBAR_H
#include "gui/browserWidgets/toolbarwidgetaction.h"
#include <QToolBar>
#include <QMenu>
#include <QToolButton>

class ToolbarWidgetAction;

class EditorButtonBar : public QToolBar
{
    Q_OBJECT
private:
    QMenu *contextMenu;
    QAction *undoVisible;
    QAction *redoVisible;
    QAction *cutVisible;
    QAction *copyVisible;
    QAction *pasteVisible;
    QAction *boldVisible;
    QAction *italicVisible;
    QAction *underlineVisible;
    QAction *strikethroughVisible;
    QAction *leftJustifyVisible;
    QAction *centerJustifyVisible;
    QAction *rightJustifyVisible;
    QAction *hlineVisible;
    QAction *shiftRightVisible;
    QAction *shiftLeftVisible;
    QAction *buttonListVisible;
    QAction *numberListVisible;
    QAction *fontVisible;
    QAction *fontSizeVisible;
    QAction *todoVisible;
    QAction *highlightVisible;
    QAction *fontColorVisible;

    ToolbarWidgetAction *undoButton;
    ToolbarWidgetAction *redoButton;
    ToolbarWidgetAction *cutButton;
    ToolbarWidgetAction *copyButton;
    ToolbarWidgetAction *pasteButton;
    ToolbarWidgetAction *boldButton;
    ToolbarWidgetAction *italicButton;
    ToolbarWidgetAction *underlineButton;
    ToolbarWidgetAction *strikethroughButton;
    ToolbarWidgetAction *leftJustifyButton;
    ToolbarWidgetAction *centerJustifyButton;
    ToolbarWidgetAction *rightJustifyButton;
    ToolbarWidgetAction *hlineButton;
    ToolbarWidgetAction *shiftRightButton;
    ToolbarWidgetAction *shiftLeftButton;
    ToolbarWidgetAction *buttonListButton;
    ToolbarWidgetAction *numberListButton;
    QAction *highlightColor;
    QAction *fontColor;
    ToolbarWidgetAction *todoButton;
    ToolbarWidgetAction *spellButton;
    QAction *fontButton;
    QAction *fontSizeButton;



public:
    explicit EditorButtonBar(QWidget *parent = 0);
    ~EditorButtonBar();
    void contextMenuEvent(QContextMenuEvent *event);
    void saveVisibleButtons();
    void setupVisibleButtons();
    void addFontName(QComboBox *fontList);
    void addFontSize(QComboBox *fontSize);
    void addHighlightColor(QToolButton *button);
    void addFontColor(QToolButton *button);

signals:
    
public slots:
    void toggleUndoButtonVisible();
    void toggleRedoButtonVisible();
    void toggleCutButtonVisible();
    void toggleCopyButtonVisible();
    void togglePasteButtonVisible();
    void toggleBoldButtonVisible();
    void toggleItalicButtonVisible();
    void toggleUnderlineButtonVisible();
    void toggleStrikethroughButtonVisible();
    void toggleLeftJustifyButtonVisible();
    void toggleCenterJustifyButtonVisible();
    void toggleRightJustifyButtonVisible();
    void toggleHlineButtonVisible();
    void toggleShiftRightButtonVisible();
    void toggleShiftLeftButtonVisible();
    void toggleButtonListButtonVisible();
    void toggleNumberListButtonVisible();
    void toggleFontButtonVisible();
    void toggleTodoButtonVisible();
    void toggleFontSizeButtonVisible();
    void addAction(ToolbarWidgetAction *action);
    void toggleFontColorVisible();
    void toggleHighlightColorVisible();
    
};

#endif // EDITORBUTTONBAR_H