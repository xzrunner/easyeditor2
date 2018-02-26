#pragma once

#include <sprite2/typedef.h>
#include <node0/typedef.h>

#include <cu/cu_macro.h>

namespace ee2
{

enum NodeType
{
	NODE_UNKNOWN = 0,

	NODE_IMAGE = 1,
	NODE_TEXT,
	NODE_MASK,
	NODE_MESH,
	NODE_SCALE9,

	NODE_SPRITE2,
};

class NodeFactory
{
public:
	n0::SceneNodePtr Create(NodeType type);

	CU_SINGLETON_DECLARATION(NodeFactory);

}; // NodeFactory

}