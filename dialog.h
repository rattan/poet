#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
//#include<QFile>
#include<QString>
//#include<QFileDialog>
//#include<QDebug>
#include<QPixmap>
#include<QDesktopWidget>
#include<QCursor>
#include<QGraphicsScene>
#include<QMouseEvent>
#include<QTimer>
#include<QScreen>
//#include<QPropertyAnimation>
//#include<QGraphicsOpacityEffect>
#include<QGraphicsRectItem>
#include<QMessageBox>
#include<QCursor>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:

    void update_screen();

    void on_zoom_out_clicked();

    void on_zoom_in_clicked();

    void on_pushButton_cross_clicked();

private:
    Ui::Dialog *ui;

    int mousex,mousey;
    int ps_w,ps_h;
    int zoom;
    QTimer * timer;
    QScreen*screen;
    char cross[6]{'n','r','g','b','k','w'};
    int cross_num;
    QColor before_color;

    bool eventFilter(QObject*target,QEvent*event);


};

#endif // DIALOG_H
