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

    int xx = ui->grid_size->value();
    ui->mouse_movement->setText(" X : "+QString::number(pos.x())+", Y : "+QString::number(pos.y()));
}
void MainWindow::Mouse_Pressed()
{

    int xx = ui->grid_size->value();
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

void MainWindow::on_pushButton_2_clicked()  // DDA line
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
           //img.setPixel(X,Y,qRgb(100,100,100));
           paint_pixel(X,Y);
           X += Xinc;
           Y += Yinc;


       }

        ui->frame->setPixmap(QPixmap::fromImage(img));


}

void MainWindow::on_pushButton_3_clicked()  // breshenham line
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
            //img.setPixel(x,y,qRgb(100,100,100));
            paint_pixel(x,y);
            y=y+1;
            p=p+2*dy-2*dx;
        }
        else
        {
           // img.setPixel(x,y,qRgb(100,100,100));
            paint_pixel(x,y);
            p=p+2*dy;
        }
        x=x+1;
    }

        ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_pushButton_4_clicked()  // mid point circle
{
    int xc = ui->frame->x;
    int yc = ui->frame->y;
    int r = ui->circle_radius->value();
    int xx = ui->grid_size->value();
    r = r*xx;
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

void MainWindow::drawCircle(int xc, int yc, int x, int y) // eight symetry circle
{

    //img.setPixel(xc+x, yc+y,qRgb(100,100,100));
    paint_pixel(xc+x, yc+y);

   // img.setPixel(xc-x, yc+y,qRgb(100,100,100));
    paint_pixel(xc-x, yc+y);

   // img.setPixel(xc+x, yc-y,qRgb(100,100,100));
    paint_pixel(xc+x, yc-y);


   // img.setPixel(xc-x, yc-y,qRgb(100,100,100));
    paint_pixel(xc-x, yc-y);


   // img.setPixel(xc+y, yc+x,qRgb(100,100,100));
    paint_pixel(xc+y, yc+x);


   // img.setPixel(xc-y, yc+x,qRgb(100,100,100));
    paint_pixel(xc-y, yc+x);


    //img.setPixel(xc+y, yc-x,qRgb(100,100,100));
    paint_pixel(xc+y, yc-x);

    //img.setPixel(xc-y, yc-x,qRgb(100,100,100));
    paint_pixel(xc-y, yc-x);
}

void MainWindow::on_pushButton_5_clicked()  // breshemham circle
{

    int x0 = ui->frame->x;
    int y0 = ui->frame->y;
    int x = ui->circle_radius->value();
    int xx = ui->grid_size->value();
    x = x*xx;
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

void MainWindow::display(int xs,int ys,int x,int y) // four symetry in ellipse
{

    //img.setPixel(xs+x,ys+y,qRgb(100,100,100));
    paint_pixel(xs+x,ys+y);


    //img.setPixel(xs-x,ys-y,qRgb(100,100,100));
            paint_pixel(xs-x,ys-y);

    //img.setPixel(xs+x,ys-y,qRgb(100,100,100));
            paint_pixel(xs+x,ys-y);

    //img.setPixel(xs-x,ys+y,qRgb(100,100,100));
            paint_pixel(xs-x,ys+y);
}

void MainWindow::on_pushButton_6_clicked()  // draw ellipse
{

        int xx = ui->grid_size->value();

        int xs1 = ui->frame->x;

        int ys1 = ui->frame->y;


        int rx = ui->rx->value();
        rx = rx*xx;
        int ry = ui->ry->value();
        ry = ry*xx;




        int x,y;
        float d1,d2,dx,dy;
        x = 0;                  // take start position as (0,ry)
        y = ry;                 // finding decision parameter d1
        d1 = pow(ry,2) - (pow(rx,2) * ry) + (0.25 * pow(rx,2));
        dx = 2 * pow(ry,2) * x;
        dy = 2 * pow(rx,2) * y;
        do                         // region one
        {
            display(xs1,ys1,x,y);
            if(d1<0)
            {
                x++;
                dx = dx + (2 * (pow(ry,2)));
                d1 = d1 + dx +(pow(ry,2));
            }
            else
            {
                x++;
                y--;
                dx = dx + (2 * (pow(ry,2)));
                dy = dy - (2 * (pow(rx,2)));
                d1 = d1 + dx - dy + (pow(ry,2));
            }
        }while(dx<dy);
        do
        {
            display(xs1,ys1,x,y);
            if(d2>0)
            {
                x = x;
                y--;
                dy = dy - (2 * (pow(rx,2)));
                d2 = d2 - dy + pow(rx,2);
            }
            else
            {
                x++;
                y--;
                dy = dy - (2 * (pow(rx,2)));
                dx = dx + (2 * (pow(ry,2)));
                d2 = d2 +dx - dy + pow(rx,2);

            }
        }while(y>0);

        ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_pushButton_7_clicked()  //draw grid
{

    on_pushButton_clicked();

    int x = ui->grid_size->value();
    for(int j=0;j<img.height();j=j+x)
    {
        for(int i=0;i<img.width();i++)
        {
            img.setPixel(i,j,qRgb(50,50,50));
        }
    }

    for(int j=0;j<img.height();j++)
    {
        for(int i=0;i<img.width();i=i+x)
        {
            img.setPixel(i,j,qRgb(50,50,50));
        }
    }

    int h = img.height();
    h = h/2;
    int w = img.width();
    w = w/2;

    int l;
    int m;

    int startx = h - h%x;
    int starty = w - w%x;

    for(l=1;l<=2*h;l++){
        for(m = startx;m<=(startx+x);m++){
            img.setPixel(l,m,qRgb(50,50,50));
        }
    }

    for(l=1;l<=2*w;l++){
        for(m = starty;m<=(starty+x);m++){
            img.setPixel(m,l,qRgb(50,50,50));
        }
    }




    ui->frame->setPixmap(QPixmap::fromImage(img));

}

void MainWindow::paint_pixel(int x, int y) //paint pixel
{
    int size = ui->grid_size->value();

    int lessx = x - x%size;
    int morex = lessx + size;
    int lessy = y - y%size;
    int morey = lessy + size;

    int i,j;

    for(i=lessx+1;i<morex;i++){
        for(j=lessy+1;j<morey;j++){
            img.setPixel(i,j,qRgb(204,0,102));
        }
    }


  //  ui->frame->setPixmap(QPixmap::fromImage(img));

}

void MainWindow::pixel_map(int x, int y)
{

}
