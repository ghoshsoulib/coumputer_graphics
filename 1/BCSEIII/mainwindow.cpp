#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <iostream>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintDevice>
#include <QPoint>
QImage img=QImage(700,700,QImage::Format_RGB888);
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->x_axis->hide();
    ui->y_axis->hide();
    connect(ui->frame,SIGNAL(Mouse_Pos()),this,SLOT(Mouse_Pressed()));
    connect(ui->frame,SIGNAL(sendMousePosition(QPoint&)),this,SLOT(showMousePosition(QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::point(int x,int y)
{

    img.setPixel(x,y,qRgb(255,255,0));
    ui->frame->setPixmap(QPixmap::fromImage(img));
}


void MainWindow::showMousePosition(QPoint &pos)
{
    ui->mouse_movement->setText(" X : "+QString::number(pos.x())+", Y : "+QString::number(pos.y()));
}
void MainWindow::Mouse_Pressed()
{
    ui->mouse_pressed->setText(" X : "+QString::number(ui->frame->x)+", Y : "+QString::number(ui->frame->y));
    point(ui->frame->x,ui->frame->y);
    ui->x_axis->move(0,ui->frame->y);
    ui->y_axis->move(ui->frame->x,0);
}

void MainWindow::on_show_axes_clicked()
{
    if(ui->show_axes->isChecked())
    {
        ui->x_axis->show();
        ui->y_axis->show();
    }
    else{
        ui->x_axis->hide();
        ui->y_axis->hide();
    }
}
void MainWindow::on_set_point1_clicked()
{
    if(ui->draw_line->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
    }
}

void MainWindow::on_set_point2_clicked()
{
    if(ui->draw_line->isChecked()){
        p2.setX(ui->frame->x);
        p2.setY(ui->frame->y);
    }
}

void MainWindow::on_Draw_clicked()
{
    int r0=ui->circle_radius->value();
    QPainter painter(&img);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::red);
    if(ui->draw_circle->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        painter.setPen(pen);
        painter.drawEllipse(p1,r0,r0);
    }
    if(ui->draw_line->isChecked()){
        painter.setPen(Qt::red);
        painter.drawLine(p1,p2);
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_pushButton_clicked()
{
    for(int j=0;j<img.height();j++)
    {
        for(int i=0;i<img.width();i++)
        {
            img.setPixel(i,j,qRgb(0,0,0));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_pushButton_2_clicked()
{
       int X0 = p1.x();
       int X1 = p2.x();
       int Y0 = p1.y();
       int Y1 = p2.y();

       int dx = X1 - X0;
       int dy = Y1 - Y0;

       // calculate steps required for generating pixels
       int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

       // calculate increment in x & y for each steps
       float Xinc = dx / (float) steps;
       float Yinc = dy / (float) steps;

       // Put pixel for each step
       float X = X0;
       float Y = Y0;
       for (int i = 0; i <= steps; i++)
       {
           img.setPixel(X,Y,qRgb(100,100,100));
           X += Xinc;
           Y += Yinc;


       }

        ui->frame->setPixmap(QPixmap::fromImage(img));


}

void MainWindow::on_pushButton_3_clicked()
{
    int x0 = p1.x();
    int x1 = p2.x();
    int y0 = p1.y();
    int y1 = p2.y();


    int dx, dy, p, x, y;

    dx=x1-x0;
    dy=y1-y0;

    x=x0;
    y=y0;

    p=2*dy-dx;

    while(x<x1)
    {
        if(p>=0)
        {
            img.setPixel(x,y,qRgb(100,100,100));
            y=y+1;
            p=p+2*dy-2*dx;
        }
        else
        {
            img.setPixel(x,y,qRgb(100,100,100));
            p=p+2*dy;
        }
        x=x+1;
    }

        ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_pushButton_4_clicked()
{
    int xc = ui->frame->x;
    int yc = ui->frame->y;
    int r = ui->circle_radius->value();
    int x = 0, y = r;
    int d = 3 - 2 * r;
    while (y >= x)
    {
        // for each pixel we will
        // draw all eight pixels
        drawCircle(xc, yc, x, y);
        x++;

        // check for decision parameter
        // and correspondingly
        // update d, x, y
        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;
        drawCircle(xc, yc, x, y);

    }

    ui->frame->setPixmap(QPixmap::fromImage(img));

}

void MainWindow::drawCircle(int xc, int yc, int x, int y)
{

    img.setPixel(xc+x, yc+y,qRgb(100,100,100));

    img.setPixel(xc-x, yc+y,qRgb(100,100,100));

    img.setPixel(xc+x, yc-y,qRgb(100,100,100));


    img.setPixel(xc-x, yc-y,qRgb(100,100,100));


    img.setPixel(xc+y, yc+x,qRgb(100,100,100));


    img.setPixel(xc-y, yc+x,qRgb(100,100,100));


    img.setPixel(xc+y, yc-x,qRgb(100,100,100));

    img.setPixel(xc-y, yc-x,qRgb(100,100,100));
}

void MainWindow::on_pushButton_5_clicked()
{

    int x0 = ui->frame->x;
    int y0 = ui->frame->y;
    int x = ui->circle_radius->value();
    int y = 0;
    int err = 0;

    while (x >= y)
    {

        drawCircle(x0,y0,x,y);

    if (err <= 0)
    {
        y += 1;
        err += 2*y + 1;
    }

    if (err > 0)
    {
        x -= 1;
        err -= 2*x + 1;
    }
    }

    ui->frame->setPixmap(QPixmap::fromImage(img));
}
