#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QColorDialog>
#include<QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public slots:
    void Mouse_Pressed();
    void showMousePosition(QPoint& pos);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_show_axes_clicked();

    void on_Draw_clicked();

    void on_set_point1_clicked();

    void on_set_point2_clicked();

    void on_pushButton_clicked();

    int changeX(int x);

    int changeY(int y);

    void on_show_grid_clicked();

    void drawLine(QPoint, QPoint, int);

    void drawCircle(QPoint, int, int);

    void drawEllipse(QPoint,int,int);

    void on_set_point_3_clicked();

    void on_Fill_clicked();

    void translate_object(int x,int y);

    void on_draw_ellipse_2_clicked();

    void on_Draw_2_clicked();

    void on_Draw_3_clicked();

    void on_Draw_4_clicked();

    void on_Draw_5_clicked();

    void on_Draw_6_clicked();

    void on_Draw_7_clicked();

    void on_Draw_8_clicked();

    void on_draw_ellipse_3_clicked();

    void on_Draw_9_clicked();

    void on_comboBox_3_activated(const QString &arg1);

    void on_Draw_10_clicked();

private:
    bool flag = false;
    QVector<QPoint> vertices;
    Ui::MainWindow *ui;
    QPoint p1,p2;
    void point(int,int);
    void point(int ,int , int , int, int);
    void drawDDA(QPoint, QPoint);
    void drawBresenham(QPoint, QPoint);
    void BresGentle(int,int,int,int);
    void BresSteep(int,int,int,int);
    void drawCirBres(QPoint, int);
    void drawCirMidPt(QPoint, int);
    void floodFill(QPoint);
    void floodFillRec(int, int, int, QRgb, int, int, int);
    void boundaryFill(QPoint);
    void boundaryFillRec(int, int, int, int, int, int, int, int, int);
    void scanLineFill(QPoint);
};

#endif // MAINWINDOW_H
