#ifndef LOGIN_FORM_H
#define LOGIN_FORM_H

#include "ui_login_form.h"
#include <QDialog>
#include <QPushButton>

namespace Ui {
    class LoginForm;
}

class LoginForm : public QDialog {
    Q_OBJECT

public:
    explicit LoginForm(QWidget* parent = nullptr);
    ~LoginForm();

private:
    QPushButton*   login;
    Ui::LoginForm* ui;
    bool           is_dragged = false;
    QPoint         offset;

signals:
    void loggedIn(QString);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
};

#endif  // LOGIN_FORM_H
