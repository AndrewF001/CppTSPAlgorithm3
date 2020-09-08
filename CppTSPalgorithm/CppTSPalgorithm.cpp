#include "CppTSPalgorithm.h"
#include "qpainter.h"
#include <thread>
#include <math.h>
#include <iostream>
#include "MyVector.h"
#include "omp.h"


//constructor, just links all the UI elements to their EventHandler slot
CppTSPalgorithm::CppTSPalgorithm(QWidget *parent)
    : QMainWindow(parent), NumThreads(omp_get_max_threads())
{
    ChoosenPoint.resize(NumThreads);
    Distance.resize(NumThreads);
    DeltaDistance.resize(NumThreads);
    BackPoint.resize(NumThreads);
    ForwardPoint.resize(NumThreads);
    ui.setupUi(this);
    //connects UI event handlers to their methods
    connect(ui.StartBtn, SIGNAL(clicked()), this, SLOT(StartBtnClicked()));
    connect(ui.DelayBox, SIGNAL(valueChanged(int)), this,SLOT(DelayBoxChanged(int)));
    connect(ui.DotsBtn, SIGNAL(clicked()), this, SLOT(DotsBtnClicked()));
    connect(ui.DotsBox, QOverload<int>::of(&QSpinBox::valueChanged), [=](int i) {NumDots = i; });
    connect(ui.ShowGridCheck, SIGNAL(toggled(bool)),this, SLOT(ShowGridMeth(bool)));
    connect(ui.FastModeCheck, SIGNAL(toggled(bool)), this, SLOT(FastModeMeth(bool)));
    connect(ui.FPSBox, SIGNAL(valueChanged(int)), this, SLOT(FPSBoxChanged(int)));
}

//All grathics are done here
void CppTSPalgorithm::paintEvent(QPaintEvent* event)
{
    QPainter Painter(this);
    DrawCanvas(&Painter);
    DrawPoints(&Painter);
    UpdateTimers();
    DrawGrid(&Painter);
    DrawRoute(&Painter);
}

//All grapical methods bellow untill at the event handlers
void CppTSPalgorithm::DrawCanvas(QPainter* painter)
{
    QPen Pen(Qt::black);
    Pen.setWidth(3);
    painter->setPen(Pen);
    painter->drawRect(10, 80, 1820, 860);
}
void CppTSPalgorithm::DrawPoints(QPainter* painter)
{
    QPen Pen(Qt::black);
    Pen.setWidth(2);
    painter->setPen(Pen);
    if (!Points.empty())
    {
        for (int i = 0; i < Points.size(); i++)
        {
            painter->drawEllipse(Points[i].X+15, Points[i].Y+85, 2, 2);
        }
    }
}
void CppTSPalgorithm::UpdateTimers()
{
    if (ProgramRunning)
    {
        std::chrono::steady_clock::duration durration = Timer.Time();
        ui.TimerLabNS->setText(QString::number(durration.count()));
        ui.TimerLabS->setText(QString::number(durration.count() / pow(10, 9)));
    }
}
void CppTSPalgorithm::UIUpdateMethod()
{
    repaint();
    while (Render)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/FPS));
        repaint();
    }
}
void CppTSPalgorithm::DrawGrid(QPainter* painter)
{
    if(ShowGrid && ParentNode!=nullptr)
        ParentNode->DrawRect(painter);
}
void CppTSPalgorithm::DrawRoute(QPainter* painter)
{
    if (RouteSize>1)
    {
        QPen Pen(Qt::black);
        Pen.setWidth(1);
        painter->setPen(Pen);
        StartNode->DrawConenctions(painter,StartNode);
    }
}

//All event handlers are bellow untill main block of code
//StartBtnClicked is the entry point into the code.
void CppTSPalgorithm::StartBtnClicked()
{
    if (AddedDots && !SetUpRunning && !ProgramRunning)
    {
        //setup
        Timer.Reset();
        ProgramRunning = true;      
        Render = true;
        //Create QuadTree
        std::thread QuadThread (&CppTSPalgorithm::CreateQuadTreeMain,this);
        UIUpdateMethod();
        QuadThread.join();
        //Find valuable points
        Render = true;
        std::thread SearchThread(&CppTSPalgorithm::FindPointMain, this);
        UIUpdateMethod();
        SearchThread.join();
        //create the circle
        Render = true;
        std::thread ConnectThread(&CppTSPalgorithm::ConnecterMain, this);
        UIUpdateMethod();
        ConnectThread.join();
        //Makes the link towards to middle
        Render = true;
        fill(Distance.begin(), Distance.end(), 3604);
        fill(DeltaDistance.begin(), DeltaDistance.end(), 3604);

        std::thread LogicThread(&CppTSPalgorithm::LogicMain, this);
        UIUpdateMethod();
        LogicThread.join();
        //std::this_thread::sleep_for(std::chrono::seconds(5));
        Render = true;
        std::thread OverlapThread(&CppTSPalgorithm::OverlapMain, this);
        UIUpdateMethod();
        OverlapThread.join();

        AddedDots = false;
        ProgramRunning = false;
        UpdateTimers();
    }
    else if (!AddedDots && !ProgramRunning)
    {
        DotsBtnClicked();
        StartBtnClicked();
    }
}

void CppTSPalgorithm::DelayBoxChanged(int value)
{
    Delay = value;
    CheckFastMode();
}
void CppTSPalgorithm::DotsBtnClicked()
{
    if (!SetUpRunning)
    {        
        AddedDots = true;
        SetUpRunning = true;
        Render = true;
        std::thread CPointThread (&CppTSPalgorithm::CreatePoints,this);
        UIUpdateMethod();
        CPointThread.join();   
    }    
}
void CppTSPalgorithm::ShowGridMeth(bool value)
{    
    ShowGrid = value;
    CheckFastMode();
    update();
}
void CppTSPalgorithm::FastModeMeth(bool value)
{
    //when changing the value of the other UI it would override FastMode so that's why done in this order
    if (value)
    {
        ui.ShowGridCheck->setCheckState(Qt::CheckState::Unchecked);
        ui.DelayBox->setValue(0);
        ui.FPSBox->setValue(0);
    }
    FastMode = value;
    if (value)
        ui.FastModeCheck->setCheckState(Qt::CheckState::Checked);
}
void CppTSPalgorithm::FPSBoxChanged(int value)
{
    FPS = value;
    CheckFastMode();
}
//repeated code
void CppTSPalgorithm::CheckFastMode()
{
    if (FastMode)
    {
        FastMode = false;
        ui.FastModeCheck->setCheckState(Qt::CheckState::Unchecked);
    }
}

//Main block of code
//sets up the points and their location
void CppTSPalgorithm::CreatePoints()
{
    ResetVaraibles();

    std::srand((unsigned)time(0));
    ParentNode = new QuadTree(-1, -1, 1802, 842);

    Points.reserve(NumDots);
    for (int i = 0; i < NumDots; i++)
    {
        unsigned short int x = std::rand() % 1800;
        unsigned short int y = std::rand() % 840;
        Points.push_back(Point(x, y));        
        for (int c = 0; c < i; c++)
        {
            if (Points[c].X == Points[i].X && Points[c].Y == Points[i].Y)
            {
                Points.pop_back();
                i--;
                c = i;
            }
        }
    }
    SetUpRunning = false;
    Render = false;
}
void CppTSPalgorithm::ResetVaraibles()
{
    RouteSize = 0;
    Points.clear();
    delete(ParentNode);
    Top.clear();
    Right.clear();
    Bottom.clear();
    Left.clear();
    NextWidthHeight.clear();
    NextWidthHeight.push_back(3604);
    fill(Distance.begin(), Distance.end(), 3604);
    fill(DeltaDistance.begin(), DeltaDistance.end(), 3604);
}
//Quad tree creation methods
void CppTSPalgorithm::CreateQuadTreeMain()
{
    ParentNode->Insert(&Points[0]);
    ParentNode->Insert(&Points[1]);
    if (NumThreads == 1)
    {
        CreateQuadTreeSub(0);
        CreateQuadTreeSub(1);
        CreateQuadTreeSub(2);
        CreateQuadTreeSub(3);
    }
    else
    {
        std::thread TreeThreads[4];
        for (int i = 0; i < 4; i++)
        {
            //int* temp = new int(i);
            TreeThreads[i] = std::thread(&CppTSPalgorithm::CreateQuadTreeSub, this, i);
        }
        for (int i = 0; i < 4; i++)
        {
            TreeThreads[i].join();
        }
    }
    Render = false;
}
void CppTSPalgorithm::CreateQuadTreeSub(int section)
{
    for (int i = 2; i < NumDots; i++)
    {
        ParentNode->SubTree[section].Insert(&Points[i]);
    }

}
//valuable points methods
void CppTSPalgorithm::FindPointMain()
{
    std::thread SearchThreads[4];
    if (NumThreads == 1)
    {
        FindTopPoint();
        FindRightPoint();
        FindBottomPoint();
        FindLeftPoint();
    }
    else
    {
        SearchThreads[0] = std::thread(&CppTSPalgorithm::FindTopPoint, this);
        SearchThreads[1] = std::thread(&CppTSPalgorithm::FindRightPoint, this);
        SearchThreads[2] = std::thread(&CppTSPalgorithm::FindBottomPoint, this);
        SearchThreads[3] = std::thread(&CppTSPalgorithm::FindLeftPoint, this);
        for (int i = 0; i < 4; i++)
        {
            SearchThreads[i].join();
        }
    }
    Render = false;
}
void CppTSPalgorithm::FindTopPoint()
{
    int min = 99999;
    for (int i = 0; i < NumDots; i++)
    {
        if (Points[i].Y < min)
        {
            Top.clear();
            Top.push_back(&Points[i]);
            min = Points[i].Y;
        }
        else if (Points[i].Y == min)
        {
            for (int c = 0; c < Top.size(); c++)
            {
                if (Points[i].X < Top[c]->X)
                {
                    Top.insert(Top.begin() + c, &Points[i]);
                    c = Top.size();
                }
            }
            if (Points[i].X > Top.back()->X)
            {
                Top.push_back(&Points[i]);
            }
        }
    }
}
void CppTSPalgorithm::FindRightPoint()
{
    int max = -2;
    for (int i = 0; i < NumDots; i++)
    {
        if (Points[i].X > max)
        {
            Right.clear();
            Right.push_back(&Points[i]);
            max = Points[i].X;
        }
        else if (Points[i].X == max)
        {
            for (int c = 0; c < Right.size(); c++)
            {
                if (Points[i].Y < Right[c]->Y)
                {
                    Right.insert(Right.begin() + c, &Points[i]);
                    c = Right.size();
                }
            }
            if (Points[i].Y > Right.back()->Y)
            {
                Right.push_back(&Points[i]);
            }
        }
    }
}
void CppTSPalgorithm::FindBottomPoint()
{
    int max = -2;
    for (int i = 0; i < NumDots; i++)
    {
        if (Points[i].Y > max)
        {
            Bottom.clear();
            Bottom.push_back(&Points[i]);
            max = Points[i].Y;
        }
        else if (Points[i].Y == max)
        {
            for (int c = 0; c < Bottom.size(); c++)
            {
                if (Points[i].X > Bottom[c]->X)
                {
                    Bottom.insert(Bottom.begin() + c, &Points[i]);
                    c = Bottom.size();
                }
            }
            if (Points[i].X < Bottom.back()->X)
            {
                Bottom.push_back(&Points[i]);
            }
        }
    }
}
void CppTSPalgorithm::FindLeftPoint()
{
    int min = 99999;
    for (int i = 0; i < NumDots; i++)
    {
        if (Points[i].X < min)
        {
            Left.clear();
            Left.push_back(&Points[i]);
            min = Points[i].X;
        }
        else if (Points[i].X == min)
        {
            for (int c = 0; c < Left.size(); c++)
            {
                if (Points[i].Y > Left[c]->Y)
                {
                    Left.insert(Left.begin() + c, &Points[i]);
                    c = Left.size();
                }
            }
            if (Points[i].Y < Left.back()->Y)
            {
                Left.push_back(&Points[i]);
            }
        }
    }
}
//create outer circle
void CppTSPalgorithm::ConnecterMain()
{
    if (NumThreads == 1)
    {
        ConnectTopRight();
        ConnectRightBottom();
        ConnectBottomLeft();
        ConnectLeftTop();
    }
    else
    {
        std::thread Connecters[4];
        Connecters[0] = std::thread(&CppTSPalgorithm::ConnectTopRight, this);
        Connecters[1] = std::thread(&CppTSPalgorithm::ConnectRightBottom, this);
        Connecters[2] = std::thread(&CppTSPalgorithm::ConnectBottomLeft, this);
        Connecters[3] = std::thread(&CppTSPalgorithm::ConnectLeftTop, this);
        for (int i = 0; i < 4; i++)
        {
            Connecters[i].join();
        }
    }
    //This is just to make sure there is no duplication(can occure if the point is the most vaulable in to sides)
    if (Right.back() == Bottom[0])
        Right.erase(Right.begin() + Right.size()-1);
    if (Bottom.back() == Left[0])
        Bottom.erase(Bottom.begin() + Bottom.size()-1);
    if(Left.back() == Top[0])
        Left.erase(Left.begin() + Left.size() - 1);
    
    //Create the linked list
    StartNode = Top[0];
    RouteSize = Top.size() + Right.size() + Bottom.size() + Left.size();    
    //This merging of all the list and then decoding it into a link list isn't required and can hard code it for each vector
    //It's quicker that way but means you need 4 times the code. I did it this way because the performance difference
    //isn't going to be that noticable(I hope)
    std::vector<Point*> Route;
    Route.reserve(RouteSize);
    Route.insert(Route.end(), Top.begin(), Top.end());
    Route.insert(Route.end(), Right.begin(), Right.end());
    Route.insert(Route.end(), Bottom.begin(), Bottom.end());
    Route.insert(Route.end(), Left.begin(), Left.end());
    Top.clear();
    Right.clear();
    Bottom.clear();
    Left.clear();
    //Decoding the vector to make a linked list
    StartNode->Connected = true;
    StartNode->NextPoint = Route[1];
    StartNode->PreviousePoint = Route.back();
    for (int i = 1; i < Route.size() - 1; i++)
    {
        Route[i]->Connected = true;
        Route[i]->NextPoint = Route[i + 1];
        Route[i]->PreviousePoint = Route[i - 1];
    }
    Route.back()->Connected = true;
    Route.back()->NextPoint = Route[0];
    Route.back()->PreviousePoint = Route[Route.size() - 2];
        
    Render = false;
}
void CppTSPalgorithm::ConnectTopRight()
{
    bool Run = true;
    std::vector<Point*> TempList;
    short int Rx;
    short int Ry;
    unsigned short int Rwidth;
    unsigned short int Rheight;
    while (Run)
    {
        TempList.clear();
        Rx = Top.back()->X;
        Ry = Top.back()->Y;
        Rwidth = Right[0]->X - Rx + 1;
        Rheight = Right[0]->Y - Ry + 1;
        ParentNode->ContainArea(&TempList, Rx, Ry, Rwidth, Rheight);
        Point* NextNode;
        float SmallestGradient = 9999999;
        float TempGradient;
        if (!TempList.empty())
        {
            for (int i = 0; i < TempList.size(); i++)
            {
                if (TempList[i] != Top.back())
                {
                    TempGradient = ((double)TempList[i]->Y - (double)Top.back()->Y) / ((double)TempList[i]->X - (double)Top.back()->X);
                    if (TempGradient < SmallestGradient)
                    {
                        SmallestGradient = TempGradient;
                        NextNode = TempList[i];
                    }
                }
            }
            if (NextNode == Right[0]) //That warning shouldn't be a problem
                Run = false;
            else
                Top.push_back(NextNode);
        }
        else
        {
            Run = false;
        }
    }
}
void CppTSPalgorithm::ConnectRightBottom()
{
    bool Run = true;
    std::vector<Point*> TempList;
    short int Rx;
    short int Ry;
    unsigned short int Rwidth;
    unsigned short int Rheight;
    while (Run)
    {
        TempList.clear();
        Rx = Bottom[0]->X-1;
        Ry = Right.back()->Y;
        Rwidth = Right.back()->X - Rx+3;
        Rheight = Bottom[0]->Y - Ry+1;
        ParentNode->ContainArea(&TempList, Rx, Ry, Rwidth, Rheight);
        Point* NextNode;
        float SmallestGradient = 999999;
        float TempGradient;
        if (!TempList.empty())
        {
            for (int i = 0; i < TempList.size(); i++)
            {
                if (TempList[i] != Right.back())
                {
                    TempGradient = ((double)TempList[i]->Y - (double)Right.back()->Y) / ((double)TempList[i]->X - (double)Right.back()->X);
                    if (TempGradient < SmallestGradient)
                    {
                        SmallestGradient = TempGradient;
                        NextNode = TempList[i];
                    }
                }
            }
            if (NextNode == Bottom[0]) //That warning shouldn't be a problem
                Run = false;
            else
                Right.push_back(NextNode);
        }
        else
        {
            Run = false;
        }
    }
}
void CppTSPalgorithm::ConnectBottomLeft()
{
    bool Run = true;
    std::vector<Point*> TempList;
    short int Rx;
    short int Ry;
    unsigned short int Rwidth;
    unsigned short int Rheight;
    while (Run)
    {
        TempList.clear();
        Rx = Left[0]->X-1;
        Ry = Left[0]->Y-1;
        Rwidth = Bottom.back()->X - Rx;
        Rheight = Bottom.back()->Y - Ry+1;
        ParentNode->ContainArea(&TempList, Rx, Ry, Rwidth, Rheight);
        Point* NextNode;
        float SmallestGradient = 99999;
        float TempGradient;
        if (!TempList.empty())
        {
            for (int i = 0; i < TempList.size(); i++)
            {
                if (TempList[i] != Bottom.back())
                {
                    TempGradient = ((double)TempList[i]->Y - (double)Bottom.back()->Y) / ((double)TempList[i]->X - (double)Bottom.back()->X);
                    if (TempGradient < SmallestGradient)
                    {
                        SmallestGradient = TempGradient;
                        NextNode = TempList[i];
                    }
                }
            }
            if (NextNode == Left[0]) //That warning shouldn't be a problem
                Run = false;
            else
                Bottom.push_back(NextNode);
        }
        else
        {
            Run = false;
        }
    }
}
void CppTSPalgorithm::ConnectLeftTop()
{
    bool Run = true;
    std::vector<Point*> TempList;
    short int Rx;
    short int Ry;
    unsigned short int Rwidth;
    unsigned short int Rheight;
    while (Run)
    {
        TempList.clear();
        Rx = Left.back()->X-1;
        Ry = Top[0]->Y-1;
        Rwidth = Top[0]->X - Rx + 2;
        Rheight = Left.back()->Y - Ry + 2;
        ParentNode->ContainArea(&TempList, Rx, Ry, Rwidth, Rheight);
        Point* NextNode;
        float SmallestGradient = 999999;
        float TempGradient;
        if (!TempList.empty())
        {
            for (int i = 0; i < TempList.size(); i++)
            {
                if (TempList[i] != Left.back())
                {
                    TempGradient = ((double)TempList[i]->Y - (double)Left.back()->Y) / ((double)TempList[i]->X - (double)Left.back()->X);
                    if (TempGradient < SmallestGradient)
                    {
                        SmallestGradient = TempGradient;
                        NextNode = TempList[i];
                    }
                }
            }
            if (NextNode == Top[0]) //That warning shouldn't be a problem
                Run = false;
            else
                Left.push_back(NextNode);
        }
        else
        {
            Run = false;
        }
    }
}
//Starts making the link towards the middle
void CppTSPalgorithm::LogicMain()
{
    //std::thread Worker[NumThreads];
    int shortest = 0;
    Points;
    QuadTree* ParentNodeCopy = ParentNode;
    int MaxThread = omp_get_max_threads();
    while (RouteSize != NumDots)
    {
        if (NumThreads != 1)
        {
            NextWidthHeight.clear();
            NextWidthHeight.push_back(3604);
            for (int i = 0; i < NumThreads; i++)
            {
                if (i != shortest)
                {
                    if (Distance[i] < NextWidthHeight[0])
                        NextWidthHeight[0] = Distance[i];
                }
            }
        }
        else
        {
            if (NextWidthHeight.size() == 2)
            {
                NextWidthHeight.clear();
                NextWidthHeight.push_back(3604);
            }
            else
            {
                double placehold=3604;
                for (int i = 0; i < NextWidthHeight.size(); i++)
                {
                    if (NextWidthHeight[i] < placehold)
                    {
                        placehold = NextWidthHeight[i];
                    }
                }
                NextWidthHeight[0] = placehold;
            }
        }
        fill(Distance.begin(), Distance.end(), 3604);
        //NextWidthHeight.clear();
        //NextWidthHeight.push_back(3604);
        fill(DeltaDistance.begin(), DeltaDistance.end(), 3604);
        //LogicMethodSingle();
        if (MaxThread<2)
        {
            LogicMethodSingle();
        }
        else
        {
            omp_set_num_threads(MaxThread);
#pragma omp parallel for firstprivate(ParentNodeCopy)
                for (int i = 0; i < NumDots; i++)
                {
                     int ID = omp_get_thread_num();
                     if(!Points[i].Connected)
                        LogicMethodMulti(i,ID,NextWidthHeight[0]+20,&Points[i],Points[i],Distance[ID],DeltaDistance[ID], ParentNodeCopy);
                }
        }
        shortest = 0;        
        for (int i = 1; i < NumThreads; i++)
        {
            if (DeltaDistance[i] < DeltaDistance[shortest])
            {                
                shortest = i;
            }
        }        
        ChoosenPoint[shortest]->NextPoint = ForwardPoint[shortest];
        ChoosenPoint[shortest]->PreviousePoint = BackPoint[shortest];
        BackPoint[shortest]->NextPoint = ChoosenPoint[shortest];
        ForwardPoint[shortest]->PreviousePoint = ChoosenPoint[shortest];
        ChoosenPoint[shortest]->Connected = true;
        RouteSize++;        
        if (Distance[shortest] > LongestConnection)
            LongestConnection = Distance[shortest];
    }
    Render = false;
}
void CppTSPalgorithm::LogicMethodSingle()
{    
    unsigned int i = 0;
    std::vector<Point*> TempPoints;
    Point* OtherPoint;
    unsigned short int WidthHeight;
    unsigned short int Offset;
    while (i < NumDots)
    {
        bool Changed = false;
        if (!Points[i].Connected)
        {
            TempPoints.clear();
            TempPoints.reserve(NumDots);//probably overkill so optimize it later.
            WidthHeight = Distance[0]+20; // +20 is just some wigle room
            Offset = WidthHeight / 2;
            ParentNode->ContainArea(&TempPoints, Points[i].X - Offset, Points[i].Y - Offset, WidthHeight, WidthHeight);
            for (int c = 0; c < TempPoints.size(); c++)
            {
                if (TempPoints[c]->Connected)
                {
                    double pointdistance = std::sqrt(std::pow(TempPoints[c]->X - Points[i].X,2) + std::pow(TempPoints[c]->Y - Points[i].Y, 2));
                    OtherPoint = TempPoints[c]->PreviousePoint;
                    double Distance2 = pointdistance + std::sqrt(std::pow(OtherPoint->X - Points[i].X,2) + std::pow(OtherPoint->Y - Points[i].Y,2));
                    double TempDeltaDistance = Distance2 - std::sqrt(std::pow(OtherPoint->X - TempPoints[c]->X, 2) + std::pow(OtherPoint->Y - TempPoints[c]->Y, 2));
                    if (TempDeltaDistance < DeltaDistance[0])
                    {           
                        if (!Changed)
                        {
                            NextWidthHeight.push_back(Distance[0]);
                            Changed = true;
                        }
                        DeltaDistance[0] = TempDeltaDistance;
                        Distance[0] = Distance2;
                        ChoosenPoint[0] = &Points[i];
                        BackPoint[0] = OtherPoint;
                        ForwardPoint[0] = TempPoints[c];
                    }
                    OtherPoint = TempPoints[c]->NextPoint;
                    Distance2 = pointdistance + std::sqrt(std::pow(OtherPoint->X - Points[i].X,2) + std::pow(OtherPoint->Y - Points[i].Y,2));
                    TempDeltaDistance = Distance2 - std::sqrt(std::pow(OtherPoint->X - TempPoints[c]->X, 2) + std::pow(OtherPoint->Y - TempPoints[c]->Y, 2));
                    if (TempDeltaDistance < DeltaDistance[0])
                    {                        
                        if (!Changed)
                        {
                            NextWidthHeight.push_back(Distance[0]);
                            Changed = true;
                        }
                        DeltaDistance[0] = TempDeltaDistance;
                        Distance[0] = Distance2;
                        ChoosenPoint[0] = &Points[i];
                        BackPoint[0] = TempPoints[c];
                        ForwardPoint[0] = OtherPoint;
                    }
                }
            }
        }
        i ++;
    }
}

void CppTSPalgorithm::LogicMethodMulti(int Place, int ThreadID, int Size, Point *CurrentPointP,Point CurrentPoint, double ThreadDistance, double ThreadDeltaDistance, QuadTree* ParentNodeCopy)
{
    std::vector<Point*> TempPoints;
    Point* OtherPoint;
    unsigned short int WidthHeight = Size;
    unsigned short int Offset = WidthHeight / 2;
    bool Changed = false;
    Point* ThreadChoosenPoint, * ThreadBackpoint, *ThreadForwarpPoint;
    TempPoints.reserve(NumDots);//probably overkill so optimize it later.            
    ParentNodeCopy->ContainArea(&TempPoints, CurrentPoint.X - Offset, CurrentPoint.Y - Offset, WidthHeight, WidthHeight);
    for (int c = 0; c < TempPoints.size(); c++)
    {
         if (TempPoints[c]->Connected)
         {
              double pointdistance = std::sqrt(std::pow(TempPoints[c]->X - CurrentPoint.X, 2) + std::pow(TempPoints[c]->Y - CurrentPoint.Y, 2));
              OtherPoint = TempPoints[c]->PreviousePoint;
              double Distance2 = pointdistance + std::sqrt(std::pow(OtherPoint->X - CurrentPoint.X, 2) + std::pow(OtherPoint->Y - CurrentPoint.Y, 2));
              double TempDeltaDistance = Distance2 - std::sqrt(std::pow(OtherPoint->X - TempPoints[c]->X, 2) + std::pow(OtherPoint->Y - TempPoints[c]->Y, 2));
              if (TempDeltaDistance < ThreadDeltaDistance)
              {
                  if (!Changed)
                       Changed = true;
                  ThreadDeltaDistance = TempDeltaDistance;
                  ThreadDistance = Distance2;
                  ThreadChoosenPoint = CurrentPointP;
                  ThreadBackpoint = OtherPoint;
                  ThreadForwarpPoint = TempPoints[c];
              }
              OtherPoint = TempPoints[c]->NextPoint;
              Distance2 = pointdistance + std::sqrt(std::pow(OtherPoint->X - CurrentPoint.X, 2) + std::pow(OtherPoint->Y - CurrentPoint.Y, 2));
              TempDeltaDistance = Distance2 - std::sqrt(std::pow(OtherPoint->X - TempPoints[c]->X, 2) + std::pow(OtherPoint->Y - TempPoints[c]->Y, 2));
              if (TempDeltaDistance < ThreadDeltaDistance)
              {
                  if (!Changed)
                      Changed = true;
                  ThreadDeltaDistance = TempDeltaDistance;
                  ThreadDistance = Distance2;
                  ThreadChoosenPoint = CurrentPointP;
                  ThreadBackpoint = TempPoints[c];
                  ThreadForwarpPoint = OtherPoint;
              }
         }
    }
    if (Changed)
    {
         DeltaDistance[ThreadID] = ThreadDeltaDistance;
         Distance[ThreadID] = ThreadDistance;
         ChoosenPoint[ThreadID] = ThreadChoosenPoint;
         ForwardPoint[ThreadID] = ThreadForwarpPoint;
         BackPoint[ThreadID] = ThreadBackpoint;
    }
}
void CppTSPalgorithm::OverlapMain()
{
    LongestConnection += 4;
    if (NumThreads == 1)
        OverlapMethod(0);
    else
    {
        std::vector<std::thread> Worker;
        Worker.resize(NumThreads);
        for (int i = 0; i < NumThreads; i++)
        {
            Worker[i] = std::thread(&CppTSPalgorithm::OverlapMethod, this, i);
        }
        for (int i = 0; i < NumThreads; i++)
        {
            Worker[i].join();
        }
    }
    Render = false;
}
void CppTSPalgorithm::OverlapMethod(int ThreadNum)
{
    int i = ThreadNum;
    Point* CurrentNode = &Points[i];
    bool MadeChanges = true;
    while(MadeChanges)
    {
        MadeChanges = false;
        while (i < NumDots)
        {
            std::vector<Point*> TempPoints;
            ParentNode->ContainArea(&TempPoints, CurrentNode->X - LongestConnection, CurrentNode->Y - LongestConnection, LongestConnection * 2, LongestConnection * 2);
            for (int c = 0; c < TempPoints.size(); c++)
            {
                if (TempPoints[c] != CurrentNode && TempPoints[c] != CurrentNode->NextPoint && TempPoints[c]->NextPoint != CurrentNode)
                {
                    if (OverlapCheck(CurrentNode, CurrentNode->NextPoint, TempPoints[c], TempPoints[c]->NextPoint))
                    {
                        MadeChanges = true;
                        Point P1 = *CurrentNode;
                        Point P2 = *CurrentNode->NextPoint;
                        Point Q1 = *TempPoints[c];
                        Point Q2 = *TempPoints[c]->NextPoint;
                        Point* ChangePoint = CurrentNode->NextPoint->NextPoint;

                        Q2.PreviousePoint = CurrentNode->NextPoint;
                        P2.PreviousePoint = CurrentNode->NextPoint->NextPoint;
                        P2.NextPoint = TempPoints[c]->NextPoint;
                        Q1.NextPoint = TempPoints[c]->PreviousePoint;
                        Q1.PreviousePoint = CurrentNode;
                        P1.NextPoint = TempPoints[c];

                        *TempPoints[c]->NextPoint = Q2;
                        *CurrentNode->NextPoint = P2;
                        *TempPoints[c] = Q1;
                        *CurrentNode = P1;
                        while (ChangePoint != CurrentNode->NextPoint)
                        {
                            Point* hold = ChangePoint->NextPoint;
                            ChangePoint->NextPoint = ChangePoint->PreviousePoint;
                            ChangePoint->PreviousePoint = hold;
                            ChangePoint = hold;
                        }
                        //std::this_thread::sleep_for(std::chrono::seconds(5));
                        c = TempPoints.size();
                        i -= 1;
                    }
                }
            }

            for (int c = 0; c < NumThreads; c++)
            {
                CurrentNode = CurrentNode->NextPoint;
            }
            i += NumThreads;
        }
    }
    
}
bool CppTSPalgorithm::OverlapCheck(Point* P1, Point* P2, Point* Q1, Point* Q2)
{
    VectorClass Tvector(Q1->X - P1->X, Q1->Y - P1->Y);
    //VectorClass Uvector(P1->X - Q1->X, P1->Y - Q1->Y);
    VectorClass Rvector(P2->X - P1->X, P2->Y - P1->Y);
    VectorClass Svector(Q2->X - Q1->X, Q2->Y - Q1->Y);
    double RxS = Crossproduct(&Rvector, &Svector);
    if (RxS != 0)
    {
        double t = Crossproduct(&Tvector, &Svector) / RxS;
        double u = Crossproduct(&Tvector, &Rvector) / RxS;
        if (t <= 1 && t >= 0 && u <= 1 && u >= 0)
            return true;
    }
    return false;
}
double CppTSPalgorithm::Crossproduct(VectorClass* P1, VectorClass* Q1 )
{
    return  P1->X * Q1->Y - P1->Y * Q1->X;
}
bool CppTSPalgorithm::Opt2(Point* P1, Point* P2, Point* Q1, Point* Q2)
{
    double Change1 = std::sqrt(std::pow(P2->X - Q1->X, 2) + std::pow(P2->Y - Q1->Y, 2)) + std::sqrt(std::pow(P1->X - Q2->X, 2) + std::pow(P1->Y - Q2->Y, 2));
    double Change2 = std::sqrt(std::pow(P2->X - Q2->X, 2) + std::pow(P2->Y - Q2->Y, 2)) + std::sqrt(std::pow(P1->X - Q1->X, 2) + std::pow(P1->Y - Q1->Y, 2));
    if (Change2 < Change1)
        return true;
    else
        return false;
}

