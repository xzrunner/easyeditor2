#include "ee2/WxCompImagePanel.h"

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node0/CompIdentity.h>
#include <node2/CompImage.h>
#else
#include <ee0/CompEntityEditor.h>
#include <entity0/World.h>
#include <entity2/CompImage.h>
#endif // GAME_OBJ_ECS
#include <facade/ResPool.h>
#include <facade/Image.h>

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/filedlg.h>

namespace ee2
{

WxCompImagePanel::WxCompImagePanel(const ur2::Device& dev,
                                   wxWindow* parent,
	                               ECS_WORLD_PARAM
	                               const ee0::GameObj& obj)
	: ee0::WxCompPanel(parent, "Image")
    , m_dev(dev)
	ECS_WORLD_SELF_ASSIGN
	, m_obj(obj)
{
	InitLayout();
	Expand();
}

void WxCompImagePanel::RefreshNodeComp()
{
#ifndef GAME_OBJ_ECS
	auto& cid = m_obj->GetUniqueComp<n0::CompIdentity>();
	m_filepath->SetValue(cid.GetFilepath());
#else
	auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);
	m_filepath->SetValue(*ceditor.filepath);
#endif // GAME_OBJ_ECS
}

void WxCompImagePanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	// filepath
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

#ifndef GAME_OBJ_ECS
		auto& cid = m_obj->GetUniqueComp<n0::CompIdentity>();
		auto& filepath = cid.GetFilepath();
#else
		auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(m_obj);
		auto& filepath = *ceditor.filepath;
#endif // GAME_OBJ_ECS
		sizer->Add(m_filepath = new wxTextCtrl(win, wxID_ANY, filepath,
			wxDefaultPosition, wxSize(180, -1), wxTE_READONLY));

		sizer->AddSpacer(5);

		wxButton* btn = new wxButton(win, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		sizer->Add(btn);
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(WxCompImagePanel::OnSetFilepath));

		pane_sizer->Add(sizer);
	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompImagePanel::OnSetFilepath(wxCommandEvent& event)
{
	std::string filter = "*.png;*.jpg;*.bmp;*.pvr;*.pkm";
	wxFileDialog dlg(this, wxT("Choose image"), wxEmptyString, filter);
	if (dlg.ShowModal() == wxID_OK)
	{
		auto& path = dlg.GetPath();
		auto img = facade::ResPool::Instance().Fetch<facade::Image>(path.ToStdString(), &m_dev);
#ifndef GAME_OBJ_ECS
		auto& cimage = m_obj->GetSharedComp<n2::CompImage>();
		cimage.SetTexture(img->GetTexture());
#else
		auto& cimage = m_world.GetComponent<e2::CompImage>(m_obj);
		cimage.tex = img->GetTexture();
#endif // GAME_OBJ_ECS
	}
}

}