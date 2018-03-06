#ifndef _EASYEDITOR_ARRANGE_SPRITE_IMPL_H_
#define _EASYEDITOR_ARRANGE_SPRITE_IMPL_H_

#include "ArrangeNodeCfg.h"
#include "AutoAlign.h"
#include "NodeCtrlPoint.h"
#include "RightPopupMenu.h"
#include "key_type.h"

#include <wx/menu.h>

namespace ee2
{

class EditPanelImpl;
class MultiSpritesImpl;
class PropertySettingPanel;
class ArrangeNodeState;
class Camera;
class SpriteSelection;

class ArrangeSpriteImpl
{
public:
	ArrangeSpriteImpl(wxWindow* wnd, EditPanelImpl* stage, MultiSpritesImpl* sprites_impl, 
		PropertySettingPanel* property, const ArrangeNodeCfg& cfg = ArrangeNodeCfg());
	~ArrangeSpriteImpl();

	virtual bool OnKeyDown(int keyCode);
	virtual void OnKeyUp(int keyCode);
	virtual void OnChar(int keyCode);

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
	SprPtr QueryEditedSprite(const sm::vec2& pos) const;

	bool IsSelectionEmpty() const;

protected:
	virtual void OnDirectionKeyDown(int type);
	virtual void OnSpaceKeyDown();

	virtual void SetRightPopupMenu(wxMenu& menu, int x, int y);

	virtual ArrangeNodeState* CreateTranslateState(SpriteSelection* selection, const sm::vec2& first_pos) const;
	virtual ArrangeNodeState* CreateRotateState(SpriteSelection* selection, const sm::vec2& first_pos) const;
	virtual ArrangeNodeState* CreateScaleState(const SprPtr& spr, const NodeCtrlPoint::Node& ctrl_node) const;
	virtual ArrangeNodeState* CreateShearState(const SprPtr& spr, const NodeCtrlPoint::Node& ctrl_node) const;
	virtual ArrangeNodeState* CreateOffsetState(const SprPtr& spr) const;
	virtual ArrangeNodeState* CreatePerspectiveState(const SprPtr& spr, const NodeCtrlPoint::Node& ctrl_node) const;

protected:
	void ChangeOPState(ArrangeNodeState* state);

protected:
	SpriteSelection* m_selection;

private:
	void OnDeleteKeyDown();

	void UpOneLayer();
	void DownOneLayer();

	sm::vec2 GetSprOffset(const SprPtr& spr) const;

	bool OnSpriteShortcutKey(int keycode);

public:
	static const float CTRL_NODE_RADIUS;
	static const float MAX_CTRL_NODE_RADIUS;

protected:
	EditPanelImpl* m_stage;

	MultiSpritesImpl* m_sprites_impl;

private:
	wxWindow* m_wnd;

	const ArrangeNodeCfg m_cfg;

	PropertySettingPanel* m_property_panel;

	AutoAlign m_align;

	ArrangeNodeState* m_op_state;

	sm::vec2 m_left_down_pos, m_right_down_pos;

	mutable float m_ctrl_node_radius;

	RightPopupMenu m_popup;

}; // ArrangeSpriteImpl

}

#endif // _EASYEDITOR_ARRANGE_SPRITE_IMPL_H_