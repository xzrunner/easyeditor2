#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/Config.h>
#include <ee0/GameObj.h>

#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
#endif // GAME_OBJ_ECS

class wxTextCtrl;

namespace ee2
{

class WxCompScale9Panel : public ee0::WxCompPanel
{
public:
	WxCompScale9Panel(
		wxWindow* parent, 
#ifdef GAME_OBJ_ECS
		ecsx::World& world,
#endif // GAME_OBJ_ECS
		const ee0::GameObj& obj
	);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void UpdateSpinValue(wxSpinEvent& event);

private:
#ifdef GAME_OBJ_ECS
	ecsx::World&        m_world;
#endif // GAME_OBJ_ECS
	const ee0::GameObj& m_obj;

	wxSpinCtrl* m_width;
	wxSpinCtrl* m_height;

}; // WxCompScale9Panel

}