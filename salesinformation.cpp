#include "salesinformation.h"

SalesInformation::SalesInformation()
{

}

SalesInformation::~SalesInformation()
{

}

void SalesInformation::setSalesInformation(QString pi,QDate sd,int sn,double sp,QString ci,int bt)
{
    productId=pi;
    salesDate=sd;
    salesNumber=sn;
    salesPrice=sp;
    clerkId=ci;
    bussinessType=bt;
}

void SalesInformation::ReadFromFile(QTextStream &stream)
{
    stream >> productId;
    int year, month, day;
    stream >> year >> month >> day;
    salesDate.setDate(year,month,day);
    stream >> salesNumber;
    stream >> salesPrice;
    stream >> clerkId;
    stream >> bussinessType;

}

void SalesInformation::WriteToFile(QTextStream &stream)
{
    stream << productId << ' ';
    stream << salesDate.year() << ' ' << salesDate.month() << ' ' << salesDate.day() <<' ';
    stream << salesNumber<<' ';
    stream << salesPrice<<' ';
    stream << clerkId<<' ';
    stream << bussinessType<<'\n';
}
