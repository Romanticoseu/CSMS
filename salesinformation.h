#ifndef SALESINFORMATION_H
#define SALESINFORMATION_H
#include <QString>
#include<QDate>
#include<QTextStream>
class SalesInformation
{
public:
    SalesInformation();
    ~SalesInformation();
    void setSalesInformation(QString pi,QDate sd,int sn,double sp,QString ci,int bt);
    void ReadFromFile(QTextStream &stream);
    void WriteToFile(QTextStream &stream);
    QString productId;
    QDate salesDate;
    int salesNumber;
    double salesPrice;
    QString clerkId;
    int bussinessType;//售出为1 退货为2

};

#endif // SALESINFORMATION_H
