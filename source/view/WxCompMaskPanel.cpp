#include "ee2/WxCompMaskPanel.h"

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node0/CompIdentity.h>
#include <node2/CompMask.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <ns/NodeFactory.h>
#else
#include <entity0/World.h>
#include <entity2/CompMask.h>
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

WxCompMaskPanel::WxCompMaskPanel(const ur::Device& dev,
                                 wxWindow* parent, ECS_WORLD_PARAM
	                             const ee0::GameObj& obj)
	: ee0::WxCompPanel(parent, "Mask")
    , m_dev(dev)
	ECS_WORLD_SELF_ASSIGN
	, m_obj(obj)
{
	InitLayout();
	Expand();
}

void WxCompMaskPanel::RefreshNodeComp()
{
#ifndef GAME_OBJ_ECS
	auto& cmask = m_obj->GetSharedComp<n2::CompMask>();
	auto& base = cmask.GetBaseNode();
	if (base)
	{
		auto& cid = base->GetUniqueComp<n0::CompIdentity>();
		m_base_path->SetValue(cid.GetFilepath());
	}
	auto& mask = cmask.GetMaskNode();
	if (mask)
	{
		auto& cid = base->GetUniqueComp<n0::CompIdentity>();
		m_mask_path->SetValue(cid.GetFilepath());
	}
#else
	auto& cmask = m_world.GetComponent<e2::CompMask>(m_obj);
	auto& base = cmask.base;
	if (!base.IsNull())
	{
		auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(base);
		m_base_path->SetValue(*ceditor.filepath);
	}
	auto& mask = cmask.mask;
	if (!mask.IsNull())
	{
		auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(mask);
		m_mask_path->SetValue(*ceditor.filepath);
	}
#endif // GAME_OBJ_ECS
}

void WxCompMaskPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

#ifndef GAME_OBJ_ECS
	auto& cmask = m_obj->GetSharedComp<n2::CompMask>();
#else
	auto& cmask = m_world.GetComponent<e2::CompMask>(m_obj);
#endif // GAME_OBJ_ECS

	// base filepath
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Base ")));

		std::string path;
#ifndef GAME_OBJ_ECS
		if (auto obj = cmask.GetBaseNode())
		{
			auto& cid = obj->GetUniqueComp<n0::CompIdentity>();
			path = cid.GetFilepath();
		}
#else
		if (!cmask.base.IsNull())
		{
			auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(cmask.base);
			path = *ceditor.filepath;
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
		if (auto obj = cmask.GetMaskNode())
		{
			auto& cid = obj->GetUniqueComp<n0::CompIdentity>();
			path = cid.GetFilepath();
		}
#else
		if (!cmask.mask.IsNull())
		{
			auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(cmask.mask);
			path = *ceditor.filepath;
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
	if (!GAME_OBJ_VALID(obj)) {
		return;
	}

#ifndef GAME_OBJ_ECS
	auto& cmask = m_obj->GetSharedComp<n2::CompMask>();
	cmask.SetBaseNode(obj);

	auto& cid = obj->GetUniqueComp<n0::CompIdentity>();
	cid.SetID(0);
	cid.SetName("base");
	m_base_path->SetValue(cid.GetFilepath());
#else
	auto& cmask = m_world.GetComponent<e2::CompMask>(m_obj);
	cmask.base = obj;

	auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(obj);
	m_base_path->SetValue(*ceditor.filepath);
#endif // GAME_OBJ_ECS
}

void WxCompMaskPanel::OnSetMaskPath(wxCommandEvent& event)
{
	auto obj = CreateNodeFromFile();
	if (!GAME_OBJ_VALID(obj)) {
		return;
	}

#ifndef GAME_OBJ_ECS
	auto& cmask = m_obj->GetSharedComp<n2::CompMask>();
	cmask.SetMaskNode(obj);

	auto& cid = obj->GetUniqueComp<n0::CompIdentity>();
	cid.SetID(1);
	cid.SetName("mask");
	m_mask_path->SetValue(cid.GetFilepath());

	auto& cbb = m_obj->GetUniqueComp<n2::CompBoundingBox>();
	cbb.SetSize(*m_obj, obj->GetUniqueComp<n2::CompBoundingBox>().GetSize());
#else
	auto& cmask = m_world.GetComponent<e2::CompMask>(m_obj);
	cmask.mask = obj;

	auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(obj);
	m_mask_path->SetValue(*ceditor.filepath);

	auto& cbb = m_world.GetComponent<e2::CompBoundingBox>(m_obj);
	cbb.rect = m_world.GetComponent<e2::CompBoundingBox>(obj).rect;
#endif // GAME_OBJ_ECS
}

ee0::GameObj WxCompMaskPanel::CreateNodeFromFile()
{
	std::string filter = "*.png;*.jpg;*.bmp;*.pvr;*.pkm";
	wxFileDialog dlg(this, wxT("Choose image"), wxEmptyString, filter);
	if (dlg.ShowModal() != wxID_OK) {
		return GAME_OBJ_NULL;
	}

	std::string filepath = dlg.GetPath().ToStdString();
#ifndef GAME_OBJ_ECS
	auto obj = ns::NodeFactory::Create(m_dev, filepath);
#else
	auto obj = es::EntityFactory::Create(m_world, filepath);
#endif // GAME_OBJ_ECS
	if (!GAME_OBJ_VALID(obj)) {
		return GAME_OBJ_NULL;
	}

	// editor
#ifndef GAME_OBJ_ECS
	auto& cid = obj->GetUniqueComp<n0::CompIdentity>();
	cid.SetFilepath(filepath);
#else
	auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(obj);
	ceditor.filepath = std::make_unique<std::string>(filepath);
#endif // GAME_OBJ_ECS

	return obj;
}

}