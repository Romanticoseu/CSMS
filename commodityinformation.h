#ifndef COMMODITYINFORMATION_H
#define COMMODITYINFORMATION_H
#include<QString>
#include<QDate>
#include<QTextStream>
class CommodityInformation
{
public:
    CommodityInformation();
    ~CommodityInformation();
    void setCommodityInformation(QString pi,QString pn,QString pa,double pp,int iq,QDate ed,QDate pd);
    void ReadFromFile(QTextStream &stream);
    void WriteToFile(QTextStream &stream);
    QString productId;
    QString productName;
    QString productionArea;
    double purchasePrice;
    int inventoryQuantity;
    QDate expiryDate;
    QDate purchaseDate;

};

#endif // COMMODITYINFORMATION_H
