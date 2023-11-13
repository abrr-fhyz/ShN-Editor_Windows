// customtextedit.cpp
#include "customtextedit.h"

CustomTextEdit::CustomTextEdit(QWidget *parent)
    : QTextEdit(parent)
{
}

void CustomTextEdit::keyPressEvent(QKeyEvent *event)
{
    emit customKeyPress(event);
    QTextEdit::keyPressEvent(event);
}
