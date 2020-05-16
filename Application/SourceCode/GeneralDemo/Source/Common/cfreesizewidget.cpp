/*******************************************************************************
*�ļ�����:  cfreesizewidget.cpp
*��    ��:  �ṩ���ݿؼ���С����Ҫ���챳��ͼ�Ĺ���
*��    ��:  
*��������:  
*��ǰ�汾:  
*�汾��ʷ:  
*
*ע    ��:  ������ͼ���Ź�����Ƭ, �ı�ؼ���Сʱ, �Ľǵ�ͼƬ����, �������������ƽ�̻�����
*
*******************************************************************************/

/***********************************ͷ�ļ�**************************************/
#include "cfreesizewidget.h"
#include <QDebug>

const int DRAW_TILED_MODE = 0;          //ƽ��ģʽ
const int DRAW_STRETCHED_MODE = 1;      //����ģʽ

/*******************************************************************************
* ��  ��: CFreeSizeWidget::CFreeSizeWidget(QWidget *parent)
*
* ��  ��: ���캯��
*
* ��  ��: ����:
*           QWidget *parent ��ָ��
*
* ����ֵ: ��
*
* ע  ��:
*
*******************************************************************************/
CFreeSizeWidget::CFreeSizeWidget(QWidget *parent) :
    QWidget(parent)
    , m_FillMode(DRAW_STRETCHED_MODE)
    , m_isRefresh(true)
    , m_minimumWidth(0)
    , m_minimumHeight(0)
{
    m_conerSize[0] = 0;
    m_conerSize[1] = 0;
    m_conerSize[2] = 0;
    m_conerSize[3] = 0;
}

/*******************************************************************************
* ��  ��: CFreeSizeWidget::~CFreeSizeWidget()
*
* ��  ��: ��������
*
* ��  ��: ��
*
* ����ֵ: ��
*
* ע  ��:
*
*******************************************************************************/
CFreeSizeWidget::~CFreeSizeWidget()
{

}

/*******************************************************************************
* ��  ��: CFreeSizeWidget::setConerWidthHeight(int widthLeft, int widthRight,
*                                             int heightTop, int heightBottom)
*
* ��  ��: �����Ľǵķ�Χ
*
* ��  ��: ����:
*            int widthLeft          ��߿�
*            int widthRight         �ұ߿�
*            int heightTop          ���߸�
*            int heightBottom       �ױ߸�
*
* ����ֵ: ��
*
* ע  ��:
*
*******************************************************************************/
void CFreeSizeWidget::setConerWidthHeight(int widthLeft, int widthRight,
                                          int heightTop, int heightBottom)
{
    m_conerSize[0] = widthLeft;
    m_conerSize[1] = widthRight;
    m_conerSize[2] = heightTop;
    m_conerSize[3] = heightBottom;

    createEdgePixmap(m_backgroundImage, m_edgeTopImage
                     , m_edgeBottomImage, m_edgeLeftImage
                     , m_edgeRightImage, m_paddingImage);

    m_minimumWidth = m_conerSize[0] + m_conerSize[1];
    m_minimumHeight = m_conerSize[2] + m_conerSize[3];

    this->setMinimumWidth(m_minimumWidth);
    this->setMinimumHeight(m_minimumHeight);
}

/*******************************************************************************
* ��  ��: CFreeSizeWidget::setPixmap(const QPixmap &pixmap)
*
* ��  ��: ���ñ���ͼƬ
*
* ��  ��: ����:
*           const QPixmap &pixmap ����ͼƬ
*
* ����ֵ: ��
*
* ע  ��:
*
*******************************************************************************/
void CFreeSizeWidget::setPixmap(const QPixmap &pixmap)
{
    //    if (С��coner�Ĵ�С)
    //    {
    //        //...
    //    }
    m_backgroundImage = pixmap;

    if (m_conerSize[0] == 0 && m_conerSize[1] == 0 &&
        m_conerSize[2] == 0 && m_conerSize[3] == 0)
    {
        m_conerSize[0] = m_backgroundImage.width() / 2;
        m_conerSize[1] = m_backgroundImage.width() / 2;
        m_conerSize[2] = m_backgroundImage.height() / 2;
        m_conerSize[3] = m_backgroundImage.height() / 2;
    }

    createEdgePixmap(m_backgroundImage, m_edgeTopImage
                     , m_edgeBottomImage, m_edgeLeftImage
                     , m_edgeRightImage, m_paddingImage);

    m_isRefresh = true;
    update();
}

/*******************************************************************************
* ��  ��: CFreeSizeWidget::setFillMode(int mode)
*
* ��  ��: �������ģʽ
*
* ��  ��: ����:
*           int mode ���ģʽ
*
* ����ֵ: ��
*
* ע  ��: mode����: 0: ƽ��; 1: ����
*
******************************************************************************/
void CFreeSizeWidget::setFillMode(int mode)
{
    m_FillMode = mode;

    m_isRefresh = true;
    update();
}

void CFreeSizeWidget::setMinimumWidth(int minw)
{
    if (minw > m_minimumWidth)
    {
        QWidget::setMinimumWidth(minw);
    }
}

void CFreeSizeWidget::setMinimumHeight(int minh)
{
    if (minh > m_minimumHeight)
    {
        QWidget::setMinimumHeight(minh);
    }
}

void CFreeSizeWidget::setMinimumSize(int minw, int minh)
{
    int l_minw = minw;
    int l_minh = minh;

    if (m_minimumWidth < minw)
    {
        l_minw = m_minimumWidth;
    }

    if (m_minimumHeight < minh)
    {
        l_minh = m_minimumHeight;
    }

    QWidget::setMinimumSize(l_minw, l_minh);
}

void CFreeSizeWidget::setMinimumSize(const QSize &size)
{
    QSize l_size(size);

    if (m_minimumWidth < size.width())
    {
        l_size.setWidth(m_minimumWidth);
    }

    if (m_minimumHeight < size.height())
    {
        l_size.setHeight(m_minimumHeight);
    }

    QWidget::setMinimumSize(l_size);
}


/*******************************************************************************
* ��  ��: CFreeSizeWidget::createEdgePixmap()
*
* ��  ��: ������������Ҫ����Ƭ
*
* ��  ��: ����: ��
*
* ����ֵ: ��
*
* ע  ��:
*
*******************************************************************************/
void CFreeSizeWidget::createEdgePixmap(const QPixmap &background, QPixmap &edgeTop
                                       , QPixmap &edgeBottom, QPixmap &edgeLeft
                                       , QPixmap &edgeRight, QPixmap &padding)
{
    //���߷�Χ
    QRect l_edgeTopRect(m_conerSize[0], 0, background.width()
            - (m_conerSize[0] + m_conerSize[1]), m_conerSize[2]);

    //�ױ߷�Χ
    QRect l_edgeBottomRect(m_conerSize[0], background.height()
            - m_conerSize[3], background.width() - (m_conerSize[0]
            + m_conerSize[1]), m_conerSize[3]);

    //��߷�Χ
    QRect l_edgeLeftRect(0, m_conerSize[2], m_conerSize[0],
            background.height() - (m_conerSize[2] + m_conerSize[3]));

    //�ұ߷�Χ
    QRect l_edgeRightRect(background.width() - m_conerSize[1],
            m_conerSize[2], m_conerSize[1], background.height()
                    - (m_conerSize[2] + m_conerSize[3]));

    //�м�����Χ
    QRect l_paddingRect(m_conerSize[0], m_conerSize[2],
            background.width() - (m_conerSize[0] + m_conerSize[1]),
            background.height() - (m_conerSize[2] + m_conerSize[3]));

    edgeTop = background.copy(l_edgeTopRect);
    edgeBottom = background.copy(l_edgeBottomRect);
    edgeLeft = background.copy(l_edgeLeftRect);
    edgeRight = background.copy(l_edgeRightRect);
    padding = background.copy(l_paddingRect);
}

/*******************************************************************************
* ��  ��: CFreeSizeWidget::drawBufferPixmap(const QPixmap &background
*                                          , const QPixmap &edgeTop
*                                          , const QPixmap &edgeBottom
*                                          , const QPixmap &edgeLeft
*                                          , const QPixmap &edgeRight
*                                          , const QPixmap &padding
*                                          , QPixmap &buffer
*                                          , int fillMode)
*
* ��  ��: ������������Ҫ����Ƭ
*
* ��  ��: ����:
*           const QPixmap &background       ԭʼ����ͼƬ
*           const QPixmap &edgeTop          ����ͼƬ
*           const QPixmap &edgeBottom       �ױ�ͼƬ
*           const QPixmap &edgeLeft         ���ͼƬ
*           const QPixmap &edgeRight        �ұ�ͼƬ
*           const QPixmap &padding          �м�����ͼƬ
*           QPixmap &buffer                 ���ձ���ͼƬ
*           int fillMode                    ���ģʽ
*
* ����ֵ: ��
*
* ע  ��:
*
*******************************************************************************/
void CFreeSizeWidget::drawBufferPixmap(const QPixmap &background
                                       , const QPixmap &edgeTop
                                       , const QPixmap &edgeBottom
                                       , const QPixmap &edgeLeft
                                       , const QPixmap &edgeRight
                                       , const QPixmap &padding
                                       , QPixmap &buffer, int fillMode)
{
    QPainter l_painter(&buffer);

    l_painter.setRenderHint(QPainter::SmoothPixmapTransform);

    //�����Ľ�
    //top left coner
    l_painter.drawPixmap(0, 0, m_conerSize[0], m_conerSize[2],
            background, 0, 0, m_conerSize[0], m_conerSize[2]);
    //top right coner
    l_painter.drawPixmap(width() - m_conerSize[1], 0, m_conerSize[1],
            m_conerSize[2], background, background.width()
                    - m_conerSize[1], 0, m_conerSize[1], m_conerSize[2]);
    //bottom left coner
    l_painter.drawPixmap(0, height() - m_conerSize[3], m_conerSize[0],
            m_conerSize[3], background, 0, background.height()
                    - m_conerSize[3], m_conerSize[0], m_conerSize[3]);
    //bottom right coner
    l_painter.drawPixmap(width() - m_conerSize[1], height() - m_conerSize[3],
            m_conerSize[1], m_conerSize[3], background,
            background.width() - m_conerSize[1],
            background.height() - m_conerSize[3], m_conerSize[1],
            m_conerSize[3]);

    //�����ıߺ��м�����
    QRect l_edgeTopRect(m_conerSize[0], 0, buffer.width() - (m_conerSize[0]
            + m_conerSize[1]), m_conerSize[2]);

    QRect l_edgeBottomRect(m_conerSize[0], buffer.height() - m_conerSize[3],
            buffer.width() - (m_conerSize[0] + m_conerSize[1]), m_conerSize[3]);

    QRect l_edgeLeftRect(0, m_conerSize[2], m_conerSize[0], buffer.height()
            - (m_conerSize[2] + m_conerSize[3]));

    QRect l_edgeRightRect(buffer.width() - m_conerSize[1], m_conerSize[2],
            m_conerSize[1], buffer.height() - (m_conerSize[2] + m_conerSize[3]));

    QRect l_paddingRect(m_conerSize[0], m_conerSize[2], buffer.width()
            - (m_conerSize[0] + m_conerSize[1]), buffer.height()
            - (m_conerSize[2] + m_conerSize[3]));


    if (fillMode == DRAW_TILED_MODE) //ƽ��ģʽ����
    {
        l_painter.drawTiledPixmap(l_edgeTopRect, edgeTop);
        l_painter.drawTiledPixmap(l_edgeBottomRect, edgeBottom);
        l_painter.drawTiledPixmap(l_edgeLeftRect, edgeLeft);
        l_painter.drawTiledPixmap(l_edgeRightRect, edgeRight);
        l_painter.drawTiledPixmap(l_paddingRect, padding);
    }
    else //����ģʽ
    {
        l_painter.drawPixmap(l_edgeTopRect, edgeTop);
        l_painter.drawPixmap(l_edgeBottomRect, edgeBottom);
        l_painter.drawPixmap(l_edgeLeftRect, edgeLeft);
        l_painter.drawPixmap(l_edgeRightRect, edgeRight);
        l_painter.drawPixmap(l_paddingRect, padding);
    }

}

void CFreeSizeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter l_painter(this);

    if (m_buffer.size() != size() || m_isRefresh == true)
    {
        m_isRefresh = false;
        m_buffer = QPixmap(this->size());
        m_buffer.fill(Qt::transparent);
        drawBufferPixmap(m_backgroundImage, m_edgeTopImage
                         , m_edgeBottomImage, m_edgeLeftImage
                         , m_edgeRightImage, m_paddingImage
                         , m_buffer, m_FillMode);
    }

    l_painter.drawPixmap(0, 0, m_buffer);
}
