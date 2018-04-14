#include "ee2/WxCompMaskPanel.h"

#ifndef GAME_OBJ_ECS
#include <ee0/CompNodeEditor.h>
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <ns/NodeFactory.h>
#else
#include <ecsx/World.h>
#include <entity2/CompBoundingBox.h>
#include <es/EntityFactory.h>
#include <ee0/CompEntityEditor.h>
#endif // GAME_OBJ_ECS

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/filedlg.h>

namespace ee2
{

WxCompMaskPanel::WxCompMaskPanel(wxWindow* parent, 
#ifndef GAME_OBJ_ECS
	                             n2::CompMask& cmask, 
#else
	                             ecsx::World& world,
                                 e2::CompMask& cmask,
#endif // GAME_OBJ_ECS
	                             const ee0::GameObj& obj)
	: ee0::WxCompPanel(parent, "Mask")
#ifdef GAME_OBJ_ECS
	, m_world(world)
#endif // GAME_OBJ_ECS
	, m_cmask(cmask)
	, m_obj(obj)
{
	InitLayout();
	Expand();
}

void WxCompMaskPanel::RefreshNodeComp()
{
#ifndef GAME_OBJ_ECS
	auto& base = m_cmask.GetBaseNode();
	if (base)
	{
		auto& ceditor = base->GetUniqueComp<ee0::CompNodeEditor>();
		m_base_path->SetValue(ceditor.GetFilepath());
	}
	auto& mask = m_cmask.GetMaskNode();
	if (mask)
	{
		auto& ceditor = mask->GetUniqueComp<ee0::CompNodeEditor>();
		m_mask_path->SetValue(ceditor.GetFilepath());
	}
#else
	auto& base = m_cmask.base;
	if (!base.IsNull()) 
	{
		auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(base);
		m_base_path->SetValue(ceditor.filepath);
	}
	auto& mask = m_cmask.mask;
	if (!mask.IsNull())
	{
		auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(mask);
		m_mask_path->SetValue(ceditor.filepath);
	}
#endif // GAME_OBJ_ECS
}

void WxCompMaskPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	// base filepath
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Base ")));

		std::string path;
#ifndef GAME_OBJ_ECS
		if (auto obj = m_cmask.GetBaseNode()) 
		{
			auto& ceditor = obj->GetUniqueComp<ee0::CompNodeEditor>();
			path = ceditor.GetFilepath();
		}
#else
		if (!m_cmask.base.IsNull())
		{
			auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(m_cmask.base);
			path = ceditor.filepath;
		}
#endif // GAME_OBJ_ECS
		sizer->Add(m_base_path = new wxTextCtrl(win, wxID_ANY, path,
			wxDefaultPosition, wxSize(180, -1), wxTE_READONLY));

		sizer->AddSpacer(5);

		wxButton* btn = new wxButton(win, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		sizer->Add(btn);
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(WxCompMaskPanel::OnSetBasePath));

		pane_sizer->Add(sizer);
	}
	// mask filepath
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Mask ")));

		std::string path;
#ifndef GAME_OBJ_ECS
		if (auto obj = m_cmask.GetMaskNode()) 
		{
			auto& ceditor = obj->GetUniqueComp<ee0::CompNodeEditor>();
			path = ceditor.GetFilepath();
		}
#else
		if (!m_cmask.mask.IsNull())
		{
			auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(m_cmask.mask);
			path = ceditor.filepath;
		}
#endif // GAME_OBJ_ECS
		sizer->Add(m_mask_path = new wxTextCtrl(win, wxID_ANY, path,
			wxDefaultPosition, wxSize(180, -1), wxTE_READONLY));

		sizer->AddSpacer(5);

		wxButton* btn = new wxButton(win, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		sizer->Add(btn);
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
			wxCommandEventHandler(WxCompMaskPanel::OnSetMaskPath));

		pane_sizer->Add(sizer);
	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompMaskPanel::OnSetBasePath(wxCommandEvent& event)
{
	auto obj = CreateNodeFromFile();
#ifndef GAME_OBJ_ECS
	if (!obj) {
		return;
	}

	m_cmask.SetBaseNode(obj);

	auto& ceditor = obj->GetUniqueComp<ee0::CompNodeEditor>();
	m_base_path->SetValue(ceditor.GetFilepath());
#else
	if (obj.IsNull()) {
		return;
	}

	m_cmask.base = obj;

	auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(obj);
	m_base_path->SetValue(ceditor.filepath);
#endif // GAME_OBJ_ECS
}

void WxCompMaskPanel::OnSetMaskPath(wxCommandEvent& event)
{
	auto obj = CreateNodeFromFile();
#ifndef GAME_OBJ_ECS
	if (!obj) {
		return;
	}

	m_cmask.SetMaskNode(obj);

	auto& ceditor = obj->GetUniqueComp<ee0::CompNodeEditor>();
	m_mask_path->SetValue(ceditor.GetFilepath());

	auto& cbb = m_obj->GetUniqueComp<n2::CompBoundingBox>();
	cbb.SetSize(*m_obj, obj->GetUniqueComp<n2::CompBoundingBox>().GetSize());
#else
	if (obj.IsNull()) {
		return;
	}

	m_cmask.mask = obj;

	auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(obj);
	m_mask_path->SetValue(ceditor.filepath);

	auto& cbb = m_world.GetComponent<e2::CompBoundingBox>(m_obj);
	cbb.rect = m_world.GetComponent<e2::CompBoundingBox>(obj).rect;
#endif // GAME_OBJ_ECS
}

ee0::GameObj WxCompMaskPanel::CreateNodeFromFile()
{
	std::string filter = "*.png;*.jpg;*.bmp;*.pvr;*.pkm";
	wxFileDialog dlg(this, wxT("Choose image"), wxEmptyString, filter);
	if (dlg.ShowModal() != wxID_OK) {
#ifndef GAME_OBJ_ECS
		return nullptr;
#else
		return ee0::GameObj();
#endif // GAME_OBJ_ECS
	}

	std::string filepath = dlg.GetPath().ToStdString();
#ifndef GAME_OBJ_ECS
	auto obj = ns::NodeFactory::Create(filepath);
	if (!obj) {
		return nullptr;
	}
#else
	auto obj = es::EntityFactory::Create(m_world, filepath);
	if (obj.IsNull()) {
		return ee0::GameObj();
	}
#endif // GAME_OBJ_ECS

	// editor
#ifndef GAME_OBJ_ECS
	auto& ceditor = obj->GetUniqueComp<ee0::CompNodeEditor>();
	ceditor.SetFilepath(filepath);
#else
	auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(obj);
	ceditor.filepath = filepath;
#endif // GAME_OBJ_ECS

	return obj;
}

}