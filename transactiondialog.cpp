#include "transactiondialog.h"
#include "ui_transactiondialog.h"
#include<QButtonGroup>

TransactionDialog::TransactionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionDialog)
{    
    ui->setupUi(this);
    //控件初始化
    QDate current_date=QDate::currentDate();
    ui->dateEdit->setDate(current_date);
    ui->radioButton->setChecked(true);

    //OK,Cancel按钮连接
    connect(ui->buttonBox,&QDialogButtonBox::accepted,[=](){
        confirmFlag=true;
        setSalesInformation();
        close();
    });
    connect(ui->buttonBox,&QDialogButtonBox::rejected,[=](){
        close();
    });
}

void TransactionDialog::setSalesInformation()
{
    QString pi=ui->lineEdit_2->text();
    QDate sd=ui->dateEdit->date();
    int sn=ui->spinBox->value();
    double sp=ui->doubleSpinBox->value();
    QString ci=ui->lineEdit->text();
    QButtonGroup bg;
    bg.addButton(ui->radioButton,1);
    bg.addButton(ui->radioButton_2,2);
    int bt=bg.checkedId();
    temSales.setSalesInformation(pi,sd,sn,sp,ci,bt);
}

TransactionDialog::~TransactionDialog()
{
    delete ui;
}
