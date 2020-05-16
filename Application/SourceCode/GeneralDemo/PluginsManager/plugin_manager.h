#ifndef GENERAL_DEMO_PLUGIN_MANAGER_H
#define GENERAL_DEMO_PLUGIN_MANAGER_H

#include <QObject>
#include <QPluginLoader>
#include <QDir>
#include "plugin_manager_private.h"
#include "plugin_meta_data.h"


class PluginManager : public QObject
{
    Q_OBJECT
public:
	static PluginManager &getInstance()
	{
		static PluginManager m_instance;
		return m_instance;
	}

public:
	QDir getPluginPath();
	//�������в��
	void loadAllPlugins();
	//ɨ��JSON�ļ��еĲ��Ԫ����
	void scan(const QString &filepath);
	//��������ĳ�����
	void loadPlugin(const QString &filepath);
	//ж�����в��
	void unloadAllPlugins();
	//ж��ĳ�����
	void unloadPlugin(const QString &filepath);
	//��ȡ���в��
	QList<QPluginLoader *> getAllPlugins();
	//��ȡĳ�����
	QPluginLoader* getPlugin(const QString &name);
	//��ʼ���Ͳ��ͨ�ŵ��źŲ�
	void initSignalAndSlot();

signals:
	void sendMsg2MainProcess(PluginMetaData);

public slots:
	//�������Ը��������Ϣ
	void recMsgfromPlugin(PluginMetaData);

private:
	explicit PluginManager(QObject *parent = nullptr);
	~PluginManager();

	PluginManagerPrivate *m_pluginPrivate;

};

#endif // GENERAL_DEMO_PLUGIN_MANAGER_H
