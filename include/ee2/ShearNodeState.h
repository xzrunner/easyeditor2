#ifndef _EASYEDITOR_SHEAR_SPRITE_STATE_H_
#define _EASYEDITOR_SHEAR_SPRITE_STATE_H_

#include "ArrangeNodeState.h"
#include "NodeCtrlPoint.h"
#include "Sprite.h"

namespace ee2
{

class ShearNodeState : public ee0::EditOpState
{
public:
	ShearNodeState(const SprPtr& spr, const NodeCtrlPoint::Node& ctrl_node);

	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

private:
	void Shear(const sm::vec2& curr);
	void Shear2(const sm::vec2& curr);

private:
	SprPtr m_spr;

	NodeCtrlPoint::Node m_ctrl_node;

	sm::vec2 m_first_shear;

}; // ShearNodeState

}

#endif // _EASYEDITOR_SHEAR_SPRITE_STATE_H_