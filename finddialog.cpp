#include "finddialog.h"
#include "ui_finddialog.h"

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

void FindDialog::on_buttonBox_accepted()
{

    save();
    accept();
}


void FindDialog::on_buttonBox_rejected()
{
    reject();
}

void FindDialog::goBack()
{
    save();
    goBackward = true;
    accept();

}

QString FindDialog::text() const
{
    return m_text;
}

bool FindDialog::getGoBackward() const
{
    return goBackward;
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
    QPushButton *btnBack = new QPushButton("Back",this);
    connect(btnBack,&QPushButton::clicked,this,&FindDialog::goBack);
    ui->buttonBox->addButton(btnBack,QDialogButtonBox::ActionRole);

}

void FindDialog::save()
{
    m_text = ui->lineEdit->text();
//    wholeWord = ui->chkMatchWholeWord->isChecked();
//    caseSensitive = ui->chkCaseSensitive->isChecked();
    goBackward = false;
}

