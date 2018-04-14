#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#ifndef GAME_OBJ_ECS
#include <node2/CompText.h>
#else
#include <entity2/CompText.h>
#endif // GAME_OBJ_ECS

#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
namespace e2 { struct CompText; }
#endif // GAME_OBJ_ECS
namespace pt2 { class Textbox; class Color; }

class wxTextCtrl;
class wxChoice;
class wxCheckBox;
class wxColourPickerEvent;
class wxColourPickerCtrl;

namespace ee2
{

class WxCompTextPanel : public ee0::WxCompPanel
{
public:
	WxCompTextPanel(
		wxWindow* parent, 
#ifndef GAME_OBJ_ECS
		n2::CompText& ctext,
#else
		ecsx::World& world,
		e2::CompText& ctext,
#endif // GAME_OBJ_ECS
		const ee0::GameObj& obj,
		const ee0::SubjectMgrPtr& sub_mgr
	);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void CommandEventHandler(wxCommandEvent& event);
	void SpinEventHandler(wxSpinEvent& event);

	void RefreshColorBtn(const pt2::Textbox& tb);

	void UpdateBoundingBox(const pt2::Textbox& tb);

	static wxColour ToWxColor(const pt2::Color& col);

private:
#ifndef GAME_OBJ_ECS
	n2::CompText&       m_ctext;
#else
	ecsx::World&        m_world;
	e2::CompText&       m_ctext;
#endif // GAME_OBJ_ECS
	const ee0::GameObj& m_obj;
	ee0::SubjectMgrPtr  m_sub_mgr;

	wxTextCtrl* m_text;

	wxSpinCtrl *m_width, *m_height;

	wxChoice*   m_font_type;
	wxSpinCtrl* m_font_size;
	wxButton*   m_font_color;
	
	wxCheckBox* m_has_edge;
	wxTextCtrl* m_edge_size;
	wxButton*   m_edge_color;

	wxChoice   *m_align_h, *m_align_v;

	wxTextCtrl *m_space_h, *m_space_v;

	wxChoice*   m_over_label;

	wxCheckBox* m_richtext;

}; // WxCompTextPanel

}