#include <iomanip>
#include <sstream>
#include <algorithm>

#include <QApplication>
#include <QMouseEvent>
#include <QDesktopWidget>

#include "QGLViewerWidget.h"

//#include <GL/glut.h>

#if !defined(M_PI)
#  define M_PI 3.1415926535897932
#endif

const double TRACKBALL_RADIUS = 0.6;

//using namespace Qt;

QGLViewerWidget::QGLViewerWidget(QWidget* _parent)
	: QGLWidget(_parent)
{
	init();
}

QGLViewerWidget::
QGLViewerWidget(QGLFormat& _fmt, QWidget* _parent)
	: QGLWidget(_fmt, _parent)
{
	init();
}

void QGLViewerWidget::init(void)
{
	// qt stuff
	setAttribute(Qt::WA_NoSystemBackground, true);
	setFocusPolicy(Qt::StrongFocus);
	//setAcceptDrops( true );  
	//setCursor(PointingHandCursor);

	// draw mode
	//draw_mode_ = SOLID_SMOOTH;
	mouse_mode_ = Qt::NoButton;

	//initialize
	ModelViewMatrix.resize(16, 0.0);
	ProjectionMatrix.resize(16, 0.0);
	
	Center = OpenMesh::Vec3d(0,0,0);
	Radius = 0.0;

	//SetGlWindoPosFunc();
	//BuildFont(NULL);
	pro_mode_ = PERSPECTIVE;
	set_pro_mode(PERSPECTIVE);
}

QGLViewerWidget::~QGLViewerWidget()
{
	//KillFont();
}

QSize QGLViewerWidget::minimumSizeHint() const
{
	return QSize(10, 10);
}

QSize QGLViewerWidget::sizeHint() const
{
	QRect rect = QApplication::desktop()->screenGeometry();
	return QSize(int(rect.width()*0.8),int(rect.height()*1.0));
}

void QGLViewerWidget::setDefaultMaterial(void)
{
	// material
#if 1
	GLfloat mat_a[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_d[] = { 0.88f, 0.84f, 0.76f, 1.0f };
	GLfloat mat_s[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };

	/*GLfloat mat_a[] = { 0.19225f, 0.19225f, 0.19225f, 1.0f };
	GLfloat mat_d[] = { 0.50754f, 0.50754f, 0.50754f, 1.0f };
	GLfloat mat_s[] = { 0.508273f, 0.508273f, 0.508273f, 1.0f };
	GLfloat shine[] = { 51.2f };*/
#else
	GLfloat mat_a[] = { 0.0, 0.5, 0.75, 1.0 };
	GLfloat mat_d[] = { 0.0, 0.5, 1.0, 1.0 };
	GLfloat mat_s[] = { 0.75, 0.75, 0.75, 1.0 };
	GLfloat emission[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat shine[] = { 120.0 };
#endif

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
}

void QGLViewerWidget::setDefaultLight(void)
{
#if 1
	// lighting
	GLfloat pos1[] = { 10.0f, 10.0f, -10.0f, 0.0f };
	GLfloat pos2[] = { -10.0f, 10.0f, -10.0f, 0.0f };
	GLfloat pos3[] = { 0.0f, 0.0f, 10.0f, 0.0f };
	/*GLfloat col1[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat col2[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat col3[] = { 0.8f, 0.8f, 0.8f, 1.0f };*/
	GLfloat col1[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat col2[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat col3[] = { 0.8f, 0.8f, 0.8f, 1.0f };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, col1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, col1);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, pos2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, col2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, col2);

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_POSITION, pos3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, col3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, col3);
#else
	// lighting
	GLfloat pos3[] = { 0.0f, 0.0f, 10.0f, 0.0f };
	GLfloat col3[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT2, GL_POSITION, pos3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, col3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, col3);
#endif
	
}

void QGLViewerWidget::initializeGL()
{  
	// OpenGL state
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClearColor(0.196, 0.196, 0.196, 1.0);
	glDisable( GL_DITHER );
	glEnable( GL_DEPTH_TEST );
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH); 
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); 

	// Material
	setDefaultMaterial();
	// Lighting
	glLoadIdentity();
	setDefaultLight();  
	
	// scene pos and size
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix_);

	//for initialize all the viewports
	glGetDoublev( GL_MODELVIEW_MATRIX, &ModelViewMatrix[0] );
	
	set_scene_pos(OpenMesh::Vec3d(0.0, 0.0, 0.0), 1.0);
}

void QGLViewerWidget::resizeGL( int _w, int _h )
{
	glViewport(0, 0, _w, _h);
	update_projection_matrix();
	updateGL();
}

void QGLViewerWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	draw_scene(draw_mode_);

	//renderText(1,1,"");
}

void QGLViewerWidget::draw_scene(int drawmode)
{
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixd( &ProjectionMatrix[0] );
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixd( &ModelViewMatrix[0] );
	assert(drawmode < N_DRAW_MODES);
	switch (drawmode)
	{
	case WIRE_FRAME:
		glDisable(GL_LIGHTING);
		//glutWireTeapot(0.5);
		break;
	case SOLID_FLAT:
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		//glutSolidTeapot(0.5);
		break;
	case FLAT_POINTS:
		glEnable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		//glutSolidTeapot(0.5);
		break;
	case HIDDEN_LINES:
		glDisable(GL_LIGHTING);
		break;
	case SOLID_SMOOTH:
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		//glutSolidTeapot(0.5);
		break;
	default:
		break;
	}
}

void QGLViewerWidget::mousePressEvent(QMouseEvent* _event)
{
	//assert(mouse_mode_ < N_MOUSE_MODES);
	last_point_2D_ = _event->pos();
	last_point_ok_ = map_to_sphere( last_point_2D_, last_point_3D_ );
	mouse_mode_ = _event->button();
}

void QGLViewerWidget::mouseMoveEvent(QMouseEvent* _event)
{  
	//assert(mouse_mode_ < N_MOUSE_MODES);

	QPoint newPoint2D = _event->pos(); 

	// enable GL context
	makeCurrent();

	if ( last_point_ok_ ) 
	{
		switch ( mouse_mode_ )
		{
		case Qt::LeftButton:
			rotation(newPoint2D);
			break;
		case Qt::RightButton:
			translation(newPoint2D);
			break;
		default:
			break;
		}
	} // end of if

	// remember this point
	last_point_2D_ = newPoint2D;
	last_point_ok_ = map_to_sphere(last_point_2D_,last_point_3D_);
	
	// trigger redraw
	updateGL();
}

void QGLViewerWidget::mouseReleaseEvent(QMouseEvent* /* _event */ )
{  
	//assert(mouse_mode_ < N_MOUSE_MODES);
	mouse_mode_ = Qt::NoButton;
	last_point_ok_ = false;
}

void QGLViewerWidget::wheelEvent(QWheelEvent* _event)
{
	// Use the mouse wheel to zoom in/out
	float d = -(float)_event->delta() / 120.0 * 0.05 * Radius;
	translate(OpenMesh::Vec3d(0.0, 0.0, d));
	updateGL();
	_event->accept();
}

void QGLViewerWidget::keyPressEvent( QKeyEvent* _event)
{
	switch(_event->key())
	{
	case Qt::Key_Q:
	case Qt::Key_Escape:
		qApp->quit();
		break;
	}
	_event->ignore();
}

void QGLViewerWidget::translation(QPoint p)
{
	double z = - (ModelViewMatrix[ 2]*Center[0] + 
		ModelViewMatrix[6]*Center[1] + 
		ModelViewMatrix[10]*Center[2] + 
		ModelViewMatrix[14]) /
		(ModelViewMatrix[3]*Center[0] + 
		ModelViewMatrix[7]*Center[1] + 
		ModelViewMatrix[11]*Center[2] + 
		ModelViewMatrix[15]);

	double w = width(); double h = height();
	double aspect     =  w/h;
	double near_plane = 0.01 * Radius;
	double top        = tan(fovy()/2.0f*M_PI/180.0f) * near_plane;
	double right      = aspect*top;

	double dx = p.x() - last_point_2D_.x();
	double dy = p.y() - last_point_2D_.y();

	translate(OpenMesh::Vec3d( 2.0*dx/w*right/near_plane*z, 
								-2.0*dy/h*top/near_plane*z, 
								0.0f));
}

void QGLViewerWidget::translate(const OpenMesh::Vec3d& _trans)
{
	// Translate the object by _trans
	// Update modelview_matrix_
	makeCurrent();
	glLoadIdentity();
	glTranslated( _trans[0], _trans[1], _trans[2] );
	glMultMatrixd(&ModelViewMatrix[0]);
	glGetDoublev(GL_MODELVIEW_MATRIX, &ModelViewMatrix[0]);
}

void QGLViewerWidget::rotation(QPoint p)
{
	OpenMesh::Vec3d  newPoint3D;
	bool newPoint_hitSphere = map_to_sphere(p, newPoint3D);
	if (newPoint_hitSphere)
	{
		OpenMesh::Vec3d axis = last_point_3D_ % newPoint3D;
		if (axis.sqrnorm() < 1e-7)
		{
			axis = OpenMesh::Vec3d(1,0,0);
		} 
		else 
		{
			axis.normalize();
		}
		// find the amount of rotation
		OpenMesh::Vec3d d = last_point_3D_ - newPoint3D;
		double t = 0.5*d.norm()/TRACKBALL_RADIUS;
		if (t<-1.0) t=-1.0;
		else if (t>1.0) t=1.0;
		double phi = 2.0 * asin(t);
		double  angle =  phi * 180.0 / M_PI;
		rotate( axis, angle );
	}
}
void QGLViewerWidget::rotate(const OpenMesh::Vec3d& _axis, double _angle)
{
	// Rotate around center center_, axis _axis, by angle _angle
	// Update modelview_matrix_

	OpenMesh::Vec3d t( ModelViewMatrix[0]*Center[0] + 
		ModelViewMatrix[4]*Center[1] +
		ModelViewMatrix[8]*Center[2] + 
		ModelViewMatrix[12],
		ModelViewMatrix[1]*Center[0] + 
		ModelViewMatrix[5]*Center[1] +
		ModelViewMatrix[9]*Center[2] + 
		ModelViewMatrix[13],
		ModelViewMatrix[2]*Center[0] + 
		ModelViewMatrix[6]*Center[1] +
		ModelViewMatrix[10]*Center[2] + 
		ModelViewMatrix[14] );
	
	makeCurrent();
	glLoadIdentity();
	glTranslatef(t[0], t[1], t[2]);
	glRotated( _angle, _axis[0], _axis[1], _axis[2]);
	glTranslatef(-t[0], -t[1], -t[2]); 
	glMultMatrixd(&ModelViewMatrix[0]);
	glGetDoublev(GL_MODELVIEW_MATRIX, &ModelViewMatrix[0]);
}

bool QGLViewerWidget::map_to_sphere(const QPoint& _v2D, OpenMesh::Vec3d& _v3D)
{
	// This is actually doing the Sphere/Hyperbolic sheet hybrid thing,
    // based on Ken Shoemake's ArcBall in Graphics Gems IV, 1993.
    double x =  (2.0*_v2D.x() - width())/width();
    double y = -(2.0*_v2D.y() - height())/height();
    double xval = x;
    double yval = y;
    double x2y2 = xval*xval + yval*yval;

    const double rsqr = TRACKBALL_RADIUS*TRACKBALL_RADIUS;
    _v3D[0] = xval;
    _v3D[1] = yval;
    if (x2y2 < 0.5*rsqr)
	{
        _v3D[2] = sqrt(rsqr - x2y2);
    } 
	else 
	{
        _v3D[2] = 0.5*rsqr/sqrt(x2y2);
    }
    
    return true;
}

void QGLViewerWidget::update_projection_matrix()
{
	makeCurrent();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(PERSPECTIVE == pro_mode_)
	{
		gluPerspective(45.0, (GLfloat) width() / (GLfloat) height(), 0.01*Radius, 100.0*Radius);
	}
	else if(ORTHOTROPIC2D == pro_mode_) //not work for 
	{
		gluOrtho2D(w_left, w_right, w_bottom, w_top);
	}
	else
	{
	}
	
	glGetDoublev(GL_PROJECTION_MATRIX, &ProjectionMatrix[0]);

	//glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix_);
	glMatrixMode(GL_MODELVIEW);
}

void QGLViewerWidget::update_projection_matrix_one_viewer()
{
	makeCurrent();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat) width() / (GLfloat) height(), 0.01*Radius, 100.0*Radius);
	glGetDoublev(GL_PROJECTION_MATRIX, &ProjectionMatrix[0]);
	glMatrixMode(GL_MODELVIEW);
}

void QGLViewerWidget::view_all()
{
	OpenMesh::Vec3d _trans = OpenMesh::Vec3d( -(ModelViewMatrix[0]*Center[0] + 
		ModelViewMatrix[4]*Center[1] +
		ModelViewMatrix[8]*Center[2] + 
		ModelViewMatrix[12]),
		-(ModelViewMatrix[1]*Center[0] + 
		ModelViewMatrix[5]*Center[1] +
		ModelViewMatrix[9]*Center[2] + 
		ModelViewMatrix[13]),
		-(ModelViewMatrix[2]*Center[0] + 
		ModelViewMatrix[6]*Center[1] +
		ModelViewMatrix[10]*Center[2] + 
		ModelViewMatrix[14] +
		3.0*Radius) ) ;

	makeCurrent();
	glLoadIdentity();
	glTranslated( _trans[0], _trans[1], _trans[2] );
	glMultMatrixd(&ModelViewMatrix[0]);
	glGetDoublev(GL_MODELVIEW_MATRIX, &ModelViewMatrix[0]);
}

void QGLViewerWidget::set_scene_pos(const OpenMesh::Vec3d& _center, float _radius)
{
	Center = _center;
	Radius = _radius;
	
	update_projection_matrix();
	view_all();
}