/******************************************************************************
*�ļ�����:  csystemsetting.h
*��    ��:  ���ù���
*��    ��:  
*��������:  
*��ǰ�汾:  
*�汾��ʷ:
*ע    ��:  ����ȫ�ֹ����࣬�����ý��ж�ȡ���޸Ĳ�����
*
*******************************************************************************/
#ifndef CSYSTEMSETTING_H
#define CSYSTEMSETTING_H

/***********************************ͷ�ļ�*************************************/
#include <QString>
#include <QVariant>
#include <QColor>
#include <QObject>
#include <QRect>
#include <QTranslator>
#include <QDateTime>

/***********************************������*************************************/
class CSystemSetting : public QObject
{
    Q_OBJECT

// pugang:¼���ļ�����
public:
    enum RECORD_FILE_TYPE{
        RECORD_FILE_PCM = 0, //pcm
        RECORD_FILE_WAV, //wav
        RECORD_FILE_3GP // 3gp
    };
    //ͬ����ʽ
    enum SYNC_TYPE
    {
        SYNC_SLOW, //��ͬ�������������ַ�������أ�
        SYNC_REFRESH, //ˢ��ͬ��
        SYNC_TWO_WAY //˫��ͬ��
    };
    //ͬ��Ƶ��
    enum SYNC_FREQUENCY
    {
        SYNC_NEVER, //��ͬ��
        SYNC_ONCE,//ֻͬ��һ�Σ�û�����ˣ�
        SYNC_REPEAT,//�ظ�ͬ����û�����ˣ�
        SYNC_DO//Ҫͬ��
    };

    //ͬ��ʱ����
    enum SYNC_INTERVAL
    {
        ONE_DAY,
        ONE_WEEK,
        ONE_MONTH
    };

    static CSystemSetting* getInstance(QObject *parent = 0); //ȡ����ĵ���ʵ��
    ~CSystemSetting();
    static void setSettingInstance(CSystemSetting* pointer);

    //void initDefaultStoragePosition(const QString &);

    void installAppLanguage(const QString &languageName = "");//���ó�������
    const QStringList getLanguageList();                      //��ȡ��ǰ֧�ֵ������б�
    const QString getCurrentLanguage();
    void writeSetting(bool);
    const bool readSetting();
    void initSettingInfo(const QString &fileName); // ��ʼ��������Ϣ
    void resetSettingInfo(const QString &fileName); // ���¼���������Ϣ

	// ���������ļ��������������
	void setValueToFile(const QString &key, const QVariant &value);
	QString GetConfigFileValue();
	QVariant getValueFromFile(const QString &key, const QVariant &defaultValue = QVariant());
	void createSettingFile(const QString &fileName); // ����Ĭ�������ļ�
    // �����Զ���������
    bool setAutoRun(bool enable, bool curUserOnly);
    bool isAutoRun(bool curUserOnly);

    void setSkinConfig(int type, const QVariant &value);        //����Ƥ������
    void getSkinConfig(int &type, QVariant &value);             //���Ƥ������
    void setRectOfMainWidget(const QRect &);                    //���������������
    QRect getRectOfMainWidget();                                //��ȡ�����������
	void ReadConfigureInfo();

public:
    //Ƥ��
    enum SkinType{
        DefaultType = -1,
        ColorType,
        PixmapType
    };

    int skinType;
    QColor skinColor;
    QString SkinPixmap;
    QString skinCustom1;
    QString skinCustom2;
	QString defaultStoragePosition; // Ĭ�ϴ洢λ��
	QMap<QString, QString> m_languageMap;
  
signals:
    void signalUpdateSetting(QString key);
    
    void signalChangeLanguage(const QString & strLanguage ); //�����л�
    
private:
    explicit CSystemSetting(QObject *parent = 0);
    static CSystemSetting *m_instance;
	void initParameters();
    QString m_fileName; // �����ļ�
    QTranslator *mp_currentLang; //��ǰ����, �����л�����ʱ�Ƴ��ϴε�����
    QTranslator *mp_currentQtLang; //��ǰ����, �����л�����ʱ�Ƴ��ϴε�����
    QString m_lastPath;//���ļ�ѡ����Ĭ��·��(��Ŀ¼)
   

};

#endif // CSYSTEMSETTING_H
