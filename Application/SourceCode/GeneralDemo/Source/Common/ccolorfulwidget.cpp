/******************************************************************************
*�ļ�����:  ccolorfulwidget.cpp
*��    ��:  �ṩ�������ܵĻ���, �ڱ�д��Ҫ�������ܵĿؼ�ʱ, �̳д���, ����QWidgetʹ��.
*��    ��: 
*��������:  
*��ǰ�汾:  
*�汾��ʷ:  
*
*ע    ��:
*
*******************************************************************************/

/***********************************ͷ�ļ�*************************************/
#include "ccolorfulwidget.h"
#include <QPainter>
#include <QDebug>
QList<QWidget*>* CColorfulWidget::mp_widgets = NULL;

/*******************************************************************************
* ��  ��: CColorfulWidget::changeAllSkin()
*
* ��  ��: ֪ͨ������Ҫ�����Ľ����л�Ƥ��
*
* ��  ��: ��
*
* ����ֵ: ��
*
* ע  ��:
*
*******************************************************************************/
void CColorfulWidget::changeAllSkin()
{
    if(NULL != mp_widgets)
    {
        int l_type = 0;
        QVariant l_value;
        CSystemSetting::getInstance()->getSkinConfig(l_type, l_value);

        int count = mp_widgets->size();
        for(int i = 0; i < count; i++)
        {
            CColorfulWidget* p = static_cast<CColorfulWidget*>(mp_widgets->at(i));
            if(NULL != p)
            {
                p->setSkin(l_type, l_value);
            }
        }
    }
}

/******************************************************************************
* ��  ��: CColorfulWidget::CColorfulWidget(QWidget *parent)
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
******************************************************************************/
CColorfulWidget::CColorfulWidget(QWidget *parent) :
        CFreeSizeWidget(parent)
        , m_isRefresh(true)
        , m_timeLine(NULL)
        , m_opacity(1)
{
    if(NULL == mp_widgets)
    {
        mp_widgets = new QList<QWidget*>();
    }
    mp_widgets->append(this);

    m_BGType = (BG_TYPE)CSystemSetting::getInstance()->skinType;
    m_backgroundColor = CSystemSetting::getInstance()->skinColor;
    m_backgroundPixmap = QPixmap(CSystemSetting::getInstance()->SkinPixmap);

    if (m_BGType == PIXMAP)
    {
        if (m_backgroundPixmap.isNull())
        {
            m_BGType = EMPTY;
        }
    }
    m_timeLine = new QTimeLine(800,this);   //ʱ�����
    m_timeLine->setUpdateInterval(100);
    m_timeLine->setEasingCurve(QEasingCurve::OutQuart);
    connect(m_timeLine,SIGNAL(valueChanged(qreal)),SLOT(animation(qreal)));
    connect(m_timeLine,SIGNAL(finished()),SLOT(slotFinished()));
}

/******************************************************************************
* ��  ��: CColorfulWidget::~CColorfulWidget()
*
* ��  ��: ��������
*
* ��  ��: ��
*
* ����ֵ: ��
*
* ע  ��:
*
******************************************************************************/
CColorfulWidget::~CColorfulWidget()
{
    if (NULL != mp_widgets)
    {
//        qDebug() << "SIZE 2: " << mp_widgets->size();
        mp_widgets->removeOne(this);

        if(0 == mp_widgets->size())
        {
            delete mp_widgets;
            mp_widgets = NULL;
        }
    }
}

/******************************************************************************
* ��  ��: CColorfulWidget::setSkin(int type, const QVariant &value)
*
* ��  ��: ����Ƥ��
*
* ��  ��: ����:
*         int type              ��������
*         const QVariant &value ȡֵ(��ɫģʽΪ��ɫ, ͼƬģʽΪͼƬ·��)
*
* ����ֵ: ��
*
* ע  ��:
*
******************************************************************************/
void CColorfulWidget::setSkin(int type, const QVariant &value)
{
    if (type == COLOR)
    {
        setBackGroundColor(QColor(value.toString()));
    }
    else if (type == PIXMAP)
    {
        setBackGroundImage(QPixmap(value.toString()));
    }
    else if (type == EMPTY)
    {
        setBackGroundType(EMPTY);
    }
    if(m_timeLine->state() == QTimeLine::Running)
       m_timeLine->stop();

    m_timeLine->start();
}

void CColorfulWidget::animation(qreal value)
{
    m_opacity = value;
    repaint();
}

void CColorfulWidget::slotFinished()
{
    m_timeLine->stop();
}

void CColorfulWidget::setSkinBG(const QPixmap &pix)
{
    m_backgroundSkinMode = pix;

    if (m_BGType != EMPTY)
    {
        CFreeSizeWidget::setPixmap(m_backgroundSkinMode);
    }
}

/******************************************************************************
* ��  ��: CColorfulWidget::setBackGroundColor(const QColor color)
*
* ��  ��: ����Ƥ����ɫ
*
* ��  ��: ����:
*         const QColor color ����ɫ
*
* ����ֵ: ��
*
* ע  ��:
*
******************************************************************************/
void CColorfulWidget::setBackGroundColor(const QColor color)
{
    m_backgroundColor = color;
    setBackGroundType(COLOR);
}

/******************************************************************************
* ��  ��: CColorfulWidget::setBackGroundImage(const QPixmap &pix)
*
* ��  ��: ����Ƥ��ͼƬ
*
* ��  ��: ����:
*         const QPixmap &pix ����ͼƬ
*
* ����ֵ: ��
*
* ע  ��:
*
******************************************************************************/
void CColorfulWidget::setBackGroundImage(const QPixmap &pix)
{
    if (pix.isNull())
    {
        return;
    }

    m_backgroundPixmap = pix;
    setBackGroundType(PIXMAP);
}

/******************************************************************************
* ��  ��: CColorfulWidget::setBackGroundType(int type)
*
* ��  ��: �л���ǰ����ģʽ, ��ɫ����ͼƬ, �������Ҫ, Ҳ����������ֵ, ��������Ϊ�ޱ���ģʽ
*
* ��  ��: ����:
*         int type ��������
*
* ����ֵ: ��
*
* ע  ��: �����������: 0: ��ɫģʽ; 1: ͼƬģʽ; -1: �ޱ���ģʽ, ��ͬ��QWidget
*
******************************************************************************/
void CColorfulWidget::setBackGroundType(int type)
{
    if (type != m_BGType)
    {
        if (type == EMPTY)
        {
            CFreeSizeWidget::setPixmap(m_backgroundOriginally);
        }
        else
        {
            CFreeSizeWidget::setPixmap(m_backgroundSkinMode);
        }
    }

    switch (type)
    {
        case COLOR:
        case PIXMAP:
        {
            m_BGType = (BG_TYPE)type;
            break;
        }

        default:
        {
            m_BGType = EMPTY;
            break;
        }
    }

    m_isRefresh = true;
    //update();
}

/*******************************************************************************
* ��  ��: CColorfulWidget::setConerWidthHeight(int widthLeft, int widthRight,
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
void CColorfulWidget::setConerWidthHeight(int widthLeft, int widthRight,
                                          int heightTop, int heightBottom)
{
    CFreeSizeWidget::setConerWidthHeight(widthLeft, widthRight,
                                         heightTop, heightBottom);

    createEdgePixmap(m_backgroundOriginally, m_edgeTopImage
                     , m_edgeBottomImage, m_edgeLeftImage
                     , m_edgeRightImage, m_paddingImage);
}

/*******************************************************************************
* ��  ��: CColorfulWidget::setPixmap(const QPixmap &pixmap)
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
void CColorfulWidget::setPixmap(const QPixmap &pixmap)
{
    m_backgroundOriginally = pixmap;

    if (m_BGType == EMPTY)
    {
        CFreeSizeWidget::setPixmap(m_backgroundOriginally);
    }

//    QImage l_tmpImage = m_backgroundOriginally.toImage();
//    l_tmpImage = l_tmpImage.createAlphaMask()
//                        .convertToFormat(QImage::Format_ARGB32_Premultiplied);

//    l_tmpImage.invertPixels(QImage::InvertRgb);
//    l_tmpImage.setAlphaChannel(l_tmpImage);

//    m_maskPixmap = QPixmap::fromImage(l_tmpImage);
    m_maskPixmap = m_backgroundOriginally;

    createEdgePixmap(m_maskPixmap, m_edgeTopImage
                     , m_edgeBottomImage, m_edgeLeftImage
                     , m_edgeRightImage, m_paddingImage);
}

/******************************************************************************
* ��  ��: CColorfulWidget::paintEvent(QPaintEvent *event)
*
* ��  ��: �����¼�
*
* ��  ��: ����:
*           QPaintEvent *event
*
* ����ֵ: ��
*
* ע  ��: �̳д����, �����Ҫ����paintEvent, ���������ʱ��ִ�и����paintEvent.
*
*
******************************************************************************/
void CColorfulWidget::paintEvent(QPaintEvent *event)
{
    if (m_BGType != EMPTY)
    {
        QPainter l_painter(this);
        l_painter.setOpacity(m_opacity);
        //��С�仯��, �������ɱ���
        if (m_buffer.size() != size() || m_isRefresh == true)
        {
            //��ʼ������
            m_buffer = QPixmap(this->size());
            m_buffer.fill(Qt::transparent);

            //��������ɰ�
            drawBufferPixmap(m_maskPixmap, m_edgeTopImage
                             , m_edgeBottomImage, m_edgeLeftImage
                             , m_edgeRightImage, m_paddingImage
                             , m_buffer);

            //������Ⱦģʽ
            QPainter l_tmpPainter(&m_buffer);
            l_tmpPainter.setRenderHint(QPainter::Antialiasing);
            l_tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);

            switch (m_BGType)
            {
                case COLOR: //��ɫģʽ, ��䴿ɫ
                    l_tmpPainter.fillRect(m_buffer.rect(), m_backgroundColor);
                    break;

                case PIXMAP: //ͼƬģʽ, ����ͼƬ, ���ģʽΪƽ��
                    l_tmpPainter.drawTiledPixmap(m_buffer.rect(), m_backgroundPixmap);
                    break;

                default:
                    QWidget::paintEvent(event);
                    break;
            }
            m_isRefresh = false;
        }
        //���Ʊ���
        l_painter.drawPixmap(0, 0, m_buffer);
    }
        CFreeSizeWidget::paintEvent(event);
}
void CColorfulWidget::showEvent(QShowEvent *e)
{
    scroll(1,0);
    scroll(-1,0);
    CFreeSizeWidget::showEvent(e);
}
