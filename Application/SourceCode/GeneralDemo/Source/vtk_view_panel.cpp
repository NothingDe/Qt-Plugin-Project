/*!
 * \file    vtk_view_panel.cpp
 * \date    2019.10.24
 * \author  zhusizhi
 * \version 1.0.0
 *
 * \brief   VtkView panel
 *
 */
#include "vtk_view_panel.h"

namespace general_demo
{
	VtkViewPanel::VtkViewPanel(QWidget *parent)
		: Panel(parent)
		, m_workThread(NULL)
	{
		ui.setupUi(this);

		//���Ƴ�ʼ��
		m_pointCloudFile.reset(new PointCloudTRGBA);
		m_pointCloudFile->resize(1);

		m_mainViewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
		m_mainViewer->initCameraParameters();   //��ʼ���������
		ui.screen->SetRenderWindow(m_mainViewer->getRenderWindow());
		m_mainViewer->setupInteractor(ui.screen->GetInteractor(), ui.screen->GetRenderWindow());

		// ���ñ�����ɫΪ dark
		m_mainViewer->setBackgroundColor(30 / 255.0, 255 / 255.0, 255 / 255.0); //(30 / 255.0, 30 / 255.0, 30 / 255.0);

		// create the event thread
		prepareEventThread();

		m_workThread->start();
	}

	VtkViewPanel::~VtkViewPanel()
	{
		if (m_workThread)
		{
			if (m_workThread->wait(1000) == false)
				m_workThread->terminate();
			delete m_workThread;
			m_workThread = NULL;
		}
	}
	
	bool VtkViewPanel::prepareEventThread()
	{
		if (!m_workThread)
			m_workThread = new WorkerThread(this);
		if (m_workThread->isRunning())
			return false; // Not
		connect(m_workThread, SIGNAL(started()), this, SLOT(workThreadStarted()), Qt::QueuedConnection);
		connect(m_workThread, SIGNAL(finished()), this, SLOT(workThreadFinished()), Qt::QueuedConnection);
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
		connect(m_workThread, SIGNAL(terminated()), this, SLOT(workThreadFinished()), Qt::QueuedConnection);
#endif
		return true;
	}

	void VtkViewPanel::workThreadStarted()
	{
		
	}

	void VtkViewPanel::workThreadFinished()
	{
		
	}

	void VtkViewPanel::openRun()
	{
		PointCloudTRGBA::Ptr pointCloudFileTITemp;
		pointCloudFileTITemp.reset(new PointCloudTRGBA);		
		m_pointCloudFile.reset(new PointCloudTRGBA);

		QString strFileName = QCoreApplication::applicationDirPath() + "\\A2.pcd";;
		std::string file_name = strFileName.toStdString();

		int status = -1;
		if (strFileName.endsWith(".pcd", Qt::CaseInsensitive))
		{
			status = pcl::io::loadPCDFile(file_name, *m_pointCloudFile);

			setCloudColor(0, 0, 255);
		}
		else if (strFileName.endsWith(".ply", Qt::CaseInsensitive))
		{
			status = pcl::io::loadPLYFile(file_name, *(m_pointCloudFile));
			if (m_pointCloudFile->points[0].r == 0 && m_pointCloudFile->points[0].g == 0 && m_pointCloudFile->points[0].b == 0)
			{
				setCloudColor(255, 255, 255);
			}
		}
		else if (strFileName.endsWith(".obj", Qt::CaseInsensitive))
		{
			status = pcl::io::loadOBJFile(file_name, *(m_pointCloudFile));
			if (m_pointCloudFile->points[0].r == 0 && m_pointCloudFile->points[0].g == 0 && m_pointCloudFile->points[0].b == 0)
			{
				setCloudColor(255, 255, 255);
			}
		}
		else
		{
			//��ʾ���޷���ȡ����.ply .pcd .obj������ļ�
			//emit updateSignal(tr("File format error"));
			return;
		}
		//��ʾ����׺û���⣬���ļ������޷���ȡ
		if (status != 0)
		{
			//emit updateSignal(tr("Reading file error"));
			return;
		}

		////�ӵ������Ƴ�NAN��Ҳ������Ч��
		//std::vector<int> indices;
		//indices.clear();
		//pcl::removeNaNFromPointCloud(*pointCloudFileTITemp, *m_pointCloudFile, indices);

		//pcl::copyPointCloud(*m_pointCloudFileTI, *m_pointCloudFile);
		setAlpha(200);  //���õ���Ϊ��͸��255

		m_mainViewer->addPointCloud(m_pointCloudFile);
		m_mainViewer->updatePointCloud(m_pointCloudFile);

		m_mainViewer->resetCamera();
		ui.screen->update();
	}

	void VtkViewPanel::setCloudColor(unsigned int r, unsigned int g, unsigned int b)
	{
		// Set the new color
		for (size_t i = 0; i < m_pointCloudFile->size(); i++)
		{
			m_pointCloudFile->points[i].r = r;
			m_pointCloudFile->points[i].g = g;
			m_pointCloudFile->points[i].b = b;
			m_pointCloudFile->points[i].a = 255;
		}
	}

	void VtkViewPanel::setAlpha(unsigned int a)
	{
		for (size_t i = 0; i < m_pointCloudFile->size(); i++)
		{
			m_pointCloudFile->points[i].a = a;
		}
	}

	void VtkViewPanel::resizeEvent(QResizeEvent *event)
	{
		ui.screen->resize(this->size());
		m_mainViewer->setupInteractor(ui.screen->GetInteractor(), ui.screen->GetRenderWindow());
	}
} // namespace general_demo


