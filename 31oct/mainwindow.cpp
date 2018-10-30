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
    ui->comboBox->addItem("DDA Algorithm");
    ui->comboBox->addItem("Bresenham Algorithm");
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::changeX(int x){
    int k=ui->grid_size->value();
    x-=img.width()/2;
    x/=k;
    return x;
}

int MainWindow::changeY(int y){
    int k=ui->grid_size->value();
    y=img.width()/2-y;
    y/=k;
    return y;
}

void MainWindow::point(int x,int y)
{
    int k=ui->grid_size->value();
    if(k==1)
        img.setPixel(x,y,qRgb(255,255,0));
    else{
        int i,j;
        int stx=(x/k)*k;
        int sty=(y/k)*k;
        for(i=stx+1;i<stx+k;i++){
            for(j=sty+1;j<sty+k;j++)
                img.setPixel(i,j,qRgb(255,255,0));
        }
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::point(int x,int y, int r, int g, int b)
{
    int k=ui->grid_size->value();
    if(k==1)
        img.setPixel(x,y,qRgb(r,g,b));
    else{
        int i,j;
        int stx=(x/k)*k;
        int sty=(y/k)*k;
        for(i=stx+1;i<stx+k;i++){
            for(j=sty+1;j<sty+k;j++)
                img.setPixel(i,j,qRgb(r,g,b));
        }
    }
    //ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::showMousePosition(QPoint &pos)
{
    ui->mouse_movement->setText(" X : "+QString::number(changeX(pos.x()))+", Y : "+QString::number(changeY(pos.y())));
}
void MainWindow::Mouse_Pressed()
{
    ui->mouse_pressed->setText(" X : "+QString::number(changeX(ui->frame->x))+", Y : "+QString::number(changeY(ui->frame->y)));
    point(ui->frame->x,ui->frame->y);
    ui->x_axis->move(0,ui->frame->y);
    ui->y_axis->move(ui->frame->x,0);
    //point(ui->frame->x, ui->frame->y,255,255,0);

    if(flag){
        p1.setX((ui->frame->x));
        p1.setY((ui->frame->y));

        if(vertices.size() > 0 && p1 == vertices[0]){

            flag = false;
            return;

        }

        vertices.push_back(p1);
    }
}

void MainWindow::on_show_axes_clicked()
{
    if(ui->show_axes->isChecked())
    {
        for(int i=0;i<img.height();i++)
            point(i,img.width()/2,0,255,255);
        for(int j=0;j<img.width();j++)
            point(img.height()/2,j,0,255,255);
    }
    else{
        for(int i=0;i<img.height();i++)
            point(i,img.width()/2,0,0,0);
        for(int j=0;j<img.width();j++)
            point(img.height()/2,j,0,0,0);
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
    //int r0=ui->circle_radius->value();
    //QPainter painter(&img);
    //QPen pen;
    //pen.setWidth(1);
    //pen.setColor(Qt::red);
    if(ui->draw_circle->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        /*painter.setPen(pen);
        painter.drawEllipse(p1,r0,r0);*/
        drawCircle(p1,ui->circle_radius->value(),ui->comboBox_2->currentIndex());
    }
    if(ui->draw_line->isChecked()){
        //painter.setPen(Qt::red);
        drawLine(p1,p2,ui->comboBox->currentIndex());
    }
    if(ui->draw_ellipse->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
        drawEllipse(p1,ui->ellipse_maj->value(),ui->ellipse_min->value());
    }


    //ui->frame->setPixmap(QPixmap::fromImage(img));
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

void MainWindow::on_show_grid_clicked()
{
    int i,j,k=ui->grid_size->value();
    if(ui->show_grid->isChecked()){
        for(i=0;i<=img.width();i+=k){
            for(j=0;j<=img.height();j++)
                img.setPixel(j,i,qRgb(255,0,0));
        }
        for(i=0;i<=img.height();i+=k){
            for(j=0;j<=img.width();j++)
                img.setPixel(i,j,qRgb(255,0,0));
        }
        ui->frame->setPixmap(QPixmap::fromImage(img));
    }
    else{
        for(i=0;i<=img.width();i++){
            for(j=0;j<=img.height();j++)
                img.setPixel(j,i,qRgb(0,0,0));
        }
        for(i=0;i<=img.height();i++){
            for(j=0;j<=img.width();j++)
                img.setPixel(i,j,qRgb(0,0,0));
        }
        ui->frame->setPixmap(QPixmap::fromImage(img));
    }

    on_show_axes_clicked();
}

void MainWindow::drawDDA(QPoint p1, QPoint p2){
    int k=ui->grid_size->value();
    int x1=(p1.x()/k);
    int y1=(p1.y()/k);
    int x2=(p2.x()/k);
    int y2=(p2.y()/k);
    int dx=x2-x1;
    int dy=y2-y1;
    int st=(int)((fabs(dx)>fabs(dy))?fabs(dx):fabs(dy));
    float xi=((float)dx)/st;
    float yi=((float)dy)/st;

    float x=x1*k+k/2,y=y1*k+k/2;
    for(int i=1;i<=st;i++){
        x+=xi*k;
        y+=yi*k;
        point((int)(x+0.5),(int)(y+0.5));
    }
}

void MainWindow::BresGentle(int x1,int y1, int x2, int y2){
    int k=ui->grid_size->value();
    int dx=(x2-x1)/k;
    int dy=(y2-y1)/k;
    int yi =k;
    if(dy<0){
        yi=-k;
        dy=-dy;
    }
    int dif=2*dy-dx;
    int i,j;
    j=(y1/k)*k+k/2;
    int xs=(x1/k)*k+k/2;
    int xd=(x2/k)*k+k/2;
    for(i=xs;i<=xd;i+=k){
        point(i,j);
        if (dif>0){
            j+=yi;
            dif-=2*dx;
        }
        dif+=2*dy;
    }
}

void MainWindow::BresSteep(int x1,int y1, int x2, int y2){
    int k=ui->grid_size->value();
    int dx=(x2-x1)/k;
    int dy=(y2-y1)/k;
    int xi =k;
    if(dx<0){
        xi=-k;
        dx=-dx;
    }
    int dif=2*dx-dy;
    int i,j;
    i=(x1/k)*k+k/2;
    int ys=(y1/k)*k+k/2;
    int yd=(y2/k)*k+k/2;
    for(j=ys;j<=yd;j+=k){
        point(i,j);
        if (dif>0){
            i+=xi;
            dif-=2*dy;
        }
        dif+=2*dx;
    }
}

void MainWindow::drawBresenham(QPoint p1, QPoint p2){

    if(fabs(p2.y()-p1.y())<fabs(p2.x()-p1.x())){
        if(p1.x()>p2.x())
            BresGentle(p2.x(),p2.y(),p1.x(),p1.y());
        else
            BresGentle(p1.x(),p1.y(),p2.x(),p2.y());
    }
    else{
        if(p1.y()>p2.y())
            BresSteep(p2.x(),p2.y(),p1.x(),p1.y());
        else
            BresSteep(p1.x(),p1.y(),p2.x(),p2.y());
    }
}

void MainWindow::drawCirBres(QPoint p, int r){
    int xc=p.x(),yc=p.y();
    int i,k=ui->grid_size->value(),x=0,y=r*k,d=(3-2*r)*k;
    int dirx[]={+1,+1,-1,-1};
    int diry[]={+1,-1,+1,-1};
    xc=(xc/k)*k+k/2;
    yc=(yc/k)*k+k/2;
    while(y>=x){
        for(i=0;i<4;i++)
            point(xc+dirx[i]*x,yc+diry[i]*y);
        for(i=0;i<4;i++)
            point(xc+dirx[i]*y,yc+diry[i]*x);
        x++;
        if(d>0){
            y--;
            d+=4*(x-y)+10;
        }
        else
            d+=4*x+6;
    }
}

void MainWindow::drawCirMidPt(QPoint p1, int r){
    int k=ui->grid_size->value();
    int xc=(p1.x()/k)*k+k/2,yc=(p1.y()/k)*k+k/2;
    int x=r*k,y=0;
    point(xc+x,yc+y);
    if(r>0){
        point(xc+x,yc-y);
        point(xc-x,yc+y);
        point(xc-x,yc-y);
    }
    int p=(1-r)*k;
    while(x>y){
        y++;
        if(p<=0)
            p+=2*y+1;
        else{
            x--;
            p+=2*y-2*x+1;
        }
        if(x<y)
            break;
        point(xc+x,yc+y);
        point(xc+x,yc-y);
        point(xc-x,yc+y);
        point(xc-x,yc-y);
        if(x!=y){
            point(xc+y,yc+x);
            point(xc+y,yc-x);
            point(xc-y,yc+x);
            point(xc-y,yc-x);
        }
    }

    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::drawLine(QPoint p1, QPoint p2, int i){
    if(i)
       drawBresenham(p1,p2);
    else
        drawDDA(p1,p2);
}

void MainWindow::drawCircle(QPoint p, int r, int i){
    if(i){
        drawCirBres(p,r);
    }
    else
        drawCirMidPt(p,r);
}

void MainWindow::drawEllipse(QPoint p1,int a,int b){
    int k=ui->grid_size->value();
    int xc=(p1.x()/k)*k+k/2,yc=(p1.y()/k)*k+k/2;
    int x=0,y=b,xk,yk;
    int a2=a*a,b2=b*b,ta2=2*a2,tb2=2*b2;

    int px=0,py=ta2*y;

    double p=b2-a2*b+a2/4;
    while(px<py){
        xk=x*k;
        yk=y*k;
        point(xc+xk,yc+yk);
        point(xc+xk,yc-yk);
        point(xc-xk,yc+yk);
        point(xc-xk,yc-yk);
        x++;
        px+=tb2;
        if(p<0){
            p+=b2+px;
        }
        else{
            y--;
            py-=ta2;
            p+=b2+px-py;
        }
    }

    p=b2*((double)x+0.5)*((double)x+0.5)+a2*(y-1)*(y-1)-a2*b2;
    while(y>=0){
        xk=x*k;
        yk=y*k;
        point(xc+xk,yc+yk);
        point(xc+xk,yc-yk);
        point(xc-xk,yc+yk);
        point(xc-xk,yc-yk);
        y--;
        py-=ta2;
        if(p>0){
            p+=a2-py;
        }
        else{
            x++;
            px+=tb2;
            p-=a2-py+px;
        }
    }
}

void MainWindow::on_set_point_3_clicked()
{
    if(ui->fill_color->isChecked()){
        p1.setX(ui->frame->x);
        p1.setY(ui->frame->y);
    }
}

void MainWindow::on_Fill_clicked()
{
    int i=ui->comboBox_4->currentIndex()+1;
    switch(i){
        case 1:
            floodFill(p1);
        break;
        case 2:
            boundaryFill(p1);
        break;
        /*case 3:
            scanLineFill(p1);
        break;*/
    }
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::floodFill(QPoint p){
    int k=ui->grid_size->value();
    int xc=(p.x()/k)*k+k/2;
    int yc=(p.y()/k)*k+k/2;
    floodFillRec(xc,yc,k,img.pixel(xc,yc),255,255,255);
}

void MainWindow::floodFillRec(int x, int y, int k, QRgb q, int r, int g, int b){
    if(x<0 || x>img.width() || y<0 || y>img.height() || img.pixel(x,y)!=q)
        return;
    point(x,y,r,g,b);
    floodFillRec(x+k,y,k,q,r,g,b);
    floodFillRec(x,y+k,k,q,r,g,b);
    floodFillRec(x-k,y,k,q,r,g,b);
    floodFillRec(x,y-k,k,q,r,g,b);
}

void MainWindow::boundaryFill(QPoint p){
    int k=ui->grid_size->value();
    int xc=(p.x()/k)*k+k/2;
    int yc=(p.y()/k)*k+k/2;
    boundaryFillRec(xc,yc,k,255,255,0,255,255,255);
}

void MainWindow::boundaryFillRec(int x, int y, int k, int rb, int gb, int bb, int rf, int gf, int bf){
    QColor cur(img.pixel(x,y));
    if(x<0 || x>img.width() || y<0 || y>img.height() || (cur.red()==rb && cur.green()==gb && cur.blue()==bb) || (cur.red()==rf && cur.green()==gf && cur.blue()==bf))
        return;
    point(x,y,rf,gf,bf);
    boundaryFillRec(x+k,y,k,rb,gb,bb,rf,gf,bf);
    boundaryFillRec(x,y+k,k,rb,gb,bb,rf,gf,bf);
    boundaryFillRec(x-k,y,k,rb,gb,bb,rf,gf,bf);
    boundaryFillRec(x,y-k,k,rb,gb,bb,rf,gf,bf);
}

void MainWindow::translate_object(int xx,int yy){


    /*

    int i,j;
    int p1;
    int p2;
    //int k = ui->grid_size->value();
    //int k = 5;
    QRgb a[700][700];
    for(i=1;i<=img.height();i++)
        for(j=1;j<=img.width();j++)
            a[i][j]=img.pixel(i,j);

    for(i=1;i<=img.height();i++){
        for(j=1;j<=img.width();j++){

            QColor cur(a[i][j]);
            if(cur.red() == 255 && cur.green()== 255 && cur.blue() == 0){
                p1 = i + y;
                p2 = j - x;
                point(p1,p2,255,255,0);
                point(i,j,0,0,0);
            }
        }
    }

    ui->frame->setPixmap(QPixmap::fromImage(img));

    */
    \
    on_pushButton_clicked();
    on_show_grid_clicked();

    int l = vertices.size();
    int i;

    for(i=0;i<l;i++){
        QPoint p = vertices[i];
        p.setX(p.x()+xx);
        p.setY(p.y()+yy);

    }

    for(i=0;i<l-1;i++){
        drawBresenham(vertices[i], vertices[i+1]);
    }

    drawBresenham(vertices[0], vertices[l-1]);


}




void MainWindow::on_draw_ellipse_2_clicked()
{
    flag = true;
}



void MainWindow::on_Draw_2_clicked()
{
    int l = vertices.size();
    int i;

    for(i=0;i<l-1;i++){
        drawBresenham(vertices[i], vertices[i+1]);
    }

    drawBresenham(vertices[0], vertices[l-1]);
}

void MainWindow::on_Draw_3_clicked()
{
    int x = ui->hor_trans->value();
    int y = ui->Ver_trans->value();
    int k = ui->grid_size->value();
    int xx = x*k;
    int yy = y*k;
    int l = vertices.size();
    int i;


    on_pushButton_clicked();
    on_show_grid_clicked();
    for(i=0;i<l;i++){
        QPoint p = vertices[i];
        p.setX(p.x()+xx);
        p.setY(p.y()-yy);
        vertices[i] = p;

    }

    for(i=0;i<l-1;i++){
        drawBresenham(vertices[i], vertices[i+1]);
    }

    drawBresenham(vertices[0], vertices[l-1]);
}




void MainWindow::on_Draw_4_clicked()
{

    on_pushButton_clicked();
    on_show_grid_clicked();
    int i;
    int wid = img.width();
    //int len = img.height();

    //QVector<QPoint> temp;
    int l = vertices.size();
    for(i=0;i<l;i++){
        QPoint p = vertices[i];
        p.setX(wid - p.x());
        p.setY(p.y());
        //temp.push_back(p);
        vertices[i] = p;

    }

    for(i=0;i<l-1;i++){
        drawBresenham(vertices[i], vertices[i+1]);
    }

    drawBresenham(vertices[0], vertices[l-1]);
}

void MainWindow::on_Draw_5_clicked()
{

    on_pushButton_clicked();
    on_show_grid_clicked();
    int i;
    //int wid = img.width();
    int len = img.height();

    //QVector<QPoint> temp;
    int l = vertices.size();
    for(i=0;i<l;i++){
        QPoint p = vertices[i];
        p.setX(p.x());
        p.setY( len - p.y());
      //  temp.push_back(p);
        vertices[i] = p;

    }

    for(i=0;i<l-1;i++){
        drawBresenham(vertices[i], vertices[i+1]);
    }

    drawBresenham(vertices[0], vertices[l-1]);
}

void MainWindow::on_Draw_6_clicked()
{
    on_pushButton_clicked();
    on_show_grid_clicked();

    int x = ui->scaleratio->value();
    int y = ui->scaleratio_2->value();
    int i;
    int wid = img.width();
    wid = wid/2;
    int len = img.height();
    len = len/2;

    //QVector<QPoint> temp;
    int l = vertices.size();
    for(i=0;i<l;i++){
        QPoint p = vertices[i];
        p.setX((x*(p.x() - wid)) + wid);
        p.setY((y*(p.y() - len)) + len);
        //temp.push_back(p);
        vertices[i] = p;

    }

    for(i=0;i<l-1;i++){
        drawBresenham(vertices[i], vertices[i+1]);
    }

    drawBresenham(vertices[0], vertices[l-1]);

}

void MainWindow::on_Draw_7_clicked()
{

    on_pushButton_clicked();
    on_show_grid_clicked();

    int x = ui->scaleratio->value();
    int y = ui->scaleratio_2->value();
    int i;
    int wid = img.width();
    wid = wid/2;
    int len = img.height();
    len = len/2;

 //   QVector<QPoint> temp;
    int l = vertices.size();
    for(i=0;i<l;i++){
        QPoint p = vertices[i];

        int xx = p.x() - wid;
        int yy = len - p.y();
        p.setX(xx+yy*x + wid);
        p.setY(2*len - (yy+xx*y + len));
        vertices[i] = p;
       // temp.push_back(p);

    }

    for(i=0;i<l-1;i++){
        drawBresenham(vertices[i], vertices[i+1]);
    }

    drawBresenham(vertices[0], vertices[l-1]);
}



void MainWindow::on_Draw_8_clicked()
{

    on_pushButton_clicked();
    on_show_grid_clicked();

    int wid = img.width();
    int len = img.height();
    wid = wid/2;
    len = len/2;

    int x1 = p1.x() - wid;
    int x2 = p2.x() - wid;
    int y1 = len - p1.y();
    int y2 = len - p2.y();

    int a,b,c;

    a = y2-y1;
    b = x1-x2;
    c = x1*(y1-y2) - y1*(x1-x2);

    int i;


    int l = vertices.size();
    for(i=0;i<l;i++){
        QPoint p = vertices[i];

        int x = p.x() - wid;
        int y = len - p.y();



        int xx = (x*(b*b - a*a) - 2*a*(b*y+c))/(a*a + b*b);
        int yy = (y*(a*a - b*b) - 2*b*(a*x+c))/(a*a + b*b);
        p.setX(xx + wid);
        p.setY(len - yy);
        vertices[i] = p;
    }

    for(i=0;i<l-1;i++){
        drawBresenham(vertices[i], vertices[i+1]);
    }

    drawBresenham(vertices[0], vertices[l-1]);
    drawBresenham(p1,p2);


}

void MainWindow::on_draw_ellipse_3_clicked()
{
    flag = false;
   // vertices.clear();

}



void MainWindow::on_Draw_9_clicked()
{
    flag = false;
    vertices.clear();
}

void MainWindow::on_Draw_10_clicked()
{

    on_pushButton_clicked();
    on_show_grid_clicked();

    int angle = ui->rotate_point->value();
    int x1 = p1.x();
    int y1 = p1.y();


    float rad = (3.14/180.0)*angle;
    float s = sin(rad);
    float c = cos(rad);
    int i;

    int l = vertices.size();


    for(i=0;i<l;i++){
        QPoint p = vertices[i];

        int x = p.x() - x1;
        int y = y1 - p.y();

        int xn = int(x*1.0*c - y*1.0*s);
        int yn = int(y*1.0*c + x*1.0*s);

        p.setX(xn + x1);
        p.setY(y1 - yn);
        vertices[i] = p;
    }

    for(i=0;i<l-1;i++){
        drawBresenham(vertices[i], vertices[i+1]);
    }

    drawBresenham(vertices[0], vertices[l-1]);

}
