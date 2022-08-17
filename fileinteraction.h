#ifndef FILEINTERACTION_H
#define FILEINTERACTION_H
#include<QString>
#include<QVector>
#include"commodityinformation.h"
#include"salesinformation.h"


class FileInteraction
{
public:
    FileInteraction();
    bool ReadInformationFromFile(const QString &fileAddress);
    void WriteInformationToFile(const QString &fileAddress);
    void AddInformation(const CommodityInformation& tem);
    void AddInformation(const SalesInformation& tem);
    void sort();
    QVector <CommodityInformation> commodity;
    QVector <SalesInformation> sales;
    bool conflictFlag=false;//判断商品编号是否冲突

};

#endif // FILEINTERACTION_H
