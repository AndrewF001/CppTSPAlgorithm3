#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CppTSPalgorithm.h"
#include "Point.h"
#include "MyTimer.h"
#include "QuadTree.h"
#include "VectorClass.h"
#include "omp.h"

class CppTSPalgorithm : public QMainWindow
{
    Q_OBJECT

public:
    CppTSPalgorithm(QWidget *parent = Q_NULLPTR);
    void paintEvent(QPaintEvent* event);
    Ui::CppTSPalgorithmClass ui;

private slots:    
    void StartBtnClicked();
    void DelayBoxChanged(int);
    void DotsBtnClicked();
    void ShowGridMeth(bool);
    void FastModeMeth(bool);
    void FPSBoxChanged(int);

private:
    void DrawCanvas(QPainter*);
    void DrawPoints(QPainter*);
    void DrawGrid(QPainter*);
    void DrawRoute(QPainter*);
    void CreatePoints();
    void ResetVaraibles();
    void CheckFastMode();
    void UpdateTimers();
    void UIUpdateMethod();
    void CreateQuadTreeMain();
    void CreateQuadTreeSub(int);
    void FindPointMain();
    void FindTopPoint();
    void FindRightPoint();
    void FindBottomPoint();
    void FindLeftPoint();
    void ConnecterMain();
    void ConnectTopRight();
    void ConnectRightBottom();
    void ConnectBottomLeft();
    void ConnectLeftTop();
    void LogicMain();
    void LogicMethodSingle();
    void LogicMethodMulti(int, int, int, Point*,Point, double, double,QuadTree*);
    void OverlapMain();
    void OverlapMethod(int);
    bool OverlapCheck(Point*, Point*, Point*, Point*);
    double Crossproduct( VectorClass*, VectorClass*);
    bool Opt2(Point*, Point*, Point*, Point*);
    //void LogicMulti(unsigned short int ThreadNum);


    bool ShowGrid=false;
    bool FastMode=false;
    bool AddedDots=false;
    bool SetUpRunning=false;
    bool ProgramRunning = false;
    bool Render = false;
    unsigned int NumDots=10;
    unsigned int Delay=0;
    unsigned int FPS=60;

    std::vector<Point*> Top;
    std::vector<Point*> Right;
    std::vector<Point*> Bottom;
    std::vector<Point*> Left;

    //std::vector<Point*> Route;
    Point* StartNode = nullptr;
    int RouteSize = 0;

    const int NumThreads;
    std::vector<Point*> ChoosenPoint;
    std::vector<double> Distance;
    std::vector<double> DeltaDistance;
    std::vector<double> NextWidthHeight = { 3604 };    
    std::vector<Point*> BackPoint;
    std::vector<Point*> ForwardPoint;

    double LongestConnection = 0;

    MyTimer Timer;

    std::vector<Point> Points;
    QuadTree* ParentNode = new QuadTree(-1, -1, 1802, 842);

    
};
