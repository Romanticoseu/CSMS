#include "login_form.h"
#include "ui_login_form.h"
#include <QDebug>
#include <QFile>
#include <QGridLayout>
#include <QIcon>
#include <QLinearGradient>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QTextStream>
#include <QVBoxLayout>
#include <QValidator>
#include <cmath>
#define SHADOW_WIDTH 8

LoginForm::LoginForm(QWidget* parent) : QDialog(parent), ui(new Ui::LoginForm) {
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    this->layout()->setMargin(SHADOW_WIDTH);
    QFile accounts(":/accounts.txt");

    while (true) {
        if (!accounts.open(QIODevice::ReadOnly | QIODevice::Text)) {
            auto res = QMessageBox::critical(this, windowTitle(), QStringLiteral("账户文件打开失败"), QMessageBox::Retry, QMessageBox::Abort);
            if (res)
                exit(0);
        }
        else
            break;
    }
    QTextStream ts(&accounts);
    ts.setAutoDetectUnicode(true);
    ts.skipWhiteSpace();

    std::map<QString, QString> accs;

    while (!ts.atEnd()) {
        QString account, password;
        ts >> account >> password;
        accs[account] = password;
    }
    accounts.close();

    ui->line_account->setValidator(new QIntValidator());
    connect(ui->btn_minimize, &QPushButton::clicked, this, [=] { showMinimized(); });
    connect(ui->btn_login, &QPushButton::clicked, this, [=] {
        auto acc = accs.find(ui->line_account->text());
        if (acc != accs.end() && acc->second == ui->line_password->text()) {
            emit loggedIn(acc->first);
            QDialog::accept();
        }
        else
            QMessageBox::information(this, "商品销售管理系统", "员工号不存在或密码错误");
    });
    installEventFilter(this);
}

LoginForm::~LoginForm() { delete ui; }

bool LoginForm::eventFilter(QObject* obj, QEvent* event) {
    switch (event->type()) {
    case QEvent::MouseButtonPress: {
        auto e = static_cast<QMouseEvent*>(event);
        if (e->button() == Qt::LeftButton) {
            is_dragged = true;
            offset     = e->globalPos() - this->frameGeometry().topLeft();
        }
        break;
    }
    case QEvent::MouseMove: {
        auto e = static_cast<QMouseEvent*>(event);
        if (is_dragged) {
            this->move(e->globalPos() - offset);
        }
        break;
    }
    case QEvent::MouseButtonRelease: {
        auto e = static_cast<QMouseEvent*>(event);
        if (e->button() == Qt::LeftButton) {
            is_dragged = false;
        }
        break;
    }
    default:
        break;
    }
    return QDialog::eventFilter(obj, event);
}

void LoginForm::paintEvent(QPaintEvent* event) {
    (void)event;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(QRect(SHADOW_WIDTH, SHADOW_WIDTH, this->width() - 2 * SHADOW_WIDTH, this->height() - 2 * SHADOW_WIDTH), QBrush(Qt::white));
    QLinearGradient grad(0, 30, 0, 200);
    grad.setColorAt(0.0, QColor(0x0f, 0xc3, 0xff));
    grad.setColorAt(0.5, QColor(0x07, 0xbc, 0xfc));
    grad.setColorAt(1, QColor(0x00, 0xb5, 0xf9));
    painter.fillRect(QRect(SHADOW_WIDTH, SHADOW_WIDTH, this->width() - 2 * SHADOW_WIDTH, 125), QBrush(grad));
    QColor color(50, 50, 50, 30);
    for (int i = 0; i < SHADOW_WIDTH; i++) {
        color.setAlpha(120 - std::sqrt(i) * 40);
        painter.setPen(color);
        painter.drawRect(SHADOW_WIDTH - i, SHADOW_WIDTH - i, this->width() - (SHADOW_WIDTH - i) * 2, this->height() - (SHADOW_WIDTH - i) * 2);
    }
}
