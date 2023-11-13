#include "finddialog.h"
#include "ui_finddialog.h"
#include "mainwindow.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    init();
    save();
}

FindDialog::~FindDialog()
{
    delete ui;
}

QString FindDialog::get_find_text() const
{
    return find_text;
}

QString FindDialog::get_replace_text() const
{
    return replace_text;
}


bool FindDialog::getCaseSensitive() const
{
    return caseSensitive;
}

bool FindDialog::getWholeWord() const
{
    return wholeWord;
}

void FindDialog::init()
{

}

void FindDialog::save()
{
    find_text = ui->lineEdit_find->text();
    replace_text = ui->lineEdit_replace->text();
    wholeWord = ui->wholeWord->isChecked();
    caseSensitive = ui->caseSensitive->isChecked();
    find = false;
    replace = false;
}

bool FindDialog::getReplace() const
{
    return replace;
}

bool FindDialog::getFind() const
{
    return find;
}

void FindDialog::on_find_clicked()
{
    save();
    find = true;
    replace = false;
    accept();
}

void FindDialog::on_replace_clicked()
{
    save();
    find = false;
    replace = true;
    accept();
}

