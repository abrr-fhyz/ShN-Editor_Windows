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

    bool getGoBackward() const;

    QString text() const;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void goBack();

private:
    Ui::FindDialog *ui;

    QString m_text;
    bool wholeWord;
    bool caseSensitive;
    bool goBackward;

    void init();
    void save();
};

#endif // FINDDIALOG_H
