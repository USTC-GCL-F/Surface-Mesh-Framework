#ifndef MESHPROCESSING_MESHVIEWERWIDGET_H
#define MESHPROCESSING_MESHVIEWERWIDGET_H

#include <QString>
#include <QMessageBox>
#include <QFileDialog>

#include "QGLViewerWidget.h"
#include "MeshDefinition.h"

class MeshViewerWidget : public QGLViewerWidget 
{
	Q_OBJECT
public:
	MeshViewerWidget(QWidget* parent = 0);
	MeshViewerWidget(QGLFormat& _fmt, QWidget* _parent);
	~MeshViewerWidget();
public:
	bool openMesh(const char* filename);
	void initMesh();
	bool saveMesh(const char* filename);
	bool saveScreen(const char* filePath);
	Mesh* mesh_ptr(){return &mesh;};
	Mesh& mesh_ref() { return mesh; };
	const Mesh& mesh_cref() const { return mesh; };
	void updateMesh()
	{
		updateMeshNormals();
		updateMeshCenter();
	};
	virtual void clearAllMesh()
	{
		mesh_vector.clear(); mesh_vector_index = -1;
		mesh.clear();
		//index
		//Indices.clear();
		//VIndices.clear();

		//first_init = true;
		updateGL();
	};

	void set_draw_bbox_ok()
	{
		draw_BBox_OK = draw_BBox_OK? false : true;
		updateGL();
	}
	void set_draw_mesh_boundary_ok()
	{
		draw_mesh_boundary_ok = draw_mesh_boundary_ok? false : true;
		updateGL();
	}
	void printBasicMeshInfo();
signals:
	void loadMeshOK(bool,QString);

protected:
	void updateMeshCenter(); // used by update_mesh().
	void updateMeshNormals(); // used by update_mesh().

protected:
	virtual void draw_scene(int drawmode);
	void draw_scene_mesh(int drawmode);
	
private:
	void draw_mesh_wireframe();
	void draw_mesh_hidden_lines() const;
	void draw_mesh_solidflat() const;
	void draw_mesh_solidsmooth() const;
	void draw_mesh_pointset() const;

protected:
	bool first_init;
	OpenMesh::Vec3d bbMin;
	OpenMesh::Vec3d bbMax;
	Mesh mesh;
	bool draw_BBox_OK;
	bool draw_mesh_boundary_ok;
	std::vector<Mesh> mesh_vector;
	int mesh_vector_index;

private:
	void updateIndices();
public:
	// mesh modes.
	enum { TRIANGLE = 0, QUAD, N_MESH_MODES };
	void setMeshMode(int mm) { mesh_mode_ = mm;}
	int meshMode() const { return mesh_mode_; }
	void checkMeshMode();
private:
	int mesh_mode_;
	//std::vector<unsigned int> Indices;
	//std::vector<unsigned int> VIndices;
};

#endif // MESHPROCESSING_MESHVIEWERWIDGET_H
