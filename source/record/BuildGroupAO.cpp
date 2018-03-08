#include "ee2/BuildGroupAO.h"

#include <ee0/CompNodeEditor.h>
#include <ee0/MsgHelper.h>

#include <node0/SceneNode.h>
#include <node2/CompComplex.h>
#include <node2/CompTransform.h>
#include <node2/CompBoundingBox.h>

namespace ee2
{

BuildGroupAO::BuildGroupAO(ee0::SubjectMgr& sub_mgr, const ee0::SelectionSet<n0::SceneNode>& selection)
	: m_sub_mgr(sub_mgr)
	, m_selection(selection)
{
}

void BuildGroupAO::Undo()
{

}

void BuildGroupAO::Redo()
{
	BuildGroup();
}

void BuildGroupAO::BuildGroup()
{
	if (m_selection.IsEmpty()) {
		return;
	}

	auto node = std::make_shared<n0::SceneNode>();

	// complex
	auto& ccomplex = node->AddSharedComp<n2::CompComplex>();
	m_selection.Traverse([&](const n0::SceneNodePtr& node)->bool {
		ccomplex.AddChild(node);
		return true;
	});

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
	ee0::MsgHelper::InsertNode(m_sub_mgr, node, true);
	for (auto& child : children) {
		ee0::MsgHelper::DeleteNode(m_sub_mgr, child);
	}
}

void BuildGroupAO::BreakUpGroup()
{

}

}