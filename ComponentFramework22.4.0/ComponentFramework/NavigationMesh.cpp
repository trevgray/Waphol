#include "NavigationMesh.h"
#include "EngineManager.h"
#include "MaterialComponent.h"

#define JC_VORONOI_IMPLEMENTATION
#include "jc_voronoi.h"
#include "jc_voronoi_clip.h"

#include "MeshComponent.h"
#include "ShapeComponent.h"

NavigationMesh::NavigationMesh() {
	//memset(&diagram, 0, sizeof(jcv_diagram));
	//voronoiDiagram = std::make_shared<jcv_diagram>();
}

NavigationMesh::~NavigationMesh() {
	//jcv_diagram_free(&diagram);
}

void NavigationMesh::Initialize(MATH::Vec3 bottomLeftCorner_, MATH::Vec3 topRightCorner_, std::vector<std::string> ignoreActors_) {
	bottomLeftCorner = bottomLeftCorner_;
	topRightCorner = topRightCorner_;
	ignoreActors = ignoreActors_;
}

void NavigationMesh::OnCreate() {
	//JCV Voronoi Generation
	Ref<jcv_diagram> jcvVoronoi;
	jcvVoronoi = std::make_shared<jcv_diagram>();

	jcv_rect* rect = new jcv_rect();

	jcv_point* borders = (jcv_point*)malloc(sizeof(jcv_point) * (size_t)2);
	borders[0].x = bottomLeftCorner.x;
	borders[0].y = bottomLeftCorner.y;

	borders[1].x = topRightCorner.x;
	borders[1].y = topRightCorner.y;

	rect->min = jcv_point(borders[0]);
	rect->max = jcv_point(borders[1]);

	std::vector<Vec3> actorPositions;
	bool skipActor;

	for (auto actor : EngineManager::Instance()->GetActorManager()->GetActorGraph()) {
		if (actor.second->GetComponent<MaterialComponent>() != nullptr) { //everything is an actor, so i just check if it has a texture
			skipActor = false;
			//std::cout << "BRUH" << std::endl;
			for (std::string ignoreActorName : ignoreActors) {
				if (actor.first == ignoreActorName) {
					skipActor = true;
				}
			}
			if (skipActor == true) {
				continue;
			}
			actorPositions.push_back(actor.second->GetComponent<TransformComponent>()->GetPosition());
		}
	}

	int num_points = actorPositions.size();

	jcv_point* points = 0;
	points = (jcv_point*)malloc(sizeof(jcv_point) * (size_t)num_points);

	int pointIterator = 0;

	for (Vec3 point : actorPositions) {
		points[pointIterator].x = point.x;
		points[pointIterator].y = point.y;
		pointIterator++;
	}

	jcv_clipper* clipper = 0;

	jcv_diagram_generate(num_points, points, rect, clipper, jcvVoronoi.get());

	//TURN THE JCV EDGES AND POINTS INTO GRAPH POINTS
	std::vector<Node> graphNodes;
	int nodeLabel = 0;
	//push nodes on the graph and create the graph

	const jcv_edge* edge = jcv_diagram_get_edges(jcvVoronoi.get());
	while (edge) {
		bool edge1Check = false;
		bool edge2Check = false;
		for (Node node : graphNodes) {
			if (std::round(node.GetPos().x) == std::round(edge->pos[0].x) && std::round(node.GetPos().y) == std::round(edge->pos[0].y)) {
				//if (node.GetPos().x < edge->pos[0].x - 0.01 && node.GetPos().x > edge->pos[0].x + 0.01 /*X*/ &&
				//	node.GetPos().x < edge->pos[0].y - 0.01 && node.GetPos().x > edge->pos[0].y + 0.01 /*Y*/) { //fix floating point precision errors
				edge1Check = true;
			}
			if (std::round(node.GetPos().x) == std::round(edge->pos[1].x) && std::round(node.GetPos().y) == std::round(edge->pos[1].y)) {
				//if (node.GetPos().x < edge->pos[1].x - 0.01 && node.GetPos().x > edge->pos[1].x + 0.01 /*X*/ &&
				//	node.GetPos().x < edge->pos[1].y - 0.01 && node.GetPos().x > edge->pos[1].y + 0.01 /*Y*/) {
				edge2Check = true;
			}
		}

		if (edge1Check == false) {
			graphNodes.push_back(Node(nodeLabel, MATH::Vec3(std::round(edge->pos[0].x), std::round(edge->pos[0].y), -40.0f))); //-40 rn
			nodeLabel++;
		}
		if (edge2Check == false) {
			graphNodes.push_back(Node(nodeLabel, MATH::Vec3(std::round(edge->pos[1].x), std::round(edge->pos[1].y), -40.0f)));
			nodeLabel++;
		}
		/*graphNodes.push_back(Node(nodeLabel, MATH::Vec3(edge->pos[0].x, edge->pos[0].y, -40.0f)));
		nodeLabel++;*/

		edge = jcv_diagram_get_next_edge(edge);
	}

	voronoiGraph.OnCreate(graphNodes);

	//Create connections
	edge = jcv_diagram_get_edges(jcvVoronoi.get());
	while (edge) {
		//std::cout << edge->pos[0].x << " " << edge->pos[0].y << " | " << edge->pos[1].x << " " << edge->pos[1].y << std::endl;
		voronoiGraph.AddConnectionVector(Vec3(std::round(edge->pos[0].x), std::round(edge->pos[0].y), 0.0f), Vec3(std::round(edge->pos[1].x), std::round(edge->pos[1].y), 0.0f)); //z does not matter
		voronoiGraph.AddConnectionVector(Vec3(std::round(edge->pos[1].x), std::round(edge->pos[1].y), 0.0f), Vec3(std::round(edge->pos[0].x), std::round(edge->pos[0].y), 0.0f));
		edge = jcv_diagram_get_next_edge(edge);
	}

	//CLEAN UP THE JCV Voronoi Generation
	jcv_diagram_free(jcvVoronoi.get());

	if (clipper) { delete clipper; }
	if (rect) { delete rect; }

	free(points); //used malloc
	free(borders);
}

void NavigationMesh::DebugDraw() {
	GEOMETRY::Sphere nodeSphere(Vec3(), 1.0f);
	for (auto node : EngineManager::Instance()->GetActorManager()->GetNavigationMesh()->GetVoronoiGraph().GetNodes()) {
		std::string nodeName = std::to_string(node.second.GetLabel());
		EngineManager::Instance()->GetActorManager()->AddActor<Actor>(nodeName, new Actor(nullptr));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->DeleteParent();
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->AddComponent<TransformComponent>(nullptr, node.second.GetPos(), Quaternion(1.0f, 0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->AddComponent<MeshComponent>(nullptr, "meshes/Sphere.obj");
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->AddComponent<MaterialComponent>(nullptr, "textures/surface.jpg");
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->AddComponent<ShapeComponent>(nullptr, nodeSphere);
		EngineManager::Instance()->GetActorManager()->GetActor<Actor>(nodeName)->OnCreate();
	}
}
