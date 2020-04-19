#pragma once

#include "ee2/ArrangeNodeImpl.h"

#include <ee0/EditOP.h>

#include <memory>

namespace ee0 { class WxStagePage; }
ECS_WORLD_DECL

namespace ee2
{

class ArrangeNodeOP : public ee0::EditOP
{
public:
	ArrangeNodeOP(const std::shared_ptr<pt0::Camera>& camera,
		ee0::WxStagePage& stage, ECS_WORLD_PARAM const ArrangeNodeCfg& cfg = ArrangeNodeCfg(),
		const std::shared_ptr<ee0::EditOP>& prev_op = nullptr);
	ArrangeNodeOP(const std::shared_ptr<pt0::Camera>& camera,
		ee0::WxStagePage& stage, ECS_WORLD_PARAM std::unique_ptr<ArrangeNodeImpl>& impl,
		const std::shared_ptr<ee0::EditOP>& prev_op = nullptr
	);

	virtual bool OnKeyDown(int key_code) override;
	virtual bool OnKeyUp(int key_code) override;
	virtual bool OnChar(int key_code) override;

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnPopMenuSelected(int type);

	virtual bool OnDraw(const ur2::Device& dev, ur2::Context& ctx) const override;
	virtual bool Clear() override;

	virtual void SetCamera(const std::shared_ptr<pt0::Camera>& camera) override;

//	virtual bool IsEmpty() const override;

private:
	ee0::GameObj SelectByPos(const sm::vec2& pos) const;

protected:
	std::unique_ptr<ArrangeNodeImpl> m_impl = nullptr;

private:
	ECS_WORLD_SELF_DEF

}; // ArrangeNodeOP

}
