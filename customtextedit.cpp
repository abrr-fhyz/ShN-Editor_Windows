// customtextedit.cpp
#include "customtextedit.h"
#include "linenumberarea.h"
#include <QRegularExpression>

CustomTextEdit::CustomTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, &CustomTextEdit::blockCountChanged, this, &CustomTextEdit::updateLineNumberAreaWidth);
    connect(this, &CustomTextEdit::updateRequest, this, &CustomTextEdit::updateLineNumberArea);
    connect(this, &CustomTextEdit::cursorPositionChanged, this, &CustomTextEdit::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

void CustomTextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Tab) {
        handletabpress();
    }
    else if(event->key() == Qt::Key_Return) {
        handleEnterPress();
    }
    else {
        emit customKeyPress(event);
        QPlainTextEdit::keyPressEvent(event);
    }
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_I) {
        // Cmd + I pressed, perform auto-indentation for the whole code
        autoIndent();
        return;
    }
}
void CustomTextEdit::autoIndent()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);

    // Iterate through each line and perform indentation
    while (!cursor.atEnd()) {
        QString line = cursor.block().text();
        int leadingSpaces = line.indexOf(QRegularExpression("[^\\s]"));

        // Set the cursor position to the start of the line
        cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
        // Insert the appropriate number of spaces for indentation
        cursor.insertText(QString(leadingSpaces, ' '));
        // Move to the next line
        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor);
    }

    // Restore the original cursor position
    setTextCursor(cursor);
}
void CustomTextEdit::handletabpress()
{
    QTextCursor cursor = textCursor();
    cursor.insertText("\t");
//    //  cursor jeno text document er end e na thake
//    if (!cursor.atEnd()) {
//        QString currentLine = cursor.block().text();

//        // RB er position ber korlam
//        int rightBracketPos = currentLine.lastIndexOf(')');
//        int rightBracePos = currentLine.lastIndexOf('}');
//        int rightSquareBracketPos = currentLine.lastIndexOf(']');

//        // max RB position
//        int maxBracketPos = qMax(rightBracketPos, qMax(rightBracePos, rightSquareBracketPos));

//        if (maxBracketPos != -1) {
//            // erpor soray de max position e
//            cursor.setPosition(cursor.block().position() + maxBracketPos + 1);
//            setTextCursor(cursor);
//        }
//        else {
//            cursor.insertText("    ");
//        }
//    }
}

int CustomTextEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while(max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CustomTextEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CustomTextEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CustomTextEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CustomTextEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(0x36454F);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CustomTextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

int CustomTextEdit::indentationLevelOfCurrentLine()
{
    QTextCursor original = textCursor();
    QString document = toPlainText();
    moveCursorToStartOfLine();

    int indentationLevel = 0;
    int index = textCursor().position();
    qDebug() << document.at(index) << '\n';

    while(index < document.length()) {
        if(document.at(index) == '\t') {
            indentationLevel++;
            index++;
        }
        else {
            break;
        }
    }

    setTextCursor(original);
    return indentationLevel;
}

void CustomTextEdit::moveCursorToStartOfLine()
{
    QTextCursor cursor = textCursor();

    cursor.movePosition(QTextCursor::StartOfLine);
    setTextCursor(cursor);
}

void CustomTextEdit::insertTabs(int numTabs)
{
    for(int i = 0; i < numTabs; ++i) {
        textCursor().insertText("\t");
    }
}

bool CustomTextEdit::handleEnterPress()
{
    QString document = toPlainText();
    int indexToLeftOfCursor = textCursor().position() - 1;

    int currentIndent = indentationLevelOfCurrentLine();
//    qDebug() << currentIndent << '\n';
    QChar characterToLeftOfCursor = document.at(indexToLeftOfCursor);

    if(characterToLeftOfCursor == '{') {
        insertPlainText("\n");
        insertTabs(currentIndent);
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::Up);
        cursor.movePosition(QTextCursor::EndOfLine);
        setTextCursor(cursor);
        insertPlainText("\n");
        insertTabs(currentIndent + 1);
        return true;
    }
    else{
        insertPlainText("\n");
        insertTabs(currentIndent);
        return true;
    }
}
