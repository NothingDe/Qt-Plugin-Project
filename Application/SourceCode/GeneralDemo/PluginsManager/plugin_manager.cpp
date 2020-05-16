#include "plugin_manager.h"
#include "plugin_manager_private.h"
#include "interface_plugin.h"

#include <QDir>
#include <QApplication>
#include <QPluginLoader>
#include <QLibrary>
#include <QJsonObject>
#include <QJsonArray>

// �������в��
QDir PluginManager::getPluginPath()
{
	// ������Ŀ¼
	QDir path = QDir(qApp->applicationDirPath());
	path.cd("./plugins");
	return path;
}

void PluginManager::loadAllPlugins()
{
	// ������Ŀ¼
	QDir path = getPluginPath();

	// ��ʼ������е�Ԫ����
	foreach(QFileInfo info, path.entryInfoList(QDir::Files | QDir::NoDotAndDotDot))
	{
		scan(info.absoluteFilePath());
	}
	// ���ز��
	foreach(QFileInfo info, path.entryInfoList(QDir::Files | QDir::NoDotAndDotDot))
	{
		loadPlugin(info.absoluteFilePath());
	}
}


// ɨ�� JSON �ļ��еĲ��Ԫ����
void PluginManager::scan(const QString& path)
{
	/*****
	*  �ж��Ƿ��ǿ⣨��׺��Ч�ԣ�
	* Windows��.dll��.DLL
	* Unix/Linux��.so
	*****/

	if (!QLibrary::isLibrary(path))
		return;

	// ��ȡԪ���ݣ����ơ��汾��������
	QPluginLoader *loader = new QPluginLoader(path);
	QJsonObject json = loader->metaData().value("MetaData").toObject();
	m_pluginPrivate->m_names.insert(path, json.value("name").toVariant());
	m_pluginPrivate->m_versions.insert(path, json.value("version").toVariant());
	m_pluginPrivate->m_dependencies.insert(path, json.value("dependencies").toArray().toVariantList());

	delete loader;
	loader = Q_NULLPTR;
}

// ���ز��
void PluginManager::loadPlugin(const QString& path)
{
	// �ж��Ƿ��ǿ�
	if (!QLibrary::isLibrary(path))
		return;

	// ���������
	if (!m_pluginPrivate->check(path))
		return;

	// ���ز��
	QPluginLoader *loader = new QPluginLoader(path);
	if (loader->load()) {
		// ����̳��� InterfacePlugin������Ϊ���Լ��Ĳ������ֹ�ⲿ���ע�룩��
		InterfacePlugin *plugin = qobject_cast<InterfacePlugin *>(loader->instance());
		if (plugin)
		{
			m_pluginPrivate->m_loaders.insert(path, loader);
		}
		else
		{
			delete loader;
			loader = Q_NULLPTR;
		}
	}
}

// ж�����в��
void PluginManager::unloadAllPlugins()
{
	foreach(const QString &path, m_pluginPrivate->m_loaders.keys())
		unloadPlugin(path);
}

// ж�ز��
void PluginManager::unloadPlugin(const QString& path)
{
	QPluginLoader *loader = m_pluginPrivate->m_loaders.value(path);

	// ж�ز���������ڲ����ݽṹ���Ƴ�
	if (loader->unload())
	{
		m_pluginPrivate->m_loaders.remove(path);
		delete loader;
		loader = Q_NULLPTR;
	}
}

QList<QPluginLoader *> PluginManager::getAllPlugins()
{
	return m_pluginPrivate->m_loaders.values();
}

QPluginLoader* PluginManager::getPlugin(const QString &name)
{
	QString strKey = getPluginPath().absolutePath() + QString("/") + name + ".dll";
	QPluginLoader *loader = m_pluginPrivate->m_loaders.value(strKey);
	return  loader;
}

void PluginManager::initSignalAndSlot()
{
	auto plugins = getAllPlugins();
	foreach(auto loader, plugins)
	{
		// ÿ�����������Ϣ��manager��Ȼ����manager ���� dest �ֶ�ת��
		connect(loader->instance(), SIGNAL(sendMsg2Manager(PluginMetaData)), this, SLOT(recMsgfromPlugin(PluginMetaData)));
	}
}

void PluginManager::recMsgfromPlugin(PluginMetaData metadata)
{
	QString strDest = metadata.dest;
	if ("MainProcess" == strDest)
	{
		emit sendMsg2MainProcess(metadata);
	}
	else
	{
		auto loader = getPlugin(strDest);
		if (loader)
		{
			auto interface = qobject_cast<InterfacePlugin*>(loader->instance());
			if (interface)
			{
				interface->recMsgfromManager(metadata);
			}
		}
	}
	
}

PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
	, m_pluginPrivate(NULL)
{
	m_pluginPrivate = new PluginManagerPrivate();
}

PluginManager::~PluginManager()
{

}