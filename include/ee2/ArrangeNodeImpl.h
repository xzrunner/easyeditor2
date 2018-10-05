#pragma once

#include "ee2/ArrangeNodeCfg.h"
#include "ee2/AutoAlign.h"
#include "ee2/NodeCtrlPoint.h"

#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>

#include <wx/menu.h>

namespace ee0 { class WxStagePage; class EditOpState; }
namespace pt0 { class Camera; }

namespace ee2
{

class MultiSpritesImpl;
class PropertySettingPanel;
class ArrangeNodeState;
class SpriteSelection;

class ArrangeNodeImpl
{
public:
	ArrangeNodeImpl(ee0::WxStagePage& stage, const std::shared_ptr<pt0::Camera>& camera,
		ECS_WORLD_PARAM const ArrangeNodeCfg& cfg = ArrangeNodeCfg());
	virtual ~ArrangeNodeImpl() = default;

	virtual bool OnKeyDown(int key_code);
	virtual void OnKeyUp(int key_code);
	virtual void OnChar(int key_code);

	virtual void OnMouseLeftDown(int x, int y);
	virtual void OnMouseLeftUp(int x, int y);
	virtual void OnMouseRightDown(int x, int y);
	virtual void OnMouseRightUp(int x, int y);
	virtual void OnMouseMove(int x, int y);
	virtual void OnMouseDrag(int x, int y);

	virtual void OnPopMenuSelected(int type);

	virtual void OnDraw(float cam_scale) const;
	virtual void Clear();

	void SetCamera(const std::shared_ptr<pt0::Camera>& camera) {
		m_camera = camera;
	}

	// query edited spr, include its extra edit region
	ee0::GameObj QueryEditedNode(const sm::vec2& pos) const;

	void TraversePosIn(const sm::vec2& pos,
		std::function<bool(const ee0::GameObj&)> func) const;

	bool IsSelectionEmpty() const;

	auto& GetCfgRef() { return m_cfg; }

protected:
	virtual void OnDirectionKeyDown(int type);
	virtual void OnSpaceKeyDown();

	virtual void SetRightPopupMenu(wxMenu& menu, int x, int y);

private:
	bool OnDeleteKeyDown();

	bool UpOneLayer();
	bool DownOneLayer();

	sm::vec2 GetNodeOffset(const ee0::GameObj& obj) const;

	bool OnSpriteShortcutKey(int keycode);

private:
	ee0::WxStagePage& m_stage;

	std::shared_ptr<pt0::Camera> m_camera;

	ee0::SubjectMgrPtr m_sub_mgr;
	ECS_WORLD_SELF_DEF

	ee0::SelectionSet<ee0::GameObjWithPos>& m_selection;

	ArrangeNodeCfg m_cfg;

	AutoAlign m_align;

	std::unique_ptr<ee0::EditOpState> m_op_state = nullptr;

	sm::vec2 m_left_down_pos, m_right_down_pos;

	mutable float m_ctrl_node_radius;

}; // ArrangeNodeImpl

}