#ifndef _EASYEDITOR_ARRANGE_SPRITE_OP_INL_
#define _EASYEDITOR_ARRANGE_SPRITE_OP_INL_

#include "ArrangeSpriteOP.h"
#include "ArrangeSpriteImpl.h"
#include "Sprite.h"
#include "Symbol.h"
#include "Math.h"
#include "EditPanelImpl.h"
#include "MultiSpritesImpl.h"
#include "PropertySettingPanel.h"
#include "CameraCanvas.h"

#include <sprite2/RVG.h>
#include <sprite2/OrthoCamera.h>

namespace ee2
{

template <typename TBase>
ArrangeSpriteOP<TBase>::ArrangeSpriteOP(wxWindow* wnd, EditPanelImpl* stage, MultiSpritesImpl* sprites_impl, 
	PropertySettingPanel* property/* = NULL*/, EditCMPT* callback/* = NULL*/, 
	const ArrangeNodeCfg& cfg, ArrangeSpriteImpl* impl /*= NULL*/)
	: TBase(wnd, stage, sprites_impl, callback)
{
	if (impl) {
		m_impl = impl;
	} else {
		m_impl = new ArrangeSpriteImpl(wnd, stage, sprites_impl, property, cfg);
	}
}

template <typename TBase>
ArrangeSpriteOP<TBase>::~ArrangeSpriteOP()
{
	delete m_impl;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnKeyDown(int keyCode)
{
	if (TBase::OnKeyDown(keyCode)) return true;
	if (m_impl->OnKeyDown(keyCode)) return true;
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnKeyUp(int keyCode)
{
	if (TBase::OnKeyUp(keyCode)) return true;
	m_impl->OnKeyUp(keyCode);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnChar(int keyCode)
{
	if (TBase::OnChar(keyCode)) return true;
	m_impl->OnChar(keyCode);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnMouseLeftDown(int x, int y)
{
	if (TBase::OnMouseLeftDown(x, y)) return true;
	m_impl->OnMouseLeftDown(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnMouseLeftUp(int x, int y)
{
	if (TBase::OnMouseLeftUp(x, y)) return true;
	m_impl->OnMouseLeftUp(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnMouseRightDown(int x, int y)
{
	if (TBase::OnMouseRightDown(x, y)) return true;
	m_impl->OnMouseRightDown(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnMouseRightUp(int x, int y)
{
	if (TBase::OnMouseRightUp(x, y)) return true;
	m_impl->OnMouseRightUp(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnMouseMove(int x, int y)
{
	if (TBase::OnMouseMove(x, y)) return true;
	m_impl->OnMouseMove(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnMouseDrag(int x, int y)
{
	if (TBase::OnMouseDrag(x, y)) return true;
	m_impl->OnMouseDrag(x, y);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnPopMenuSelected(int type)
{
	if (TBase::OnPopMenuSelected(type)) return true;
	m_impl->OnPopMenuSelected(type);
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::OnDraw() const
{
	if (TBase::OnDraw()) return true;

	float cam_scale = 1;
	ee::CameraCanvas* canvas = static_cast<ee::CameraCanvas*>(m_stage->GetCanvas());
	s2::Camera* cam = canvas->GetCamera();
	if (cam->Type() == s2::CAM_ORTHO2D) {
		cam_scale = static_cast<s2::OrthoCamera*>(cam)->GetScale();
	}

	m_impl->OnDraw(cam_scale);

	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::Clear()
{
	if (TBase::Clear()) return true;
	m_impl->Clear();
	return false;
}

template <typename TBase>
bool ArrangeSpriteOP<TBase>::IsEmpty() const
{
	return m_impl->IsSelectionEmpty();
}

template <typename TBase>
SprPtr ArrangeSpriteOP<TBase>::SelectByPos(const sm::vec2& pos) const
{
	auto spr = m_impl->QueryEditedSprite(pos);
	if (!spr) {
		spr = TBase::SelectByPos(pos);
	}
	return spr;
}

}

#endif // _EASYEDITOR_ARRANGE_SPRITE_OP_INL_
