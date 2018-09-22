#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QDialog>

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ResultWindow(QWidget *parent = 0);
    ~ResultWindow();
    Ui::ResultWindow *get_ui();

private:
    Ui::ResultWindow *ui;
};

#endif // RESULTWINDOW_H
