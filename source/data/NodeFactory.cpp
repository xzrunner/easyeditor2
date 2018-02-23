#include "ee2/NodeFactory.h"

#include <ee0/CompNodeEditor.h>

#include <sprite2/SymType.h>
#include <sprite2/ImageSymbol.h>
#include <node0/SceneNode.h>
#include <node2/CompImage.h>
#include <node2/CompText.h>
#include <node2/CompMask.h>
#include <node2/CompMesh.h>
#include <node2/CompSprite2.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>

namespace ee2
{

CU_SINGLETON_DEFINITION(NodeFactory);

NodeFactory::NodeFactory()
{
}

n0::SceneNodePtr NodeFactory::Create(const s2::SymPtr& sym)
{
	if (!sym) {
		return nullptr;
	}

	n0::SceneNodePtr node = nullptr;
	if (sym->Type() == s2::SYM_IMAGE)
	{
		auto img_sym = std::dynamic_pointer_cast<s2::ImageSymbol>(sym);

		node = std::make_shared<n0::SceneNode>();

		// image
		auto& cimage = node->AddComponent<n2::CompImage>();
		cimage.SetTexture(img_sym->GetTexture());

		// aabb
		sm::rect sz(img_sym->GetNoTrimedSize().x, img_sym->GetNoTrimedSize().y);
		node->AddComponent<n2::CompBoundingBox>(sz);

		// editor
		node->AddComponent<ee0::CompNodeEditor>();

		auto& tex = img_sym->GetTexture();
	}
	return node;
}

n0::SceneNodePtr NodeFactory::Create(NodeType type)
{
	if (type == NODE_UNKNOWN) {
		return nullptr;
	}

	n0::SceneNodePtr node = std::make_shared<n0::SceneNode>();
	sm::rect sz;

	switch (type)
	{
	case NODE_IMAGE:
		{
			auto& cimage = node->AddComponent<n2::CompImage>();
			sz.Build(100, 100);
		}
		break;
	case NODE_TEXT:
		{
			auto& ctext = node->AddComponent<n2::CompText>();
			auto& tb = ctext.GetText().tb;
			sz.Build(static_cast<float>(tb.width), static_cast<float>(tb.height));
		}
		break;
	case NODE_MASK:
		{
			node->AddComponent<n2::CompMask>();
			sz.Build(100, 100);
		}
		break;
	case NODE_MESH:
		{
			node->AddComponent<n2::CompMesh>();
			sz.Build(100, 100);
		}
		break;

	case NODE_SPRITE2:
		{
			node->AddComponent<n2::CompSprite2>();
			sz.Build(100, 100);
		}
		break;
	}

	// transform
	auto& ctrans = node->AddComponent<n2::CompTransform>();

	// aabb
	auto& cbounding = node->AddComponent<n2::CompBoundingBox>(sz);
	cbounding.Build(ctrans.GetTrans().GetSRT());

	// editor
	node->AddComponent<ee0::CompNodeEditor>();

	return node;
}

}