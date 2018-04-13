#pragma once

#include <ee0/GameObj.h>

#include <SM_Vector.h>

#include <vector>

namespace ee0 { class NodeContainer; }
#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
#endif // GAME_OBJ_ECS

namespace ee2
{

class AutoAlign
{
public:
	AutoAlign(ee0::NodeContainer& objs);

	void Align(
#ifdef GAME_OBJ_ECS
		ecsx::World& world,
#endif // GAME_OBJ_ECS
		const std::vector<ee0::GameObj>& objs
	);
	void Align(
#ifdef GAME_OBJ_ECS
		ecsx::World& world,
#endif // GAME_OBJ_ECS
		const ee0::GameObj& src, 
		const ee0::GameObj& dst
	);

	void Draw() const;

	void SetInvisible();

	bool IsOpen() const { return m_open; }
	void SetOpen(bool open) { m_open = open; }

private:
	ee0::NodeContainer& m_objs;

	bool m_open;
	sm::vec2 m_hor[2], m_ver[2];

}; // AutoAlign

}