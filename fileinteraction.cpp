#include "fileinteraction.h"
#include<QFile>
#include<QTextStream>
#include<QTextCodec>
#include<QDebug>
#include<QMessageBox>

FileInteraction::FileInteraction()
{

}

bool FileInteraction::ReadInformationFromFile(const QString &fileAddress)
{
    QFile file(fileAddress);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file); //用文本流读取文件
    stream.setCodec("UTF-8");//设置编码格式，否则会出现乱码
    commodity.clear();//初始化
    sales.clear();
    QString id;
    stream>>id;
    if (id!="ID20220817")
        return false;
    while(!stream.atEnd())
    {
        static int type;
        static CommodityInformation temCommodity;
        static SalesInformation temSales;
        stream>>type;
        if(type==1)
        {
            temCommodity.ReadFromFile(stream);
            commodity.push_back(temCommodity);
        }
        else if(type==2)
        {
            temSales.ReadFromFile(stream);
            sales.push_back(temSales);
        }
    }
    file.close();
    sort();
    return true;

}

void FileInteraction::WriteInformationToFile(const QString &fileAddress)
{
    QFile file(fileAddress);
    file.open(QIODevice::WriteOnly| QIODevice::Text);
    QTextStream stream(&file);//用文本流写入文件
    stream.setCodec("UTF-8");//设置编码格式
    stream<<"ID20220817"<<'\n';
    for (int i=0;i<commodity.size();i++)
    {
        stream<<"1"<<' ';
        commodity[i].WriteToFile(stream);
    }
    for (int j=0;j<sales.size();j++)
    {
        stream<<"2"<<' ';
        sales[j].WriteToFile(stream);
    }
    file.close(); //关闭文件
}

void FileInteraction::AddInformation(const CommodityInformation& tem)
{
    bool flag=true;
    for (int i=0;i<commodity.size();i++)
    {
        if (commodity[i].productId==tem.productId && commodity[i].productName==tem.productName)
            //判断商品编号，商品名称(如果有，则进行数据合并,进价计算平均值)
        {
            commodity[i].inventoryQuantity+=tem.inventoryQuantity;
            commodity[i].purchasePrice=(commodity[i].purchasePrice*(commodity[i].inventoryQuantity-tem.inventoryQuantity)+tem.purchasePrice*tem.inventoryQuantity)/commodity[i].inventoryQuantity;
            flag=false;
            break;
        }
        if (commodity[i].productId==tem.productId && commodity[i].productName != tem.productName)
        {
            conflictFlag=true;
            break;
        }
    }
    if (flag==true && conflictFlag==false)
    {
        commodity.push_back(tem);
        sort();
    }
}

void FileInteraction::AddInformation(const SalesInformation& tem)
{
    bool flag=true;
    for (int i=0;i<sales.size();i++)
    {
        if (sales[i].productId==tem.productId && sales[i].salesDate==tem.salesDate && sales[i].bussinessType==tem.bussinessType && sales[i].salesPrice==tem.salesPrice && sales[i].clerkId==tem.clerkId)
            //判断商品编号，销售日期，售价、营业员编号以及业务类型是否相等(如果有，则进行数据合并)
        {
            sales[i].salesNumber+=tem.salesNumber;
            flag=false;
            break;
        }
    }
    if (flag==true)
    {
        sales.push_back(tem);
        sort();
    }

}

void FileInteraction::sort()
{
    //冒泡排序
    //商品信息排序(按商品编号)
    for (int i=0;i<commodity.size()-1;i++)
        for(int j=0;j<commodity.size()-1-i;j++)
        {
            CommodityInformation temCommodityInformation;
            if (commodity[j].productId>commodity[j+1].productId)
            {
                temCommodityInformation=commodity[j+1];
                commodity[j+1]=commodity[j];
                commodity[j]=temCommodityInformation;
            }
        }
    //销售信息排序(按销售日期和业务内容)
    for (int i=0;i<sales.size()-1;i++)
        for(int j=0;j<sales.size()-1-i;j++)
        {
            SalesInformation temSalesInformation;
            if (sales[j].salesDate>sales[j+1].salesDate)
            {
                temSalesInformation=sales[j+1];
                sales[j+1]=sales[j];
                sales[j]=temSalesInformation;
            }
        }
    for (int i=0;i<sales.size()-1;i++)
        for(int j=0;j<sales.size()-1-i;j++)
        {
            SalesInformation temSalesInformation;
            if (sales[j].bussinessType>sales[j+1].bussinessType)
            {
                temSalesInformation=sales[j+1];
                sales[j+1]=sales[j];
                sales[j]=temSalesInformation;
            }
        }

}
