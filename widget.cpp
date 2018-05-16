#include "widget.h"
#include <QDesktopServices>
#include <QMessageBox>
#include <QDebug>
#define FRAME_SIZE 32

Widget::Widget(CameraCtrl* camCtrl, QWidget *parent) :
    QWidget(parent),
    m_bROIFlag(false),
    m_iChangeSizeFlag(-1),
    m_camCtrl(camCtrl)
{
    this->setWindowOpacity(0.5);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(640+FRAME_SIZE * 2,512+FRAME_SIZE * 2);//+60
   // createContentMenu();
}

Widget::~Widget()
{

}

void Widget::setPicture(QPixmap &pic)
{
    m_curPic = pic;
}

void Widget::mousePressEvent(QMouseEvent * event){  
    if (event->button() == Qt::LeftButton){
        QRect rec(ptTopLeft,ptBottomRight);
        lastPoint = event->pos();
        if(recTopLeft.contains(lastPoint)){
            m_iChangeSizeFlag = 1;
            qDebug()<<"change size";
        }else if(recTopRight.contains(lastPoint)){
            m_iChangeSizeFlag = 2;
        }else if(recBottomLeft.contains(lastPoint)){
            m_iChangeSizeFlag = 3;
        }else if(recBottomRight.contains(lastPoint)){
            m_iChangeSizeFlag = 4;
        }else if(rec.contains(lastPoint)){
            m_iChangeSizeFlag = 0;
        }else{
            m_iChangeSizeFlag = -1;
        }
    }
}

void Widget::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton &&
            (m_iChangeSizeFlag != -1)){
        endPoint = event->pos();
        ptTopLeft.setX((ptTopLeft.x() + 8)/16 * 16);
        ptTopLeft.setY((ptTopLeft.y() + 1)/2 * 2);
        int w = ptBottomRight.x() - ptTopLeft.x();
        int h = ptBottomRight.y() - ptTopLeft.y();
        w = (w + 8)/16 * 16;
        h = (h + 1)/2*2;
        ptBottomRight.setX(ptTopLeft.x()+w);
        ptBottomRight.setY(ptTopLeft.y()+h);
        update(FRAME_SIZE,FRAME_SIZE,640,512);
        event->accept();
        m_iChangeSizeFlag = -1;
        qDebug()<<"release";
        updateRectsInfo();
        emit m_camCtrl->rectROIChanged(QRect(ptTopLeft,ptBottomRight),true);
      }
}

void Widget::mouseMoveEvent(QMouseEvent * event){
    if(!m_bROIFlag)
        return;

    if(recTopLeft.contains(event->pos()) ||
            recBottomRight.contains(event->pos())){
        this->setCursor(Qt::SizeFDiagCursor);
    }else if(recTopRight.contains(event->pos()) ||
             recBottomLeft.contains(event->pos())){
        this->setCursor(Qt::SizeBDiagCursor);
    }else{
        this->setCursor(Qt::ArrowCursor);
    }

    if (event->buttons() == Qt::LeftButton && m_iChangeSizeFlag != -1){
        endPoint = event->pos();
        QRect rectROI = QRect(ptTopLeft,ptBottomRight);
        QRect rect(FRAME_SIZE,FRAME_SIZE,640,512);
        switch(m_iChangeSizeFlag){
        case 0:
            if(ptTopLeft.x() < rect.x()){
                ptTopLeft.setX(rect.topLeft().x() + 1);
                ptBottomRight.setX(ptTopLeft.x()+rectROI.width()-1);
                            qDebug()<<"not 1";
            }else if(ptTopLeft.y() < rect.y()){
                ptTopLeft.setY(rect.topLeft().y() + 1);
                ptBottomRight.setY(ptTopLeft.y()+rectROI.height()-1);
            }else if(ptBottomRight.x() > rect.bottomRight().x()){
                            qDebug()<<"not 2";
                ptBottomRight.setX(rect.bottomRight().x() - 1);
                ptTopLeft.setX(ptBottomRight.x()-rectROI.width()+1);
                                    qDebug()<<"not 3";
            }else if(ptBottomRight.y() > rect.bottomRight().y()){
                ptBottomRight.setY(rect.bottomRight().y() - 1);
                ptTopLeft.setY(ptBottomRight.y()-rectROI.height()+1);
                                   qDebug()<<"not 4";
            }else{
                ptTopLeft = ptTopLeft + endPoint-lastPoint;
                ptBottomRight = ptBottomRight + endPoint-lastPoint;
            }
            qDebug()<<"point:"<<ptTopLeft;
            lastPoint = endPoint;
            qDebug()<<"move" << ptTopLeft;
            recPath.translate(endPoint-lastPoint);
            break;
        case 1:
            if(endPoint.x() >= rect.topLeft().x() && endPoint.x() <= ptBottomRight.x() - 160&&
                    endPoint.y() >= rect.topLeft().y() && endPoint.y() <= ptBottomRight.y()-128){
                ptTopLeft = endPoint;//TopLeft
            }
            break;
        case 2:
            if(endPoint.x() <= rect.topRight().x() && endPoint.x() >= ptTopLeft.x()+160&&
                    endPoint.y() >= rect.topRight().y()&&endPoint.y() <=  ptBottomRight.y()-128){
                ptTopLeft.setY(endPoint.y());//TopRight
                ptBottomRight.setX(endPoint.x());
            }
            break;
        case 3:
            if(endPoint.x() >= rect.bottomLeft().x() && endPoint.x() <= ptBottomRight.x()-160&&
                    endPoint.y() <= rect.bottomLeft().y() && endPoint.y() >= ptTopLeft.y()+128){
                ptTopLeft.setX(endPoint.x());//bottomleft
                ptBottomRight.setY(endPoint.y());
            }
            break;
        case 4:
            if(endPoint.x() <= rect.bottomRight().x() && endPoint.x() >= ptTopLeft.x()+160&&
                    endPoint.y() <= rect.bottomRight().y() && endPoint.y() >= ptTopLeft.y()+128){
                ptBottomRight = endPoint;//bottomright
            }
            break;
        default:
            break;
        }
        emit m_camCtrl->rectROIChanged(QRect(ptTopLeft,ptBottomRight),false);
        update(FRAME_SIZE,FRAME_SIZE,640,512);
        event->accept();
    }
}

void Widget::paintEvent(QPaintEvent *){
    QPainter p(this);

    p.fillRect(rect(), QColor(255,255,255,150));
    p.drawPixmap(FRAME_SIZE,FRAME_SIZE,640,512,m_curPic);
    if(m_bROIFlag){
        p.setPen(QPen(QColor("#ff0000"), 1, Qt::SolidLine));
        QRect rectROI = QRect(ptTopLeft,ptBottomRight);
        QRect rect(FRAME_SIZE,FRAME_SIZE,640,512);
        if (!rect.contains(rectROI)){
            qDebug()<<"not contains";

        }
        p.drawRect(QRect(ptTopLeft,ptBottomRight));
    }
    drawXLine();
    drawYLine();

    int iwidth=this->width();
    int iheight=this->height();

    QPainter p2(this);
    p2.setPen(QPen(QColor("#ddd"), 1, Qt::SolidLine));
    p2.drawRect(0,0,iwidth-1,iheight-1);
}

void Widget::drawXLine(){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setPen(QPen(QColor("#000"), 1, Qt::SolidLine));
    QString str;
    int lineHeight = 15;

    int mid = this->width() / 2;
    QFont font(QFont("Arial", 8, QFont::Bold));
    p.setFont(font);
    p.drawText(mid - 3, 0, 25, 12, Qt::AlignLeft, QString::number(0));
    p.drawLine(QPoint(mid, FRAME_SIZE), QPoint(mid, FRAME_SIZE-lineHeight));

    p.drawLine(QPoint(mid, this->height()-FRAME_SIZE), QPoint(mid, this->height()-FRAME_SIZE+lineHeight));
    for (int i = 0; mid - i > 0; i+=10) {
        if( i == 0)
            continue;
        if((mid - i) < FRAME_SIZE)
            break;

        if(i % 10 == 0){
            lineHeight = 10;
        }
        if (i % 50 == 0) {
            p.setFont(QFont("Arial", 8, QFont::Bold));
            str = QString::number(i);
            p.drawText( mid - i -3 * str.length(),
                        0, 25, 12, Qt::AlignLeft, str);
            p.drawText( mid + i -3 * str.length(),
                        0, 25, 12, Qt::AlignLeft, str);
            lineHeight = 15;
        }
        p.drawLine(QPoint(mid - i, FRAME_SIZE), QPoint(mid - i, FRAME_SIZE-lineHeight));
        p.drawLine(QPoint(mid + i, FRAME_SIZE), QPoint(mid + i, FRAME_SIZE-lineHeight));

        p.drawLine(QPoint(mid - i, this->height() - FRAME_SIZE), QPoint(mid - i, this->height() - FRAME_SIZE+lineHeight));
        p.drawLine(QPoint(mid + i, this->height() - FRAME_SIZE), QPoint(mid + i, this->height() - FRAME_SIZE+lineHeight));
    }
}

void Widget::drawYLine(){
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setPen(QPen(QColor("#000"), 1, Qt::SolidLine));

    int lineHeight = 15;
    QString str;
    int mid = this->height() / 2;
    QFont font(QFont("Arial", 8, QFont::Bold));
    p.setFont(font);
    p.drawText(0, mid - 7, 25, 12, Qt::AlignLeft, QString::number(0));
    p.drawLine(QPoint(FRAME_SIZE,  mid), QPoint(FRAME_SIZE-lineHeight, mid));

    p.drawLine(QPoint(this->width()-FRAME_SIZE,  mid), QPoint(this->width()-FRAME_SIZE+lineHeight, mid));
    for (int i = 0; mid - i > 0; i+=10) {
        if(i == 0)
            continue;
        if((mid - i) < FRAME_SIZE || i == 0)
            break;

        if(i % 10 == 0){
            lineHeight = 10;
        }

        if (i % 50 == 0 && i>0) {
            p.drawText(0, mid-i-7, 25, 12, Qt::AlignLeft, QString::number(i));
             p.drawText(0, mid+i-7, 25, 12, Qt::AlignLeft, QString::number(i));
            lineHeight = 15;
        }

        p.drawLine(QPoint(FRAME_SIZE,  mid - i), QPoint(FRAME_SIZE-lineHeight, mid - i));
        p.drawLine(QPoint(FRAME_SIZE,  mid + i), QPoint(FRAME_SIZE-lineHeight, mid + i));

        p.drawLine(QPoint(this->width()-FRAME_SIZE,  mid - i), QPoint(this->width()-FRAME_SIZE+lineHeight, mid - i));
        p.drawLine(QPoint(this->width()-FRAME_SIZE,  mid + i), QPoint(this->width()-FRAME_SIZE+lineHeight, mid + i));
    }
}



void Widget::updateRectsInfo()
{
    recTopLeft = QRect(QPoint(ptTopLeft.x()-2,ptTopLeft.y()-2),QSize(4,4));
    recTopRight =QRect(QPoint(ptBottomRight.x()-2,ptTopLeft.y()-2),QSize(4,4));
    recBottomLeft = QRect(QPoint(ptTopLeft.x()-2,ptBottomRight.y()-2),QSize(4,4));
    recBottomRight = QRect(QPoint(ptBottomRight.x()-2,ptBottomRight.y()-2),QSize(4,4));
}

void Widget::selectROI(QRect& rect)
{
    m_bROIFlag = !m_bROIFlag;
    if(m_bROIFlag){
        ptTopLeft = QPoint(50,50);
        ptBottomRight = QPoint(370,306);
        roiSize = QSize(320,256);
        updateRectsInfo();
        recPath.addRect(recTopLeft);
        recPath.addRect(recTopRight);
        recPath.addRect(recBottomLeft);
        recPath.addRect(recBottomRight);
        qDebug()<<"selectROI****"<<rect;
    }
    update(FRAME_SIZE,FRAME_SIZE,640,512);
}

void Widget::rectROIChanged(QRect& rect,bool modifyCam)
{
    ptTopLeft = rect.topLeft();
    ptBottomRight = rect.bottomRight();
    update(FRAME_SIZE,FRAME_SIZE,640,512);
    qDebug()<<"widget changed" << rect;
}
