#ifndef _EASYEDITOR_SCALE_SPRITE_STATE_H_
#define _EASYEDITOR_SCALE_SPRITE_STATE_H_

#include "ArrangeNodeState.h"
#include "NodeCtrlPoint.h"
#include "Sprite.h"

namespace ee2
{

class ScaleNodeState : public ee0::EditOpState
{
public:
	ScaleNodeState(const SprPtr& spr, const NodeCtrlPoint::Node& ctrl_node);

	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

protected:
	virtual void Scale(const sm::vec2& curr);

	virtual void SetScaleTimes(const sm::vec2& st);

protected:
	SprPtr m_spr;

private:
	NodeCtrlPoint::Node m_ctrl_node;

	sm::vec2 m_first_pos, m_first_scale;

}; // ScaleNodeState

}

#endif // _EASYEDITOR_SCALE_SPRITE_STATE_H_