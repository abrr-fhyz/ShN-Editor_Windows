#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QCheckBox>
#include <QStringMatcher>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();


    bool getWholeWord() const;

    bool getCaseSensitive() const;


    QString get_find_text() const;
    QString get_replace_text() const;

    bool getFind() const;

    bool getReplace() const;

private slots:

    void on_find_clicked();

    void on_replace_clicked();

private:
    Ui::FindDialog *ui;

    QString find_text;
    QString replace_text;
    bool wholeWord;
    bool caseSensitive;

    void init();
    void save();

    bool find;
    bool replace;
};

#endif // FINDDIALOG_H
