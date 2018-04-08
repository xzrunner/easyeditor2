#include "ee2/NodeGroupHelper.h"

#include <ee0/CompNodeEditor.h>
#include <ee0/MsgHelper.h>
#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <node0/SceneNode.h>
#include <node2/CompComplex.h>
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>

namespace ee2
{

void NodeGroupHelper::BuildGroup(ee0::SubjectMgr& sub_mgr, 
	                             const std::vector<n0::NodeWithPos>& objs)
{
	if (objs.empty()) {
		return;
	}

	auto obj = std::make_shared<n0::SceneNode>();

	// complex
	auto& ccomplex = obj->AddSharedComp<n2::CompComplex>();
	for (auto& obj : objs) {
		ccomplex.AddChild(obj.GetNode());
	}

	// aabb
	auto bounding = ccomplex.GetBounding();
	auto center = bounding.Center();
	auto& children = ccomplex.GetAllChildren();
	for (auto& child : children)
	{
		auto& ctrans = child->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*child, ctrans.GetTrans().GetPosition() - center);
	}
	bounding.Translate(-center);
	obj->AddUniqueComp<n2::CompBoundingBox>(bounding);

	// transform
	auto& ctrans = obj->AddUniqueComp<n2::CompTransform>();
	ctrans.SetPosition(*obj, center);

	// editor
	obj->AddUniqueComp<ee0::CompNodeEditor>();

	// insert & delete
	sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_CLEAR);

	ee0::MsgHelper::InsertNode(sub_mgr, obj, true);
	for (auto& child : children) {
		ee0::MsgHelper::DeleteNode(sub_mgr, child);
	}
}

void NodeGroupHelper::BreakUp(ee0::SubjectMgr& sub_mgr, const n0::NodeWithPos& obj)
{
	if (!obj.GetNode()->HasSharedComp<n2::CompComplex>()) {
		return;
	}

	auto& ccomplex = obj.GetNode()->GetSharedComp<n2::CompComplex>();

	// children
	auto children = ccomplex.GetAllChildren();

	// transform
	auto& psrt = obj.GetNode()->GetUniqueComp<n2::CompTransform>().GetTrans().GetSRT();
	for (auto& child : children)
	{
		auto& ctrans = child->GetUniqueComp<n2::CompTransform>();
		auto csrt = psrt * ctrans.GetTrans().GetSRT();
		ctrans.SetSRT(*child, csrt);
	}

	// insert & delete
	sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_CLEAR);
	for (auto& child : children) {
		ee0::MsgHelper::InsertNode(sub_mgr, child, true);
	}
	ee0::MsgHelper::DeleteNode(sub_mgr, obj.GetNode());
}

}