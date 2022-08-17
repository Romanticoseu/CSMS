#include "purchasedialog.h"
#include "ui_purchasedialog.h"
#include<QDialogButtonBox>
#include<QDateEdit>
PurchaseDialog::PurchaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PurchaseDialog)
{
    ui->setupUi(this);
    //控件初始化
    QDate current_date=QDate::currentDate();
    ui->dateEdit_1->setDate(current_date);
    ui->dateEdit_2->setDate(current_date.addYears(1));

    //OK,Cancel按钮连接
    connect(ui->buttonBox,&QDialogButtonBox::accepted,[=](){
        confirmFlag=true;
        setCommodityInformation();
        close();
    });
    connect(ui->buttonBox,&QDialogButtonBox::rejected,[=](){
        close();
    });

}

PurchaseDialog::~PurchaseDialog()
{
    delete ui;
}

void PurchaseDialog::setCommodityInformation()
{
    PurchaseDialog dlg;
    QString pi=ui->lineEdit->text();
    QString pn=ui->lineEdit_2->text();
    QString pa=ui->lineEdit_3->text();
    double pp=ui->doubleSpinBox->value();
    int iq=ui->spinBox->value();
    QDate pd=ui->dateEdit_1->date();
    QDate ed=ui->dateEdit_2->date();
    temCommodity.setCommodityInformation(pi,pn,pa,pp,iq,ed,pd);
}
