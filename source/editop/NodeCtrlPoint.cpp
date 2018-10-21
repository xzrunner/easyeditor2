#include "ee2/NodeCtrlPoint.h"

#include <SM_Calc.h>
#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#else
#include <entity0/World.h>
#include <entity2/CompBoundingBox.h>
#include <entity2/CompTransform.h>
#include <entity2/SysTransform.h>
#endif // GAME_OBJ_ECS

namespace ee2
{

void NodeCtrlPoint::GetNodeCtrlPoints(ECS_WORLD_PARAM const ee0::GameObj& obj, sm::vec2 pos[8])
{
#ifndef GAME_OBJ_ECS
	auto& cbb = obj->GetUniqueComp<n2::CompBoundingBox>();
	auto& r = cbb.GetSize();

	auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
	auto& mt = ctrans.GetTrans().GetMatrix();
#else
	auto& cbb = world.GetComponent<e2::CompBoundingBox>(obj);
	auto& r = cbb.rect;

	auto& mt = world.GetComponent<e2::CompLocalMat>(obj).mat;
#endif // GAME_OBJ_ECS

	// scale
	pos[0] = mt * sm::vec2(r.xmin, r.ymax);
	pos[1] = mt * sm::vec2(r.xmax, r.ymax);
	pos[2] = mt * sm::vec2(r.xmin, r.ymin);
	pos[3] = mt * sm::vec2(r.xmax, r.ymin);
	// shear
	sm::vec2 center = r.Center();
	pos[4] = mt * sm::vec2(center.x, r.ymax);
	pos[5] = mt * sm::vec2(r.xmin, center.y);
	pos[6] = mt * sm::vec2(r.xmax, center.y);
	pos[7] = mt * sm::vec2(center.x, r.ymin);
	// fix for offset
#ifndef GAME_OBJ_ECS
	auto& offset = ctrans.GetTrans().GetOffset();
	auto angle = ctrans.GetTrans().GetAngle();
#else
	sm::vec2 offset = e2::SysTransform::GetOffset(world, obj);
	float angle = e2::SysTransform::GetAngle(world, obj);
#endif // GAME_OBJ_ECS
	sm::vec2 fix = sm::rotate_vector(-offset, angle) + offset;
	for (int i = 0; i < 8; ++i) {
		pos[i] += fix;
	}
}

//void NodeCtrlPoint::GetNodeCtrlPointsExt(const n0::SceneNode& obj, sm::vec2 pos[4])
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
//	pos[0] = t * sm::vec2(r.xmin+px, r.ymax+py);
//	pos[1] = t * sm::vec2(r.xmax-px, r.ymax-py);
//	pos[2] = t * sm::vec2(r.xmin-px, r.ymin-py);
//	pos[3] = t * sm::vec2(r.xmax+px, r.ymin+py);
//	// fix for offset
//	sm::vec2 offset = spr.GetOffset();
//	sm::vec2 fix = sm::rotate_vector(-offset, spr.GetAngle()) + offset;
//	for (int i = 0; i < 4; ++i) {
//		pos[i] += fix;
//	}
//}

}