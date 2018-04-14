#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/Config.h>

#ifndef GAME_OBJ_ECS
namespace n2 { class CompScale9; }
#else
namespace ecsx { class World; }
namespace e2 { struct CompScale9; }
#endif // GAME_OBJ_ECS

class wxTextCtrl;

namespace ee2
{

class WxCompScale9Panel : public ee0::WxCompPanel
{
public:
	WxCompScale9Panel(
		wxWindow* parent, 
#ifndef GAME_OBJ_ECS
		n2::CompScale9& cscale9
#else
		ecsx::World& world,
		e2::CompScale9& cscale9
#endif // GAME_OBJ_ECS
		);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void UpdateSpinValue(wxSpinEvent& event);

private:
#ifndef GAME_OBJ_ECS
	n2::CompScale9& m_cscale9;
#else
	ecsx::World&    m_world;
	e2::CompScale9& m_cscale9;
#endif // GAME_OBJ_ECS

	wxSpinCtrl* m_width;
	wxSpinCtrl* m_height;

}; // WxCompScale9Panel

}