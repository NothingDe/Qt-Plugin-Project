/*******************************************************************************
*�ļ�����:  cfreesizewidget.h
*��    ��:  �ṩ���ݿؼ���С����Ҫ���챳��ͼ�Ĺ���
*��    ��:  
*��������:  
*��ǰ�汾:  
*�汾��ʷ: 
*          
*ע    ��:  ������ͼ���Ź�����Ƭ, �ı�ؼ���Сʱ, �Ľǵ�ͼƬ����, �������������ƽ�̻�����
*
*******************************************************************************/
#ifndef CFREESIZEWIDGET_H
#define CFREESIZEWIDGET_H

/**********************************ͷ�ļ�***************************************/
#include <QtWidgets/QWidget>
#include <QPainter>

/**********************************������***************************************/
class CFreeSizeWidget : public QWidget
{
    Q_OBJECT

public:
    CFreeSizeWidget(QWidget *parent = 0);
    ~CFreeSizeWidget();

    void setConerWidthHeight(int widthLeft, int widthRight,
                             int heightTop, int heightBottom);//�����Ľǵķ�Χ
    void setPixmap(const QPixmap &pixmap);                    //���ñ���ͼƬ
    void setFillMode(int mode);                               //�������ģʽ
    void setMinimumWidth(int minw);
    void setMinimumHeight(int minh);
    void setMinimumSize(int minw, int minh);
    void setMinimumSize(const QSize &);

protected:
    //������������Ҫ����Ƭ
    void createEdgePixmap(const QPixmap &background, QPixmap &edgeTop
                          , QPixmap &edgeBottom, QPixmap &edgeLeft
                          , QPixmap &edgeRight, QPixmap &padding);

    //���Ʊ���ͼƬ
    void drawBufferPixmap(const QPixmap &background, const QPixmap &edgeTop
                          , const QPixmap &edgeBottom, const QPixmap &edgeLeft
                          , const QPixmap &edgeRight, const QPixmap &padding
                          , QPixmap &buffer, int fillMode = 1);
    void paintEvent(QPaintEvent *event);

//private:
protected:
    QPixmap m_buffer;           //���ձ���ͼƬ

    QPixmap m_backgroundImage;  //ԭʼ����ͼ
    QPixmap m_edgeTopImage;     //����ͼƬ
    QPixmap m_edgeBottomImage;  //�ױ�ͼƬ
    QPixmap m_edgeLeftImage;    //���ͼƬ
    QPixmap m_edgeRightImage;   //�ұ�ͼƬ
    QPixmap m_paddingImage;     //�м�����ͼƬ

    bool m_isRefresh;           //�Ƿ���ˢ��


    int m_conerSize[4];         //�ĽǵĴ�С
    int m_FillMode;             //���ģʽ

    int m_minimumWidth;
    int m_minimumHeight;
};

#endif // CFREESIZEWIDGET_H
