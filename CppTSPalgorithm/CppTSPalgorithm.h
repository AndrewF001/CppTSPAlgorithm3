#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CppTSPalgorithm.h"
#include "Point.h"
#include "MyTimer.h"
#include "QuadTree.h"

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
    void LogicMethod(unsigned short int ThreadNum);
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

    static const int NumThreads = 1;


    Point* ChoosenPoint[NumThreads];
    double Distance[NumThreads];
    double DeltaDistance[NumThreads];
    //bool ThreadDone[NumThreads];
    //bool ThreadRestart[NumThreads];
    //bool Threadsrunning = false;
    //bool ThreadsWaiting = true;
    std::vector<double> NextWidthHeight = { 3604 };    
    Point* BackPoint[NumThreads];
    Point* ForwardPoint[NumThreads];

    MyTimer Timer;

    std::vector<Point> Points;
    QuadTree* ParentNode = new QuadTree(-1, -1, 1802, 842);

    
};
