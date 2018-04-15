#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/GameObj.h>

#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
#endif // GAME_OBJ_ECS

class wxTextCtrl;

namespace ee2
{

class WxCompMaskPanel : public ee0::WxCompPanel
{
public:
	WxCompMaskPanel(
		wxWindow* parent, 
#ifdef GAME_OBJ_ECS
		ecsx::World& world,
#endif // GAME_OBJ_ECS
		const ee0::GameObj& obj
	);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void OnSetBasePath(wxCommandEvent& event);
	void OnSetMaskPath(wxCommandEvent& event);

private:
	ee0::GameObj CreateNodeFromFile();

private:
#ifdef GAME_OBJ_ECS
	ecsx::World&        m_world;
#endif // GAME_OBJ_ECS
	const ee0::GameObj& m_obj;

	wxTextCtrl *m_base_path, *m_mask_path;

}; // WxCompMaskPanel

}