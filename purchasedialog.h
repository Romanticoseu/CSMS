#ifndef PURCHASEDIALOG_H
#define PURCHASEDIALOG_H

#include <QDialog>
#include"commodityinformation.h"

namespace Ui {
class PurchaseDialog;
}

class PurchaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PurchaseDialog(QWidget *parent = nullptr);
    ~PurchaseDialog();
    CommodityInformation temCommodity;
    bool confirmFlag=false;//设置一个状态量，判断是否成功进行添加行为
    void setCommodityInformation();


private:
    Ui::PurchaseDialog *ui;
};

#endif // PURCHASEDIALOG_H
