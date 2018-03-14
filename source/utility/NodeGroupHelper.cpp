#include "ee2/NodeGroupHelper.h"

#include <ee0/CompNodeEditor.h>
#include <ee0/MsgHelper.h>
#include <ee0/SubjectMgr.h>

#include <node0/SceneNode.h>
#include <node2/CompComplex.h>
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>

namespace ee2
{

void NodeGroupHelper::BuildGroup(ee0::SubjectMgr& sub_mgr, 
	                             const std::vector<n0::NodeWithPos>& nodes)
{
	if (nodes.empty()) {
		return;
	}

	auto node = std::make_shared<n0::SceneNode>();

	// complex
	auto& ccomplex = node->AddSharedComp<n2::CompComplex>();
	for (auto& node : nodes) {
		ccomplex.AddChild(node.node);
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
	node->AddUniqueComp<n2::CompBoundingBox>(bounding);

	// transform
	auto& ctrans = node->AddUniqueComp<n2::CompTransform>();
	ctrans.SetPosition(*node, center);

	// editor
	node->AddUniqueComp<ee0::CompNodeEditor>();

	// insert & delete
	sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_CLEAR);

	ee0::MsgHelper::InsertNode(sub_mgr, node, true);
	for (auto& child : children) {
		ee0::MsgHelper::DeleteNode(sub_mgr, child);
	}
}

void NodeGroupHelper::BreakUp(ee0::SubjectMgr& sub_mgr, const n0::NodeWithPos& node)
{
	if (!node.node->HasSharedComp<n2::CompComplex>()) {
		return;
	}

	auto& ccomplex = node.node->GetSharedComp<n2::CompComplex>();

	// children
	auto children = ccomplex.GetAllChildren();

	// transform
	auto& psrt = node.node->GetUniqueComp<n2::CompTransform>().GetTrans().GetSRT();
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
	ee0::MsgHelper::DeleteNode(sub_mgr, node.node);
}

}