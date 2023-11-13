#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include "customtextedit.h"

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CustomTextEdit *editor) : QWidget(editor), codeEditor(editor)
    {}

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CustomTextEdit *codeEditor;
};

#endif // LINENUMBERAREA_H
