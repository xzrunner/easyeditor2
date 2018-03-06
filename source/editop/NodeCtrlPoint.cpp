#include "ee2/NodeCtrlPoint.h"

#include <SM_Calc.h>
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>

namespace ee2
{

void NodeCtrlPoint::GetNodeCtrlPoints(const n0::SceneNode& node, sm::vec2 nodes[8])
{
	auto& cbb = node.GetUniqueComp<n2::CompBoundingBox>();
	auto& r = cbb.GetSize();

	auto& ctrans = node.GetUniqueComp<n2::CompTransform>();
	auto& mt = ctrans.GetTrans().GetMatrix();

	// scale
	nodes[0] = mt * sm::vec2(r.xmin, r.ymax);
	nodes[1] = mt * sm::vec2(r.xmax, r.ymax);
	nodes[2] = mt * sm::vec2(r.xmin, r.ymin);
	nodes[3] = mt * sm::vec2(r.xmax, r.ymin);
	// shear
	sm::vec2 center = r.Center();
	nodes[4] = mt * sm::vec2(center.x, r.ymax);
	nodes[5] = mt * sm::vec2(r.xmin, center.y);
	nodes[6] = mt * sm::vec2(r.xmax, center.y);
	nodes[7] = mt * sm::vec2(center.x, r.ymin);
	// fix for offset
	auto& offset = ctrans.GetTrans().GetOffset();
	sm::vec2 fix = sm::rotate_vector(-offset, ctrans.GetTrans().GetAngle()) + offset;
	for (int i = 0; i < 8; ++i) {
		nodes[i] += fix;
	}
}

//void NodeCtrlPoint::GetNodeCtrlPointsExt(const n0::SceneNode& node, sm::vec2 nodes[4])
//{
//	auto& cbb = node.GetUniqueComp<n2::CompBoundingBox>();
//	auto& r = cbb.GetSize();
//
//	auto& ctrans = node.GetUniqueComp<n2::CompTransform>();
//	auto& mt = ctrans.GetTrans().GetMatrix();
//
//	// perspective
//	float px = 0, py = 0;
//	px = spr.GetPerspective().x;
//	py = spr.GetPerspective().y;
//	nodes[0] = t * sm::vec2(r.xmin+px, r.ymax+py);
//	nodes[1] = t * sm::vec2(r.xmax-px, r.ymax-py);
//	nodes[2] = t * sm::vec2(r.xmin-px, r.ymin-py);
//	nodes[3] = t * sm::vec2(r.xmax+px, r.ymin+py);
//	// fix for offset
//	sm::vec2 offset = spr.GetOffset();
//	sm::vec2 fix = sm::rotate_vector(-offset, spr.GetAngle()) + offset;
//	for (int i = 0; i < 4; ++i) {
//		nodes[i] += fix;
//	}
//}

}