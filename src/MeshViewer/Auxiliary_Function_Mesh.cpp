#include "InteractiveViewerWidget.h"
#include <Eigen\Dense>

void InteractiveViewerWidget::inverse_mesh_connectivity()
{
	Mesh temp_mesh;

	for(Mesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		temp_mesh.add_vertex(mesh.point(v_it));
	}
	Mesh::FaceIter f_it; Mesh::FaceVertexIter fv_it;
	std::vector<Mesh::VertexHandle> face_vertex;
	for(f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
	{
		fv_it = mesh.fv_iter(f_it);
		face_vertex.clear();
		for(fv_it;fv_it;++fv_it)
		{
			face_vertex.push_back(fv_it);
		}
		std::reverse(face_vertex.begin(), face_vertex.end());

		temp_mesh.add_face(face_vertex);
	}

	clearAllMesh();
	temp_mesh.request_vertex_status();
	temp_mesh.request_edge_status();
	temp_mesh.request_face_status();

	temp_mesh.request_face_normals();
	temp_mesh.request_vertex_normals();
	mesh = temp_mesh;
	initMesh();
	setDrawMode(FLAT_POINTS);
	setMouseMode(TRANS);
}

void InteractiveViewerWidget::scale_mesh_using_BBox(int max_len)
{
	double x = bbMax[0] - bbMin[0];
	double y = bbMax[1] - bbMin[1];
	double z = bbMax[2] - bbMin[2];
	double x_dst = max_len; double y_dst = max_len; double z_dst = max_len;
	if(x > y && x > z && x > 0 )
	{
		y_dst = y*x_dst/x;
		z_dst = z*x_dst/x;
	}
	else if(y > z && y > z && y > 0 )
	{
		x_dst = x*y_dst/y;
		z_dst = z*y_dst/y;
	}
	else if(z > x && z > y && z > 0 )
	{
		x_dst = x*z_dst/z;
		y_dst = y*z_dst/z;
	}


	OpenMesh::Vec3d v_pos ;
	for(Mesh::VertexIter vIt = mesh.vertices_begin(); vIt != mesh.vertices_end(); ++vIt)
	{
		v_pos = mesh.point(vIt);
		if(std::abs(x) > 1e-20)
		{
			v_pos[0] = bbMin[0]+(v_pos[0] - bbMin[0])*x_dst/x;
		}
		if(std::abs(y) > 1e-20)
		{
			v_pos[1] = bbMin[1]+(v_pos[1] - bbMin[1])*y_dst/y;
		}
		if(std::abs(z) > 1e-20)
		{
			v_pos[2] = bbMin[2]+(v_pos[2] - bbMin[2])*z_dst/z;
		}
		mesh.set_point(vIt, v_pos);
	}

	first_init = true;
	initMesh();
	setDrawMode(FLAT_POINTS);
	setMouseMode(TRANS);

}

void InteractiveViewerWidget::split_quad_mesh()
{
	if( meshMode() == QUAD )
	{
		Mesh temp_mesh;

		for(Mesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
		{
			temp_mesh.add_vertex(mesh.point(v_it));
		}
		Mesh::FaceIter f_it; Mesh::FaceVertexIter fv_it;
		std::vector<Mesh::VertexHandle> face_vertex;
		std::vector<Mesh::VertexHandle> face0;
		std::vector<Mesh::VertexHandle> face1;
		for(f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
		{
			fv_it = mesh.fv_iter(f_it);
			face_vertex.clear();
			for(fv_it;fv_it;++fv_it)
			{
				face_vertex.push_back(fv_it);
			}
			face0.clear(); face0.push_back(face_vertex[0]);  face0.push_back(face_vertex[1]);  face0.push_back(face_vertex[2]);
			face1.clear(); face1.push_back(face_vertex[2]);  face1.push_back(face_vertex[3]);  face1.push_back(face_vertex[0]);
			temp_mesh.add_face(face0); temp_mesh.add_face(face1);
		}

		clearAllMesh();
		temp_mesh.request_vertex_status();
		temp_mesh.request_edge_status();
		temp_mesh.request_face_status();

		temp_mesh.request_face_normals();
		temp_mesh.request_vertex_normals();
		mesh = temp_mesh;
		initMesh();
		setDrawMode(FLAT_POINTS);
		setMouseMode(TRANS);
	}
}

void InteractiveViewerWidget::transform_mesh(const std::vector<double>& m)
{
	if(mesh.n_vertices() ==0) return;

	Eigen::Matrix4d T; Eigen::Vector4d p; Eigen::Vector4d x; OpenMesh::Vec3d vp;
	T(0,0)  = m[0]; T(0,1)  = m[1]; T(0,2)  = m[2]; T(0,3)  = m[3];
	T(1,0)  = m[4]; T(1,1)  = m[5]; T(1,2)  = m[6]; T(1,3)  = m[7];
	T(2,0)  = m[8]; T(2,1)  = m[9]; T(2,2) = m[10]; T(2,3) = m[11];
	T(3,0) = m[12]; T(3,1) = m[13]; T(3,2) = m[14]; T(3,3) = m[15];
	std::cout << T << "\n";
	for (Mesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		vp = mesh.point(v_it.handle());
		p(0) = vp[0]; p(1) = vp[1]; p(2) = vp[2]; p(3) = 1.0;
		x = T * p; 
		if(std::abs( x(3) ) < 1e-10 ) x(3) = 1.0;
		mesh.set_point(v_it.handle(), OpenMesh::Vec3d( x(0)/x(3), x(1)/x(3), x(2)/x(3) ) );
	}
	first_init = true;
	initMesh();
	setDrawMode(FLAT_POINTS);
	setMouseMode(TRANS);
}

void InteractiveViewerWidget::find_vertex_by_id(int id)
{
	if(id < mesh.n_vertices() && id >= 0 )
	{
		selectedVertex.push_back(id);
		updateGL();
	}
}

void InteractiveViewerWidget::find_vertex_by_valance(int valance)
{
	if( valance > 0 )
	{
		selectedVertex.clear();
		for(Mesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
		{
			if(mesh.valence(v_it) == valance)
			{
				selectedVertex.push_back(v_it.handle().idx() );
			}
		}
		printf("Vertex : %d; %f\n", selectedVertex.size(), double(selectedVertex.size())/mesh.n_vertices() );
		updateGL();
	}
}

void InteractiveViewerWidget::find_face_by_id(int id)
{
	if(id < mesh.n_faces() && id >= 0 )
	{
		selectedFace.push_back(id);
		updateGL();
	}
}

void InteractiveViewerWidget::find_edge_by_id(int id)
{
	if(id < mesh.n_edges() && id >= 0 )
	{
		selectedEdge.push_back(id);
		updateGL();
	}
}

void InteractiveViewerWidget::delete_vertex_valence_four()
{
}

void InteractiveViewerWidget::delete_vertex_valence_three()
{
}

void InteractiveViewerWidget::split_vertex_valence_eight()
{
}