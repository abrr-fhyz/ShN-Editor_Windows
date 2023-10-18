#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTextStream>
#include <QColorDialog>
#include <QFont>
#include <QDebug>
#include <QDir>
#include <QString>
#include "finddialog.h"
#include "qsourcehighliter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionExit_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionRedo_triggered();

    void on_actionUndo_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZoom_Out_triggered();

    void on_actionBold_triggered();

    void on_actionItalic_triggered();

    void on_actionUnderline_triggered();

    void on_actionColor_triggered();

    void on_textEdit_textChanged();

    void on_actionFind_triggered();

    void on_actionReplace_triggered();

    void on_actionFont_triggered();

private:
    Ui::MainWindow *ui;
    bool m_changed;
    QString file_path;
    QSourceHighlite::QSourceHighliter *highlighter;

    void newFile();
    void openFile();
    void saveFile(QString path);
    void saveFileAs();
    void checksave();


    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
