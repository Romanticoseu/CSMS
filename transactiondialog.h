#ifndef TRANSACTIONDIALOG_H
#define TRANSACTIONDIALOG_H

#include <QDialog>
#include"salesinformation.h"

namespace Ui {
class TransactionDialog;
}

class TransactionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionDialog(QWidget *parent = nullptr);
    ~TransactionDialog();
    SalesInformation temSales;
    bool confirmFlag=false;//设置一个状态量，判断是否成功进行添加行为
    void setSalesInformation();

private:
    Ui::TransactionDialog *ui;
};

#endif // TRANSACTIONDIALOG_H
