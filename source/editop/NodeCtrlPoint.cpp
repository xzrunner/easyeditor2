#include "ee2/NodeCtrlPoint.h"

#include <SM_Calc.h>
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>

namespace ee2
{

void NodeCtrlPoint::GetNodeCtrlPoints(const n0::SceneNode& obj, sm::vec2 objs[8])
{
	auto& cbb = obj.GetUniqueComp<n2::CompBoundingBox>();
	auto& r = cbb.GetSize();

	auto& ctrans = obj.GetUniqueComp<n2::CompTransform>();
	auto& mt = ctrans.GetTrans().GetMatrix();

	// scale
	objs[0] = mt * sm::vec2(r.xmin, r.ymax);
	objs[1] = mt * sm::vec2(r.xmax, r.ymax);
	objs[2] = mt * sm::vec2(r.xmin, r.ymin);
	objs[3] = mt * sm::vec2(r.xmax, r.ymin);
	// shear
	sm::vec2 center = r.Center();
	objs[4] = mt * sm::vec2(center.x, r.ymax);
	objs[5] = mt * sm::vec2(r.xmin, center.y);
	objs[6] = mt * sm::vec2(r.xmax, center.y);
	objs[7] = mt * sm::vec2(center.x, r.ymin);
	// fix for offset
	auto& offset = ctrans.GetTrans().GetOffset();
	sm::vec2 fix = sm::rotate_vector(-offset, ctrans.GetTrans().GetAngle()) + offset;
	for (int i = 0; i < 8; ++i) {
		objs[i] += fix;
	}
}

//void NodeCtrlPoint::GetNodeCtrlPointsExt(const n0::SceneNode& obj, sm::vec2 objs[4])
//{
//	auto& cbb = obj.GetUniqueComp<n2::CompBoundingBox>();
//	auto& r = cbb.GetSize();
//
//	auto& ctrans = obj.GetUniqueComp<n2::CompTransform>();
//	auto& mt = ctrans.GetTrans().GetMatrix();
//
//	// perspective
//	float px = 0, py = 0;
//	px = spr.GetPerspective().x;
//	py = spr.GetPerspective().y;
//	objs[0] = t * sm::vec2(r.xmin+px, r.ymax+py);
//	objs[1] = t * sm::vec2(r.xmax-px, r.ymax-py);
//	objs[2] = t * sm::vec2(r.xmin-px, r.ymin-py);
//	objs[3] = t * sm::vec2(r.xmax+px, r.ymin+py);
//	// fix for offset
//	sm::vec2 offset = spr.GetOffset();
//	sm::vec2 fix = sm::rotate_vector(-offset, spr.GetAngle()) + offset;
//	for (int i = 0; i < 4; ++i) {
//		objs[i] += fix;
//	}
//}

}