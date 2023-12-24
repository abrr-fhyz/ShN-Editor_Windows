#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QTextEdit>
#include <QKeyEvent>
#include <QTextCursor>
#include <iostream>
#include <QTextBlock>
#include <QRegularExpression>
#include <QSplitter>
#include <QProcess>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QInputDialog>
#include <QLineEdit>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    file_path = "";
    m_changed = false;
    newFile();

    mainSplitter = new QSplitter(Qt::Horizontal, this);
    setCentralWidget(mainSplitter);

    customTextEdit = new CustomTextEdit(this);
    mainSplitter->addWidget(customTextEdit);

    terminalTextEdit = new QPlainTextEdit(this);
    terminalTextEdit->setReadOnly(true);
    mainSplitter->addWidget(terminalTextEdit);
    terminalTextEdit->setStyleSheet("background-color: black; color: white;"
                                    "font: 11pt \"Code New Roman\";");

    terminalProcess = new QProcess(this);

    connect(terminalProcess, &QProcess::readyReadStandardOutput, this, &MainWindow::terminalReadyRead);
    connect(terminalProcess, QOverload<QProcess::ProcessError>::of(&QProcess::errorOccurred),
            this, &MainWindow::terminalProcessError);


    startTerminalProcess();

    statusBar()->showMessage("Character: 0 Word: 0 Row: 1 Column: 1");

    connect(customTextEdit, &QPlainTextEdit::textChanged, this, &MainWindow::on_textEdit_textChanged);
    connect(customTextEdit, &QPlainTextEdit::textChanged, this, &MainWindow::updateCharacterCount);
    connect(customTextEdit, &CustomTextEdit::customKeyPress, this, &MainWindow::handleKeyPress);
    connect(customTextEdit, &QPlainTextEdit::textChanged, this, &MainWindow::handleTextChanged);

}


//PowerShell Commands
bool outputflag = false;
void MainWindow::terminalReadyRead()
{
    QByteArray data = terminalProcess->readAll();
    QString output = QString::fromUtf8(data);
    QStringList outputLines = output.split('\n', Qt::SkipEmptyParts);

    for (const QString& line : outputLines) {
        //stop at beginning of next powershell prompt
        if (line.contains("PS")) {
            outputflag = false;
        }
        if (outputflag) {
            terminalTextEdit->insertPlainText(line + "\n");
            qDebug() << line;
        }
        //start if execution command
        if (line.contains("./a")) {
            terminalTextEdit->insertPlainText("<Output>\n");
            outputflag = true;
        }
    }
}

void MainWindow::terminalProcessError(QProcess::ProcessError error)
{
    qDebug() << "Terminal process error: " << error;
}

void MainWindow::startTerminalProcess()
{
    QString powerShellExecutable = "C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe"; //default location for powershell executable

    if (!QFile::exists(powerShellExecutable)) {
        qDebug() << "Error: PowerShell executable not found at " << powerShellExecutable;
        return;
    }

    terminalProcess->start(powerShellExecutable, QStringList());
    if (!terminalProcess->waitForStarted()) {
        qDebug() << "Error starting PowerShell process: " << terminalProcess->errorString();
        QMessageBox::critical(this, "Error", "Failed to start PowerShell process.");
    }
}


void MainWindow::runTerminalCommand(const QString &command, const QString &input)
{
    if (terminalProcess->state() != QProcess::Running) {
        qDebug() << "Error: PowerShell process is not running.";
        return;
    }

    terminalProcess->setProcessChannelMode(QProcess::MergedChannels);

    //running the command line first in PowerShell and then putting in the input
    QString Command = command + "\n";
    QString Input= input + "\n";
    terminalProcess->write(Command.toUtf8());
    terminalProcess->waitForBytesWritten();
    terminalProcess->write(Input.toUtf8());
    terminalProcess->waitForBytesWritten();

}





// The rest of your code remains unchanged


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





void MainWindow::handleTextChanged()
{
        // Get the current text in the editor
        QString currentText = customTextEdit->toPlainText();

        // Check if the current text ends with "#include"
        if (currentText.endsWith("#include")) {
        // Append "<>" to the next line
        customTextEdit->textCursor().insertText("<>");
        customTextEdit->moveCursor(QTextCursor::Left);
        }
}





void MainWindow::handleKeyPress(QKeyEvent *event)
{

//PowerShell Equivalent of Input Intake
    QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_F2) {
        bool ok;
        QString input = QInputDialog::getMultiLineText(this, "Input", "Enter input:", "", &ok);

        if (ok) {
            // User provided input, construct the complete command without input redirection
            QFileInfo fileInfo(file_path);
            QString file_name = fileInfo.filePath();
            //QString command = "gcc " + file_name + " -o output && ./output";
            QString PSScript= "g++ '"+ file_name + "' -o a\n./a";
            runTerminalCommand(PSScript, input);  // Pass input directly to the command
        }
    }
    
    if(key->key() == Qt::Key_Enter) {
        qDebug() << "Enter Pressed\n";
    }
    QString pressedText = key->text();
    QString closingBracket = "";
    if (pressedText == "{") closingBracket = "}";
    else if (pressedText == "(") closingBracket = ")";
    else if (pressedText == "[") closingBracket = "]";
    else if (pressedText == "\"") closingBracket = "\"";

    if (!closingBracket.isEmpty()) {
        customTextEdit->textCursor().insertText(closingBracket);
        customTextEdit->moveCursor(QTextCursor::Left);
    }
}

void MainWindow::updateCharacterCount()
{
        QString text = customTextEdit->toPlainText();
        int characterCount = text.length() - text.count('\t');
        int wordCount = text.split(QRegularExpression("\\W+"), Qt::SkipEmptyParts).count();
        QTextCursor cursor = customTextEdit->textCursor();
        int row = cursor.blockNumber() + 1;  // Add 1 because blockNumber() is zero-based
        int column = cursor.columnNumber() + 1;  // Add 1 because columnNumber() is zero-based
        statusBar()->showMessage("Character: " + QString::number(characterCount) +
                                 "  Word: " + QString::number(wordCount) +
                                 " Row: " + QString::number(row) +
                                 " Column: " + QString::number(column) );
}

void MainWindow::on_textEdit_textChanged()
{
    m_changed = true;
}

void MainWindow::newFile()
{
//    if (!customTextEdit) {
//            qDebug() << "Error: QPlainTextEdit is not valid.";
//            return;
//    }

    // Check if d_func() returns a valid pointer

//    customTextEdit->clear();
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

    QMessageBox::StandardButton value = QMessageBox::question(this,"Save File","You have unsaved changes, would you like to save them?");
    if(value != QMessageBox::StandardButton::Yes) return;

    if(file_path.isEmpty()){
        saveFileAs();
    }
    else{
        saveFile(file_path);
    }


}

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




