/*!
 * \file    opencv_panel.cpp
 * \date    2019.10.24
 * \author  zhusizhi
 * \version 1.0.0
 *
 * \brief   OPENCV panel
 *
 */
#include "opencv_panel.h"

namespace general_demo
{
	int interNum = 100;//��������
	int internum1;
	OpenCVPanel::OpenCVPanel(QWidget *parent) :
		QMainWindow(parent)
	{
		ui.setupUi(this);
		
		//m_bIsDrawing = false;
		QTimer *timer = new QTimer(this);//��ǰʱ����ʾ
		connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
		timer->start(1000);
		Mat start = imread("images/start.png");
		img_display(start);
	}

	OpenCVPanel::~OpenCVPanel()
	{

	}

	void OpenCVPanel::resizeEvent(QResizeEvent *event)
	{
		this->resize(this->size());
	}

	void OpenCVPanel::timerUpdate()//��ʾʱ�亯��
	{
		QDateTime time = QDateTime::currentDateTime();
		QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
		ui.label_time->setText(str);
	}

	//-------------Mat��QImage��ת��----------------------//
	QImage OpenCVPanel::cvMat2QImage(const cv::Mat& mat)
	{
		// 8-bits unsigned, NO. OF CHANNELS = 1
		if (mat.type() == CV_8UC1)
		{
			QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
			// Set the color table (used to translate colour indexes to qRgb values)
			image.setColorCount(256);
			for (int i = 0; i < 256; i++)
			{
				image.setColor(i, qRgb(i, i, i));
			}
			// Copy input Mat
			uchar *pSrc = mat.data;
			for (int row = 0; row < mat.rows; row++)
			{
				uchar *pDest = image.scanLine(row);
				memcpy(pDest, pSrc, mat.cols);
				pSrc += mat.step;
			}
			return image;
		}
		// 8-bits unsigned, NO. OF CHANNELS = 3
		else if (mat.type() == CV_8UC3)
		{
			// Copy input Mat
			const uchar *pSrc = (const uchar*)mat.data;
			// Create QImage with same dimensions as input Mat
			QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
			return image.rgbSwapped();
		}
		else if (mat.type() == CV_8UC4)
		{
			qDebug() << "CV_8UC4";
			// Copy input Mat
			const uchar *pSrc = (const uchar*)mat.data;
			// Create QImage with same dimensions as input Mat
			QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
			return image.copy();
		}
		else
		{
			qDebug() << "ERROR: Mat could not be converted to QImage.";
			return QImage();
		}
	}

	Mat OpenCVPanel::QImage2cvMat(QImage image)
	{
		cv::Mat mat;
		qDebug() << image.format();
		switch (image.format())
		{
		case QImage::Format_ARGB32:
		case QImage::Format_RGB32:
		case QImage::Format_ARGB32_Premultiplied:
			mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
			break;
		case QImage::Format_RGB888:
			mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
			cv::cvtColor(mat, mat, CV_BGR2RGB);
			break;
		case QImage::Format_Indexed8:
			mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
			break;
		}
		return mat;
	}

	//-------------��ʾͼƬ����----------------------//
	void OpenCVPanel::img_display(Mat image)
	{
		dispimg = cvMat2QImage(image);
		ui.label->clear();
		ui.label->setPixmap(QPixmap::fromImage(dispimg));
		ui.label->resize(ui.label->pixmap()->size());
	}

	void OpenCVPanel::img_display_1(Mat image)
	{
		ui.label_1->setVisible(true);
		dispimg = cvMat2QImage(image);
		ui.label_1->clear();
		ui.label_1->setPixmap(QPixmap::fromImage(dispimg));
		ui.label_1->resize(ui.label_1->pixmap()->size());
	}
	void OpenCVPanel::img_display_2(Mat image)
	{
		ui.label_2->setVisible(true);
		dispimg = cvMat2QImage(image);
		ui.label_2->clear();
		ui.label_2->setPixmap(QPixmap::fromImage(dispimg));
		ui.label_2->resize(ui.label_2->pixmap()->size());
	}

	//--------------��ʱ���ز���Ҫ�Ĳ���--------------//
	void OpenCVPanel::ui_hide()
	{
		ui.thresh_Slider->setVisible(false);
		ui.label_thresh->setVisible(false);
		ui.thresh_Slider_1->setVisible(false);
		ui.label_thresh_1->setVisible(false);
		ui.comboBox1->setVisible(false);
		ui.comboBox2->setVisible(false);
		ui.label_1->setVisible(false);
		ui.label_2->setVisible(false);
		ui.tip2->setVisible(false);
		ui.tip3->setVisible(false);
		ui.spinBox->setVisible(false);
	}

	//--------------���û���Ĳ���--------------//
	void OpenCVPanel::setSlider(int Slider_Min, int Slider_Max, int Slider_Val)
	{
		ui.thresh_Slider->setVisible(true);//��ʾ����
		ui.label_thresh->setVisible(true);//��ʾ����ֵ
		ui.thresh_Slider->setMinimum(Slider_Min);//���û������ؼ�����Сֵ
		ui.thresh_Slider->setMaximum(Slider_Max);//���û������ؼ������ֵ
		ui.thresh_Slider->setValue(Slider_Val);//���û������ؼ��ĳ�ʼֵ
		ui.thresh_Slider->setPageStep(1);//���û������ؼ��ĵ������ֵ,��ϧֻ��int
	}

	//--------------���û���_1�Ĳ���--------------//
	void OpenCVPanel::setSlider_1(int Slider_Min, int Slider_Max, int Slider_Val)
	{
		ui.thresh_Slider_1->setVisible(true);//��ʾ����
		ui.label_thresh_1->setVisible(true);//��ʾ����ֵ
		ui.thresh_Slider_1->setMinimum(Slider_Min);//���û������ؼ�����Сֵ
		ui.thresh_Slider_1->setMaximum(Slider_Max);//���û������ؼ������ֵ
		ui.thresh_Slider_1->setValue(Slider_Val);//���û������ؼ��ĳ�ʼֵ
		ui.thresh_Slider_1->setPageStep(1);//���û������ؼ��ĵ������ֵ,��ϧֻ��int
	}

	//----------------------��ʼ----------------------//
	void OpenCVPanel::on_open_image_triggered()//��ͼƬ
	{
		QString filename;
		filename = QFileDialog::getOpenFileName(this, tr("ѡ��ͼ��"), "", tr("Images(*.png *.bmp *.jpg *.tif *.GIF )"));
		QTextCodec *code = QTextCodec::codecForName("gb18030");//Ϊ��ʹ������·��
		std::string name = code->fromUnicode(filename).data();
		image = cv::imread(name);//����ԭʼͼ�����
		image.copyTo(init_img);//�Ѵ򿪵�ͼ�񱣴浽ȫ�ֱ���init_img�У����ڳ�ʼ����ԭͼ��
		image.copyTo(rep_img);//��ͼ�񱣴浽ȫ�ֱ���rep_img�У����ڳ�������
		if (filename.isEmpty())
			return;
		else 
			img_display(image);
		//m_pix = QPixmap(image.cols, image.rows);
		//m_pix.fill(Qt::white);
		ui_hide();//���ز���Ҫ�Ĳ���
		thresh = 1;
		thresh_1 = 1;
		MORPH = 0;
		//����Ϊ�����ͼƬ������
		x = 0;
		y = 0;
		x1 = ui.label->width();
		y1 = ui.label->height();
		ui.tip1->setText(tr("ԭʼͼ."));
		statusBar()->showMessage(tr("����ȷ��ͼƬ."));
	}

	void OpenCVPanel::on_print_image_triggered()//��ӡͼƬ
	{
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		QPrinter printer;
		QPrintDialog printDialog(&printer, this);
		if (printDialog.exec())//�ж��Ƿ����ˡ���ӡ��
		{
			QPainter painter(&printer);
			QRect rect = painter.viewport();
			QSize size = dispimg.size();
			size.scale(rect.size(), Qt::KeepAspectRatio);//ʹͼ����������ҳ��
			painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
			painter.setWindow(dispimg.rect());
			painter.drawImage(0, 0, dispimg);
		}
		statusBar()->showMessage(tr("����ȷ��ӡͼ��."));
	}

	void OpenCVPanel::on_save_image_triggered()//����ͼƬ
	{
		QString filename;
		filename = QFileDialog::getSaveFileName(this, tr("����ͼ��"), "", tr("Images(*.png)"));
		QTextCodec *code = QTextCodec::codecForName("gb18030");//Ϊ��ʹ������·��
		std::string name = code->fromUnicode(filename).data();
		if (filename.isEmpty()) return;
		else cv::imwrite(name, image);//����image��ָ���ļ����£���BGR��ʽ
		statusBar()->showMessage(tr("����ȷ����ͼ��."));
	}

	void OpenCVPanel::on_exit_image_triggered()//�˳�ϵͳ
	{
		if (!(QMessageBox::information(this, tr("��ʾ"), tr("ȷ���˳�ϵͳ?"), tr("Yes"), tr("No"))))
		{
			this->close();
		}
	}

	void OpenCVPanel::on_camera_image_triggered()//������ͷ
	{
		VideoCapture capture(0);
		while (1)  //ѭ����ʾÿһ֡
		{
			Mat frame;  //����һ��Mat���������ڴ洢ÿһ֡��ͼ��
			if (!capture.isOpened())return;
			capture >> frame;  //��ȡ��ǰ֡
			imshow("Camera", frame);  //��ʾ��ǰ֡
			char c = waitKey(30);  //��ʱ30ms
			if (c == 13)//����Enter����ͼ��
			{
				frame.copyTo(image);
				img_display(image);
				break;
			}
			if (c == 27)break;//����ESC�˳�
		}
		//cvDestroyWindow("Camera");
		statusBar()->showMessage(tr("����ȷ������ͷ����Enter����������."));
	}

	//-------------����������---------------------//
	void OpenCVPanel::on_repeal_image_triggered()//����
	{
		img_display(rep_img);
		rep_img.copyTo(image);
		statusBar()->showMessage(tr("�ѳ�����һ��ͼ�����."));
	}

	void OpenCVPanel::on_init_image_triggered()//��ԭ
	{
		img_display(init_img);
		ui.tip1->setText(tr("ԭʼͼ."));
		init_img.copyTo(image);
		ui_hide();
		flag = 0;
		thresh = 1;
		thresh_1 = 1;
		MORPH = 0;
		interNum = 100;
		ui.comboBox2->setCurrentIndex(0);
		ui.spinBox->setValue(50);
		x = 0;
		y = 0;
		x1 = ui.label->width();
		y1 = ui.label->height();
		statusBar()->showMessage(tr("�ѻָ�ԭʼͼ��."));
		ui.tip1->setText(tr("ԭʼͼ."));
	}

	void OpenCVPanel::on_OK_image_triggered()//ȷ��
	{
		flag = 255;
		dst_image.copyTo(image);
		ui_hide();
		statusBar()->showMessage(tr("��ȷ�ϱ�����."));
	}

	void OpenCVPanel::on_print_image_action_triggered()//��ӡ
	{
		on_print_image_triggered();
	}

	void OpenCVPanel::on_open_action_triggered()//��
	{
		on_open_image_triggered();
		ui_hide();
	}

	void OpenCVPanel::on_save_action_triggered()//����
	{
		on_save_image_triggered();
	}

	void OpenCVPanel::on_camera_action_triggered()//����ͷ
	{
		on_camera_image_triggered();
	}

	void OpenCVPanel::on_zoom_in_triggered()//�Ŵ�
	{
		QImage dst;
		QImage src;
		image.copyTo(rep_img);
		src = cvMat2QImage(image);
		if (src.isNull()) return;
		QMatrix matrix;
		matrix.scale(1.5, 1.5);
		dst = src.transformed(matrix);
		Mat dst1 = QImage2cvMat(dst);
		img_display(dst1);
		dst1.copyTo(image);
		statusBar()->showMessage(tr("ÿ�ε���Ŵ�1.5��."));
	}

	void OpenCVPanel::on_zoom_out_triggered()//��С
	{
		QImage dst;
		QImage src;
		image.copyTo(rep_img);
		src = cvMat2QImage(image);
		if (src.isNull()) return;
		QMatrix matrix;
		matrix.scale(0.66, 0.66);
		dst = src.transformed(matrix);
		Mat dst1 = QImage2cvMat(dst);
		img_display(dst1);
		dst1.copyTo(image);
		statusBar()->showMessage(tr("ÿ�ε����С1.5��."));
	}

	void OpenCVPanel::on_ROI_action_triggered()//����ROI
	{
		flag = 11;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		statusBar()->showMessage(tr("���϶����ѡ��һ����������."));
	}

	//-------------����¼�---------------------//
	void OpenCVPanel::mousePressEvent(QMouseEvent*event)//����
	{
		if (event->button() == Qt::LeftButton)//����������
		{
			//m_previousPoint = event->pos();
			//m_bIsDrawing = true;
			x = event->x() - (ui.label->x());
			y = event->y() - (ui.label->y()) - 55;//��ȥmenubar+toobar�ĸ߶�
			if (flag == 11)//ROI
			{
				statusBar()->showMessage(tr("�����Կؼ�����:(") + QString::number(x) + tr(",") + QString::number(y) + tr(")"));
			}
		}
	}

	void OpenCVPanel::mouseMoveEvent(QMouseEvent *event)//�ƶ�
	{
		if (event->buttons()&Qt::LeftButton)//���������µ�ͬʱ�ƶ����
		{
			//m_currentPoint = event->pos();
			//update();
			if (flag == 11 | flag == 21)//ֻ��ROI��Grabcutʱ��
			{
				QPainter paint;
				QPixmap pix;
				x1 = event->x() - (ui.label->x());
				y1 = event->y() - (ui.label->y()) - 55;
				paint.setPen(QColor(Qt::green)); //�趨������ɫ
				paint.setBrush(QColor(Qt::red)); //�趨�����ɫ
												 // paint.drawRect(QRect(70,70,80,80));
				paint.drawRect(QRect(x, y, x1, y1));
			}
		}
	}

	void OpenCVPanel::mouseReleaseEvent(QMouseEvent *event)//�ͷ�
	{
		if (event->button() == Qt::LeftButton)//�������ͷ�
		{
			//m_currentPoint = event->pos();
			//m_bIsDrawing = false;
			//update();
			if (flag == 11)//ֻ��ROIʱ��
			{
				if (x >= 0 && y >= 0 && x1 <= ui.label->width() && y1 <= ui.label->height())
				{
					Mat dst, src;
					image.copyTo(rep_img);
					image.copyTo(src);
					cv::Rect rect(x, y, x1 - x, y1 - y);
					src(rect).copyTo(dst);
					img_display(dst);
					dst.copyTo(image);
					ui.tip1->setText(tr("ROIͼ."));
					statusBar()->showMessage(tr("�ѻ��ROIͼ��."));
					flag = 0;
					x = 0;
					y = 0;
					x1 = ui.label->width();
					y1 = ui.label->height();
				}
			}
			if (flag == 21)//ֻ��Grabcutʱ��
			{
				if (x >= 0 && y >= 0 && x1 <= ui.label->width() && y1 <= ui.label->height())
				{
					Mat dst, src, mask;
					image.copyTo(rep_img);
					image.copyTo(src);
					dst = Mat(src.size(), CV_8UC1, cv::Scalar(cv::GC_BGD));//��ģͼ�������8UC1
					if (src.channels() == 1) cvtColor(src, src, CV_GRAY2RGB);//����ͼ�������8UC3
					src.convertTo(src, CV_8UC3);
					cv::Rect rectangle(x, y, x1 - x, y1 - y);// ������������Ǳ���
					cv::Mat result;
					//������ʱ�����������Ϊ�㷨���м����ʹ�ã�����care
					cv::Mat bgModel, fgModel;
					// GrabCut �ֶ�
					cv::grabCut(src,    //����ͼ��
						result,   //�ֶν��
						rectangle,// ����ǰ���ľ���
						bgModel, fgModel, // ǰ��������
						1,        // ��������
						cv::GC_INIT_WITH_RECT); // �þ���
												// �õ�������ǰ��������
												//�ȽϺ�������ֵΪGC_PR_FGD������
					cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);
					// �������ͼ��
					cv::Mat foreground(src.size(), CV_8UC3, cv::Scalar(255, 255, 255));
					//����ֵΪ GC_BGD=0����Ϊ����
					image.copyTo(foreground, result);
					img_display(foreground);
					foreground.copyTo(image);
					ui.tip1->setText(tr("��GrabCut�㷨�ָ���ͼ."));
					statusBar()->showMessage(tr("�Ѷ�ͼ����GrabCut�㷨�ָ�."));
				}
			}
		}
	}

	//void OpenCVPanel::paintEvent(QPaintEvent *event)
	//{
	//	int x, y, w, h;
	//	x = m_previousPoint.x();
	//	y = m_previousPoint.y();
	//	w = m_currentPoint.x() - x;
	//	h = m_currentPoint.y() - y;
	//	QPainter painter(this);
	//	if (m_bIsDrawing)
	//	{
	//		m_tempPix = m_pix;
	//		QPainter pp(&m_tempPix);
	//		pp.drawRect(x, y, w, h);
	//		painter.drawPixmap(0, 0, m_tempPix);
	//	}
	//	else
	//	{
	//		QPainter pp(&m_pix);
	//		pp.drawRect(x, y, w, h);
	//		painter.drawPixmap(0, 0, m_pix);
	//	}
	//}

	//-------------ͼ����ǿ���ҶȻ�---------------------//
	void OpenCVPanel::on_togray_triggered()
	{
		Mat dst, src;
		image.copyTo(rep_img);//����ǰͼ�񱣴浽ȫ�ֱ���rep_img�У����ڳ�������
		image.copyTo(src);//��ԭͼ�񱣴浽�ֲ�����src�н��в���
		if (!src.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		dst.create(src.size(), src.type()); // ������srcͬ���ͺʹ�С�ľ���(dst)
		if (src.channels() == 1) src.copyTo(dst);
		else cvtColor(src, dst, CV_BGR2GRAY); //ת��Ϊ�Ҷ�ͼ
		img_display(dst);
		dst.copyTo(image);//�Ѵ�����ͼ�����ȫ�ֱ���image�У������ٴ���
		ui.tip1->setText(tr("�Ҷ�ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ��ҶȻ�."));
	}

	//-------------ͼ����ǿ����ֵ��---------------------//
	void OpenCVPanel::on_tobinary_before_triggered()
	{
		flag = 10;
		Mat dst, src;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		image.copyTo(rep_img);
		image.copyTo(src);
		dst.create(src.size(), src.type());
		if (src.channels() == 3) { cvtColor(src, src, CV_BGR2GRAY); } //ת��Ϊ�Ҷ�ͼ
		threshold(src, dst, thresh, 255, CV_THRESH_BINARY);  //��ֵ��
		img_display(dst);
		if (thresh == 1)thresh = 128;//thresh��ʼ��
		setSlider(0, 255, thresh);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("��ֵͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ���ֵ��."));
	}

	//-------------ͼ����ǿ���ԱȶȺ�����---------------------//
	void OpenCVPanel::on_contrast_image_triggered()
	{
		flag = 7;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		int beta = thresh_1;//����
		float alpha = thresh * 3 / 255;//�Աȶ�0-3��
		Mat dst, src;
		image.copyTo(rep_img);
		image.copyTo(src);
		dst.create(src.size(), src.type());
		if (src.channels() == 3) //BGRͼ��
		{
			for (int y = 0; y < src.rows; y++)
			{
				for (int x = 0; x < src.cols; x++)
				{
					for (int c = 0; c < 3; c++)
					{
						dst.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(src.at<Vec3b>(y, x)[c] + beta));
					}
				}
			}
		}
		else //����BGRͼ��
		{
			for (int y = 0; y < src.rows; y++)
			{
				for (int x = 0; x < src.cols; x++)
				{
					dst.at<uchar>(y, x) = saturate_cast<uchar>(alpha*(src.at<uchar>(y, x)) + beta);
				}
			}
		}
		img_display(dst);
		if (thresh == 1)thresh = 60;//thresh��ʼ��
		setSlider(0, 255, thresh);
		if (thresh_1 == 1)thresh_1 = 60;//thresh��ʼ��
		setSlider_1(0, 255, thresh_1);
		dst.copyTo(dst_image);//ok������ʱ��dstimage���浽image�У����ڽ������ٴ���
		ui.tip1->setText(tr("�����ͼ"));
		statusBar()->showMessage(tr("�ѽ������ȺͶԱȶȵĵ���."));
	}

	//-------------ͼ����ǿ��ֱ��ͼ���⻯---------------------//
	void OpenCVPanel::on_histogram_equalization_triggered()
	{
		Mat dst, src;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		image.copyTo(rep_img);
		image.copyTo(src);
		if (src.channels() == 1)
			src.copyTo(dst);
		else
			cvtColor(src, dst, CV_BGR2GRAY); //ת��Ϊ�Ҷ�ͼ
		equalizeHist(src, dst);
		img_display(dst);
		dst.copyTo(image);
		ui.tip1->setText(tr("ֱ��ͼ���⻯���ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ�����ֱ��ͼ���⻯."));
	}

	//-------------����ROI����---------------------//
	void OpenCVPanel::on_action_ROI_triggered()
	{
		on_ROI_action_triggered();
	}

	//-------------ͼ����̬ѧ����---------------------//
	void OpenCVPanel::on_dilate_image_triggered()//����
	{
		flag = 22;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat dst, src;
		image.copyTo(rep_img);
		image.copyTo(src);
		Mat element = getStructuringElement(MORPH, Size(thresh, thresh));
		morphologyEx(src, dst, MORPH_DILATE, element, Point(-1, -1), thresh_1);//����
		img_display(dst);
		setSlider(1, 15, thresh);
		setSlider_1(1, 5, thresh_1);
		ui.comboBox1->setVisible(true);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("���ͺ�ͼ"));
		statusBar()->showMessage(tr("�Ѷ�ͼ���������."));
	}

	void OpenCVPanel::on_erode_image_triggered()//��ʴ
	{
		flag = 1;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat dst, src;
		image.copyTo(rep_img);
		image.copyTo(src);
		Mat element = getStructuringElement(MORPH, Size(thresh, thresh));
		morphologyEx(src, dst, MORPH_ERODE, element, Point(-1, -1), thresh_1);//��ʴ
		img_display(dst);
		setSlider(1, 15, thresh);
		setSlider_1(1, 5, thresh_1);
		ui.comboBox1->setVisible(true);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("��ʴ��ͼ"));
		statusBar()->showMessage(tr("�Ѷ�ͼ����и�ʴ."));
	}

	void OpenCVPanel::on_opening_image_triggered()//������
	{
		flag = 2;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat dst, src;
		image.copyTo(rep_img);
		image.copyTo(src);
		Mat element = getStructuringElement(MORPH, Size(thresh, thresh));
		morphologyEx(src, dst, MORPH_OPEN, element, Point(-1, -1), thresh_1);//������
		img_display(dst);
		setSlider(1, 15, thresh);
		setSlider_1(1, 5, thresh_1);
		ui.comboBox1->setVisible(true);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("��������ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ����п�����."));
	}

	void OpenCVPanel::on_closing_image_triggered()//������
	{
		flag = 3;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat dst, src;
		image.copyTo(rep_img);
		image.copyTo(src);
		Mat element = getStructuringElement(MORPH, Size(thresh, thresh));
		morphologyEx(src, dst, MORPH_CLOSE, element, Point(-1, -1), thresh_1);//������
		img_display(dst);
		setSlider(1, 15, thresh);
		setSlider_1(1, 5, thresh_1);
		ui.comboBox1->setVisible(true);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("��������ͼ"));
		statusBar()->showMessage(tr("�Ѷ�ͼ����б�����."));
	}

	void OpenCVPanel::on_gradient_image_triggered()//�ݶ�
	{
		flag = 4;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat dst, src;
		image.copyTo(rep_img);
		image.copyTo(src);
		Mat element = getStructuringElement(MORPH, Size(thresh, thresh));
		morphologyEx(src, dst, MORPH_GRADIENT, element, Point(-1, -1), thresh_1);//�ݶ�
		img_display(dst);
		setSlider(1, 15, thresh);
		setSlider_1(1, 5, thresh_1);
		ui.comboBox1->setVisible(true);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("�ݶȻ�ͼ"));
		statusBar()->showMessage(tr("�Ѷ�ͼ������ݶȻ�."));
	}

	void OpenCVPanel::on_tophat_image_triggered()//��ñ
	{
		flag = 5;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat dst, src;
		image.copyTo(rep_img);
		image.copyTo(src);
		Mat element = getStructuringElement(MORPH, Size(thresh, thresh));
		morphologyEx(src, dst, MORPH_TOPHAT, element, Point(-1, -1), thresh_1);//��ñ
		img_display(dst);
		setSlider(1, 15, thresh);
		setSlider_1(1, 5, thresh_1);
		ui.comboBox1->setVisible(true);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("��ñ�����ͼ"));
		statusBar()->showMessage(tr("�Ѷ�ͼ����ж�ñ����."));
	}

	void OpenCVPanel::on_blackhat_image_triggered()//��ñ
	{
		flag = 6;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat dst, src;
		image.copyTo(rep_img);
		image.copyTo(src);
		Mat element = getStructuringElement(MORPH, Size(thresh, thresh));
		morphologyEx(src, dst, MORPH_BLACKHAT, element, Point(-1, -1), thresh_1);//��ñ
		img_display(dst);
		setSlider(1, 15, thresh);
		setSlider_1(1, 5, thresh_1);
		ui.comboBox1->setVisible(true);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("��ñ������ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ����к�ñ����."));
	}

	//----------------------�׶����----------------------//
	void OpenCVPanel::on_hole_fill_triggered()
	{
		flag = 8;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat src;
		image.copyTo(rep_img);
		image.copyTo(src);
		Mat dst = Mat::zeros(src.size(), CV_8UC1);
		if (src.channels() != 1)
			cvtColor(src, src, CV_BGR2GRAY); //src����Ϊһ��2ֵ��ͨ��ͼ��
		src.convertTo(src, CV_8UC1);
		RemoveSmallRegion(src, dst, thresh, 0, 0);
		img_display(dst);
		if (thresh == 1)
			thresh = 100;//thresh��ʼ��
		setSlider(0, 500, thresh);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("�׶������ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ����п׶����."));
	}

	//---------------------ȥ��С�������----------------------//
	void OpenCVPanel::on_deduct_small_area_triggered()
	{
		flag = 9;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat src;
		image.copyTo(rep_img);
		image.copyTo(src);
		Mat dst = Mat::zeros(src.size(), CV_8UC1);
		if (src.channels() != 1)
			cvtColor(src, src, CV_BGR2GRAY); //src����Ϊһ��2ֵ��ͨ��ͼ��
		src.convertTo(src, CV_8UC1);
		RemoveSmallRegion(src, dst, thresh, 1, 1);
		img_display(dst);
		if (thresh == 1)thresh = 200;//thresh��ʼ��
		setSlider(0, 1500, thresh);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("ȥ���趨���������ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ��ȥ���趨�������."));
	}

	//CheckMode: 0����ȥ��������1����ȥ��������; NeihborMode��0����4����1����8����;��¼ÿ�����ص����״̬�ı�ǩ��0����δ��飬1�������ڼ��,2�����鲻�ϸ���Ҫ��ת��ɫ����3������ϸ������
	void OpenCVPanel::RemoveSmallRegion(Mat& Src, Mat& Dst, int AreaLimit, int CheckMode, int NeihborMode)
	{
		int RemoveCount = 0;       //��¼��ȥ�ĸ���
		Mat Pointlabel = Mat::zeros(Src.size(), CV_8UC1);
		if (CheckMode == 1)//ȥ��С����
		{
			for (int i = 0; i < Src.rows; ++i)
			{
				uchar* iData = Src.ptr<uchar>(i);
				uchar* iLabel = Pointlabel.ptr<uchar>(i);
				for (int j = 0; j < Src.cols; ++j)
				{
					if (iData[j] < 10)
					{
						iLabel[j] = 3;
					}
				}
			}
		}

		else//ȥ���׶�
		{
			for (int i = 0; i < Src.rows; ++i)
			{
				uchar* iData = Src.ptr<uchar>(i);
				uchar* iLabel = Pointlabel.ptr<uchar>(i);
				for (int j = 0; j < Src.cols; ++j)
				{
					if (iData[j] > 10)
					{
						iLabel[j] = 3;
					}
				}
			}
		}
		vector<Point2i> NeihborPos;  //��¼�����λ��
		NeihborPos.push_back(Point2i(-1, 0));
		NeihborPos.push_back(Point2i(1, 0));
		NeihborPos.push_back(Point2i(0, -1));
		NeihborPos.push_back(Point2i(0, 1));
		if (NeihborMode == 1)
		{
			NeihborPos.push_back(Point2i(-1, -1));
			NeihborPos.push_back(Point2i(-1, 1));
			NeihborPos.push_back(Point2i(1, -1));
			NeihborPos.push_back(Point2i(1, 1));
		}

		int NeihborCount = 4 + 4 * NeihborMode;
		int CurrX = 0, CurrY = 0;
		//��ʼ���
		for (int i = 0; i < Src.rows; ++i)
		{
			uchar* iLabel = Pointlabel.ptr<uchar>(i);
			for (int j = 0; j < Src.cols; ++j)
			{
				if (iLabel[j] == 0)
				{
					//********��ʼ�õ㴦�ļ��**********
					vector<Point2i> GrowBuffer;   //��ջ�����ڴ洢������
					GrowBuffer.push_back(Point2i(j, i));
					Pointlabel.at<uchar>(i, j) = 1;
					int CheckResult = 0;   //�����жϽ�����Ƿ񳬳���С����0Ϊδ������1Ϊ����
					for (int z = 0; z<GrowBuffer.size(); z++)
					{
						for (int q = 0; q<NeihborCount; q++)
						{
							CurrX = GrowBuffer.at(z).x + NeihborPos.at(q).x;
							CurrY = GrowBuffer.at(z).y + NeihborPos.at(q).y;
							if (CurrX >= 0 && CurrX<Src.cols&&CurrY >= 0 && CurrY<Src.rows)
							{
								if (Pointlabel.at<uchar>(CurrY, CurrX) == 0)
								{
									GrowBuffer.push_back(Point2i(CurrX, CurrY));  //��������buffer
									Pointlabel.at<uchar>(CurrY, CurrX) = 1; //���������ļ���ǩ�������ظ����
								}
							}
						}
					}
					if (GrowBuffer.size()>AreaLimit) CheckResult = 2;//�жϽ�����Ƿ񳬳��޶��Ĵ�С����1Ϊδ������2Ϊ����
					else
					{
						CheckResult = 1;
						RemoveCount++;
					}
					for (int z = 0; z<GrowBuffer.size(); z++)                         //����Label��¼
					{
						CurrX = GrowBuffer.at(z).x;
						CurrY = GrowBuffer.at(z).y;
						Pointlabel.at<uchar>(CurrY, CurrX) += CheckResult;
					}
					//********�����õ㴦�ļ��**********
				}
			}
		}
		CheckMode = 255 * (1 - CheckMode);
		//��ʼ��ת�����С������
		for (int i = 0; i < Src.rows; ++i)
		{
			uchar* iData = Src.ptr<uchar>(i);
			uchar* iDstData = Dst.ptr<uchar>(i);
			uchar* iLabel = Pointlabel.ptr<uchar>(i);
			for (int j = 0; j < Src.cols; ++j)
			{
				if (iLabel[j] == 2)
				{
					iDstData[j] = CheckMode;
				}
				else if (iLabel[j] == 3)
				{
					iDstData[j] = iData[j];
				}
			}
		}
	}

	//----------------------��Ϊ����-------------------//
	void OpenCVPanel::on_seed_action_triggered()
	{
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat src, seed;
		image.copyTo(rep_img);
		image.copyTo(seed1);
		QMessageBox::StandardButton res = QMessageBox::information(this, tr("��ʾ"), tr("����(���)ͼ���趨��ϣ���ѡ����Ҫ�����ԭʼͼ��."), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		if (res == QMessageBox::Yes)
		{
			QString filename;
			filename = QFileDialog::getOpenFileName(this, tr("ѡ����Ҫ�����ͼ��."), "", tr("Images(*.png *.bmp *.jpg *.tif *.GIF )"));
			QTextCodec *code = QTextCodec::codecForName("gb18030");
			std::string name = code->fromUnicode(filename).data();
			src = cv::imread(name);
			if (filename.isEmpty())
				return;
			else
			{
				img_display(src);
				img_display_1(seed1);
				ui.tip2->setVisible(true);
				ui.tip2->setText(tr("����ͼ."));
				ui.tip1->setText(tr("ԭʼͼ."));
				src.copyTo(image1);//����Ϊimage1
			}
		}
		flag = 100;//�����趨��ϵı�־
		statusBar()->showMessage(tr("�����趨��ϵı�־."));
	}

	//----------------------�ع�����--------------------//
	Mat OpenCVPanel::morphReconstruct(Mat src, Mat seed1)
	{
		/*���ܣ�ʵ����̬ѧ�ع�
		src��ԭͼ��seed1�Ǳ��ͼ��dst���ع�������ͼ*/
		Mat se = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat tmp1(src.size(), CV_8U), tmp2(src.size(), CV_8U);
		Mat dst;
		min(src, seed1, dst);
		do
		{
			dst.copyTo(tmp1);
			dilate(dst, seed1, se);
			min(src, seed1, dst);
			tmp2 = abs(tmp1 - dst);
		} while (sum(tmp2).val[0] != 0);
		return dst;
	}
	//----------------------��̬ѧ�ع�---------------------//
	void OpenCVPanel::on_Reconstruct_action_triggered()
	{
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		if (flag == 100)
		{
			Mat src, dst;
			image1.copyTo(src);
			dst = morphReconstruct(src, seed1);
			img_display_2(dst);
			flag = 0;
			dst.copyTo(image);
			ui.tip3->setVisible(true);
			ui.tip3->setText(tr("�ع����ͼ."));
			statusBar()->showMessage(tr("�Ѷ�ͼ������ع�."));
		}
		else
			QMessageBox::warning(this, tr("Warning"), tr("�����趨����ͼ��."));
	}

	//----------------------��������ȡ������----------------------//
	void OpenCVPanel::on_contours_describe_triggered()
	{
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat src, dst;
		image.copyTo(rep_img);
		image.copyTo(src);
		if (src.channels() != 1) cvtColor(src, src, CV_BGR2GRAY); //ת��Ϊ�Ҷ�ͼ,src����Ϊһ��2ֵ��ͨ��ͼ��
		dst = Mat::zeros(image.size(), CV_8UC1);//��ʼ��dst��ͨ��ͼ��
		blur(src, src, Size(3, 3));
		Canny(src, src, 128, 128 * 2, 3);
		vector<vector<Point> >contours;
		findContours(src, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
		//qDebug("%d", contours.size());
		drawContours(dst, contours, -1, Scalar(255), 2);//-1��ʾ���е���������
		img_display(dst);
		ui.tip1->setText(tr("����ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ�������������ȡ."));

		//����Щ����ת��Ϊ�������õ�������Ҳ�����漰���������������⣬��ʱ���ж��ַ�������ѡ�񣬱���ʸ����Ϊ����Ρ����Ρ���Բ�ȡ�
		ui.comboBox2->setVisible(true);
		ui.comboBox2->setItemText(0, tr("��ѡ����������."));
		ui.comboBox2->setItemText(1, tr("Բ��"));
		ui.comboBox2->setItemText(2, tr("����"));
		ui.comboBox2->setItemText(3, tr("�����."));
		ui.comboBox2->setItemText(4, tr("͹�����"));
		if (ui.comboBox2->currentIndex() == 0) return;
		if (ui.comboBox2->currentIndex() == 1)
		{
			vector<Point2f>center(contours.size());
			vector<float>radius(contours.size());
			for (unsigned int i = 0; i < contours.size(); i++)
			{
				minEnclosingCircle(contours[i], center[i], radius[i]);
				circle(dst, center[i], static_cast<int>(radius[i]), Scalar(255), 2);
			}
			img_display(dst);
			statusBar()->showMessage(tr("��Բ�ζ�ͼ���������������."));
		}
		if (ui.comboBox2->currentIndex() == 2)
		{
			vector<Rect> boundRect(contours.size());
			for (unsigned int i = 0; i < contours.size(); i++)
			{
				boundRect[i] = boundingRect(Mat(contours[i]));
				rectangle(dst, boundRect[i].tl(), boundRect[i].br(), Scalar(255), 2);
			}
			img_display(dst);
			statusBar()->showMessage(tr("�þ��ζ�ͼ���������������."));
		}
		if (ui.comboBox2->currentIndex() == 3)
		{
			vector<Point> poly;
			for (int i = 0; i < contours.size(); i++)
			{
				approxPolyDP(Mat(contours[i]), poly, 5, true);
				vector<Point>::const_iterator itp = poly.begin();
				while (itp != (poly.end() - 1))
				{
					line(dst, *itp, *(itp + 1), Scalar(255), 2);
					++itp;
				}
			}
			img_display(dst);
			statusBar()->showMessage(tr("�ö���ζ�ͼ���������������."));
		}
		if (ui.comboBox2->currentIndex() == 4)
		{
			vector<Point> hull;
			for (int i = 0; i < contours.size(); i++)
			{
				convexHull(Mat(contours[i]), hull);
				vector<Point>::const_iterator ith = hull.begin();
				while (ith != (hull.end() - 1))
				{
					line(dst, *ith, *(ith + 1), Scalar(255), 2);
					++ith;
				}
				line(dst, *ith, *(hull.begin()), Scalar(255), 2);
			}
			img_display(dst);
			statusBar()->showMessage(tr("��͹����ζ�ͼ���������������."));
		}
		dst.copyTo(dst_image);
		//���ǿ��Խ�һ������ÿһ����ͨ������������ԣ����磺���ġ����ľص�������
		//���Գ��ԣ�minAreaRect������һ����С�������Ӿ��Σ�contourArea���Լ�����������ͨ������� ����
		//pointPolygenTest���������ж�һ�����Ƿ���һ��������ڡ�mathShapes���ԱȽ�������״�������ԡ�
	}

	//----------------------UIС����������----------------------//
	void OpenCVPanel::on_thresh_Slider_valueChanged(int value)//thresh���������
	{
		thresh = ui.thresh_Slider->value();
		QString str;
		switch (flag) {

		case 22:
			str = "��ģ��С.";
			str.append(QString::number(thresh));
			on_dilate_image_triggered();
			break;
		case 1:
			str = "��ģ��С.";
			str.append(QString::number(thresh));
			on_erode_image_triggered();
			break;
		case 2:
			str = "��ģ��С.";
			str.append(QString::number(thresh));
			on_opening_image_triggered();
			break;
		case 3:
			str = "��ģ��С.";
			str.append(QString::number(thresh));
			on_closing_image_triggered();
			break;
		case 4:
			str = "��ģ��С.";
			str.append(QString::number(thresh));
			on_gradient_image_triggered();
			break;
		case 5:
			str = "��ģ��С.";
			str.append(QString::number(thresh));
			on_tophat_image_triggered();
			break;
		case 6:
			str = "��ģ��С.";
			str.append(QString::number(thresh));
			on_blackhat_image_triggered();
			break;
		case 7:
			str = "�Ա�ϵ��.";
			str.append(QString::number(thresh * 3 / 100));
			on_contrast_image_triggered();
			break;
		case 8:
			str = "�׶�����ٽ�ֵ.";
			str.append(QString::number(thresh));
			on_hole_fill_triggered();
			break;
		case 9:
			str = "С��������ٽ�ֵ.";
			str.append(QString::number(thresh));
			on_deduct_small_area_triggered();
			break;
		case 10:
			str = "��ֵ.";
			str.append(QString::number(thresh));
			on_tobinary_before_triggered();
			break;
		case 11:
			str = "�ͺ�����ֵ.";
			str.append(QString::number(thresh));
			on_actionCanny_triggered();
			break;
		case 12:
			str = "Sobel�˵Ĵ�С.";
			if (thresh == 2 | thresh == 4 | thresh == 6) return;
			else
			{
				on_actionSobel_triggered();
				str.append(QString::number(2 * thresh + 1));
			}
			break;
		case 13:
			str = "Laplacian�˵Ĵ�С.";
			if (thresh == 2 | thresh == 4 | thresh == 6) return;
			else
			{
				on_actionLaplacian_triggered();
				str.append(QString::number(2 * thresh + 1));
			}
			break;
		case 14:
			str = "�����˲��ں˵Ĵ�С.";
			str.append(QString::number(thresh));
			on_BoxFilter_triggered();
			break;
		case 15:
			str = "��ֵ�˲��ں˵Ĵ�С.";
			str.append(QString::number(thresh));
			on_MeanBlur_triggered();
			break;
		case 16:
			str = "��˹�˲��ں˵Ĵ�С.";
			if (thresh == 2 | thresh == 4 | thresh == 6) return;
			else
			{
				on_GaussianBlur_triggered();
				str.append(QString::number(2 * thresh + 1));
			}
			break;
		case 17:
			str = "��ֵ�˲��ں˵Ĵ�С.";
			if (thresh == 2 | thresh == 4 | thresh == 6) return;
			else
			{
				on_MedianBlur_triggered();
				str.append(QString::number(2 * thresh + 1));
			}
			break;
		case 18:
			str = "˫���˲��ں˵Ĵ�С.";
			str.append(QString::number(thresh));
			on_BilateralFilter_triggered();
			break;
		case 19:
			str = "���������ݶȵ���ֵ.";
			str.append(QString::number(thresh));
			on_region_grow_triggered();
			break;
		default:
			break;
		}
		ui.label_thresh->setText(str);
	}

	void OpenCVPanel::on_thresh_Slider_1_valueChanged(int value)//thresh_1����_1������
	{
		thresh_1 = ui.thresh_Slider_1->value();
		QString str;
		switch (flag) {
		case 22:
			str = "��������.";
			on_dilate_image_triggered();
			break;
		case 1:
			str = "��������.";
			on_erode_image_triggered();
			break;
		case 2:
			str = "��������.";
			on_opening_image_triggered();
			break;
		case 3:
			str = "��������.";
			on_closing_image_triggered();
			break;
		case 4:
			str = "��������.";
			on_gradient_image_triggered();
			break;
		case 5:
			str = "��������.";
			on_tophat_image_triggered();
			break;
		case 6:
			str = "��������.";
			on_blackhat_image_triggered();
			break;
		case 7:
			str = "����ϵ��.";
			on_contrast_image_triggered();
			break;
		default:
			break;
		}
		str.append(QString::number(thresh_1));
		ui.label_thresh_1->setText(str);
	}

	void OpenCVPanel::on_comboBox1_currentIndexChanged(int index)//ѡ���1������
	{
		switch (ui.comboBox1->currentIndex()) {
		case 0:
			MORPH = 0;
			break;
		case 1:
			MORPH = 1;
			break;
		case 2:
			MORPH = 2;
			break;
		default:
			break;
		}
		on_thresh_Slider_valueChanged(0);
	}

	void OpenCVPanel::on_comboBox2_currentIndexChanged(const QString &arg1)//ѡ���ִ����������������
	{
		on_contours_describe_triggered();
	}

	void OpenCVPanel::on_spinBox_editingFinished()//��������
	{
		interNum = ui.spinBox->value();
		if ((interNum != internum1) && (flag == 21)) on_actionLever_set_triggered();
		if ((interNum != internum1) && (flag == 20)) on_action_FCM_triggered();
	}

	//------------------------ͼ���˲�---------------------//
	void OpenCVPanel::on_BoxFilter_triggered()//�����˲�
	{
		flag = 14;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat dst, src;
		image.copyTo(rep_img);
		image.copyTo(src);
		dst.create(src.size(), src.type());
		boxFilter(src, dst, -1, Size(thresh, thresh));
		img_display(dst);
		setSlider(1, 15, thresh);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("�����˲����ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ����з����˲�."));
	}

	void OpenCVPanel::on_MeanBlur_triggered()//��ֵ�˲�������ƽ���˲���
	{
		flag = 15;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat dst, src;
		image.copyTo(rep_img);
		image.copyTo(src);
		dst.create(src.size(), src.type());
		blur(src, dst, Size(thresh, thresh), Point(-1, -1));
		img_display(dst);
		setSlider(1, 15, thresh);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("��ֵ�˲����ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ����о�ֵ�˲�."));
	}
	void OpenCVPanel::on_GaussianBlur_triggered()//��˹�˲�
	{
		flag = 16;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat dst, src;
		image.copyTo(rep_img);
		image.copyTo(src);
		dst.create(src.size(), src.type());
		GaussianBlur(src, dst, Size(2 * thresh + 1, 2 * thresh + 1), 0, 0);
		img_display(dst);
		setSlider(1, 9, thresh);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("��˹�˲����ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ����и�˹�˲�."));
	}
	void OpenCVPanel::on_MedianBlur_triggered()//��ֵ�˲�
	{
		flag = 17;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat dst, src;
		image.copyTo(rep_img);
		image.copyTo(src);
		dst.create(src.size(), src.type());
		medianBlur(src, dst, 2 * thresh + 1);
		img_display(dst);
		setSlider(1, 7, thresh);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("��ֵ�˲����ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ�������ֵ�˲�."));
	}
	void OpenCVPanel::on_BilateralFilter_triggered()//˫���˲�
	{
		flag = 18;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat dst, src;
		image.copyTo(rep_img);
		image.copyTo(src);
		dst.create(src.size(), src.type());
		bilateralFilter(src, dst, thresh, thresh * 2, thresh / 2);
		img_display(dst);
		setSlider(1, 15, thresh);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("˫���˲����ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ�����˫���˲�."));
	}

	//-------------ͼ��ָ�����ֵ�ָ�---------------------//
	void OpenCVPanel::on_OTSU_2_triggered()
	{
		Mat dst, src;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		image.copyTo(rep_img);
		image.copyTo(src);
		dst.create(src.size(), src.type());
		if (src.channels() == 3) { cvtColor(src, src, CV_BGR2GRAY); } //ת��Ϊ�Ҷ�ͼ
		threshold(src, dst, 0, 255, CV_THRESH_OTSU);  //��򷨶�ֵ��
		img_display(dst);
		dst.copyTo(image);
		ui.tip1->setText(tr("��򷨷ָ���ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ����д����ֵ�ָ�."));
	}

	//-------------ͼ��ָ��Ե����ָ�---------------------//
	void OpenCVPanel::on_actionCanny_triggered()//Canny����
	{
		flag = 11;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat src, dst, g_cannyDetectedEdges;
		image.copyTo(rep_img);
		image.copyTo(src);
		if (src.channels() == 3) { cvtColor(src, src, CV_BGR2GRAY); } //ת��Ϊ�Ҷ�ͼ
																	  // ��ʹ�� 3x3�ں�������
		blur(src, g_cannyDetectedEdges, Size(3, 3));
		// �������ǵ�Canny����
		Canny(g_cannyDetectedEdges, g_cannyDetectedEdges, thresh, thresh * 3, 3);
		//�Ƚ�g_dstImage�ڵ�����Ԫ������Ϊ0
		dst.create(src.size(), src.type());
		dst = Scalar::all(0);
		//ʹ��Canny��������ı�Եͼg_cannyDetectedEdges��Ϊ���룬����ԭͼg_srcImage����Ŀ��ͼg_dstImage��
		src.copyTo(dst, g_cannyDetectedEdges);
		img_display(dst);
		if (thresh == 1)thresh = 128;//thresh��ʼ��
		setSlider(0, 255, thresh);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("��Canny���ӷָ���ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ����Canny���ӷָ�."));
	}
	void OpenCVPanel::on_actionSobel_triggered()//Sobel����
	{
		flag = 12;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat src, dst, g_sobelGradient_X, g_sobelGradient_Y, g_sobelAbsGradient_X, g_sobelAbsGradient_Y;
		image.copyTo(rep_img);
		image.copyTo(src);
		if (src.channels() == 3) { cvtColor(src, src, CV_BGR2GRAY); } //ת��Ϊ�Ҷ�ͼ
		dst.create(src.size(), src.type());
		// �� X�����ݶ�
		Sobel(src, g_sobelGradient_X, CV_16S, 1, 0, thresh, 1, 1, BORDER_DEFAULT);
		convertScaleAbs(g_sobelGradient_X, g_sobelAbsGradient_X);//�������ֵ���������ת����8λ
																 // ��Y�����ݶ�
		Sobel(src, g_sobelGradient_Y, CV_16S, 0, 1, thresh, 1, 1, BORDER_DEFAULT);
		convertScaleAbs(g_sobelGradient_Y, g_sobelAbsGradient_Y);//�������ֵ���������ת����8λ
																 // �ϲ��ݶ�
		addWeighted(g_sobelAbsGradient_X, 0.5, g_sobelAbsGradient_Y, 0.5, 0, dst);

		img_display(dst);
		setSlider(1, 7, thresh);
		ui.thresh_Slider->setPageStep(2);//threshֻ����������
		ui.thresh_Slider->setSingleStep(2);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("��Sobel���ӷָ���ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ����Sobel���ӷָ�."));
	}

	void OpenCVPanel::on_actionLaplacian_triggered()//Laplacian����
	{
		flag = 13;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat src, dst, g_cannyDetectedEdges;
		image.copyTo(rep_img);
		image.copyTo(src);
		if (src.channels() == 3)
		{
			cvtColor(src, src, CV_BGR2GRAY);
		} //ת��Ϊ�Ҷ�ͼ
		GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
		Laplacian(src, dst, CV_16S, thresh, 1, 0, BORDER_DEFAULT);
		convertScaleAbs(dst, dst); //�������ֵ���������ת����8λ
		img_display(dst);
		setSlider(1, 7, thresh);
		ui.thresh_Slider->setPageStep(2);//threshֻ����������
		ui.thresh_Slider->setSingleStep(2);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("��Laplacian���ӷָ���ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ����Laplacian���ӷָ�."));
	}

	//-------------��������ķָ�:����������---------------------//
	void OpenCVPanel::on_region_grow_triggered()
	{
		flag = 19;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		Mat src;
		image.copyTo(rep_img);
		image.copyTo(src);
		IplImage imgTmp = src;  //Mat����ת����IplImage����
		IplImage *tmp = cvCloneImage(&imgTmp);
		IplImage* newimg = cvCreateImage(cvSize(tmp->width, tmp->height), 8, 3);
		int i;  //---ѭ������
		int j;
		int k = 0;
		int height = tmp->height;
		int wide = tmp->width;
		int   zhongzi = 0;
		int   zhongzi2 = 0;
		int   zhongzi3 = 0;
		for (i = height / 2 - 4; i<height / 2 + 5; i++)
		{
			for (j = wide / 2 - 4; j<wide / 2 + 5; j++)
			{
				zhongzi += ((uchar*)(tmp->imageData + tmp->widthStep*i))[j * 3];
				zhongzi2 += ((uchar*)(tmp->imageData + tmp->widthStep*i))[j * 3 + 1];
				zhongzi3 += ((uchar*)(tmp->imageData + tmp->widthStep*i))[j * 3 + 2];
				k++;
			}
		}
		zhongzi = zhongzi / k;
		zhongzi2 = zhongzi2 / k;
		zhongzi3 = zhongzi3 / k;

		for (j = 0; j < height; j++)                //---�Ը����ؽ��лҶ�ת��
		{
			for (i = 0; i < wide; i++)
			{
				int  temp = ((uchar*)(tmp->imageData + tmp->widthStep*j))[i * 3];
				int  temp2 = ((uchar*)(tmp->imageData + tmp->widthStep*j))[i * 3 + 1];
				int  temp3 = ((uchar*)(tmp->imageData + tmp->widthStep*j))[i * 3 + 2];
				if (abs(temp - zhongzi)<thresh&&abs(temp2 - zhongzi2)<thresh&&abs(temp3 - zhongzi3)<thresh)    //��ǰ��ͬ����һ�Ҷ�ֵ�ȽϽӽ�
				{								     //---�����ӵ���ɫ������ǰλ��
					((newimg->imageData + newimg->widthStep*j)[i * 3]) = temp;
					((newimg->imageData + newimg->widthStep*j)[i * 3 + 1]) = temp2;
					((newimg->imageData + newimg->widthStep*j)[i * 3 + 2]) = temp3;
				}
			}
		}

		//cv::Mat dst(newimg);
		Mat dst = cvarrToMat(newimg);
		img_display(dst);
		if (thresh == 1)thresh = 60;//thresh��ʼ��
		setSlider(1, 255, thresh);
		dst.copyTo(dst_image);
		ui.tip1->setText(tr("�������������ָ���ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ���������������ָ�."));
	}

	//-------------������̬ѧ�ķָ�---------------------//
	void OpenCVPanel::on_watershed_2_triggered()//��ˮ���㷨
	{
		Mat dst, src, binary;
		image.copyTo(rep_img);
		image.copyTo(src);
		if (src.channels() == 1)
			cvtColor(src, src, CV_GRAY2RGB);//����ͼ�������8UC3
		src.convertTo(src, CV_8UC3);

		cvtColor(src, binary, CV_BGR2GRAY);
		threshold(binary, binary, 0, 255, CV_THRESH_OTSU);  //��򷨶�ֵ��

															// Eliminate noise and smaller objects
		cv::Mat fg;
		cv::erode(binary, fg, cv::Mat(), cv::Point(-1, -1), 2);

		// Identify image pixels without objects
		cv::Mat bg;
		cv::dilate(binary, bg, cv::Mat(), cv::Point(-1, -1), 3);
		cv::threshold(bg, bg, 1, 128, cv::THRESH_BINARY_INV);

		// Create markers image
		cv::Mat markers(binary.size(), CV_8U, cv::Scalar(0));
		markers = fg + bg;
		markers.convertTo(markers, CV_32S);

		watershed(src, markers);
		markers.convertTo(dst, CV_8U);

		img_display(dst);
		dst.copyTo(image);
		ui.tip1->setText(tr("�÷�ˮ���㷨�ָ���ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ���÷�ˮ���㷨�ָ�."));

	}

	//-------------����ͼ�۵ķָ�---------------------//
	void OpenCVPanel::on_GrabCut_triggered()//Grab_Cut
	{
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		if (!(QMessageBox::information(this, tr("��ʾ"), tr("���������ק��������ǰ���ľ�������."), tr("Yes"), tr("No"))))
		{
			flag = 21;
		}
		else
			return;
	}

	//IplImage* K_Means(IplImage* src)//K��ֵ����
	//{
	//	int i, j, k = 0, value;
	//	int nCuster = 2;//����������Ŀ
	//					//��������cluster���Ա�־ÿ��������Ӧ�����ȡֵ��Χ0,1,2...nCuster-1;
	//	CvMat* clusters = cvCreateMat(src->height*src->width, 1, CV_32SC1);//Opencv�ڲ�����cvKMeans2Ҫ��label���������CV_32SC1��
	//	CvMat* samples = cvCreateMat(src->height*src->width, 1, CV_32FC2);//Ҫ��sampels���������CV_32FC2��
	//	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 1);
	//	for (i = 0; i<src->width; i++)
	//		for (j = 0; j<src->height; j++)
	//		{
	//			CvScalar s;
	//			//��ȡͼ��������ص����ͨ��ֵ(BGR)
	//			s.val[0] = (float)cvGet2D(src, j, i).val[0];
	//			s.val[1] = (float)cvGet2D(src, j, i).val[1];
	//			s.val[2] = (float)cvGet2D(src, j, i).val[2];
	//			cvSet2D(samples, k++, 0, s);

	//		}

	//	cvKMeans2(samples, nCuster, clusters, cvTermCriteria(CV_TERMCRIT_ITER, 100, 1.0));
	//	//���ƾ�����ͼ��
	//	k = 0;
	//	float step = 255 / (nCuster - 1);
	//	for (i = 0; i<src->width; i++)
	//	{
	//		for (j = 0; j<src->height; j++)
	//		{
	//			value = clusters->data.i[k++];
	//			CvScalar s;
	//			s.val[0] = 255 - value*step;
	//			cvSet2D(dst, j, i, s);

	//		}
	//	}
	//	return(dst);
	//}
	//-------------���ھ���ķָ�---------------------//
	void OpenCVPanel::on_action_K_Means_triggered()//K-Means
	{
		//if (!image.data)
		//{
		//	QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
		//	return;
		//}
		//Mat src;
		//image.copyTo(rep_img);
		//image.copyTo(src);
		//IplImage imgTmp = src;  //Mat����ת����IplImage����
		//IplImage *tmp = cvCloneImage(&imgTmp);
		//IplImage* newimg = cvCreateImage(cvSize(tmp->width, tmp->height), 8, 1);
		//newimg = K_Means(tmp);
		//Mat dst = cvarrToMat(newimg);
		//img_display(dst);
		//dst.copyTo(dst_image);
		//ui.tip1->setText(tr("��K-Means����ָ���ͼ."));
		//statusBar()->showMessage(tr("�Ѷ�ͼ����K-Means����ָ�."));
	}

	//ģ��C�����㷨׼������:������ݼ����׼������
	double** Standardize(double **data, int row, int col)
	{
		int i, j;
		double *a = new double[col];//����ÿ�����ֵ
		double *b = new double[col];//����ÿ����Сֵ
		double *c = new double[row];//������ʱ�洢����ĳһ��Ԫ��
		for (i = 0; i<col; i++)
		{
			//ȡ�����ݾ���ĸ���Ԫ��
			for (j = 0; j<row; j++)
			{
				c[j] = data[j][i];
			}
			a[i] = c[0];
			b[i] = c[0];
			for (j = 0; j<row; j++)
			{
				//�����ֵ
				if (c[j]>a[i])
					a[i] = c[j];
				//����Сֵ
				if (c[j]<b[i])
					b[i] = c[j];
			}
		}
		for (i = 0; i<row; i++)
		{
			for (j = 0; j<col; j++)
			{
				data[i][j] = (data[i][j] - b[j]) / (a[j] - b[j]);
			}
		}
		delete[]a;
		delete[]b;
		delete[]c;

		return(data);

	}
	//�������������Ⱦ���
	void Initialize(double **u, int k, int row)//kΪ������
	{
		int i, j;
		//��ʼ�����������Ⱦ���
		//srand((unsigned)time(0));
		for (i = 0; i<k; i++)
		{
			for (j = 0; j<row; j++)
			{
				u[i][j] = (double)rand() / RAND_MAX;//������ȡֵ��Χ��Ϊ0~1
			}//rand()��������0~RANDN_MAX֮���һ��α�����
		}
		//���������ݹ�һ��
		double *sum = new double[row];//�����Ⱦ���ÿ�еĺ�
		for (j = 0; j<row; j++)//������row��ʾ��������ƴд���������Ϊ�����Ⱦ������������data�����������row��data�������
		{
			double dj = 0;
			for (i = 0; i<k; i++)
			{
				dj += u[i][j];
			}
			sum[j] = dj;//�����Ⱦ������֮��

		}
		for (i = 0; i<k; i++)
		{
			for (j = 0; j<row; j++)
			{
				u[i][j] /= sum[j];//��һ��
			}
		}

		delete[]sum;
	}
	//��������
	double Update(double **u, double **data, double **center, int row, int col, int k, int m, double **U, double **dis, double *a, double *b)
	{
		int i, j, t;
		double si = 0;//center(i,j) �ķ���
		double sj = 0;//center(i,j) �ķ�ĸ
					  //���������Ⱦ������������ģ��μ�����3.4ʽ
		for (i = 0; i<k; i++)
		{
			for (j = 0; j<row; j++)
			{
				U[i][j] = pow(u[i][j], m);//mΪģ��ָ����Խ��Խģ����ԽСԽ�ӽ�K��ֵ����
			}

		}
		for (j = 0; j<col; j++)
		{
			for (i = 0; i<k; i++)
			{
				for (t = 0; t<row; t++)
				{
					si += U[i][t] * data[t][j];
					sj += U[i][t];
				}
				center[i][j] = si / sj;
			}
		}
		//���������������i�ֱ����е�j�ľ������dis(i,j)
		/*double *a=new double[col];
		double *b=new double[col];
		double **dis=new double *[k];//��������������֮��ľ������

		for(i=0;i<k;i++)
		{
		dis[i]=new double[row];
		}*/
		for (i = 0; i<k; i++)
		{
			for (j = 0; j<col; j++)
				a[j] = center[i][j];//�ݴ�һ��������
			for (j = 0; j<row; j++)
			{
				for (t = 0; t<col; t++)
					b[t] = data[j][t];//�ݴ�һ������
				double d = 0;
				//�����������������֮��ľ���
				for (t = 0; t<col; t++)
				{
					d += (a[t] - b[t])*(a[t] - b[t]);//dΪһ��������һ��������ŷ����þ����ƽ��
				}
				dis[i][j] = sqrt(d);//ŷ����þ���
			}
		}
		//�����Ⱦ���ĸ��£��μ����ĵ�3.5ʽ
		for (i = 0; i<k; i++)
		{
			for (j = 0; j<row; j++)
			{
				double temp = 0;
				for (t = 0; t<k; t++)
				{
					//ģ��ָ��Ϊm
					temp += pow(dis[i][j] / dis[t][j], 2 / (m - 1));
				}
				u[i][j] = 1 / temp;
			}
		}
		//����FCM�ļ�ֵ������Ŀ�꺯�������������Ч�����۲����μ�������3.2ʽ
		double func1 = 0;
		for (i = 0; i<k; i++)
		{
			double func2 = 0;
			for (j = 0; j<row; j++)
			{
				func2 += U[i][j] * (dis[i][j] * dis[i][j]);
			}
			func1 += func2;
		}
		double obj_fun = func1;
		//double obj_fun=1/(1+func1);
		return(obj_fun);
		/*
		//�ڴ��ͷ�
		for(j=0;j<k;j++)
		{
		delete[]U[j];
		}
		delete[]U;
		delete[]a;
		delete[]b;
		for(i=0;i<k;i++)
		delete[]dis[i];
		delete[]dis;
		*/
	}
	//do_FCM��Ҫ������������
	//IplImage* do_FCM(IplImage* src)
	//{

	//	double **data;//���ݾ���һ������һ�У�
	//	double **center;//�������ľ���
	//	double **u;//���������Ⱦ���
	//	int m;//ģ��ָ��
	//	int row = src->width*src->height;//��������
	//	int col = src->nChannels;//������������ͼ��ͨ������
	//							 //cout<<"ͼ��ߴ磺"<<src->width<<'*'<<src->height<<endl;//ͼ��ߴ�ֱ�ӹ�ϵ���������ݵĹ�ģ
	//	int k;//�趨���ֵ����
	//		  //cout<<"������ģ��ָ��m��"<<endl;
	//		  //cin>>m;
	//	m = 2;//mΪģ��ָ����Խ��Խģ����ԽСԽ�ӽ�K��ֵ����
	//		  //cout<<"�����������Ŀk��"<<endl;
	//		  //cin>>k;
	//	k = 2;
	//	int mum;//�㷨���д���
	//			//cout<<"�趨������������"<<endl;
	//			//cin>>mum;
	//	mum = interNum;
	//	//�������н������Ŀ�꺯��ֵ
	//	double *Index = new double[mum];

	//	//FCM�����㷨��ʼ���У���������mum
	//	int i, j, t;
	//	data = new double *[row];
	//	for (i = 0; i<row; i++)
	//	{
	//		data[i] = new double[col];
	//	}
	//	t = 0;
	//	//�����ⲿ�ֵ����ݶ�����Ҫ��Ϊ�˱����ڴ�ľ��ķ�����Update�����а�������
	//	double **U = new double *[k]; //Ϊ�˼��㷽�㶨��Ķ�ά����U,U[i][j]=pow(u[i][j],m);
	//	for (j = 0; j<k; j++)
	//	{
	//		U[j] = new double[row];
	//	}
	//	double *a = new double[col];
	//	double *b = new double[col];
	//	double **dis = new double *[k];//��������������֮��ľ������

	//	for (i = 0; i<k; i++)
	//	{
	//		dis[i] = new double[row];
	//	}
	//	////////////////////////////
	//	//ͼ��������ȡ
	//	for (i = 0; i<src->width; i++)
	//		for (j = 0; j<src->height; j++)
	//		{

	//			for (int t1 = 0; t1<col; t1++)
	//			{
	//				data[t][t1] = (double)cvGet2D(src, j, i).val[t1];
	//			}
	//			t++;
	//		}//��ͼ���е����ظ�ͨ��ǿ��ֵ��������data�У�ÿ������һ��


	//	double eps = 1e-4;
	//	int e = 0;//��������ѭ�����Ʊ���

	//			  //��¼�����޸Ľ�����
	//	int nx = 0;
	//	//���ݼ����׼������
	//	data = Standardize(data, row, col);
	//	/////////////////////��������û����////////////////////////


	//	//�������ļ������Ⱦ�����ڴ����
	//	center = new double *[k];
	//	u = new double *[k];
	//	for (j = 0; j<k; j++)
	//	{
	//		center[j] = new double[col];
	//		u[j] = new double[row];
	//	}
	//	//���������Ⱦ��󣨳�ʼ�����һ����
	//	Initialize(u, k, row);


	//	//Ŀ�꺯������10���޸Ľ���ֹͣ�ôξ����������
	//	for (i = 0; i<mum; i++)
	//	{

	//		//�����������
	//		Index[nx] = Update(u, data, center, row, col, k, m, U, dis, a, b);

	//		if (nx>0 && abs(Index[nx] - Index[nx - 1])<eps)
	//			e++;
	//		else
	//			e = 0;
	//		nx++;
	//		//cout<<nx<<'\t'<<e<<endl;
	//		if (e >= 10)
	//			break;

	//	}


	//	//���ƾ����ͼ��
	//	int t1 = 0;
	//	double step = 255 / (k - 1);
	//	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 1);
	//	for (i = 0; i<src->width; i++)
	//	{
	//		for (j = 0; j<src->height; j++)
	//		{

	//			CvScalar s;
	//			s.val[0] = 0;
	//			for (t = 0; t<k; t++)
	//			{
	//				s.val[0] += (255 - t*step)*u[t][t1];
	//			}
	//			cvSet2D(dst, j, i, s);
	//			t1++;
	//		}

	//	}
	//	return(dst);

	//	//�ڴ��ͷ�
	//	for (j = 0; j<k; j++)
	//	{
	//		delete[]center[j];
	//		delete[]u[j];
	//	}
	//	for (j = 0; j<col; j++)
	//	{
	//		delete[]data[j];

	//	}
	//	delete[]data;
	//	for (j = 0; j<k; j++)
	//	{
	//		delete[]U[j];
	//	}
	//	delete[]U;
	//	delete[]a;
	//	delete[]b;
	//	for (i = 0; i<k; i++)
	//		delete[]dis[i];
	//	delete[]dis;
	//	delete[]center;
	//	delete[]u;
	//	delete[]Index;

	//}
	void OpenCVPanel::on_action_FCM_triggered()//FCM
	{
		flag = 20;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		ui.spinBox->setVisible(true);
		ui.label_thresh->setVisible(true);
		QString str;
		str = "������Ϊ:2����������:";
		str.append(QString::number(interNum));
		ui.label_thresh->setText(str);
		internum1 = interNum;
		//Mat src;
		//image.copyTo(rep_img);
		//image.copyTo(src);
		//IplImage imgTmp = src;  //Mat����ת����IplImage����
		//IplImage *tmp = cvCloneImage(&imgTmp);
		//double t = (double)getTickCount();//��ʱ
		//IplImage*newimg = do_FCM(tmp);
		//t = ((double)getTickCount() - t - interNum) / getTickFrequency();
		//ui.label_thresh_1->setVisible(true);
		//QString str1;
		//str1 = "��ʱ(��).";
		//str1.append(QString::number(t));
		//ui.label_thresh_1->setText(str1);
		////cv::Mat dst(newimg);
		//Mat dst = cvarrToMat(newimg);
		//img_display(dst);
		//dst.copyTo(dst_image);
		ui.tip1->setText(tr("��FCM����ָ���ͼ."));
		statusBar()->showMessage(tr("�Ѷ�ͼ����FCM����ָ�."));
	}

	//-------------���ڻ����ģ�͵ķָ�----------------//
	void OpenCVPanel::on_actionLever_set_triggered()//Lever set
	{
		flag = 21;
		if (!image.data)
		{
			QMessageBox::warning(this, tr("Warning"), tr("���ȴ�ͼƬ."));
			return;
		}
		ui.spinBox->setVisible(true);
		ui.label_thresh->setVisible(true);
		ui.label_thresh_1->setVisible(true);
		QString str;
		str = "��������:";
		str.append(QString::number(interNum));
		ui.label_thresh->setText(str);
		internum1 = interNum;
		//cvWaitKey(100);
		//Mat src;
		//image.copyTo(rep_img);
		//image.copyTo(src);
		//IplImage imgTmp = src;  //Mat����ת����IplImage����
		//IplImage *imgOrig = cvCloneImage(&imgTmp);
		//leverset ls;
		//IplImage *imgGray = ls.getGray(imgOrig);
		//int i, j;
		//CvScalar cur1, cur2, cur;
		//CvSize size = cvSize(imgOrig->width, imgOrig->height);
		//IplImage *imgGauss = cvCreateImage(size, IPL_DEPTH_32F, 1);
		//IplImage *Ix = cvCreateImage(size, IPL_DEPTH_32F, 1);
		//IplImage *Iy = cvCreateImage(size, IPL_DEPTH_32F, 1);
		//IplImage *imgG = cvCreateImage(size, IPL_DEPTH_32F, 1);
		//IplImage *imgU = cvCreateImage(size, IPL_DEPTH_32F, 1);
		//IplImage *imgDisplay = cvCreateImage(size, IPL_DEPTH_8U, 3);

		//double epsilon = 1.5; // the papramater in the definition of smoothed Dirac function
		//double timestep = 7;
		//double mu = 0.2 / timestep;  // coefficient of the internal (penalizing) energy term P
		//							 // Note: the product timestep*mu must be less than 0.25 for stability!
		//double lambda = 5; // coefficient of the weighted length term Lg
		//double alf = 1.5;  // coefficient of the weighted area term Ag
		//				   // Note: Choose a positive(negative) alf if the initial contour is outside(inside) the object.
		//double c0 = 4.0;
		//int w = 5;	//initial boundary

		//cvSmooth(imgGray, imgGauss, CV_GAUSSIAN, 0, 0, 1.5, 0);
		//ls.Sobel(imgGauss, Ix, Iy);
		//for (i = 0; i<size.height; i++)
		//{
		//	for (j = 0; j<size.width; j++)
		//	{
		//		cur1 = cvGet2D(Ix, i, j);
		//		cur2 = cvGet2D(Iy, i, j);
		//		cur.val[0] = 1.0 / (1.0 + cur1.val[0] * cur1.val[0] + cur2.val[0] * cur2.val[0]);
		//		cvSet2D(imgG, i, j, cur);
		//	}
		//}

		////define initial level set function (LSF) as -c0, 0, c0 at points outside, on
		////the boundary, and inside of a region R, respectively.
		//for (i = 0; i<size.height; i++)
		//{
		//	for (j = 0; j<size.width; j++)
		//	{
		//		if (i<w || i>size.height - w - 1 || j<w || j>size.width - w - 1)
		//		{
		//			cur.val[0] = c0;
		//		}
		//		else if (i>w && i<size.height - w - 1 && j>w && j<size.width - w - 1)
		//		{
		//			cur.val[0] = -c0;
		//		}
		//		else cur.val[0] = 0;
		//		// Note: this can be commented out. The intial LSF does NOT necessarily need a zero level set.
		//		cvSet2D(imgU, i, j, cur);
		//	}
		//}

		//ls.ImgDraw(imgOrig, imgU, imgDisplay);
		//namedWindow("LevelSet", WINDOW_AUTOSIZE);
		//cvShowImage("LevelSet", imgDisplay);

		//double t = (double)getTickCount();//��ʱ
		//float count;
		//for (int k = 0; k<interNum; k++)
		//{
		//	ls.EVolution(imgU, imgG, lambda, mu, alf, epsilon, timestep, 1);
		//	if (k % 10 == 0)
		//	{
		//		ls.ImgDraw(imgOrig, imgU, imgDisplay);
		//		cvShowImage("LevelSet", imgDisplay);
		//		cvWaitKey(10);
		//	}
		//	str = "���ڵ�����:";
		//	str.append(QString::number(k + 1));
		//	str.append("��.");
		//	ui.label_thresh->setText(str);
		//}
		//t = ((double)getTickCount() - t - interNum) / getTickFrequency();;
		//QString str1;
		//str1 = "��ʱ(��):";
		//str1.append(QString::number(t));
		//ui.label_thresh_1->setText(str1);
		////cv::Mat dst(imgDisplay);
		//Mat dst = cvarrToMat(imgDisplay);
		//img_display(dst);
		//dst.copyTo(dst_image);
		//cvWaitKey(50);
		//cvDestroyWindow("LevelSet");
		//cvReleaseImage(&imgGauss);
		//cvReleaseImage(&Ix);
		//cvReleaseImage(&Iy);
		//cvReleaseImage(&imgG);
		//cvReleaseImage(&imgU);
		//cvReleaseImage(&imgDisplay);
		//cvReleaseImage(&imgOrig);
		//cvReleaseImage(&imgGray);
		//ui.tip1->setText(tr("��Leverset�ָ���ͼ."));
		//statusBar()->showMessage(tr("�Ѷ�ͼ����Leverset�ָ�."));

	}	

} // namespace general_demo


