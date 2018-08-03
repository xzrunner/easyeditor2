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
class Camera;
class SpriteSelection;

class ArrangeNodeImpl
{
public:
	ArrangeNodeImpl(ee0::WxStagePage& stage, pt0::Camera& cam, 
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

	// query edited spr, include its extra edit region
	ee0::GameObj QueryEditedNode(const sm::vec2& pos) const;

	bool IsSelectionEmpty() const;

protected:
	virtual void OnDirectionKeyDown(int type);
	virtual void OnSpaceKeyDown();

	virtual void SetRightPopupMenu(wxMenu& menu, int x, int y);

private:
	void OnDeleteKeyDown();

	void UpOneLayer();
	void DownOneLayer();

	sm::vec2 GetNodeOffset(const ee0::GameObj& obj) const;

	bool OnSpriteShortcutKey(int keycode);

private:
	ee0::WxStagePage& m_stage;
	pt0::Camera&      m_cam;

	ee0::SubjectMgrPtr m_sub_mgr;
	ECS_WORLD_SELF_DEF

	ee0::SelectionSet<ee0::GameObjWithPos>& m_selection;

	const ArrangeNodeCfg m_cfg;

	AutoAlign m_align;

	std::unique_ptr<ee0::EditOpState> m_op_state = nullptr;

	sm::vec2 m_left_down_pos, m_right_down_pos;

	mutable float m_ctrl_node_radius;

}; // ArrangeNodeImpl

}