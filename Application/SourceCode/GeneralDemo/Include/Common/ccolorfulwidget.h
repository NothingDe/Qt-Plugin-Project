/*******************************************************************************
*�ļ�����:  ccolorfulwidget.h
*��    ��:  �ṩ�������ܵĻ���, �ڱ�д��Ҫ�������ܵĿؼ�ʱ, �̳д���, ����QWidgetʹ��.
*��    ��:  
*��������:  
*��ǰ�汾:  
*�汾��ʷ:  
*
*ע    ��:
*
*******************************************************************************/
#ifndef CCOLORFULWIDGET_H
#define CCOLORFULWIDGET_H

/**********************************ͷ�ļ�***************************************/
#include <QtWidgets/QWidget>
#include <QVariant>
#include <QList>
#include <QTimeLine>
#include "csystemsetting.h"
#include "cfreesizewidget.h"

/**********************************������***************************************/
class CColorfulWidget : public CFreeSizeWidget
{
    Q_OBJECT

public:
    CColorfulWidget(QWidget *parent = 0);
    virtual ~CColorfulWidget();

    virtual void setSkin(int type, const QVariant &value);  //����Ƥ��
    virtual void setSkinBG(const QPixmap &pix);
    virtual void setBackGroundColor(const QColor color);    //����Ƥ����ɫ
    virtual void setBackGroundImage(const QPixmap &pix);    //����Ƥ��ͼƬ
    virtual void setBackGroundType(int type);               //���õ�ǰ��������(��ɫ����ͼƬ)

    virtual void setConerWidthHeight(int widthLeft, int widthRight,
                             int heightTop, int heightBottom);//�����Ľǵķ�Χ
    virtual void setPixmap(const QPixmap &pixmap);                    //���ñ���ͼƬ

protected:
    void paintEvent(QPaintEvent *event);        //�����¼�

public:
    enum BG_TYPE{EMPTY = -1, COLOR, PIXMAP};

protected:
    void showEvent(QShowEvent *);
    QPixmap m_maskPixmap;           //�����ɰ�ͼƬ
    QPixmap m_buffer;               //����ͼƬ

    QPixmap m_edgeTopImage;         //����ͼƬ
    QPixmap m_edgeBottomImage;      //�ױ�ͼƬ
    QPixmap m_edgeLeftImage;        //���ͼƬ
    QPixmap m_edgeRightImage;       //�ұ�ͼƬ
    QPixmap m_paddingImage;         //�м�����ͼƬ

    BG_TYPE m_BGType;               //��������(��ɫ, ͼƬ, Ĭ��Ϊ�ޱ���)
    QColor m_backgroundColor;       //����ɫ��ɫֵ
    QPixmap m_backgroundPixmap;     //����ͼƬ
    QPixmap m_backgroundOriginally; //�󱳾�ͼ
    QPixmap m_backgroundSkinMode;
    bool m_isRefresh;               //�Ƿ���ˢ��

public:
    static void changeAllSkin();

private slots:
    void animation(qreal);
    void slotFinished();
private:
    static QList<QWidget*>  *mp_widgets;
    QTimeLine *m_timeLine;
    qreal m_opacity;
};

#endif // CCOLORFULWIDGET_H
