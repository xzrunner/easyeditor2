#include "ee2/NodeGroupHelper.h"

#include <ee0/MsgHelper.h>
#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#ifndef GAME_OBJ_ECS
#include <ee0/CompNodeEditor.h>
#include <node0/SceneNode.h>
#include <node0/CompComplex.h>
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>
#include <node2/AABBSystem.h>
#else
#include <ee0/CompEntityEditor.h>
#include <entity0/World.h>
#include <entity2/CompComplex.h>
#include <entity2/CompBoundingBox.h>
#include <entity2/SysBoundingBox.h>
#include <entity2/SysTransform.h>
#include <SM_Rect.h>
#endif // GAME_OBJ_ECS

namespace ee2
{

void NodeGroupHelper::BuildGroup(ECS_WORLD_PARAM ee0::SubjectMgr& sub_mgr, 
	                             const std::vector<ee0::GameObjWithPos>& objs)
{
	if (objs.empty()) {
		return;
	}

	// create
#ifndef GAME_OBJ_ECS
	auto obj = std::make_shared<n0::SceneNode>();
#else
	auto obj = world.CreateEntity();
#endif // GAME_OBJ_ECS

	// complex
#ifndef GAME_OBJ_ECS
	auto& ccomplex = obj->AddSharedComp<n0::CompComplex>();
	for (auto& obj : objs) {
		ccomplex.AddChild(obj.GetNode());
	}
#else
	auto& ccomplex = world.GetComponent<e2::CompComplex>(obj);
	for (auto& obj : objs) {
		ccomplex.children->push_back(obj);
	}
#endif // GAME_OBJ_ECS

	// aabb
#ifndef GAME_OBJ_ECS
	auto aabb = n2::AABBSystem::GetBounding(ccomplex);
	auto center = aabb.Center();
	auto& children = ccomplex.GetAllChildren();
	for (auto& child : children)
	{
		auto& ctrans = child->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*child, ctrans.GetTrans().GetPosition() - center);
	}
	aabb.Translate(-center);
	obj->AddUniqueComp<n2::CompBoundingBox>(aabb);
#else
	sm::rect bounding;
	auto& children = *ccomplex.children;
	for (auto& child : children) {
		e2::SysBoundingBox::Combine(bounding, world, child);
	}
	auto center = bounding.Center();
	for (auto& child : children) {
		auto& pos = e2::SysTransform::GetPosition(world, child);
		e2::SysTransform::SetPosition(world, child, pos - center);
	}
	bounding.Translate(-center);
	world.AddComponent<e2::CompBoundingBox>(obj, bounding);
#endif // GAME_OBJ_ECS

	// transform
#ifndef GAME_OBJ_ECS
	auto& ctrans = obj->AddUniqueComp<n2::CompTransform>();
	ctrans.SetPosition(*obj, center);
#else
	e2::SysTransform::SetPosition(world, obj, center);
#endif // GAME_OBJ_ECS

	// editor
#ifndef GAME_OBJ_ECS
	obj->AddUniqueComp<ee0::CompNodeEditor>();
#else
	world.AddComponent<ee0::CompEntityEditor>(obj);
#endif // GAME_OBJ_ECS

	// insert & delete
	sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_CLEAR);

	ee0::MsgHelper::InsertNode(sub_mgr, obj, true);
	for (auto& child : children) {
		ee0::MsgHelper::DeleteNode(sub_mgr, child);
	}
}

void NodeGroupHelper::BreakUp(ECS_WORLD_PARAM ee0::SubjectMgr& sub_mgr, 
	                          const ee0::GameObjWithPos& obj
)
{
#ifndef GAME_OBJ_ECS
	if (!obj.GetNode()->HasSharedComp<n0::CompComplex>()) {
		return;
	}
	auto& ccomplex = obj.GetNode()->GetSharedComp<n0::CompComplex>();
#else
	if (!world.HasComponent<e2::CompComplex>(obj)) {
		return;
	}
	auto& ccomplex = world.GetComponent<e2::CompComplex>(obj);
#endif // GAME_OBJ_ECS

	// children
#ifndef GAME_OBJ_ECS
	auto& children = ccomplex.GetAllChildren();
#else
	auto& children = *ccomplex.children;
#endif // GAME_OBJ_ECS

	// transform
#ifndef GAME_OBJ_ECS
	auto& psrt = obj.GetNode()->GetUniqueComp<n2::CompTransform>().GetTrans().GetSRT();
	for (auto& child : children)
	{
		auto& ctrans = child->GetUniqueComp<n2::CompTransform>();
		auto csrt = psrt * ctrans.GetTrans().GetSRT();
		ctrans.SetSRT(*child, csrt);
	}
#else
	for (auto& child : children) 
	{
		auto pos = e2::SysTransform::GetPosition(world, obj) + e2::SysTransform::GetPosition(world, child);
		e2::SysTransform::SetPosition(world, child, pos);

		auto angle = e2::SysTransform::GetAngle(world, obj) + e2::SysTransform::GetAngle(world, child);
		e2::SysTransform::SetAngle(world, child, angle);

		auto scale = e2::SysTransform::GetScale(world, obj) * e2::SysTransform::GetScale(world, child);
		e2::SysTransform::SetScale(world, child, scale);

		auto shear = e2::SysTransform::GetShear(world, obj) + e2::SysTransform::GetShear(world, child);
		e2::SysTransform::SetShear(world, child, shear);
	}
#endif // GAME_OBJ_ECS

	// insert & delete
	sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_CLEAR);
	for (auto& child : children) {
		ee0::MsgHelper::InsertNode(sub_mgr, child, true);
	}
#ifndef GAME_OBJ_ECS
	ee0::MsgHelper::DeleteNode(sub_mgr, obj.GetNode());
#else
	ee0::MsgHelper::DeleteNode(sub_mgr, obj);
#endif // GAME_OBJ_ECS
}

}