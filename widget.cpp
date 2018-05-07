#include "widget.h"
#include <QDesktopServices>
#include <QMessageBox>
#include <QDebug>
#define FRAME_SIZE 30

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    m_bROIFlag(false),
    m_bMoveFlag(false),
    m_iChangeSizeFlag(0)
{
    this->setWindowOpacity(0.5);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize(640+FRAME_SIZE * 2,512+FRAME_SIZE * 2);//+60
    ptTopLeft = QPoint(50,50);
    ptBottomRight = QPoint(370,306);
    updateRectsInfo();
    recPath.addRect(recTopLeft);
    recPath.addRect(recTopRight);
    recPath.addRect(recBottomLeft);
    recPath.addRect(recBottomRight);
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
            m_bMoveFlag = true;
        }else{
            m_bMoveFlag = false;
            m_iChangeSizeFlag = 0;
        }
    }
}

void Widget::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton){
        endPoint = event->pos();
        update(FRAME_SIZE,FRAME_SIZE,640,512);
        event->accept();
        m_bMoveFlag = false;
        m_iChangeSizeFlag = 0;
        qDebug()<<"release";
        updateRectsInfo();
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

    if (event->buttons() == Qt::LeftButton){
        QPoint pt = event->pos();
        QRect tempRec = QRect(ptTopLeft,ptBottomRight);
        qDebug()<<"size"<<tempRec;
        if(m_iChangeSizeFlag != 0){
            //limit the size!!!!
            switch(m_iChangeSizeFlag){
            case 1:
                ptTopLeft = pt;//TopLeft
                break;
            case 2:
                ptTopLeft.setY(pt.y());//TopRight
                ptBottomRight.setX(pt.x());
                break;
            case 3:
                ptTopLeft.setX(pt.x());//bottomleft
                ptBottomRight.setY(pt.y());
                break;
            case 4:
                ptBottomRight = pt;//bottomright
                break;
            default:
                break;
            }
            update(FRAME_SIZE,FRAME_SIZE,640,512);
            qDebug()<<"change size";
        }else if(m_bMoveFlag){
            endPoint = event->pos();
            ptTopLeft += endPoint-lastPoint;
            ptBottomRight += endPoint-lastPoint;
            qDebug()<<"point:"<<ptTopLeft;
            lastPoint = endPoint;
            update(FRAME_SIZE,FRAME_SIZE,640,512);
            event->accept();
        }
    }
}

void Widget::paintEvent(QPaintEvent *){
    QPainter p(this);

    p.fillRect(rect(), QColor(255,255,255,150));
    p.drawPixmap(FRAME_SIZE,FRAME_SIZE,640,512,m_curPic);
    if(m_bROIFlag){
        p.setPen(QPen(QColor("#ff0000"), 1, Qt::SolidLine));
        recPath.translate(endPoint-lastPoint);
        p.drawRect(QRect(ptTopLeft,ptBottomRight));
    }
    drawXLine();
    drawYLine();

    int iwidth=this->width();
    int iheight=this->height();

    QPainter p2(this);
    p2.setPen(QPen(QColor("#ddd"), 1, Qt::SolidLine));
    p2.drawRect(0,0,iwidth-1,iheight-1);

//    QString showText="W:"+QString::number(iwidth)+"px\r\nH:"+QString::number(iheight)+"px";

//    p2.setPen(QPen(QColor("#940080"), 1, Qt::SolidLine));
//    p2.setFont(QFont("Arial", 10));
//    p2.drawText(this->rect(),  Qt::AlignCenter,showText);
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

void Widget::createContentMenu(){
    QAction *Tray_quit = new QAction("Quit", this);
    //Tray_quit->setIcon(QIcon(":/image/image/delete.png"));
    connect(Tray_quit, SIGNAL(triggered(bool)), this, SLOT(slotQuit()));

    QAction *Tray_homepage = new QAction("Homepage", this);
    connect(Tray_homepage, SIGNAL(triggered(bool)), this, SLOT(slotHomepage()));

    QAction *Tray_changelog = new QAction("Changelog", this);
    connect(Tray_changelog, SIGNAL(triggered(bool)), this, SLOT(slotChangelog()));

    QAction *menuAbout = new QAction("About", this);
    connect(menuAbout, SIGNAL(triggered(bool)), this, SLOT(soltAbout()));

    contentMenu = new QMenu(this);
    contentMenu->addAction(Tray_homepage);
    contentMenu->addAction(Tray_changelog);
    contentMenu->addAction(menuAbout);
    contentMenu->addSeparator();
    contentMenu->addAction(Tray_quit);
}

void Widget::updateRectsInfo()
{
    recTopLeft = QRect(QPoint(ptTopLeft.x()-2,ptTopLeft.y()-2),QSize(4,4));
    recTopRight =QRect(QPoint(ptBottomRight.x()-2,ptTopLeft.y()-2),QSize(4,4));
    recBottomLeft = QRect(QPoint(ptTopLeft.x()-2,ptBottomRight.y()-2),QSize(4,4));
    recBottomRight = QRect(QPoint(ptBottomRight.x()-2,ptBottomRight.y()-2),QSize(4,4));
}

void Widget::slotQuit(){
    this->close();
   // QApplication::quit();
}

void Widget::slotHomepage(){
    QDesktopServices::openUrl(QUrl("https://github.com/Awesomez-Qt/IRuler"));
}

void Widget::slotChangelog(){
    QDesktopServices::openUrl(QUrl("https://github.com/Awesomez-Qt/IRuler/commits/master"));
}

void Widget::soltAbout(){
    QMessageBox::information(this,"About","IRuler by Awesomez V1.2.20130814",QMessageBox::Yes);
}

void Widget::selectROI()
{
    m_bROIFlag = !m_bROIFlag;
    qDebug()<<"selectROI****"<<m_bROIFlag;
    update(FRAME_SIZE,FRAME_SIZE,640,512);
}
