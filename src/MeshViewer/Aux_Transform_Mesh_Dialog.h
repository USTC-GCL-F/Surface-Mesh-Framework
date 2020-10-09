#ifndef AUX_TRANSFORM_MESH_H
#define AUX_TRANSFORM_MESH_H

#include <QDialog>
#include <QtGui>
#include <QtWidgets>

class aux_transform_mesh_dialog : public QDialog
{
	Q_OBJECT
private:
	QPushButton *m_OKButton;
	QPushButton *m_CancelButton;
	QLineEdit* m11; QLineEdit* m12; QLineEdit* m13; QLineEdit* m14;
	QLineEdit* m21; QLineEdit* m22; QLineEdit* m23; QLineEdit* m24;
	QLineEdit* m31; QLineEdit* m32; QLineEdit* m33; QLineEdit* m34;
	QLineEdit* m41; QLineEdit* m42; QLineEdit* m43; QLineEdit* m44;

public:
	aux_transform_mesh_dialog(QWidget *parent = 0)
		: QDialog(parent)
	{
		m11 = new QLineEdit("1.0"); m12 = new QLineEdit("0.0"); m13 = new QLineEdit("0.0"); m14 = new QLineEdit("0.0");
		m21 = new QLineEdit("0.0"); m22 = new QLineEdit("1.0"); m23 = new QLineEdit("0.0"); m24 = new QLineEdit("0.0");
		m31 = new QLineEdit("0.0"); m32 = new QLineEdit("0.0"); m33 = new QLineEdit("1.0"); m34 = new QLineEdit("0.0");
		m41 = new QLineEdit("0.0"); m42 = new QLineEdit("0.0"); m43 = new QLineEdit("0.0"); m44 = new QLineEdit("1.0");
		m_OKButton = new QPushButton("OK"); m_CancelButton = new QPushButton("Cancel");
		QGridLayout* t_layout = new QGridLayout();
		t_layout->addWidget(m11, 0, 0, 1, 1); t_layout->addWidget(m12, 0, 1, 1, 1); t_layout->addWidget(m13, 0, 2, 1, 1); t_layout->addWidget(m14, 0, 3, 1, 1);
		t_layout->addWidget(m21, 1, 0, 1, 1); t_layout->addWidget(m22, 1, 1, 1, 1); t_layout->addWidget(m23, 1, 2, 1, 1); t_layout->addWidget(m24, 1, 3, 1, 1);
		t_layout->addWidget(m31, 2, 0, 1, 1); t_layout->addWidget(m32, 2, 1, 1, 1); t_layout->addWidget(m33, 2, 2, 1, 1); t_layout->addWidget(m34, 2, 3, 1, 1);
		t_layout->addWidget(m41, 3, 0, 1, 1); t_layout->addWidget(m42, 3, 1, 1, 1); t_layout->addWidget(m43, 3, 2, 1, 1); t_layout->addWidget(m44, 3, 3, 1, 1);
		t_layout->addWidget(m_CancelButton, 4, 0, 1, 2); t_layout->addWidget(m_OKButton, 4, 2, 1, 2);
		
		this->setLayout(t_layout);
		this->setWindowTitle("Transform Matrix");
		this->resize(300, 150);
		connect(m_OKButton, SIGNAL(clicked()), this, SLOT(accept()));  
		connect(m_CancelButton, SIGNAL(clicked()), this, SLOT(reject())); 
	}
	~aux_transform_mesh_dialog()
	{
		delete m11; delete m12; delete m13; delete m14;
		delete m21; delete m22; delete m23; delete m24; 
		delete m31; delete m32; delete m33; delete m34; 
		delete m41; delete m42; delete m43; delete m44; 
	};

	void get_transform_matrix(std::vector<double>& m)
	{
		m.resize(16);
		m[0]  = m11->text().toDouble(); m[1]  = m12->text().toDouble(); m[2]  = m13->text().toDouble(); m[3]  = m14->text().toDouble();
		m[4]  = m21->text().toDouble(); m[5]  = m22->text().toDouble(); m[6]  = m23->text().toDouble(); m[7]  = m24->text().toDouble();
		m[8]  = m31->text().toDouble(); m[9]  = m32->text().toDouble(); m[10] = m33->text().toDouble(); m[11] = m34->text().toDouble();
		m[12] = m41->text().toDouble(); m[13] = m42->text().toDouble(); m[14] = m43->text().toDouble(); m[15] = m44->text().toDouble();
	}

	virtual void accept() { QDialog::accept(); }
	virtual void reject() { QDialog::reject(); }
};

class aux_generate_rect_mesh_dialog : public QDialog
{
	Q_OBJECT
private:
	QPushButton *m_OKButton;
	QPushButton *m_CancelButton;
	QLabel* left; QLineEdit* left_line; QLabel* bottom; QLineEdit* bottom_line;
	QLabel* width; QLineEdit* width_line; QLabel* height; QLineEdit* height_line;
	QLabel* width_resolution; QLineEdit* width_resolution_line; QLabel* height_resolution; QLineEdit* height_resolution_line;

public:
	aux_generate_rect_mesh_dialog(QWidget *parent = 0)
		: QDialog(parent)
	{
		left = new QLabel("Left"); left_line = new QLineEdit("0.0");
		bottom = new QLabel("Bottom"); bottom_line = new QLineEdit("0.0"); 
		width = new QLabel("Width"); width_line = new QLineEdit("4.0");
		height = new QLabel("Height"); height_line = new QLineEdit("56.0"); 
		width_resolution = new QLabel("Width Sample"); width_resolution_line = new QLineEdit("16");
		width_resolution_line->setValidator(new QIntValidator(1, 100000000000) );
		height_resolution = new QLabel("Height Sample"); height_resolution_line = new QLineEdit("224"); 
		height_resolution_line->setValidator(new QIntValidator(1, 100000000000) );
		m_OKButton = new QPushButton("OK"); m_CancelButton = new QPushButton("Cancel");
		QGridLayout* t_layout = new QGridLayout();
		t_layout->addWidget(left, 0, 0, 1, 1); t_layout->addWidget(left_line, 0, 1, 1, 1); 
		t_layout->addWidget(bottom, 0, 2, 1, 1); t_layout->addWidget(bottom_line, 0, 3, 1, 1); 
		t_layout->addWidget(width, 1, 0, 1, 1); t_layout->addWidget(width_line, 1, 1, 1, 1); 
		t_layout->addWidget(height, 1, 2, 1, 1); t_layout->addWidget(height_line, 1, 3, 1, 1); 
		t_layout->addWidget(width_resolution, 2, 0, 1, 1); t_layout->addWidget(width_resolution_line, 2, 1, 1, 1); 
		t_layout->addWidget(height_resolution, 2, 2, 1, 1); t_layout->addWidget(height_resolution_line, 2, 3, 1, 1); 
		t_layout->addWidget(m_CancelButton, 3, 0, 1, 2); t_layout->addWidget(m_OKButton, 3, 2, 1, 2);

		this->setLayout(t_layout);
		this->setWindowTitle("Rect Mesh");
		this->resize(300, 150);
		connect(m_OKButton, SIGNAL(clicked()), this, SLOT(accept()));  
		connect(m_CancelButton, SIGNAL(clicked()), this, SLOT(reject())); 
	}
	~aux_generate_rect_mesh_dialog()
	{
		delete left; delete left_line; delete bottom; delete bottom_line;
		delete width; delete width_line; delete height; delete height_line;
	};

	void get_rect_para(std::vector<double>& m)
	{
		m.resize(6);
		m[0]  = left_line->text().toDouble(); m[1]  = bottom_line->text().toDouble();
		m[2]  = width_line->text().toDouble(); m[3]  = height_line->text().toDouble();
		m[4]  = width_resolution_line->text().toDouble(); m[5]  = height_resolution_line->text().toDouble();
	}

	virtual void accept() { QDialog::accept(); }
	virtual void reject() { QDialog::reject(); }
};

#endif