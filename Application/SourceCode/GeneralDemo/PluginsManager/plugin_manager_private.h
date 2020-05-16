#ifndef GENERAL_DEMO_PLUGINMANAGERPRIVATE_H
#define GENERAL_DEMO_PLUGINMANAGERPRIVATE_H

#include <QVariant>
#include <QHash>
#include <QDebug>
#include <QPluginLoader>

class PluginManagerPrivate
{
public:
    PluginManagerPrivate();

    bool check(const QString& path); //���������

    QHash<QString, QVariant> m_names; //���·�� - ����
    QHash<QString, QVariant> m_versions; //���·�� - �汾
    QHash<QString, QVariantList> m_dependencies; //���·�� - ����������Ĳ��
    QHash<QString, QPluginLoader *> m_loaders; //���·�� - QPluginLoaderʵ��
};

#endif // GENERAL_DEMO_PLUGINMANAGERPRIVATE_H
