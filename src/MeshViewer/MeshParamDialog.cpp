#include "MeshParamDialog.h"
#include <QApplication>
#include <QDesktopWidget>

#include "../Common/CommonDefinitions.h"

MeshParamDialog::MeshParamDialog(QWidget* parent /* = 0 */)
	:QDialog(parent)
{
	initDialog();
}

MeshParamDialog::~MeshParamDialog()
{
}

void MeshParamDialog::initDialog()
{
	createWidget();
	createLayout();
}

void MeshParamDialog::createWidget()
{
	create_Basic_Operation_Information_Widget();
}

void MeshParamDialog::createLayout()
{
	tabWidget = new QTabWidget();
	tabWidget->addTab(view_BOI, "QP");

	QGridLayout *layout = new QGridLayout();
	layout->addWidget(tabWidget, 0, 0, 1, 1);
	setLayout(layout);
}

void MeshParamDialog::create_Basic_Operation_Information_Widget()
{
	print_info = new QPushButton("Print Mesh Information");
	leftLabel_BOI = new QLabel("");

	QGridLayout* mainLayout = new QGridLayout(); int main_index = 0;
	mainLayout->addWidget(print_info, main_index, 0, 1, 2); main_index += 1;
	mainLayout->addWidget(leftLabel_BOI, main_index, 0, 1, 40);

	Basic_Operation_And_Information = new QWidget();
	Basic_Operation_And_Information->setLayout(mainLayout);

	view_BOI = new QScrollArea;
	view_BOI->setFocusPolicy(Qt::NoFocus);
	view_BOI->setFrameStyle(QFrame::NoFrame);
	view_BOI->setWidget(Basic_Operation_And_Information);
	view_BOI->setWidgetResizable(true);

	connect(print_info, SIGNAL(clicked()), SIGNAL(print_info_signal()));
}
