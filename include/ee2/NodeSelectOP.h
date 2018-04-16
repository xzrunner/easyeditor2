#pragma once

#include "ee2/CamControlOP.h"

#include <ee0/NodeSelectOP.h>

#include <SM_Rect.h>

#include <memory>

namespace ee0 { class EditOpState; class WxStagePage; }
ECS_WORLD_DECL

namespace ee2
{

class CamControlOP;

class NodeSelectOP : public ee0::NodeSelectOP
{
public:
	NodeSelectOP(ECS_WORLD_PARAM ee0::WxStagePage& stage);
	virtual ~NodeSelectOP() = default;

	virtual bool OnKeyDown(int key_code) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw() const override;

private:
	virtual ee0::GameObj QueryByPos(int screen_x, int screen_y) const override;
	virtual void QueryByRect(const sm::ivec2& p0, const sm::ivec2& p1,
		bool contain, std::vector<ee0::GameObj>& result) const;

	ee0::GameObj QueryByPos(const ee0::GameObj& obj, const sm::vec2& pos) const;
	void QueryByRect(const ee0::GameObj& obj, const sm::rect& rect,
		bool contain, std::vector<ee0::GameObj>& result) const;

	void BuildGroup();
	void BreakUpGroup();

private:
	ECS_WORLD_SELF_DEF

	std::unique_ptr<ee0::EditOpState> m_draw_state = nullptr;
	mutable bool m_draw_state_disable;

}; // NodeSelectOP

}