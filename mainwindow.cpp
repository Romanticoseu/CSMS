#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"purchasedialog.h"
#include"transactiondialog.h"
#include"login_form.h"
#include<QString>
#include<QFileDialog>
#include<QDebug>
#include<QFile>
#include<QLabel>
#include<QInputDialog>
#include<QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("商品管理系统");
    //文件  打开资源文件
    connect(ui->action0,&QAction::triggered,[=](){
        //打开文件
        fileAddress=QFileDialog::getOpenFileName(this,"选择资源文件",QDir::currentPath(),"(*.txt)");
        //加载数据
        if (tem.ReadInformationFromFile(fileAddress))
        {showData(tem); openFlag=true;}
        else
            QMessageBox::warning(this,"警告","请打开正确的资源文件");
    });

    //文件  保存当前修改
    connect(ui->actionx,&QAction::triggered,[=](){
        if (openFlag==true)
        {
          tem.sort();
          tem.WriteInformationToFile(fileAddress);
          QMessageBox::information(this,"提示","保存成功");
        }
        else
            QMessageBox::information(this,"提示","请先打开资源文件");
    });


    //业务 进货
    connect(ui->action1,&QAction::triggered,[=]()
    {
      if (openFlag==true)
      {
        PurchaseDialog dlg(this);
        dlg.exec();
        if(dlg.confirmFlag==true)
        {
            if(dlg.temCommodity.productId=="" || dlg.temCommodity.productName=="" || dlg.temCommodity.productionArea=="" || dlg.temCommodity.purchasePrice==0 || dlg.temCommodity.inventoryQuantity==0)
                QMessageBox::warning(this,"警告","请输入正确的数据");
            else
            {
                tem.AddInformation(dlg.temCommodity);
                if (tem.conflictFlag==false)
                {
                    QMessageBox::information(this,"提示","添加成功");
                    showData(tem);
                }
                else
                    QMessageBox::warning(this,"警告","商品编号冲突");
            }
        }
      }
      else
          QMessageBox::information(this,"提示","请先打开资源文件");
    });

    //业务 售出/退货
    connect(ui->action2,&QAction::triggered,[=](){
      if (openFlag==true)
      {
        TransactionDialog dlg(this);
        dlg.exec();

        bool inputFlag=true;//判断数据输入是否正确
        bool idFlag=false;//判断产品编号是否存在
        bool dataFlag=false;//判断商品存货是否足够
        bool overdueFlag=false;//判断商品是否过期
        if (dlg.temSales.productId=="" || dlg.temSales.salesNumber==0 || dlg.temSales.salesPrice==0 || dlg.temSales.clerkId=="")
            inputFlag=false;
        for (int i=0;i<tem.commodity.size();i++)
        {
            if (tem.commodity[i].productId==dlg.temSales.productId)
            {
                idFlag=true;
                break;
            }
        }

        int existingNumber;
        for (int i=0;i<tem.commodity.size();i++)
        {
            if (tem.commodity[i].productId==dlg.temSales.productId)
            {
                existingNumber=tem.commodity[i].inventoryQuantity;
                break;
            }
        }
        for (int j=0;j<tem.sales.size();j++)
        {
            if (tem.sales[j].productId==dlg.temSales.productId && tem.sales[j].bussinessType==1)
                existingNumber-=tem.sales[j].salesNumber;
            if (tem.sales[j].productId==dlg.temSales.productId && tem.sales[j].bussinessType==2)
                existingNumber+=tem.sales[j].salesNumber;
        }
        if (dlg.temSales.bussinessType==2)
            dataFlag=true;
        else if (dlg.temSales.bussinessType==1 && existingNumber-dlg.temSales.salesNumber>=0)
            dataFlag=true;
        for (int i=0;i<tem.commodity.size();i++)
        {
            if (tem.commodity[i].productId==dlg.temSales.productId)
            {
                if (tem.commodity[i].expiryDate>dlg.temSales.salesDate)
                    overdueFlag=true;
                break;
            }
        }


        if (dlg.confirmFlag==true && !inputFlag)
                QMessageBox::warning(this,"警告","请输入正确的数据");
        if (!idFlag && inputFlag)
            QMessageBox::warning(this,"警告","此产品编号不存在");
        if (!dataFlag && idFlag && inputFlag )
            QMessageBox::warning(this,"警告","库存不足,请重新检查数据");
        if (!overdueFlag && idFlag && inputFlag && dataFlag)
            QMessageBox::warning(this,"警告","添加失败:商品已过期,请注意");
        if(dlg.confirmFlag==true && inputFlag && dataFlag && idFlag && overdueFlag)
        {
            tem.AddInformation(dlg.temSales);
            QMessageBox::information(this,"提示","添加成功");
            showData(tem);
        }
      }
      else
          QMessageBox::information(this,"提示","请先打开资源文件");
    });

    //筛选 显示所有信息
    connect(ui->actiony,&QAction::triggered,[=](){
      if (openFlag==true)
      {
        showData(tem);
      }
      else
          QMessageBox::information(this,"提示","请先打开资源文件");
    });

    //筛选 按商品编号
    connect(ui->action3,&QAction::triggered,[=](){
      if(openFlag==true)
      {
        bool flag=false;//查询状态量，判断是否存在该商品
        QString id=QInputDialog::getText(this,"按商品编号查询","请输入商品编号");
        int commodity_total_num=tem.commodity.size();
        int sales_total_num=tem.sales.size();
        for (int i=0;i<commodity_total_num;i++)
        {
            if (tem.commodity[i].productId==id)
                flag=true;
        }
        if (flag==true)
        {
            QMessageBox::information(this,"提示信息","查询成功");
            for (int i=0;i<commodity_total_num;i++)
            {
                if (tem.commodity[i].productId==id)
                {
                    ui->treeWidget->clear();
                    ui->treeWidget->setHeaderLabels(QStringList()<<"编号"<<"品名"<<"产地"<<"进价"<<"总进货量"<<"进货时间"<<"到期日期");
                    ui->treeWidget->header()->resizeSection(0,150);
                    QTreeWidgetItem *item=new QTreeWidgetItem(QStringList()<<tem.commodity[i].productId<<tem.commodity[i].productName<<tem.commodity[i].productionArea
                                                            <<QString::number(tem.commodity[i].purchasePrice,'f',2)<<QString::number(tem.commodity[i].inventoryQuantity)
                                                            <<tem.commodity[i].expiryDate.toString("yyyy.MM.dd")<<tem.commodity[i].purchaseDate.toString("yyyy.MM.dd"));
                    ui->treeWidget->addTopLevelItem(item);
                    bool temFlag=false;//判断是否有销售信息
                    for (int j=0;j<sales_total_num;j++)
                    {
                     if (tem.commodity[i].productId==tem.sales[j].productId)
                        {
                            temFlag=true;
                            break;
                        }
                    }
                    if (temFlag)
                    {
                        QTreeWidgetItem *sales=new QTreeWidgetItem(QStringList()<<"交易日期"<<"交易数量"<<"交易价格"<<"营业员工号"<<"业务类型");
                        item->addChild(sales);
                    }

                    for (int j=0;j<sales_total_num;j++)
                    {
                     if (tem.commodity[i].productId==tem.sales[j].productId)
                        {
                           QTreeWidgetItem *itemsales=new QTreeWidgetItem(QStringList()<<tem.sales[j].salesDate.toString("yyyy.MM.dd")
                                                                        <<QString::number(tem.sales[j].salesNumber)<<QString::number(tem.sales[j].salesPrice,'f',2)
                                                                        <<tem.sales[j].clerkId<<((tem.sales[j].bussinessType==1)?"售出":"退货"));
                         item->addChild(itemsales);
                        }
                    }
                }

            }

        }
        else
        {
            if (id!="")
                QMessageBox::information(this,"提示信息","查无此商品，请检测输入编号是否正确");
        }
      }
      else
          QMessageBox::information(this,"提示","请先打开资源文件");
    });


    //筛选 按销售日期
    connect(ui->action4,&QAction::triggered,[=]()
    {
      if (openFlag==true)
      {
        bool yearFlag=false;
        bool monthFlag=false;
        bool dayFlag=false;  //查询状态量
        int year=QInputDialog::getText(this,"按销售日期查询","请输入年份").toInt();
        int sales_total_num=tem.sales.size();
        int commodity_total_num=tem.commodity.size();
        QString productName;
        for (int i=0;i<sales_total_num;i++)
        {
            if (tem.sales[i].salesDate.year()==year)
                yearFlag=true;
        }
        if (yearFlag==false)
            {
                if(year>=2000 && year<=2100)
                    QMessageBox::information(this,"提示信息","查无此年的销售记录");
                else if((year<2000 || year>2100) && year!=0)
                    QMessageBox::warning(this,"警告","请输入正确的年份");
            }
        else
        {
            int month=QInputDialog::getText(this,"按销售日期查询","请输入月份").toInt();
            for (int i=0;i<sales_total_num;i++)
            {
                if (tem.sales[i].salesDate.month()==month)
                    monthFlag=true;
            }
            if (monthFlag==false)
            {
                if (month>=1 && month<=12)
                    QMessageBox::information(this,"提示信息","查无此月的销售记录");
                else if((month<0 ||month>12) && month!=0)
                    QMessageBox::warning(this,"警告","请输入正确的月份");
            }
            else
            {
                int day=QInputDialog::getText(this,"按销售日期查询","请输入日期").toInt();
                for (int i=0;i<sales_total_num;i++)
                {
                    if (tem.sales[i].salesDate.day()==day)
                        dayFlag=true;
                }
                if (dayFlag==false)
                {
                    if (day>=1 && day<=31)
                        QMessageBox::information(this,"提示信息","查无此日的销售记录");
                    else if((day<0 || day>31) && day!=0)
                        QMessageBox::warning(this,"警告","请输入正确的日期");
                }
                else
                {
                    QMessageBox::information(this,"提示信息","查询成功");
                    QDate date;
                    date.setDate(year,month,day);
                    ui->treeWidget->clear();
                    ui->treeWidget->setHeaderLabels(QStringList()<<"交易日期"<<"商品编号"<<"商品名称"<<"交易数量"<<"交易价格"<<"营业员工号"<<"业务类型"<<"");
                    for (int i=0;i<sales_total_num;i++)
                    {
                        if (tem.sales[i].salesDate==date)
                        {
                            for (int j=0;j<commodity_total_num;j++)
                            {
                                if (tem.commodity[j].productId==tem.sales[i].productId)
                                    productName=tem.commodity[j].productName;
                            }
                            QTreeWidgetItem *item=new QTreeWidgetItem(QStringList()<<tem.sales[i].salesDate.toString("yyyy.MM.dd")<<tem.sales[i].productId<<productName
                                                                      <<QString::number(tem.sales[i].salesNumber)<<QString::number(tem.sales[i].salesPrice,'f',2)
                                                                      <<tem.sales[i].clerkId<<((tem.sales[i].bussinessType==1)?"售出":"退货"));
                            ui->treeWidget->addTopLevelItem(item);

                        }

                    }


                }
            }
        }
      }
      else
          QMessageBox::information(this,"提示","请先打开资源文件");

    });


    //查询 商品库存
    connect(ui->actionp,&QAction::triggered,[=](){
      if (openFlag==true)
      {
        QString id=QInputDialog::getText(this,"查询商品库存","请输入需要查询的商品编号");
        int existingNumber;
        QString name;
        bool flag=false;
        for (int i=0;i<tem.commodity.size();i++)
        {
            if (tem.commodity[i].productId==id)
            {
                existingNumber=tem.commodity[i].inventoryQuantity;
                name=tem.commodity[i].productName;
                flag=true;
                break;
            }
        }
       if (flag==true)
       {
        for (int j=0;j<tem.sales.size();j++)
        {
            if (tem.sales[j].productId==id && tem.sales[j].bussinessType==1)
                existingNumber-=tem.sales[j].salesNumber;
            if (tem.sales[j].productId==id && tem.sales[j].bussinessType==2)
                existingNumber+=tem.sales[j].salesNumber;
        }
        QString temString;
        temString=name+"("+id+") 库存:"+QString::number(existingNumber);
        QMessageBox::information(this,"商品库存",temString);
       }
       else
       {
           if(id!="")
                QMessageBox::warning(this,"提示信息","查无此商品，请检测输入编号是否正确");
       }
     }
    else
        QMessageBox::information(this,"提示","请先打开资源文件");
    });


    //查询 商品毛利
    connect(ui->actionq,&QAction::triggered,[=](){
      if (openFlag==true)
      {
        QString id=QInputDialog::getText(this,"查询商品毛利","请输入需要查询的商品编号");
        int total_sales_price=0;
        int total_purchase_price=0;
        QString name;
        bool flag=false;
        for (int i=0;i<tem.commodity.size();i++)
        {
            if (tem.commodity[i].productId==id)
            {
                total_purchase_price=tem.commodity[i].purchasePrice*tem.commodity[i].inventoryQuantity;
                name=tem.commodity[i].productName;
                flag=true;
                break;
            }
        }
       if (flag==true)
       {
        for (int j=0;j<tem.sales.size();j++)
        {
            if (tem.sales[j].productId==id && tem.sales[j].bussinessType==1)
                total_sales_price+=(tem.sales[j].salesNumber*tem.sales[j].salesPrice);
            if (tem.sales[j].productId==id && tem.sales[j].bussinessType==2)
                total_sales_price-=(tem.sales[j].salesNumber*tem.sales[j].salesPrice);
        }
        QString temString;
        temString=name+"("+id+") "+"毛利:￥"+QString::number(total_sales_price-total_purchase_price,'f',2);
        QMessageBox::information(this,"商品毛利",temString);
      }
      else
       {
           if(id!="")
                QMessageBox::warning(this,"提示信息","查无此商品，请检测输入编号是否正确");
       }
     }
      else
          QMessageBox::information(this,"提示","请先打开资源文件");
    });

    LoginForm login;
    connect(&login, &LoginForm::loggedIn, this, [=]() {/* model->setEmployeeNumber(number);*/ });
    int res = login.exec();
    if (res == QDialog::Accepted)
        show();

}

void MainWindow::showData(FileInteraction &temporary)
{
    tem.sort();
    ui->treeWidget->clear();
    ui->treeWidget->setHeaderLabels(QStringList()<<"编号"<<"品名"<<"产地"<<"进价"<<"总进货量"<<"进货时间"<<"到期日期");
    ui->treeWidget->header()->resizeSection(0,150);
    int commodity_total_num=temporary.commodity.size();
    int sales_total_num=temporary.sales.size();
    for (int i=0;i<commodity_total_num;i++)
    {
        QTreeWidgetItem *item=new QTreeWidgetItem(QStringList()<<temporary.commodity[i].productId<<temporary.commodity[i].productName<<temporary.commodity[i].productionArea
                                                  <<QString::number(temporary.commodity[i].purchasePrice,'f',2)<<QString::number(temporary.commodity[i].inventoryQuantity)
                                                  <<temporary.commodity[i].purchaseDate.toString("yyyy.MM.dd")
                                                  <<temporary.commodity[i].expiryDate.toString("yyyy.MM.dd"));
        ui->treeWidget->addTopLevelItem(item);
        bool temFlag=false;//判断是否有销售信息
        for (int j=0;j<sales_total_num;j++)
        {
         if (tem.commodity[i].productId==tem.sales[j].productId)
            {
                temFlag=true;
                break;
            }
        }
        if (temFlag)
        {
            QTreeWidgetItem *sales=new QTreeWidgetItem(QStringList()<<"交易日期"<<"交易数量"<<"交易价格"<<"营业员工号"<<"业务类型");
            item->addChild(sales);
        }
        for (int j=0;j<sales_total_num;j++)
        {
            if (temporary.commodity[i].productId==temporary.sales[j].productId)
            {
                QTreeWidgetItem *itemsales=new QTreeWidgetItem(QStringList()<<temporary.sales[j].salesDate.toString("yyyy.MM.dd")
                                                               <<QString::number(temporary.sales[j].salesNumber)<<QString::number(temporary.sales[j].salesPrice,'f',2)
                                                               <<temporary.sales[j].clerkId<<((temporary.sales[j].bussinessType==1)?"售出":"退货"));
                item->addChild(itemsales);
            }
        }

    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

