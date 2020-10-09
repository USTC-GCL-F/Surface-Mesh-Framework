#include "surfacemeshprocessing.h"
#include "MeshViewer/MainViewerWidget.h"
#include "MeshViewer/Aux_Transform_Mesh_Dialog.h"

SurfaceMeshProcessing::SurfaceMeshProcessing(QWidget *parent)
	: QMainWindow(parent)
{
	viewer = new MainViewerWidget(this);
	setCentralWidget(viewer);
	initWindow();
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
}

SurfaceMeshProcessing::~SurfaceMeshProcessing()
{

}

void SurfaceMeshProcessing::initWindow()
{
	connect(viewer,SIGNAL(setMouseMode_signal_main(int)),this,SLOT(setMouseMode_slot(int)));
	connect(viewer,SIGNAL(setDrawMode_signal_main(int)),this,SLOT(setDrawMode_slot(int)));
}

void SurfaceMeshProcessing::createActions()
{
	openAction = new QAction(tr("&Open"), this);
	openAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/Open.png"));
	openAction->setShortcut(QKeySequence::Open);
	openAction->setStatusTip(tr("Open a mesh file"));
	connect(openAction, SIGNAL(triggered()), viewer, SLOT(open_mesh_query()));

	saveAction = new QAction(tr("&Save"), this);
	saveAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/Save.png"));
	saveAction->setShortcut(QKeySequence::Save);
	saveAction->setStatusTip(tr("Save the mesh to file"));
	connect(saveAction, SIGNAL(triggered()), viewer, SLOT(save_mesh_query()));

	saveAsAction = new QAction(tr("Save &As..."), this);
	saveAsAction->setStatusTip(tr("Save the mesh under a new name"));
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

	exitAction = new QAction(tr("E&xit"), this);
	exitAction->setShortcut(tr("Ctrl+Q"));
	exitAction->setStatusTip(tr("Exit the application"));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

	clearAction = new QAction(("Clear Mesh"), this);
	clearAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/ClearMesh.png"));
	clearAction->setStatusTip(tr("Clear the Current Mesh"));
	connect(clearAction, SIGNAL(triggered()), viewer, SLOT(clear_all_mesh()));

	wireFrameAction = new QAction(tr("&WireFrame"), this);
	wireFrameAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/wire.png"));
	wireFrameAction->setStatusTip(tr("Using wireFrame showing method"));
	wireFrameAction->setCheckable(true);
	wireFrameAction->setChecked(false);
	connect(wireFrameAction, SIGNAL(triggered()), this, SLOT(wireFrameShow()));

	hiddenLinesAction = new QAction(tr("Hidden&Lines"), this);
	hiddenLinesAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/hiddenlines.png"));
	hiddenLinesAction->setStatusTip(tr("Using hiddenlines showing method"));
	hiddenLinesAction->setCheckable(true);
	hiddenLinesAction->setChecked(false);
	connect(hiddenLinesAction, SIGNAL(triggered()), this, SLOT(hiddenLinesShow()));

	solidFlatAction = new QAction(tr("Solid&Flat"), this);
	solidFlatAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/flat.png"));
	solidFlatAction->setStatusTip(tr("Using solidflat showing method"));
	solidFlatAction->setCheckable(true);
	solidFlatAction->setChecked(false);
	connect(solidFlatAction, SIGNAL(triggered()), this, SLOT(solidFlatShow()));

	flatPointsAction = new QAction(tr("Flat&Points"), this);
	flatPointsAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/flatlines.png"));
	flatPointsAction->setStatusTip(tr("Using flatpoints showing method"));
	flatPointsAction->setCheckable(true);
	flatPointsAction->setChecked(false);
	connect(flatPointsAction, SIGNAL(triggered()), this, SLOT(flatPointsShow()));

	solidSmoothAction = new QAction(tr("Solid&Smooth"), this);
	solidSmoothAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/smooth.png"));
	solidSmoothAction->setStatusTip(tr("Using solidsmooth showing method"));
	solidSmoothAction->setCheckable(true);
	solidSmoothAction->setChecked(false);
	connect(solidSmoothAction, SIGNAL(triggered()), this, SLOT(solidSmoothShow()));

	pointSetAction = new QAction(tr("&PointSet"), this);
	pointSetAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/points.png"));
	pointSetAction->setStatusTip(tr("Using pointset showing method"));
	pointSetAction->setCheckable(true);
	pointSetAction->setChecked(false);
	connect(pointSetAction, SIGNAL(triggered()), this, SLOT(pointSetShow()));

	curvatureAction = new QAction("Curvature",this);
	curvatureAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/curvature.png"));
	curvatureAction->setStatusTip(tr("Using Curvature showing method"));
	curvatureAction->setCheckable(true);
	curvatureAction->setChecked(false);
	connect(curvatureAction, SIGNAL(triggered()), this, SLOT(curvatureShow()));

	drawBoundingBox = new QAction("Draw BBox",this);
	drawBoundingBox->setIcon(QIcon(":/SurfaceMeshProcessing/Images/bbox.png"));
	drawBoundingBox->setStatusTip(tr("Draw Bounding Box?"));
	connect(drawBoundingBox, SIGNAL(triggered()), this, SLOT(DrawBBox()));

	drawMeshBoundaryAction = new QAction("Draw Mesh Boundary", this);
	drawMeshBoundaryAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/boundary.png"));
	drawMeshBoundaryAction->setStatusTip(tr("Draw Mesh Boundary?"));
	connect(drawMeshBoundaryAction, SIGNAL(triggered()), this, SLOT(DrawMeshBoundary()));

	updateMeshAction = new QAction(tr("&Update Mesh"), this);
	updateMeshAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/Update.png"));
	updateMeshAction->setStatusTip(tr("Update the normals and the center of the mesh"));
	connect(updateMeshAction, SIGNAL(triggered()), viewer, SLOT(update_mesh()));

	PointPickAction = new QAction(tr("&Select Point"), this);
	PointPickAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/selections.png"));
	PointPickAction->setStatusTip(tr("Select Point of the mesh"));
	PointPickAction->setCheckable(true);
	PointPickAction->setChecked(false);
	connect(PointPickAction, SIGNAL(triggered()), this, SLOT(pointPick()));

	VertexPickAction = new QAction(tr("&Select Vertex"), this);
	VertexPickAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/selectVertices.png"));
	VertexPickAction->setStatusTip(tr("Select Vertex of the mesh"));
	VertexPickAction->setCheckable(true);
	VertexPickAction->setChecked(false);
	connect(VertexPickAction, SIGNAL(triggered()), this, SLOT(vertexPick()));

	FacePickAction = new QAction(tr("&Select Face"), this);
	FacePickAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/selectFaces.png"));
	FacePickAction->setStatusTip(tr("Select Face of the mesh"));
	FacePickAction->setCheckable(true);
	FacePickAction->setChecked(false);
	connect(FacePickAction, SIGNAL(triggered()), this, SLOT(facePick()));

	EdgePickAction = new QAction(tr("&Select Edge"), this);
	EdgePickAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/selectEdges.png"));
	EdgePickAction->setStatusTip(tr("Select Edge of the mesh"));
	EdgePickAction->setCheckable(true);
	EdgePickAction->setChecked(false);
	connect(EdgePickAction, SIGNAL(triggered()), this, SLOT(edgePick()));

	clearSelectedAction = new QAction("Clear Selected",this);
	clearSelectedAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/clear_select.png"));
	clearSelectedAction->setStatusTip(tr("Clear All Selected"));
	connect(clearSelectedAction, SIGNAL(triggered()), viewer, SLOT(clear_all_selected()));

	moveVertexAction = new QAction(tr("&Move Vertex"), this);
	moveVertexAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/move_vertex.png"));
	moveVertexAction->setStatusTip(tr("Move Vertex of the mesh"));
	moveVertexAction->setCheckable(true);
	moveVertexAction->setChecked(false);
	connect(moveVertexAction, SIGNAL(triggered()), this, SLOT(moveVertex()));

	EditUndoAction = new QAction(tr("&Undo"), this);
	EditUndoAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/edit-undo.png"));
	EditUndoAction->setStatusTip(tr("Undo"));
	EditUndoAction->setCheckable(true);
	EditUndoAction->setChecked(false);
	EditUndoAction->setEnabled(false);
	connect(EditUndoAction, SIGNAL(triggered()), this, SLOT(edit_undo()));
	connect(viewer, SIGNAL(set_edit_undo_enable_signal(bool)), this, SLOT(set_edit_undo_enable(bool)));

	EditRedoAction = new QAction(tr("&Redo"), this);
	EditRedoAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/edit-redo.png"));
	EditRedoAction->setStatusTip(tr("Redo"));
	EditRedoAction->setCheckable(true);
	EditRedoAction->setChecked(false);
	EditRedoAction->setEnabled(false);
	connect(EditRedoAction, SIGNAL(triggered()), this, SLOT(edit_redo()));
	connect(viewer, SIGNAL(set_edit_redo_enable_signal(bool)), this, SLOT(set_edit_redo_enable(bool)));

	EdgeCollpaseAction = new QAction(tr("&Collapse Edge"), this);
	EdgeCollpaseAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/topology-edgeCollapse.png"));
	EdgeCollpaseAction->setStatusTip(tr("Select Edge of the mesh"));
	EdgeCollpaseAction->setCheckable(true);
	EdgeCollpaseAction->setChecked(false);
	connect(EdgeCollpaseAction, SIGNAL(triggered()), this, SLOT(edge_collpase()));

	EdgeSplitAction = new QAction(tr("&Split Edge"), this);
	EdgeSplitAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/topology-edgeSplit.png"));
	EdgeSplitAction->setStatusTip(tr("Select Edge of the mesh"));
	EdgeSplitAction->setCheckable(true);
	EdgeSplitAction->setChecked(false);
	connect(EdgeSplitAction, SIGNAL(triggered()), this, SLOT(edge_split()));

	EdgeFlipAction = new QAction(tr("&Flip Edge"), this);
	EdgeFlipAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/topology-edgeFlip.png"));
	EdgeFlipAction->setStatusTip(tr("Select Edge of the mesh"));
	EdgeFlipAction->setCheckable(true);
	EdgeFlipAction->setChecked(false);
	connect(EdgeFlipAction, SIGNAL(triggered()), this, SLOT(edge_flip()));

	saveScreenAction = new QAction("Save Screen",this);
	saveScreenAction->setIcon(QIcon(":/SurfaceMeshProcessing/Images/saveScreen.png"));
	saveScreenAction->setStatusTip(tr("Save Screen"));
	connect(saveScreenAction, SIGNAL(triggered()), viewer, SLOT(saveOpenGLScreen()));

	inverse_mesh_connectivity = new QAction("Inverse Connectivity",this);
	inverse_mesh_connectivity->setStatusTip("Inverse Connectivity");
	connect(inverse_mesh_connectivity, SIGNAL(triggered()), this, SLOT(aux_inverse_mesh_connectivity()));

	scale_mesh_BBox = new QAction("isotropic scale mesh",this);
	scale_mesh_BBox->setStatusTip("isotropic scale mesh");
	connect(scale_mesh_BBox, SIGNAL(triggered()), this, SLOT(aux_scale_mesh_BBox()));

	split_quad_mesh = new QAction("split quad Mesh", this);
	split_quad_mesh->setStatusTip("split quad mesh");
	connect(split_quad_mesh, SIGNAL(triggered()), this, SLOT(aux_split_quad_mesh()));

	transform_mesh = new QAction("Transform Mesh", this);
	transform_mesh->setStatusTip("Transform mesh");
	connect(transform_mesh, SIGNAL(triggered()), this, SLOT(aux_transform_mesh()));

	find_vertex_by_id = new QAction("Find Vertex(ID)", this);
	find_vertex_by_id->setStatusTip("Find Vertex");
	connect(find_vertex_by_id, SIGNAL(triggered()), this, SLOT(aux_find_vertex_by_index()));

	find_face_by_id = new QAction("Find Face(ID)", this);
	find_face_by_id->setStatusTip("Find Face");
	connect(find_face_by_id, SIGNAL(triggered()), this, SLOT(aux_find_face_by_index()));

	find_edge_by_id = new QAction("Find Edge(ID)", this);
	find_edge_by_id->setStatusTip("Find Edge");
	connect(find_edge_by_id, SIGNAL(triggered()), this, SLOT(aux_find_edge_by_index()));

	find_vertex_by_valance = new QAction("Find Vertex(Valence)", this);
	find_vertex_by_valance->setStatusTip("Find Vertex");
	connect(find_vertex_by_valance, SIGNAL(triggered()), this, SLOT(aux_find_vertex_by_valance()));

	delete_vertex_valence_four = new QAction("Delete Vertex(Four)", this);
	delete_vertex_valence_four->setStatusTip("Find Vertex");
	connect(delete_vertex_valence_four, SIGNAL(triggered()), this, SLOT(aux_delete_vertex_valence_four()));

	delete_vertex_valence_three = new QAction("Delete Vertex(Three)", this);
	delete_vertex_valence_three->setStatusTip("Find Vertex");
	connect(delete_vertex_valence_three, SIGNAL(triggered()), this, SLOT(aux_delete_vertex_valence_three()));

	split_vertex_valence_eight = new QAction("Split Vertex(Eight)", this);
	split_vertex_valence_eight->setStatusTip("Find Vertex");
	connect(split_vertex_valence_eight, SIGNAL(triggered()), this, SLOT(aux_split_vertex_valence_eight()));
}

void SurfaceMeshProcessing::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addAction(clearAction);
	fileMenu->addSeparator();
	fileMenu->addAction(saveScreenAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);

	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(wireFrameAction);
	viewMenu->addAction(hiddenLinesAction);
	viewMenu->addAction(solidFlatAction);
	viewMenu->addAction(flatPointsAction);
	viewMenu->addAction(solidSmoothAction);
	viewMenu->addAction(pointSetAction);
	viewMenu->addAction(curvatureAction);
	viewMenu->addSeparator();
	viewMenu->addAction(drawBoundingBox);
	viewMenu->addAction(drawMeshBoundaryAction);

	mouseMenu = menuBar()->addMenu(tr("&Pick"));
	mouseMenu->addAction(PointPickAction);
	mouseMenu->addAction(VertexPickAction);
	mouseMenu->addAction(FacePickAction);
	mouseMenu->addAction(EdgePickAction);
	mouseMenu->addAction(clearSelectedAction);
	mouseMenu->addAction(moveVertexAction);

	Auxiliary_Menu = menuBar()->addMenu("Auxiliary");
	Auxiliary_Menu->addAction(inverse_mesh_connectivity);
	Auxiliary_Menu->addAction(scale_mesh_BBox);
	Auxiliary_Menu->addAction(split_quad_mesh);
	Auxiliary_Menu->addAction(transform_mesh);
	Auxiliary_Menu->addSeparator();
	Auxiliary_Menu->addAction(find_vertex_by_id);
	Auxiliary_Menu->addAction(find_face_by_id);
	Auxiliary_Menu->addAction(find_edge_by_id);
	Auxiliary_Menu->addAction(find_vertex_by_valance);
	Auxiliary_Menu->addSeparator();
	Auxiliary_Menu->addAction(delete_vertex_valence_four);
	Auxiliary_Menu->addAction(delete_vertex_valence_three);
	Auxiliary_Menu->addAction(split_vertex_valence_eight);
	helpMenu = menuBar()->addMenu(tr("&Help"));
}

void SurfaceMeshProcessing::createToolBars()
{
	fileToolBar = addToolBar(tr("&File"));
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);
	fileToolBar->addAction(clearAction);
	fileToolBar->addAction(saveScreenAction);

	viewToolBar = addToolBar(tr("&View"));
	viewToolBar->addAction(wireFrameAction);
	viewToolBar->addAction(hiddenLinesAction);
	viewToolBar->addAction(solidFlatAction);
	viewToolBar->addAction(flatPointsAction);
	viewToolBar->addAction(solidSmoothAction);
	viewToolBar->addAction(pointSetAction);
	viewToolBar->addAction(curvatureAction);

	otherViewBar = addToolBar("OtherView");
	otherViewBar->addAction(drawBoundingBox);
	otherViewBar->addAction(drawMeshBoundaryAction);

	mouseToolBar = addToolBar(tr("MousePick"));
	mouseToolBar->addAction(PointPickAction);
	mouseToolBar->addAction(VertexPickAction);
	mouseToolBar->addAction(FacePickAction);
	mouseToolBar->addAction(EdgePickAction);
	mouseToolBar->addAction(clearSelectedAction);
	mouseToolBar->addAction(moveVertexAction);

	localOperationBar = addToolBar(tr("Local Operation"));
	localOperationBar->addAction(EditUndoAction);
	localOperationBar->addAction(EditRedoAction);
	localOperationBar->addAction(EdgeCollpaseAction);
	localOperationBar->addAction(EdgeSplitAction);
	localOperationBar->addAction(EdgeFlipAction);
}

void SurfaceMeshProcessing::createStatusBar()
{
	statusLabel = new QLabel(tr("no mesh"));
	statusLabel->setAlignment(Qt::AlignHCenter);

	connect(viewer,SIGNAL( haveLoadMesh(QString) ),statusLabel,SLOT( setText(QString) ) );

	statusBar()->addWidget(statusLabel);
}


bool SurfaceMeshProcessing::save()
{
	return true;
}

bool SurfaceMeshProcessing::saveAs()
{
	return true;
}

void SurfaceMeshProcessing::wireFrameShow()
{
	viewer->setDrawMode(InteractiveViewerWidget::WIRE_FRAME);
	viewer->setMouseMode(InteractiveViewerWidget::TRANS);
}

void SurfaceMeshProcessing::hiddenLinesShow()
{
	viewer->setDrawMode(InteractiveViewerWidget::HIDDEN_LINES);
	viewer->setMouseMode(InteractiveViewerWidget::TRANS);
}

void SurfaceMeshProcessing::solidFlatShow()
{
	viewer->setDrawMode(InteractiveViewerWidget::SOLID_FLAT);
	viewer->setMouseMode(InteractiveViewerWidget::TRANS);
}

void SurfaceMeshProcessing::flatPointsShow()
{
	viewer->setDrawMode(InteractiveViewerWidget::FLAT_POINTS);
	viewer->setMouseMode(InteractiveViewerWidget::TRANS);
}

void SurfaceMeshProcessing::solidSmoothShow()
{
	viewer->setDrawMode(InteractiveViewerWidget::SOLID_SMOOTH);
	viewer->setMouseMode(InteractiveViewerWidget::TRANS);
}

void SurfaceMeshProcessing::pointSetShow()
{
	viewer->setDrawMode(InteractiveViewerWidget::POINT_SET);
	viewer->setMouseMode(InteractiveViewerWidget::TRANS);
}

void SurfaceMeshProcessing::curvatureShow()
{
	viewer->setDrawMode(InteractiveViewerWidget::CURVATURE);
	viewer->setMouseMode(InteractiveViewerWidget::TRANS);
}

void SurfaceMeshProcessing::DrawBBox()
{
	viewer->set_show_BBox();
}

void SurfaceMeshProcessing::DrawMeshBoundary()
{
	viewer->set_show_mesh_boundary();
}

void SurfaceMeshProcessing::setAllViewActionChecked(bool b)
{
	wireFrameAction->setChecked(b);
	hiddenLinesAction->setChecked(b);
	solidFlatAction->setChecked(b);
	flatPointsAction->setChecked(b);
	solidSmoothAction->setChecked(b);
	pointSetAction->setChecked(b);
	curvatureAction->setChecked(b);
}

void SurfaceMeshProcessing::pointPick()
{
	setAllViewActionChecked(false);
	setAllMouseActionChecked(false);
	PointPickAction->setChecked(true);
	viewer->setMouseMode(InteractiveViewerWidget::POINTPICK);
}

void SurfaceMeshProcessing::vertexPick()
{
	setAllViewActionChecked(false);
	setAllMouseActionChecked(false);
	VertexPickAction->setChecked(true);
	viewer->setMouseMode(InteractiveViewerWidget::VERTEXPICK);
}
void SurfaceMeshProcessing::edgePick()
{
	setAllViewActionChecked(false);
	setAllMouseActionChecked(false);
	EdgePickAction->setChecked(true);
	viewer->setMouseMode(InteractiveViewerWidget::EDGEPICK);
}
void SurfaceMeshProcessing::facePick()
{
	setAllViewActionChecked(false);
	setAllMouseActionChecked(false);
	FacePickAction->setChecked(true);
	viewer->setMouseMode(InteractiveViewerWidget::FACEPICK);
}
void SurfaceMeshProcessing::moveVertex()
{
	setAllViewActionChecked(false);
	setAllMouseActionChecked(false);
	moveVertexAction->setChecked(true);
	viewer->setMouseMode(InteractiveViewerWidget::MOVE);
}

void SurfaceMeshProcessing::edge_collpase()
{
	setAllViewActionChecked(false);
	setAllMouseActionChecked(false);
	EdgeCollpaseAction->setChecked(true);
	viewer->setMouseMode(InteractiveViewerWidget::EDGECOLLAPSE);
}

void SurfaceMeshProcessing::edge_flip()
{
	setAllViewActionChecked(false);
	setAllMouseActionChecked(false);
	EdgeFlipAction->setChecked(true);
	viewer->setMouseMode(InteractiveViewerWidget::EDGEFLIP);
}

void SurfaceMeshProcessing::edge_split()
{
	setAllViewActionChecked(false);
	setAllMouseActionChecked(false);
	EdgeSplitAction->setChecked(true);
	viewer->setMouseMode(InteractiveViewerWidget::EDGESPLIT);
}

void SurfaceMeshProcessing::setAllMouseActionChecked(bool b)
{
	PointPickAction->setChecked(b);
	VertexPickAction->setChecked(b);
	FacePickAction->setChecked(b);
	EdgePickAction->setChecked(b);
	moveVertexAction->setChecked(b);
	EditUndoAction->setChecked(b);
	EditRedoAction->setChecked(b);
	EdgeCollpaseAction->setChecked(b);
	EdgeSplitAction->setChecked(b);
	EdgeFlipAction->setChecked(b);
}

void SurfaceMeshProcessing::setMouseMode_slot(int mm)
{
	if(mm == InteractiveViewerWidget::TRANS)
	{
		setAllViewActionChecked(false);
		setAllMouseActionChecked(false);
		switch(latestDrawMode)
		{
		case InteractiveViewerWidget::WIRE_FRAME:
			wireFrameAction->setChecked(true);
			break;
		case InteractiveViewerWidget::HIDDEN_LINES:
			hiddenLinesAction->setChecked(true);
			break;
		case InteractiveViewerWidget::SOLID_FLAT:
			solidFlatAction->setChecked(true);
			break;
		case InteractiveViewerWidget::FLAT_POINTS:
			flatPointsAction->setChecked(true);
			break;
		case InteractiveViewerWidget::SOLID_SMOOTH:
			solidSmoothAction->setChecked(true);
			break;
		case InteractiveViewerWidget::POINT_SET:
			pointSetAction->setChecked(true);
			break;
		case InteractiveViewerWidget::CURVATURE:
			curvatureAction->setChecked(true);
			break;
		}
	}
	else
	{
		setAllViewActionChecked(false);
	}
}

void SurfaceMeshProcessing::setDrawMode_slot(int dm)
{
	setAllMouseActionChecked(false);
	latestDrawMode = dm;
}

void SurfaceMeshProcessing::open_mesh_from_main(char* filename)
{
	viewer->openMesh_fromMain(filename);
}

void SurfaceMeshProcessing::edit_undo()
{
	setAllViewActionChecked(false);
	setAllMouseActionChecked(false);
	EditUndoAction->setChecked(true);
	viewer->setDrawMode(InteractiveViewerWidget::FLAT_POINTS);
	viewer->setMouseMode(InteractiveViewerWidget::TRANS);
	viewer->edit_undo();
}

void SurfaceMeshProcessing::set_edit_undo_enable(bool b)
{
	EditUndoAction->setEnabled(b);
}

void SurfaceMeshProcessing::edit_redo()
{
	setAllViewActionChecked(false);
	setAllMouseActionChecked(false);
	EditRedoAction->setChecked(true);
	viewer->setDrawMode(InteractiveViewerWidget::FLAT_POINTS);
	viewer->setMouseMode(InteractiveViewerWidget::TRANS);
	viewer->edit_redo();
}

void SurfaceMeshProcessing::set_edit_redo_enable(bool b)
{
	EditRedoAction->setEnabled(b);
}

void SurfaceMeshProcessing::aux_inverse_mesh_connectivity()
{
	viewer->aux_inverse_mesh_connectivity();
}

void SurfaceMeshProcessing::aux_scale_mesh_BBox()
{
	bool value_Ok = false;
	double val = QInputDialog::getDouble(this, "Scale Mesh","Desired Max Length of XYZ", 1.0, 1e-6, 1e6,1,&value_Ok);
	if(value_Ok)
	{
		viewer->aux_scale_mesh_using_BBox(val);
	}
};

void SurfaceMeshProcessing::aux_split_quad_mesh()
{
	viewer->aux_split_quad_mesh();
}

void SurfaceMeshProcessing::aux_transform_mesh()
{
	aux_transform_mesh_dialog* d = new aux_transform_mesh_dialog();
	int r = d->exec();
	if(r == QDialog::Accepted)
	{
		std::vector<double> tm; d->get_transform_matrix(tm);
		viewer->transform_mesh(tm);
	}
	else if(r == QDialog::Rejected)
	{

	}
	delete d;
}

void SurfaceMeshProcessing::aux_find_vertex_by_index()
{
	bool value_Ok = false;
	int id = QInputDialog::getInt(this, "Find Vertex","Vertex ID", 0 , 0, 1e30, 1,&value_Ok);
	if(value_Ok)
	{
		viewer->aux_find_vertex_by_id(id);
	}
}

void SurfaceMeshProcessing::aux_find_face_by_index()
{
	bool value_Ok = false;
	int id = QInputDialog::getInt(this, "Find Face","Face ID", 0 , 0, 1e30, 1,&value_Ok);
	if(value_Ok)
	{
		viewer->aux_find_face_by_id(id);
	}
}

void SurfaceMeshProcessing::aux_find_edge_by_index()
{
	bool value_Ok = false;
	int id = QInputDialog::getInt(this, "Find Edge","Edge ID", 0 , 0, 1e30, 1,&value_Ok);
	if(value_Ok)
	{
		viewer->aux_find_edge_by_id(id);
	}
}

void SurfaceMeshProcessing::aux_find_vertex_by_valance()
{
	bool value_Ok = false;
	int valance = QInputDialog::getInt(this, "Find Vertex","Vertex Valance", 0 , 0, 1e30, 1,&value_Ok);
	if(value_Ok)
	{
		viewer->aux_find_vertex_by_valance(valance);
	}
}

void SurfaceMeshProcessing::aux_delete_vertex_valence_four()
{
	viewer->aux_delete_vertex_valence_four();
}

void SurfaceMeshProcessing::aux_delete_vertex_valence_three()
{
	viewer->aux_delete_vertex_valence_three();
}

void SurfaceMeshProcessing::aux_split_vertex_valence_eight()
{
	viewer->aux_split_vertex_valence_eight();
}

