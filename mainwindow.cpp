#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QTextEdit>
#include <QKeyEvent>
#include <QTextCursor>
#include <iostream>
#include <QTextBlock>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    customTextEdit = new CustomTextEdit(this);  // Use CustomTextEdit instead of QTextEdit
    setCentralWidget(customTextEdit);

    file_path = "";
    m_changed = false;
    newFile();

    statusBar()->showMessage("Character: 0 Word: 0 Row: 0 Column: 0");

    connect(customTextEdit, &QPlainTextEdit::textChanged, this, &MainWindow::on_textEdit_textChanged);
    connect(customTextEdit, &QPlainTextEdit::textChanged, this, &MainWindow::updateCharacterCount);
    connect(customTextEdit, &CustomTextEdit::customKeyPress, this, &MainWindow::handleKeyPress);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered()
{
    checksave();
    newFile();
}


void MainWindow::on_actionOpen_triggered()
{
        checksave();
        openFile();
}


void MainWindow::on_actionSave_triggered()
{
        saveFile(file_path);
}


void MainWindow::on_actionSave_As_triggered()
{
        saveFileAs();
}


void MainWindow::on_actionExit_triggered()
{
        checksave();
        close();
}


void MainWindow::on_actionCut_triggered()
{
        customTextEdit->cut();
}


void MainWindow::on_actionCopy_triggered()
{
        customTextEdit->copy();
}


void MainWindow::on_actionPaste_triggered()
{
        customTextEdit->paste();
}


void MainWindow::on_actionRedo_triggered()
{
        customTextEdit->redo();
}


void MainWindow::on_actionUndo_triggered()
{
        customTextEdit->undo();
}


void MainWindow::on_actionZoom_In_triggered()
{
        customTextEdit->zoomIn();
}


void MainWindow::on_actionZoom_Out_triggered()
{
        customTextEdit->zoomOut();
}


void MainWindow::on_actionBold_triggered()
{
//        QFont font = customTextEdit->currentFont();
//        font.bold() ? font.setBold(false) : font.setBold(true);
//        customTextEdit->setCurrentFont(font);
//        m_changed = true;

}


void MainWindow::on_actionItalic_triggered()
{
//        QFont font = customTextEdit->currentFont();
//        font.italic() ? font.setItalic(false) : font.setItalic(true);
//        customTextEdit->setCurrentFont(font);
//        m_changed = true;

}


void MainWindow::on_actionUnderline_triggered()
{
//        QFont font = customTextEdit->currentFont();
//        font.underline() ? font.setUnderline(false) : font.setUnderline(true);
//        customTextEdit->setCurrentFont(font);
//        m_changed = true;

}


void MainWindow::on_actionColor_triggered()
{
//        QColor current = customTextEdit->currentCharFormat().foreground().color();
//        QColor color = QColorDialog::getColor(current,this,"Choose a color");
//        customTextEdit->setTextColor(color);
//        m_changed = true;
}

void MainWindow::on_actionFont_triggered()
{
//        bool what;
//        QFont font = QFontDialog::getFont(&what,customTextEdit->currentFont(),
//                                          this,"Choose a font");
//        if(what) customTextEdit->setCurrentFont(font);
}

//QString MainWindow::getClosingBracket(const QString &openingBracket)
//{
//        if (openingBracket == "{")
//            return "}";
//        else if (openingBracket == "(")
//            return ")";
//        else if (openingBracket == "[")
//            return "]";
//        return "";
//}

void MainWindow::handleKeyPress(QKeyEvent *event)
{
        QString pressedText = event->text();
        QString closingBracket = "";
        if (pressedText == "{") closingBracket = "}";
        else if (pressedText == "(") closingBracket = ")";
        else if (pressedText == "[") closingBracket = "]";
        //event->text() = closingBracket;
//        QTextCursor cursor = customTextEdit->textCursor();
//        int currentPos = cursor.position();
//        QTextBlock currentBlock = cursor.block();
//        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);

//        // Remove the undesired text
//        cursor.removeSelectedText();

//        // Move the cursor back to the original position
//        cursor.setPosition(currentBlock.position());

//        // Set the modified cursor back to the text edit
//        customTextEdit->setTextCursor(cursor);

        if (!closingBracket.isEmpty()) {
//            int cursorPosition = customTextEdit->textCursor().position();
//            qDebug() << pressedText + closingBracket << '\n';

            customTextEdit->textCursor().insertText(closingBracket);
            customTextEdit->moveCursor(QTextCursor::Left);

//            customTextEdit->setTextCursor(QTextCursor(customTextEdit->document()->find(closingBracket,cursorPosition)));
//            event->accept();
        }
//        cursor.select(QTextCursor::WordUnderCursor);
//        cursor.insertText(closingBracket);
//            cursor.setPosition(currentPos);
//        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);

//            customTextEdit->setTextCursor(cursor);



}

void MainWindow::updateCharacterCount()
{
        QString text = customTextEdit->toPlainText();
        int characterCount = text.length() - text.count('\t');
        int wordCount = text.split(QRegularExpression("\\W+"), Qt::SkipEmptyParts).count();
        QTextCursor cursor = customTextEdit->textCursor();
        int row = cursor.blockNumber() + 1;  // Add 1 because blockNumber() is zero-based
        int column = cursor.columnNumber() + 1;  // Add 1 because columnNumber() is zero-based
        statusBar()->showMessage("Character: " + QString::number(characterCount) + "  Word: " + QString::number(wordCount) + " Row: " + QString::number(row) + " Column: " + QString::number(column) );
}

void MainWindow::on_textEdit_textChanged()
{
    m_changed = true;
}

void MainWindow::newFile()
{
    customTextEdit->clear();
    ui->statusbar->showMessage("New File");
    file_path = "";
    m_changed = false;
}

void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this,"Open File");
    if(path.isEmpty()) return;

    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this,"Error",file.errorString());
    }

    QTextStream stream(&file);
    customTextEdit->setPlainText(stream.readAll());
    file.close();

    file_path = path;
    ui->statusbar->showMessage(file_path);
    m_changed = false;
}


void MainWindow::saveFile(QString path)
{
    if(path.isEmpty()){
        saveFileAs();
        return;
    }

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)){
        QMessageBox::critical(this,"Error",file.errorString());
        ui->statusbar->showMessage("Error Couldn't save the file");
        saveFileAs();
        return;
    }
    QTextStream stream(&file);
    stream << customTextEdit->toPlainText();
    file.close();

    file_path = path;
    ui->statusbar->showMessage(file_path);
    m_changed = false;

}

void MainWindow::saveFileAs()
{
    QString path = QFileDialog::getSaveFileName(this,"Save File");
    if(path.isEmpty()) return;
    saveFile(path);
}

void MainWindow::checksave()
{
    std::cout << std::boolalpha << m_changed << '\n';
    if(!m_changed) return;

    QMessageBox::StandardButton value = QMessageBox::question(this,"Save File","You have un-saved changes, would like to save them?");
    if(value != QMessageBox::StandardButton::Yes) return;

    if(file_path.isEmpty()){
        saveFileAs();
    }
    else{
        saveFile(file_path);
    }


}

//void MainWindow::autoBracketClose()
//{
//    QTextCursor cursor = customTextEdit->textCursor();
//    int currentPos = customTextEdit->cursor.position();
//    QTextBlock currentBlock = customTextEdit->cursor.block();

//    cursor.select(QTextCursor::WordUnderCursor);
//    QString selectedText = cursor.selectedText();

//    if (selectedText == "(")
//    {
//        cursor.insertText(")");
//    }
//    else if (selectedText == "{")
//    {
//        cursor.insertText("}");
//    }
//    else if (selectedText == "[")
//    {
//        cursor.insertText("]");
//    }

//    // Restore the cursor position
//    cursor.setPosition(currentPos);
//    cursor.setBlock(currentBlock);
//    customTextEdit->setTextCursor(cursor);
//}

void MainWindow::closeEvent(QCloseEvent *event)
{
    checksave();
    event->accept();
}


void MainWindow::on_actionFind_triggered()
{
    FindDialog *dialogue = new FindDialog();
    bool flag = false;
    QTextDocument *document = customTextEdit->document();

    QTextCursor highlightCursor(document);
    QTextCursor cursor(document);
    while(!flag) {
        if(!dialogue->exec()) return;

        QTextDocument::FindFlags flags;

        cursor.beginEditBlock();

        if(dialogue->getCaseSensitive()) flags = flags |
                    QTextDocument::FindFlag::FindCaseSensitively;

        if(dialogue->getWholeWord()) flags = flags |
                    QTextDocument::FindFlag::FindWholeWords;
        //        std::cout << dialogue->getFind() << '\n';

        if(dialogue->getFind()) {

            highlightCursor = document->find(dialogue->get_find_text(),
                                             highlightCursor,
                                             flags);

            highlightCursor.movePosition(QTextCursor::Left,
                                         QTextCursor::KeepAnchor,
                                         dialogue->get_find_text().length());

            customTextEdit->setTextCursor(highlightCursor);

            //            qDebug() << dialogue->text().length() << '\n';

            for(int i = 0; i < dialogue->get_find_text().length(); ++i) {
                customTextEdit->moveCursor(QTextCursor::Right,
                                         QTextCursor::KeepAnchor);
            }

            highlightCursor.movePosition(QTextCursor::Right,
                                         QTextCursor::KeepAnchor,
                                         dialogue->get_find_text().length());
            //            cursor.endEditBlock();
        }

        else if(dialogue->getReplace()) {
            QString text_to_be_replaced = customTextEdit->toPlainText();
            text_to_be_replaced = text_to_be_replaced.replace(
                dialogue->get_find_text(), dialogue->get_replace_text());
            customTextEdit->setPlainText(text_to_be_replaced);
            flag = true;
        }
        cursor.endEditBlock();
    }

}




