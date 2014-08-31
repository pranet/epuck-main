#ifndef ALGOWORKER_H
#define ALGOWORKER_H

#include <QThread>
#include <QObject>
#include "serial.h"
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <QMutex>
#include <QPixmap>
#include "camcapture.h"
#include "featuredetection.h"
#include "commondefs.h"


//non simulation values
//#define ANGLE_THRESHOLD_COARSE 7.0*CV_PI/180.0
//#define DISTANCE_THRESHOLD_COARSE 100.0
//#define ANGLE_THRESHOLD_FINE 20.0*CV_PI/180.0
//#define DISTANCE_THRESHOLD_FINE 8.0
//#define FINAL_REACHED_THRESHOLD 20.0

//simulation values
#define ANGLE_THRESHOLD_COARSE 7.0*CV_PI/180.0
#define DISTANCE_THRESHOLD_COARSE 10.0
#define ANGLE_THRESHOLD_FINE 3.0*CV_PI/180.0
#define DISTANCE_THRESHOLD_FINE 8.0
#define FINAL_REACHED_THRESHOLD 18.0

enum AlgoState {SAVE_CURRENT_POSITION, MOVE_TO_SAVED_POSITION,GETTING_LINE_POINTS, MOVING_TO_LINE, ORIENTING, POSITIONING, MAKE_CIRCLE, MOVE_TO_CIRCLE, POSITIONING_ON_CIRCLE_1, POSITIONING_ON_CIRCLE_2, FINISHED};
enum Algorithm {PROBABILISTIC_0, PROBABILISTIC_1, PROBABILISTIC_HALF, DINING_PHILOSOPHER, NEW_ALGO};
enum MoveDirection {FORWARD, BACKWARD, LEFT, RIGHT, STOP};
class AlgoWorker : public QObject
{
    Q_OBJECT
public:
    AlgoWorker(QThread* _myThread, BeliefState **_bs, QMutex* _bsMutex);
public slots:
    void process();
    void onTimeout();
    void onStop();
    void onStopAlgo();
signals:
    void gotLine(int x1, int y1, int x2, int y2);
    void printDestination(PointList p);
    void printCircle(Circle c);
    void finished();
    void error(QString err);


private:
    HAL::Serial s[NUMBOTS];
    QTimer *timer;
    QMutex* bsMutex;
    QThread* myThread;
    BeliefState** bs;
    double getDistance(CvPoint a, CvPoint b);
    BeliefState localBS;
    AlgoState currentState;
    Algorithm currentAlgo;
    int line_x1, line_x2, line_y1, line_y2;
    void moveLeft(int n, int speed = 150);
    void moveRight(int n, int speed = 150);
    void moveForward(int n, int speed = 500);
    void moveBack(int n, int speed = 500);
    void moveStop(int n);
    void moveStopAll();
    void turnLedOn(int n);
    void turnLedOff(int n);
    CvPoint linePoint[2];
    CvPoint destinationPoint[NUMBOTS];
    CvPoint savedPosition[NUMBOTS];
    Circle destinationCircle;
    int robotActive;
    bool moveToPoint(CvPoint p, Bot bot, int n);
    bool moveToPointOpti(CvPoint p, Bot bot, int n);
    bool turnToPoint(CvPoint p, Bot bot, int n);
    int frameCounter;
    bool isRunning;
    void nextIteration();
    bool isLeftMost(int i);
    bool isRightMost(int i);
    bool isBotMoving[NUMBOTS];
    MoveDirection lastMove[NUMBOTS];
    int numBotMoving;
    int numActivations;
    int numRounds;
    double totDistance;
    void getLinePoints();
    CvPoint getPerpendicularPoint(int n);
    CvPoint getPerpendicularPoint(CvPoint p);
    CvPoint getMidPoint(CvPoint p1, CvPoint p2);
    int getCCWNeighbour(int n);
    int getCWNeighbour(int n);
    CvPoint getPerpPointOnCircle(int n);
    CvPoint getPerpPointOnCircle(CvPoint p, bool closer = true);
    CvPoint getPointToMoveAlgo1(int n);
    CvPoint getPerpPointOnCircleCloserToBot(CvPoint p, int n);
    CvPoint getMidOfNeighbours(int n);
    CvPoint getPointNewAlgo(int n);
    double pointToAngle(CvPoint p);
    double pointToAngle(int n);
    CvPoint angleToPoint(double angle);
    bool isAngleReachable(int n, double angle);
    bool isOneStableCCW(int n);
    bool isOneStableCW(int n);
    CvPoint getClosest(CvPoint p1, CvPoint p2, int n);
    void getNeighbourIndicesCircle(int n, int* indexList);
};

#endif // ALGOWORKER_H
