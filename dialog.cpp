#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    mousex=QCursor::pos().x();
    mousey=QCursor::pos().y();
    screen=QGuiApplication::primaryScreen();

    ps_w=ps_h=91;
    zoom=2;
    cross_num=0;

    installEventFilter(this);


//    QPropertyAnimation* animation=new QPropertyAnimation(this->ui->label_zoom_rate,"styleSheet");
//    animation->setDuration(1000);
//    animation->setStartValue("color:rgba(0,0,0,1);");
//    animation->setEndValue("color:rgba(0,0,0,0);");

//    animation->start();


//    QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(this);
//    opacityEffect->setOpacity(1.0);
//    ui->label_zoom_rate->setGraphicsEffect(opacityEffect);
//    QPropertyAnimation* anim = new QPropertyAnimation(this);
//    anim->setTargetObject(opacityEffect);
//    anim->setPropertyName("opacity");
//    anim->setDuration(5000);
//    anim->setStartValue(opacityEffect->opacity());
//    anim->setEndValue(0);
//    anim->setEasingCurve(QEasingCurve::OutQuad);
//    anim->start(QAbstractAnimation::DeleteWhenStopped);


    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(update_screen()));
    timer->start(10);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::update_screen()
{
    QString str;


    //screen
    QPixmap screenshot = screen->grabWindow(QApplication::desktop()->winId(),QCursor::pos().x()-(ps_w/(zoom*2)),QCursor::pos().y()-(ps_h/(zoom*2)),ps_w/zoom,ps_h/zoom);
    this->ui->label_screen->setPixmap(screenshot.scaled(this->ui->label_screen->geometry().width(),this->ui->label_screen->geometry().height()));



    //pointed pixel
    QColor pointed_color = screenshot.toImage().pixel((ps_w/(zoom*2)),(ps_h/(zoom*2)));
    if(this->before_color==pointed_color) return;
    this->before_color = pointed_color;

    QPalette pointed_pallet;
    pointed_pallet.setColor(this->ui->label_color->backgroundRole(),pointed_color);
    pointed_pallet.setColor(this->ui->label_color->foregroundRole(),pointed_color);
    this->ui->label_color->setPalette(pointed_pallet);

    //rgb
    pointed_pallet.setColor(this->ui->label_color_R->backgroundRole(),QColor(pointed_color.red(),0,0));
    pointed_pallet.setColor(this->ui->label_color_R->foregroundRole(),QColor(pointed_color.red(),0,0));
    this->ui->label_color_R->setPalette(pointed_pallet);
    str.sprintf("R : %d",pointed_color.red());
    this->ui->label_R->setText(str);
    this->ui->progress_R->setValue(pointed_color.red());

    pointed_pallet.setColor(this->ui->label_color_G->backgroundRole(),QColor(0,pointed_color.green(),0));
    pointed_pallet.setColor(this->ui->label_color_G->foregroundRole(),QColor(0,pointed_color.green(),0));
    this->ui->label_color_G->setPalette(pointed_pallet);
    str.sprintf("G : %d",pointed_color.green());
    this->ui->label_G->setText(str);
    this->ui->progress_G->setValue(pointed_color.green());

    pointed_pallet.setColor(this->ui->label_color_B->backgroundRole(),QColor(0,0,pointed_color.blue()));
    pointed_pallet.setColor(this->ui->label_color_B->foregroundRole(),QColor(0,0,pointed_color.blue()));
    this->ui->label_color_B->setPalette(pointed_pallet);
    str.sprintf("B : %d",pointed_color.blue());
    this->ui->label_B->setText(str);
    this->ui->progress_B->setValue(pointed_color.blue());

    str.sprintf("#%X%X%X",pointed_color.red(),pointed_color.green(),pointed_color.blue());
    this->ui->label_HEX_RGB->setText(str);

    //cymk
    QColor cmyk;
    cmyk.setCmyk(pointed_color.cyan(),0,0,0);
    pointed_pallet.setColor(this->ui->label_color_C->backgroundRole(),cmyk);
    pointed_pallet.setColor(this->ui->label_color_C->foregroundRole(),cmyk);
    this->ui->label_color_C->setPalette(pointed_pallet);
    str.sprintf("C:%.1f",pointed_color.cyanF()*100);
    this->ui->label_C->setText(str);
    this->ui->progress_C->setValue(pointed_color.cyanF()*100+0.5);

    cmyk.setCmyk(0,pointed_color.magenta(),0,0);
    pointed_pallet.setColor(this->ui->label_color_M->backgroundRole(),cmyk);
    pointed_pallet.setColor(this->ui->label_color_M->foregroundRole(),cmyk);
    this->ui->label_color_M->setPalette(pointed_pallet);
    str.sprintf("M:%.1f",pointed_color.magentaF()*100);
    this->ui->label_M->setText(str);
    this->ui->progress_M->setValue(pointed_color.magentaF()*100+0.5);

    cmyk.setCmyk(0,0,pointed_color.yellow(),0);
    pointed_pallet.setColor(this->ui->label_color_Y->backgroundRole(),cmyk);
    pointed_pallet.setColor(this->ui->label_color_Y->foregroundRole(),cmyk);
    this->ui->label_color_Y->setPalette(pointed_pallet);
    str.sprintf("Y:%.1f",pointed_color.yellow()*100);
    this->ui->label_Y->setText(str);
    this->ui->progress_Y->setValue(pointed_color.yellowF()*100+0.5);

    cmyk.setCmyk(0,0,0,pointed_color.black());
    pointed_pallet.setColor(this->ui->label_color_K->backgroundRole(),cmyk);
    pointed_pallet.setColor(this->ui->label_color_K->foregroundRole(),cmyk);
    this->ui->label_color_K->setPalette(pointed_pallet);
    str.sprintf("K:%.1f",pointed_color.blackF()*100);
    this->ui->label_K->setText(str);
    this->ui->progress_K->setValue(pointed_color.blackF()*100+0.5);

}


void Dialog::on_zoom_out_clicked()
{
    QString str;
    if(zoom==1) return;
    else zoom--;
    str.sprintf("Zoom x%d",zoom);
    this->ui->label_zoom_rate->setText(str);

    update_screen();
}

void Dialog::on_zoom_in_clicked()
{
    QString str;
    if(zoom==10) return;
    else zoom++;
    str.sprintf("Zoom x%d",zoom);
    this->ui->label_zoom_rate->setText(str);

    update_screen();
}

void Dialog::on_pushButton_cross_clicked()
{
    QString str;
    str.sprintf(":/resource/cross_%c.png",cross[++cross_num]);
    this->ui->pushButton_cross->setIcon(QIcon(str));
    if(cross_num==6) cross_num=0;
}

bool Dialog::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::EnterWhatsThisMode)
    {
        QMessageBox infobox;
        infobox.setIconPixmap(QPixmap(":/resource/this color.png").scaled(100,100));
        infobox.setIcon(QMessageBox::Information);
        infobox.setText("This Color [Color Pick Application]\nhttp://recloud.tistory.com");
        infobox.exec();

    }
}
