#ifndef MESHPROCESSING_QGLVIEWERWIDGET_HH
#define MESHPROCESSING_QGLVIEWERWIDGET_HH

#include <OpenMesh/Core/Geometry/VectorT.hh>
#include "OpenglHeaders.h"

#include <vector>

class QGLViewerWidget : public QGLWidget 
{
	Q_OBJECT
public:
	// Constructors.
	QGLViewerWidget( QWidget* _parent=0 );
	QGLViewerWidget( QGLFormat& _fmt, QWidget* _parent=0 );
	// Destructor.
	virtual ~QGLViewerWidget();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;
private:
	void init(void);
public:
	/* Sets the center and size of the whole scene. 
	   The _center is used as fixpoint for rotations and for adjusting
	   the camera/viewer (see view_all()). */
	void set_scene_pos(const OpenMesh::Vec3d& _center, float _radius);  

	/* view the whole scene: the eye point is moved far enough from the
	   center so that the whole scene is visible. */
	void view_all();

	float radius() const { return Radius; }
	const OpenMesh::Vec3d& center() const { return Center; }

	const GLdouble* modelview_matrix() const  { return &ModelViewMatrix[0]; }
	void reset_modelview_matrix()
	{
		makeCurrent();
		glLoadIdentity();
		glGetDoublev(GL_MODELVIEW_MATRIX, &ModelViewMatrix[0]);
		updateGL();
	}
	enum PROJECTION_MODE
	{
		PERSPECTIVE = 0,
		ORTHOTROPIC2D,
		N_PRO_MODE
	};
	const GLdouble* projection_matrix() const { return &ProjectionMatrix[0]; }

	float fovy() const { return 45.0f; }
public:
	// draw modes.
	enum { WIRE_FRAME=0, HIDDEN_LINES, SOLID_FLAT, FLAT_POINTS, SOLID_SMOOTH, POINT_SET, CURVATURE ,VORONOI_DIAGRAM, N_DRAW_MODES };
	void setDrawMode(int dm)
	{ 
		draw_mode_ = dm; updateGL();
		emit setDrawMode_signal(dm);
	}
	int draw_mode() const { return draw_mode_; }

	void set_pro_mode(PROJECTION_MODE pm)
	{ 
		pro_mode_ = pm;
		update_projection_matrix();
		view_all();
		updateGL();
	}
	int pro_mode() const { return pro_mode_; }
signals:
	void setDrawMode_signal(int);

protected:
	// draw the scene: will be called by the painGL() method.
	virtual void draw_scene(int drawmode);
	void setDefaultMaterial(void);
	void setDefaultLight(void);
	void render_text(int x, int y, const QString& str)
	{
		glColor3f(1.0,1.0,0.0);
		renderText(x, y, str);
	}
	void render_text(double x, double y, double z, const QString& str)
	{
		glColor3f(1.0,1.0,0.0);
		renderText(x, y, z , str );
	}
	void render_text(double x, double y, double z, const QString& str, double c1, double c2, double c3)
	{
		glColor3f(c1,c2,c3);
		renderText(x, y, z , str );
	}

private: // inherited
	// initialize OpenGL states (triggered by Qt)
	void initializeGL();
	// draw the scene (triggered by Qt)
	void paintGL();
	// handle resize events (triggered by Qt)
	void resizeGL(int w, int h);
public slots:
	void UpdateGL_Slot()
	{
		//printf("update GL\n");
		updateGL();
	}
	void rotate_mesh_by_angle_slot(double angle)
	{
		rotate(OpenMesh::Vec3d(0,1,0), angle);
		updateGL();
	}
	void set_ORTHOTROPIC2D_slot(double w_left_, double w_right_, double w_bottom_, double w_top_)
	{
		/*w_left = w_left_; w_right = w_right_; w_top = w_top_; w_bottom = w_bottom_;
		set_pro_mode(ORTHOTROPIC2D);*/

		OpenMesh::Vec3d c((w_left_ + w_right_)*0.5, (w_left_ + w_right_)*0.5, 0.0);
		set_scene_pos(c, c.norm());
	}
protected:
	// Qt mouse events
	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);
	virtual void mouseMoveEvent(QMouseEvent*);
	virtual void wheelEvent(QWheelEvent*);
	virtual void keyPressEvent(QKeyEvent*);
private:
	// updates projection matrix
	void update_projection_matrix();
	void update_projection_matrix_one_viewer();
	// translate the scene and update modelview matrix
	void translate(const OpenMesh::Vec3d& _trans);
	// rotate the scene (around its center) and update modelview matrix
	void rotate(const OpenMesh::Vec3d& _axis, double _angle);

	void translation(QPoint p);
	void rotation(QPoint p);
protected:
	int draw_mode_;
	PROJECTION_MODE pro_mode_;
	double w_left; double w_right; double w_top; double w_bottom;
	int mouse_mode_;
	/*OpenMesh::Vec3d  center_;
	float            radius_;
	GLdouble projection_matrix_[16];
	GLdouble modelview_matrix_[16];*/

	OpenMesh::Vec3d Center;
	double Radius;
	std::vector<double> ProjectionMatrix;
	std::vector<double> ModelViewMatrix;

	// virtual trackball: map 2D screen point to unit sphere
	bool map_to_sphere(const QPoint& _point, OpenMesh::Vec3d& _result);
	
	QPoint           last_point_2D_;
	OpenMesh::Vec3d  last_point_3D_;
	bool             last_point_ok_;
};

#endif // MESHPROCESSING_QGLVIEWERWIDGET_HH
