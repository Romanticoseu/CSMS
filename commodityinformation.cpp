#include "commodityinformation.h"
CommodityInformation::CommodityInformation()
{

}

CommodityInformation::~CommodityInformation()
{

}

void CommodityInformation::setCommodityInformation(QString pi,QString pn,QString pa,double pp,int iq,QDate ed,QDate pd)
{
    productId=pi;
    productName=pn;
    productionArea=pa;
    purchasePrice=pp;
    inventoryQuantity=iq;
    expiryDate=ed;
    purchaseDate=pd;

}

void CommodityInformation::ReadFromFile(QTextStream &stream)
{
     stream >> productId;
     stream >> productName;
     stream >> productionArea;
     stream >> purchasePrice;
     stream >> inventoryQuantity;
    int year, month, day;
    stream>>year>>month>>day;
    purchaseDate.setDate(year, month, day);
    stream>>year>>month>>day;
    expiryDate.setDate(year,month,day);
}

void CommodityInformation::WriteToFile(QTextStream &stream)
{

    stream << productId << ' ';
    stream << productName << ' ';
    stream << productionArea << ' ';
    stream << purchasePrice << ' ';
    stream << inventoryQuantity << ' ';
    stream << purchaseDate.year() << ' ' << purchaseDate.month() << ' ' << purchaseDate.day() << ' ';
    stream << expiryDate.year() << ' ' << expiryDate.month() << ' ' << expiryDate.day() << '\n';
}
