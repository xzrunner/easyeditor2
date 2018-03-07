#pragma once

#include "ee2/ArrangeNodeImpl.h"

#include <ee0/EditOP.h>

#include <memory>

namespace ee0 { class WxStagePage; }

namespace ee2
{

class ArrangeNodeImpl;

class ArrangeNodeOP : public ee0::EditOP
{
public:
	ArrangeNodeOP(ee0::WxStagePage& stage, pt2::Camera& cam,
		const ArrangeNodeCfg& cfg = ArrangeNodeCfg(),
		const std::shared_ptr<ee0::EditOP>& prev_op = nullptr);
	ArrangeNodeOP(ee0::WxStagePage& stage, pt2::Camera& cam,
		std::unique_ptr<ArrangeNodeImpl>& impl,
		const std::shared_ptr<ee0::EditOP>& prev_op = nullptr);

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

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

//	virtual bool IsEmpty() const override;

private:
	n0::SceneNodePtr SelectByPos(const sm::vec2& pos) const;

private:
	pt2::Camera& m_cam;

	std::unique_ptr<ArrangeNodeImpl> m_impl = nullptr;
	
}; // ArrangeNodeOP

}
